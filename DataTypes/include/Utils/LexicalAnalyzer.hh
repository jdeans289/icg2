#ifndef LEXICAL_ANALYZER_HH
#define LEXICAL_ANALYZER_HH

#include <string>
#include <sstream>

namespace Token {
    enum e {
        Error        ,
        EndOfFile    ,
        Identifier   ,
        Void         ,
        Boolean      ,
        Char         ,
        Short        ,
        Int          ,
        Long         ,
        WideChar     ,
        Signed       ,
        Unsigned     ,
        Float        ,
        Double       ,
        Const        ,
        LeftParen    ,
        RightParen   ,
        LeftBracket  ,
        RightBracket ,
        Asterisk     ,
        IntegerLiteral,
        FloatLiteral ,
        StringLiteral,
        LeftAngle    ,
        RightAngle   ,
        Colon        ,
        Semicolon    ,
        EqSign       ,
        Ampersand    ,
        Comma        ,
        Period
    };
};

class LexicalAnalyzer {

    public:
    LexicalAnalyzer();
    void load( const std::string& s );
    Token::e getToken();
    Token::e nextToken();
    Token::e matchToken(Token::e expectedToken );
    std::string getText();
    Token::e token;

    private:
    char nextChar;
    std::stringstream inStream;
    std::string lexemeText;
};

#endif
