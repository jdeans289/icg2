#ifndef PARSED_DECLARATION_HH
#define PARSED_DECLARATION_HH

#include <string>
#include <iostream>
#include <vector>

#include "LexicalAnalyzer.hh"


class ParsedDeclaration {

    public:

    ParsedDeclaration( std::string s );
    std::string getTypeSpecifier() const;
    std::string getVariableName() const;
    unsigned int getNumberOfDimensions() const;
    int getDimensionSize( unsigned int index) const;

    std::vector<int> getDims() const;

    private:
    bool parseTypeSpecifier();
    bool parseDeclaration();
    bool parseDeclarator();
    bool parseDirectDeclarator();

    bool match(Token::e expected_token);

    Token::e nextToken;
    LexicalAnalyzer lexer;

    std::string typeSpec;
    std::string varName;
    std::vector<int> dims;
};

#endif
