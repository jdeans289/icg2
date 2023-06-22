#include "Utils/ParsedAssignment.hh"

#include "Value/IntegerValue.hh"
#include "Value/FloatingPointValue.hh"
#include "Value/StringValue.hh"


const ParsedAssignment::RegexTable ParsedAssignment::regex_table = {
     {  std::regex("-?[0-9]+"), INT  },
     {  std::regex("0[x|X][a-f0-9]+"), INT  },  
     {  std::regex("-?[0-9]*\.[0-9]+"), FLOAT  },  
     {  std::regex("\".*\""), STRING  },
     {  std::regex("&.+"), POINTER  }
};

ParsedAssignment::ParsedAssignment (std::string assignment_string) : val (NULL), val_type (INVALID) {
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

    size_t eq_pos = assignment_string.find("=");
    size_t semicolon_pos = assignment_string.find(";");
    if (eq_pos == std::string::npos) {
        throw std::logic_error ("Assignment does not contain an '=' character, cannot be parsed.");
    }
    if (semicolon_pos == std::string::npos) {
        throw std::logic_error ("Assignment does not contain an ';' character, cannot be parsed.");
    }

    var_name = assignment_string.substr(0, eq_pos);
    trim(var_name);

    // Make some representation for the value
    size_t value_length = semicolon_pos - (eq_pos + 1);
    val_str = assignment_string.substr(eq_pos + 1, value_length);
    trim(val_str);

    val_type = matchType(val_str);
    
    switch (val_type) {
        case INT: {
            val = new IntegerValue (stoi(val_str));
        }
        break;
        case FLOAT: {
            val = new FloatingPointValue (stod(val_str));
        }
        break;
        case POINTER: {
            // Put this in a string i guess
            val = new StringValue(val_str.c_str());
        }
        break;
        case STRING: {
            // Cut off the quotation marks 
            val = new StringValue(val_str.substr(1, val_str.size()-2).c_str());
        }
        break;
        case INVALID: {
            std::string message = "Cannot parse the string \"" + val_str + "\" as any kind of value.";
            throw std::logic_error (message);
        }
    }
}

ParsedAssignment::ValueType ParsedAssignment::matchType (std::string value) {
    for (auto item : regex_table) {
        auto regex = item.first;
        if (std::regex_match(value, regex)) {
            return item.second;
        }
    }
    return INVALID;
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

ParsedAssignment::ValueType ParsedAssignment::getValueType() {
    return val_type;
}
