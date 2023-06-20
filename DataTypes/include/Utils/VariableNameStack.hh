#pragma once

#include <vector>
#include <string>
#include <sstream>

class VariableNameStack {
    public:
        VariableNameStack() {}

        void pushName(std::string name) {
            nameStack.push_back(name);
        }
        void pushIndex(int i) {
            nameStack.push_back(std::string("[" + std::to_string(i) + "]"));
        }

        void pop() {
            nameStack.pop_back();
        }

        std::vector<std::string> getStack() const {
            return nameStack;
        }

        std::string toString() const {
            if (nameStack.empty()) {
                return "";
            }

            std::stringstream ss;

            ss << nameStack[0];
            for (unsigned int i = 1; i < nameStack.size(); i++) {
                if (nameStack[i].at(0) != '[') {
                    ss << ".";
                } 

                ss << nameStack[i];
            }

            return ss.str();
        }

    private:
        std::vector<std::string> nameStack;
};