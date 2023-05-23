#pragma once

#include <string>
#include <iostream>
#include <vector>


class DeclarationBuilder {

    public:

    DeclarationBuilder( std::string base_type, std::vector<int> dimensions);
    DeclarationBuilder( std::string base_type);

    std::string getDeclarator(std::string varName);
    std::string getAbstractDeclarator();

    int popDimension();
    void pushDimension(int dim);

    private:

    bool validateDimensions();
    static bool is_valid_dimension (int dim);

    std::string typeSpec;
    std::vector<int> dims;
};
