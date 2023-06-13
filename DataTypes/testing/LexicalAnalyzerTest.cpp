#include <gtest/gtest.h>
#include <stddef.h>
#include "Utils/LexicalAnalyzer.hh"
#include <iostream>

TEST(LexicalAnalyzer, test1) {

    Token::e token;
    LexicalAnalyzer lexer;

    lexer.load("float");
    token = lexer.getToken();
    EXPECT_EQ(Token::Float, token );

    token = lexer.nextToken();
    EXPECT_EQ(Token::EndOfFile, token );
}

TEST(LexicalAnalyzer, test2) {

    Token::e token;
    LexicalAnalyzer lexer;

    lexer.load("double(*)[123]");

    token = lexer.getToken();

    EXPECT_EQ(Token::Double, token );

    token = lexer.matchToken(Token::Double);

    EXPECT_EQ(Token::LeftParen, token );

    token = lexer.matchToken(Token::LeftParen);

    EXPECT_EQ(Token::Asterisk, token );

    token = lexer.matchToken(Token::Asterisk);

    EXPECT_EQ(Token::RightParen, token );

    token = lexer.matchToken(Token::RightParen);

    EXPECT_EQ(Token::LeftBracket, token );

    token = lexer.matchToken(Token::LeftBracket);

    EXPECT_EQ(Token::Integer, token );

    token = lexer.matchToken(Token::Integer);

    EXPECT_EQ(Token::RightBracket, token );

    token = lexer.matchToken(Token::RightBracket);

    EXPECT_EQ(Token::EndOfFile, token );
}

TEST(LexicalAnalyzer, underscore) {

    LexicalAnalyzer lexer;
    lexer.load("My_Valid_Type_Name[5]");

    EXPECT_EQ(Token::Identifier, lexer.getToken());
    EXPECT_EQ("My_Valid_Type_Name", lexer.getText());

    EXPECT_EQ(Token::LeftBracket, lexer.nextToken());
    EXPECT_EQ(Token::Integer, lexer.nextToken());
    EXPECT_EQ(Token::RightBracket, lexer.nextToken());



}

TEST(LexicalAnalyzer, underscore_leading) {

    LexicalAnalyzer lexer;
    lexer.load("_My_Valid_Type_Name[5]");

    EXPECT_EQ(Token::Identifier, lexer.getToken());
    EXPECT_EQ("_My_Valid_Type_Name", lexer.getText());

    EXPECT_EQ(Token::LeftBracket, lexer.nextToken());
    EXPECT_EQ(Token::Integer, lexer.nextToken());
    EXPECT_EQ(Token::RightBracket, lexer.nextToken());



}


TEST(LexicalAnalyzer, template1) {
    LexicalAnalyzer lexer;

    lexer.load("MyClass<int>");

    EXPECT_EQ(Token::Identifier, lexer.getToken());
    EXPECT_EQ("MyClass", lexer.getText());

    EXPECT_EQ(Token::LeftAngle, lexer.nextToken());

    EXPECT_EQ(Token::Int, lexer.nextToken());

    EXPECT_EQ(Token::RightAngle, lexer.nextToken());

    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}

