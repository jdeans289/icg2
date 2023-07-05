#ifndef _IO_TEMPLATE_ENGINE_HH_
#define _IO_TEMPLATE_ENGINE_HH_

#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <sstream>
#include "../icg-main.hh"

/******************************************/
/*       String manipulation helpers      */
/******************************************/


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

// split by given delimeter
static inline std::vector<std::string> split (const std::string& str, const char delim) {
    std::stringstream ss(str);
    std::string s;
    std::vector<std::string> ret;
    while (std::getline(ss, s, delim)) {
        ret.push_back(s);
    }
    return ret;
}


int get_next_token (const std::string& format, std::string& token) {
    int start_pos = format.find(std::string("{{"), 0);
    if (start_pos == std::string::npos) return -1;
    int end_pos = format.find("}}", 0);

    if (end_pos == std::string::npos || end_pos < start_pos) { throw std::runtime_error("Token is incorrect: " + format); }

    token = format.substr(start_pos+2, end_pos-start_pos-2);

    return start_pos;
}

// Shamefully stolen from https://stackoverflow.com/questions/2896600/how-to-replace-all-occurrences-of-a-character-in-string
std::string ReplaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

std::string replace_token(const std::string& full, const std::string& token_name, const std::string& replace_val) {
    std::string full_token = "{{" + token_name + "}}";
    int pos = full.find(full_token);

    if (pos == std::string::npos) { throw std::runtime_error("Could not find token: " + full_token); }

    // Try to fix indention
    std::string extra_whitespace = "";
    int whitespace_index = pos-1;
    while (whitespace_index >= 0 && (full.at(whitespace_index) == ' ' || full.at(whitespace_index) == '\t')) whitespace_index--;
    extra_whitespace = "\n" + full.substr(whitespace_index+1, pos-whitespace_index-1);
    std::string indented_replace_val = ReplaceAll(replace_val, "\n", extra_whitespace);


    std::string result = full;
    result.replace(pos, full_token.size(), indented_replace_val);

    return result;
}

typedef std::map<std::string, std::string> Dictionary;

std::string big_format(const std::string& fmt_string, const Dictionary& token_dictionary, const std::vector<const recursable *>& recursable_list) {
    std::string result_string = fmt_string;

    std::string token;
    while (get_next_token(result_string, token) != -1) {
        // First, look up the token in the dictionary
        // If it starts with list, strip it off
        bool list_token = false;
        std::string lookup_token = token;
        if (token.rfind("list_", 0) == 0) {
            list_token = true;
            lookup_token = token.substr(5, token.size()-5);
        }

        auto token_lookup_val = token_dictionary.find(lookup_token);
        if (token_lookup_val == token_dictionary.end()) { throw std::runtime_error("Got invalid token " + lookup_token + "\nResult string so far: " + result_string); }
        std::string token_val = token_lookup_val->second;

        // Recurse into the token to populate it
        if (list_token) {
            // We'll have to go through the list of functions and concat the results together
            std::stringstream list_result;
            for (const recursable * next_level_obj : recursable_list) {
                Dictionary next_dictionary = token_dictionary;
                Dictionary obj_dictionary = next_level_obj->toDictionary();
            
                next_dictionary.insert(obj_dictionary.begin(), obj_dictionary.end());
                std::vector<const recursable *> next_recursable = next_level_obj->nextLevel();
                
                list_result << big_format(token_val, next_dictionary, next_recursable);
            }

            token_val = list_result.str();

        } else {
            token_val = big_format(token_val, token_dictionary, recursable_list);
        }

        result_string = replace_token(result_string, token, token_val);
    }

    return result_string;
}



#endif