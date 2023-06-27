#include "Utils/LexicalAnalyzer.hh"
#include <iostream>

struct {
    const char *         key;
    Token::e token;
} keytable[] = {
    {"void",     Token::Void},
    {"bool",     Token::Boolean},
    {"char",     Token::Char},
    {"short",    Token::Short},
    {"int",      Token::Int},
    {"long",     Token::Long},
    {"wchar_t",  Token::WideChar},
    {"signed",   Token::Signed},
    {"unsigned", Token::Unsigned},
    {"float",    Token::Float},
    {"double",   Token::Double},
    {"const",    Token::Const}
};

typedef enum {
    BEGINNING               ,
    ID_COLLECTION           ,
    NUM_LITERAL_COLLECTION  ,
    STR_LITERAL_COLLECTION  ,
    DONE             
} LexState;

// why
#define NKEYS ( sizeof keytable / sizeof keytable[0] )


LexicalAnalyzer::LexicalAnalyzer() {

    lexemeText.clear();
    token = Token::EndOfFile;
}

void LexicalAnalyzer::load( const std::string& s ) {

    inStream << s;
    lexemeText.clear();
    inStream >> std::noskipws >> nextChar;
    token = nextToken();
}

Token::e LexicalAnalyzer::matchToken(Token::e expectedToken) {
    if ( token == expectedToken) {
        token = nextToken();
        return token;
    } else {
        std::cerr << "Syntax Error. Expected token(" << expectedToken << ")."
                  << " Found token(" << token << ") instead." << std::endl;
        return Token::Error;
    }
}

std::string LexicalAnalyzer::getText() {
    return lexemeText;
}

Token::e LexicalAnalyzer::getToken() {
    return token;
}

Token::e LexicalAnalyzer::nextToken() {

    token = Token::EndOfFile;
    lexemeText = "EOF";

    LexState state = BEGINNING;

    while ( !inStream.eof() && state != DONE ) {

        switch ( state ) {

            case BEGINNING : {
                if ( isalpha( nextChar ) || nextChar == '_') {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Identifier;
                    state = ID_COLLECTION;
                } else if ( isdigit( nextChar )) {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::IntegerLiteral;
                    state = NUM_LITERAL_COLLECTION;
                } else if ( isspace( nextChar )) {
                    inStream >> std::noskipws >> nextChar;
                } else if ( nextChar == '*')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Asterisk;
                    state = DONE;
                } else if ( nextChar == '(')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::LeftParen;
                    state = DONE;
                } else if ( nextChar == ')')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::RightParen;
                    state = DONE;
                } else if ( nextChar == '[')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::LeftBracket;
                    state = DONE;
                } else if ( nextChar == ']')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::RightBracket;
                    state = DONE;
                } else if ( nextChar == '<')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::LeftAngle;
                    state = DONE;
                } else if ( nextChar == '>')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::RightAngle;
                    state = DONE;                            
                } else if ( nextChar == ':')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Colon;
                    state = DONE;                            
                } else if ( nextChar == ';')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Semicolon;
                    state = DONE;                            
                } else if ( nextChar == '=')  {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::EqSign;
                    state = DONE;                            
                } else if ( nextChar == '\"') {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::StringLiteral;
                    state = STR_LITERAL_COLLECTION;
                } else if ( nextChar == '&') {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Ampersand;
                    state = DONE;
                } else if ( nextChar == ',') {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Comma;
                    state = DONE;
                } else if ( nextChar == '.') {
                    lexemeText = nextChar;
                    inStream >> std::noskipws >> nextChar;
                    token = Token::Period;
                    state = DONE;
                } else  {
                    token = Token::Error;
                    state = DONE;
                }
            } break;

            case ID_COLLECTION : {

                while (!inStream.eof() && (isalnum(nextChar) || nextChar == '_')) {
                    lexemeText += nextChar;
                    inStream >> std::noskipws >> nextChar;
                }
                for (unsigned int ii=0; ii < NKEYS; ii++) {
                    if ( lexemeText.compare( keytable[ii].key ) == 0 ) {
                        token = keytable[ii].token;
                        break;
                    }
                }
                state = DONE;
            } break;

            case NUM_LITERAL_COLLECTION : {
                // TODO: Support other formats, such as exponential (1e10)
                while (!inStream.eof() && isdigit(nextChar)) {
                    lexemeText += nextChar;
                    inStream >> std::noskipws >> nextChar;
                }
                if (nextChar == '.') {
                    // If a . is present, switch to float literal parsing and keep going
                    token = Token::FloatLiteral;
                    lexemeText += nextChar;
                    inStream >> std::noskipws >> nextChar;
                    while (!inStream.eof() && isdigit(nextChar)) {
                        lexemeText += nextChar;
                        inStream >> std::noskipws >> nextChar;
                    }
                }
                state = DONE;
            } break;

            case STR_LITERAL_COLLECTION : {
                while (!inStream.eof() && nextChar != '\"') {
                    lexemeText += nextChar;
                    inStream >> std::noskipws >> nextChar;
                    // TODO: HANDLE ESCAPE CHARACTERS
                }
                lexemeText += nextChar;
                inStream >> std::noskipws >> nextChar;
                state = DONE;
            } break;

            default: {
                state = DONE;
                token = Token::Error;
            }

        }
    }
    return token;
}
