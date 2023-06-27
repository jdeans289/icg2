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

    EXPECT_EQ(Token::IntegerLiteral, token );

    token = lexer.matchToken(Token::IntegerLiteral);

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
    EXPECT_EQ(Token::IntegerLiteral, lexer.nextToken());
    EXPECT_EQ(Token::RightBracket, lexer.nextToken());



}

TEST(LexicalAnalyzer, underscore_leading) {

    LexicalAnalyzer lexer;
    lexer.load("_My_Valid_Type_Name[5]");

    EXPECT_EQ(Token::Identifier, lexer.getToken());
    EXPECT_EQ("_My_Valid_Type_Name", lexer.getText());

    EXPECT_EQ(Token::LeftBracket, lexer.nextToken());
    EXPECT_EQ(Token::IntegerLiteral, lexer.nextToken());
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

TEST(LexicalAnalyzer, Assignment) {
    LexicalAnalyzer lexer;

    lexer.load("int my_class = 1234;");

    EXPECT_EQ(Token::Int, lexer.getToken());

    EXPECT_EQ(Token::Identifier, lexer.nextToken());
    EXPECT_EQ("my_class", lexer.getText());

    EXPECT_EQ(Token::EqSign, lexer.nextToken());

    EXPECT_EQ(Token::IntegerLiteral, lexer.nextToken());
    EXPECT_EQ("1234", lexer.getText());

    EXPECT_EQ(Token::Semicolon, lexer.nextToken());
    
    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}

TEST(LexicalAnalyzer, FloatAssignment) {
    LexicalAnalyzer lexer;

    lexer.load("double my_double = 12.34;");

    EXPECT_EQ(Token::Double, lexer.getToken());

    EXPECT_EQ(Token::Identifier, lexer.nextToken());
    EXPECT_EQ("my_double", lexer.getText());

    EXPECT_EQ(Token::EqSign, lexer.nextToken());

    EXPECT_EQ(Token::FloatLiteral, lexer.nextToken());
    EXPECT_EQ("12.34", lexer.getText());
    
    EXPECT_EQ(Token::Semicolon, lexer.nextToken());
    
    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}

TEST(LexicalAnalyzer, namespace_qualifier) {
    LexicalAnalyzer lexer;

    lexer.load("std::string my_str");

    EXPECT_EQ(Token::Identifier, lexer.getToken());
    EXPECT_EQ("std", lexer.getText());

    EXPECT_EQ(Token::Colon, lexer.nextToken());
    EXPECT_EQ(Token::Colon, lexer.nextToken());

    EXPECT_EQ(Token::Identifier, lexer.nextToken());
    EXPECT_EQ("string", lexer.getText());

    EXPECT_EQ(Token::Identifier, lexer.nextToken());
    EXPECT_EQ("my_str", lexer.getText());
    
    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}

TEST(LexicalAnalyzer, const_qualifier) {
    LexicalAnalyzer lexer;

    lexer.load("const char * my_str_literal");

    EXPECT_EQ(Token::Const, lexer.getToken());

    EXPECT_EQ(Token::Char, lexer.nextToken());

    EXPECT_EQ(Token::Asterisk, lexer.nextToken());

    EXPECT_EQ(Token::Identifier, lexer.nextToken());
    EXPECT_EQ("my_str_literal", lexer.getText());
    
    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}

TEST(LexicalAnalyzer, string_literal) {
    LexicalAnalyzer lexer;

    lexer.load("my_str_literal = \"Here is a string literal\"");

    EXPECT_EQ(Token::Identifier, lexer.getToken());
    EXPECT_EQ("my_str_literal", lexer.getText());

    EXPECT_EQ(Token::EqSign, lexer.nextToken());

    EXPECT_EQ(Token::StringLiteral, lexer.nextToken());
    EXPECT_EQ("\"Here is a string literal\"", lexer.getText());

    
    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}

// TODO: I'm gonna have to think about what actually makes sense here.
TEST(LexicalAnalyzer, DISABLED_string_literal_escape_chars) {
    LexicalAnalyzer lexer;

    // Wow this is hard to think about lol
    lexer.load("const char * my_str_literal = \"Here is a \\\"string\\\" literal\"");

    EXPECT_EQ(Token::Const, lexer.getToken());

    EXPECT_EQ(Token::Char, lexer.nextToken());

    EXPECT_EQ(Token::Asterisk, lexer.nextToken());

    EXPECT_EQ(Token::Identifier, lexer.nextToken());
    EXPECT_EQ("my_str_literal", lexer.getText());

    EXPECT_EQ(Token::EqSign, lexer.nextToken());

    EXPECT_EQ(Token::StringLiteral, lexer.nextToken());
    EXPECT_EQ("\"Here is a \"string\" literal\"", lexer.getText());

    
    EXPECT_EQ(Token::EndOfFile, lexer.nextToken());
}