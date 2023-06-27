#include "Utils/ParsedDeclaration.hh"

/*
 type-name: type-specifier
          | type-specifier declarator
          ;

 declarator : pointer
            | pointer direct-declarator
            | direct-declarator
            ;

 direct-declarator: identifier
                  | ( declarator )
                  | [ constant-exp ]
                  | [ ]
                  | direct-declarator [ constant-exp ]
                  | direct-declarator [ ]
                  ;
*/

ParsedDeclaration::ParsedDeclaration(std::string declaration_str) : typeSpec("") {
    lexer.load(declaration_str);
    parseDeclaration();
}


// ===============================================
// type-name: type-specifier
//          | type-specifier declarator
//          ;
// ===============================================
bool ParsedDeclaration::parseDeclaration() {

    // std::cout << __FUNCTION__ << std::endl;
    bool errorCondition = parseTypeSpecifier();
    if ( !errorCondition ) {
        Token::e token = lexer.getToken();
        if ((token == Token::Asterisk) ||
            (token == Token::Identifier ) ||
            (token == Token::LeftParen ) ||
            (token == Token::LeftBracket)) {
            errorCondition = parseDeclarator();
        }
        if (lexer.matchToken(Token::EndOfFile) == Token::Error) {
            errorCondition = true;
        }
    }
    return errorCondition;
}

// ==============================================================
// declarator : pointer
//            | pointer direct-declarator
//            | direct-declarator
//            ;
//
// NOTE: a pointer is one or more asterisks.
// ==============================================================
bool ParsedDeclaration::parseDeclarator() {
   
//    std::cout << __FUNCTION__ << std::endl;
   bool errorCondition = false;
   int asteriskCount = 0;

   Token::e token = lexer.getToken();
   while (token == Token::Asterisk ) {
       token = lexer.matchToken( Token::Asterisk );
       asteriskCount ++;
   }
   if ((token == Token::LeftParen) || (token == Token::LeftBracket) || (token == Token::Identifier)) {
       errorCondition |= parseDirectDeclarator();
   }

   if (asteriskCount != 0) {
       for (int ii=0 ; ii < asteriskCount ; ii++) {
           dims.push_back(-1);
       }
   }
   return errorCondition;
}

// ==============================================================
// direct-declarator: identifier
//                  | ( declarator )
//                  | [ integer ]
//                  | direct-declarator [ integer ]
//                  ;
// ==============================================================
bool ParsedDeclaration::parseDirectDeclarator() {
   bool errorCondition = false;

   Token::e token = lexer.getToken();
   if (token == Token::Identifier) {
       varName = lexer.getText();
       token = lexer.matchToken( Token::Identifier );
   } else if (token == Token::LeftParen) {
       token = lexer.matchToken( Token::LeftParen );
       errorCondition |= parseDeclarator();
       if ( !errorCondition ) {
           if ((token = lexer.matchToken( Token::RightParen )) == Token::Error) {
           errorCondition = true;
           }
       }
   } else if (token == Token::LeftBracket) {
       token = lexer.matchToken( Token::LeftBracket );
       if (token == Token::IntegerLiteral) {
           dims.push_back( std::atoi( (lexer.getText()).c_str() ));
           token = lexer.matchToken(Token::IntegerLiteral);
       }
       if ((token = lexer.matchToken( Token::RightBracket )) == Token::Error) {
           errorCondition = true;
       }
   } else {
       // Note: this is the case of an anonymous declaration.
       varName.clear();
   }

   while (token == Token::LeftBracket) {
       token = lexer.matchToken( Token::LeftBracket );
       if (token == Token::IntegerLiteral) {
           dims.push_back(std::atoi( (lexer.getText()).c_str() ));
           token = lexer.matchToken(Token::IntegerLiteral);
       }
       if ((token = lexer.matchToken( Token::RightBracket )) == Token::Error) {
           errorCondition = true;
       }
   }
   return errorCondition;
}


// ==============================================================
// type-specifier   : sign-qualifier integral-type-keyword
//                  | float-type-keyword
//                  | qualified-identifier
//                  ;
// ==============================================================

// ==============================================================
// sign-qualifier   : <NOTHING>
//                  | signed
//                  | unsigned
// ==============================================================
// am I actually allowed to have <NOTHING> in a bnf grammar?
// probably not

