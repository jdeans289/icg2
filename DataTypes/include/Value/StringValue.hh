#ifndef STRING_VALUE_H
#define STRING_VALUE_H

#include "Value/Value.hh"

/**
 */
class StringValue : public Value {

    public:

    /**
     Constructor.
     @param value The value of this StringValue.
     */
    StringValue(const char* value);
    StringValue(std::string value);


    /**
     Print this StringValue to a stream.
     */
    void print(std::ostream &s);

    /**
     Get a string representation of this StringValue.
     */
    std::string toString();
    std::string getRawString();
    std::string getEscapedString();

    private:

    std::string value;
};

#endif
