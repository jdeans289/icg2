#include "Utils/ParsedAssignment.hpp"

#include "Utils/LexicalAnalyzer.hpp"

#include "Value/IntegerValue.hpp"
#include "Value/FloatingPointValue.hpp"
#include "Value/StringValue.hpp"


// const ParsedAssignment::RegexTable ParsedAssignment::regex_table = {
//      {  std::regex("-?[0-9]+"), INT  },
//      {  std::regex("0[x|X][a-f0-9]+"), INT  },  
//      {  std::regex("-?[0-9]*\.[0-9]+"), FLOAT  },  
//      {  std::regex("\".*\""), STRING  },
//      {  std::regex("&.+"), POINTER  }
// };

ParsedAssignment::ParsedAssignment (std::string assignment_string) : val (NULL) {
    try {
        parse(assignment_string);
    } catch (std::exception& e) {
        // re-throw with more descriptive message
        std::string message = "Assignment parsing of string \"" + assignment_string + "\" failed with message:\n\t" + e.what();
        throw std::logic_error(message);
    }
}


// String trimming from https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring
// trim from start (in place)
static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}

// trim from end (in place)
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) {
    rtrim(s);
    ltrim(s);
}

void ParsedAssignment::parse(std::string assignment_string) {

    // format should be
    // <varname> = <value> ;

    LexicalAnalyzer lexer;
    lexer.load(assignment_string);

    // Everything until the EqSign is part of the varname
    while (lexer.getToken() != Token::EqSign) {
        var_name += lexer.getText();
        lexer.nextToken();
    }

    Token::e value_type = lexer.nextToken();
    std::string tokenText = lexer.getText();

    switch(value_type) {
        case Token::IntegerLiteral: {
            val = new IntegerValue (stoi(tokenText));
        }
        break;
        case Token::FloatLiteral: {
            val = new FloatingPointValue (stod(tokenText));
        }
        break;
        case Token::Ampersand : {
            // Put this in a string i guess
            Token::e nextToken;
            while ((nextToken = lexer.nextToken()) != Token::Semicolon) {
                tokenText += lexer.getText();
            }
            val = new StringValue(tokenText);
        }
        break;
        case Token::StringLiteral: {
            // Cut off the quotation marks 
            val = new StringValue(tokenText.substr(1, tokenText.size()-2));
        }
        break;
        default: {
            std::string message = "Cannot parse the string \"" + tokenText + "\" as any kind of value.";
            throw std::logic_error (message);
        }
    }
}

std::string ParsedAssignment::getVariableName() {
    return var_name;
}

std::string ParsedAssignment::getValueRawString() {
    return val_str;
}

Value * ParsedAssignment::getValue() {
    return val;
}
