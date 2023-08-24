#include <deque>
#include <string>
#include <sstream>

class ScopeInfo {

    public:
    
    void push_qualifier(std::string name) {
        qualified_name_parts.push_back(name);
    }

    std::string pop_qualifier() {
        std::string result = qualified_name_parts.back();
        qualified_name_parts.pop_back();
        return result;
    }

    std::string make_scoped_name(std::string name) {
        std::stringstream ss;
        for (auto part : qualified_name_parts) {
            ss << part << "::";
        }
        ss << name;
        return ss.str();
    }

    private:
    std::deque<std::string> qualified_name_parts;
};