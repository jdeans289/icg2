#include <cstdlib>

#include "Utils/MutableDeclaration.hh"
#include "Utils/LexicalAnalyzer.hh"

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

// CONSTRUCTOR
MutableDeclaration::MutableDeclaration( std::string s ) : MutableDeclaration(s, std::vector<int>()) {}

MutableDeclaration::MutableDeclaration( std::string base_type, std::vector<int> dimensions) : dims(dimensions) {
    lexer.load(base_type);
    parseDeclaration();
}

// ===============================================
// type-name: type-specifier
//          | type-specifier declarator
//          ;
// ===============================================
bool MutableDeclaration::parseDeclaration() {

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
bool MutableDeclaration::parseDeclarator() {
   
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
bool MutableDeclaration::parseDirectDeclarator() {

//    std::cout << __FUNCTION__ << std::endl;
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
       if (token == Token::Integer) {
           dims.push_back( std::atoi( (lexer.getText()).c_str() ));
           token = lexer.matchToken(Token::Integer);
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
       if (token == Token::Integer) {
           dims.push_back(std::atoi( (lexer.getText()).c_str() ));
           token = lexer.matchToken(Token::Integer);
       }
       if ((token = lexer.matchToken( Token::RightBracket )) == Token::Error) {
           errorCondition = true;
       }
   }
   return errorCondition;
}

bool MutableDeclaration::parseTypeSpecifier() {

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
            typeSpec = lexer.getText();
            token = lexer.matchToken(Token::Identifier);
        } break;
        default : {
            errorCondition = true;
        }
    }
    return errorCondition;
}

std::string MutableDeclaration::getTypeSpecifier() const {
    return typeSpec;
}

std::string MutableDeclaration::getVariableName() const {
    return varName;
}

unsigned int MutableDeclaration::getNumberOfDimensions() const {
    return dims.size();
}

int MutableDeclaration::getDimensionSize( unsigned int index) const {
    // FIXME: needs bounds checking.
    return dims[index];
}

std::vector<int> MutableDeclaration::getDims() const {
    return dims;
}

// Reference for this algorithm: https://eli.thegreenplace.net/2008/07/18/reading-c-type-declarations/
std::string MutableDeclaration::getDeclarator(std::string varName) {
    if (!validateDimensions()) {
        std::cerr << "Invalid dimensions. All dimensions must be a positive number or -1 to denote a pointer." << std::endl;
        return "";
    }

    std::stringstream ss;

    // Type goes first
    ss << typeSpec;

    if (varName != "") {
        ss << " ";
    }

    // Start from the varname and go out
    std::string curr_declarator = varName;

    enum Direction {LEFT, RIGHT};
    Direction dir = RIGHT;

    // Then go through the dimensions
    for (int dim : dims) {
        if (dim >= 1) {
            // Constrained dimension

            if (dir == LEFT) {
                // If we have changed directions, add a grouping
                curr_declarator = "(" + curr_declarator + ")";
            }
            curr_declarator = curr_declarator + "[" + std::to_string(dim) + "]";

            // Arrays are always to the right
            dir = RIGHT;
        }

        if (dim == -1) {
            // Pointer
            curr_declarator = "*" + curr_declarator;

            // Pointers are always to the left
            dir = LEFT;
        }

    }

    ss << curr_declarator;
    
    return ss.str();

}

std::string MutableDeclaration::getAbstractDeclarator() {
    return getDeclarator("");
}

bool MutableDeclaration::is_valid_dimension (int dim) {
    return dim == -1 || dim >= 1;
}

bool MutableDeclaration::validateDimensions() {
    for (int dim : dims) {
        if (!is_valid_dimension(dim)) {
            return false;
        }
    }

    return true;
}

int MutableDeclaration::popDimension() {
    int ret = dims.front();

    // Maybe we should use a deque or something
    dims.erase(dims.begin());

    return ret;
}

void MutableDeclaration::pushDimension(int dim) {

    dims.insert(dims.begin(), dim);

}