#include <string>
#include <regex>
#include <map>

#include "Value/Value.hpp"

class ParsedAssignment {
    public:

        ParsedAssignment (std::string assignment_string);

        std::string getVariableName();
        std::string getValueRawString();
        Value * getValue();

        // enum ValueType {
        //     INT,
        //     FLOAT,
        //     POINTER,
        //     STRING,
        //     INVALID
        // };

        // ValueType getValueType();

    private:
        void parse(std::string s);
        void parse2(std::string s);


        std::string var_name;
        Value * val;
        std::string val_str;
        // ValueType val_type;

        // typedef std::vector<std::pair<std::regex, ValueType>> RegexTable;
        
        // const static RegexTable regex_table;
        // static ValueType matchType (std::string value);


};

