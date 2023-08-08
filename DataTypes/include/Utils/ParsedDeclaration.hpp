#pragma once

#include <string>
#include <iostream>
#include <deque>
#include <vector>


#include "Utils/LexicalAnalyzer.hpp"



class ParsedDeclaration {

    public:

    ParsedDeclaration( std::string s );

    std::string getTypeSpecifier() const;
    std::deque<std::string> getQualifiedNameParts() const;
    std::string getVariableName() const;
    std::vector<int> getDims() const;

    private:
    
    bool parseTypeSpecifier();
    bool parseDeclaration();
    bool parseDeclarator();
    bool parseDirectDeclarator();
    bool parseQualifiedIdentifier();
    bool parseIdentifier();
    
    bool match(Token::e expected_token);

    Token::e nextToken;
    LexicalAnalyzer lexer;

    std::deque<std::string> qualifiedTypeNameParts;
    std::string typeSpec;
    std::string varName;
    std::vector<int> dims;
};