bool ParsedDeclaration::parseTypeSpecifier() {

    // std::cout << __FUNCTION__ << std::endl;
    bool errorCondition = false;
    Token::e token = lexer.getToken();

    switch ( token ) {
        case Token::Void : {
            token = lexer.matchToken(Token::Void);
            typeSpec = "void";
        } break;
        case Token::Boolean : {
            token = lexer.matchToken(Token::Boolean);
            typeSpec = "bool";
        } break;
        case Token::Char : {
            token = lexer.matchToken(Token::Char);
            typeSpec = "char";
        } break;
        case Token::Short : {
            token = lexer.matchToken(Token::Short);
            typeSpec = "short";
        } break;
        case Token::Int : {
            token = lexer.matchToken(Token::Int);
            typeSpec = "int";
        } break;
        case Token::Long : {
            token = lexer.matchToken(Token::Long);
            if (token == Token::Long) {
                token = lexer.matchToken(Token::Long);
                typeSpec = "long long";
            } else {
                typeSpec = "long";
            }
        } break;
        case Token::Unsigned : {
            token = lexer.matchToken(Token::Unsigned);
            switch ( token ) {
                case Token::Char : {
                    token = lexer.matchToken(Token::Char);
                    typeSpec = "unsigned char";
                } break;
                case Token::Short : {
                    token = lexer.matchToken(Token::Short);
                    typeSpec = "unsigned short";
                } break;
                case Token::Int : {
                    token = lexer.matchToken(Token::Int);
                    typeSpec = "unsigned int";
                } break;
                case Token::Long : {
                    token = lexer.matchToken(Token::Long);
                    if (token == Token::Long) {
                        token = lexer.matchToken(Token::Long);
                        typeSpec = "unsigned long long";
                    } else {
                        typeSpec = "unsigned long";
                    }
                } break;
                default: {
                    typeSpec = "unsigned int";
                }
            }
        } break;
        case Token::Signed : {
            token = lexer.matchToken(Token::Signed);
            switch ( token ) {
                case Token::Char : {
                    token = lexer.matchToken(Token::Char);
                    typeSpec = "char";
                } break;
                case Token::Short : {
                    token = lexer.matchToken(Token::Short);
                    typeSpec = "short";
                } break;
                case Token::Int : {
                    token = lexer.matchToken(Token::Int);
                    typeSpec = "int";
                } break;
                case Token::Long : {
                    token = lexer.matchToken(Token::Long);
                    if (token == Token::Long) {
                        token = lexer.matchToken(Token::Long);
                        typeSpec = "long long";
                    } else {
                        typeSpec = "long";
                    }
                } break;
                default: {
                    typeSpec = "int";
                }
            }
        } break;
        case Token::WideChar : {
            token = lexer.matchToken(Token::WideChar);
            typeSpec = "wchar_t";
        } break;
        case Token::Float : {
            token = lexer.matchToken(Token::Float);
            typeSpec = "float";
        } break;
        case Token::Double : {
            token = lexer.matchToken(Token::Double);
            typeSpec = "double";
        } break;
        case Token::Identifier : {
            errorCondition |= parseQualifiedIdentifier();
        } break;
        default : {
            errorCondition = true;
        }
    }
    return errorCondition;
}


// ==============================================================
// qualified-identifier   : identifier
//                        | identifier::qualified-identifier
// ==============================================================
bool ParsedDeclaration::parseQualifiedIdentifier() {
    // TODO: do we actually want to shove the whole typeSpecifier into a string? 
    // We should probably do something better

    // We should see an identifier first
    Token::e token = lexer.getToken();
    if (token != Token::Identifier) {
        // This is an error condition
        return true;
    }
    typeSpec += lexer.getText();
    
    // Next two tokens must be ::
    // Or something completely different
    token = lexer.matchToken(Token::Identifier);
    if (token != Token::Colon) {
        // Reached the end of the identifier, move on
        return false;
    }

    // Otherwise, we have a colon.
    typeSpec += lexer.getText();
    token = lexer.matchToken(Token::Colon);

    // If this one isn't also a colon, something went bad    
    if (token != Token::Colon) {
        // Error condition has occcurred
        return true;
    }

    // Advance the lexer and do it all over again
    typeSpec += lexer.getText();
    lexer.matchToken(Token::Colon);

    return parseQualifiedIdentifier();
}

std::string ParsedDeclaration::getTypeSpecifier() const {
    return typeSpec;
}

std::string ParsedDeclaration::getVariableName() const {
    return varName;
}

std::vector<int> ParsedDeclaration::getDims() const {
    return dims;
}