
#include <iostream>
#include <sstream>
#include <queue>

#include "DeclarationBuilder.hh"


DeclarationBuilder::DeclarationBuilder( std::string base_type, std::vector<int> dimensions) : typeSpec(base_type), dims(dimensions) {}
DeclarationBuilder::DeclarationBuilder( std::string base_type) : typeSpec(base_type)  {}

// Reference for this algorithm: https://eli.thegreenplace.net/2008/07/18/reading-c-type-declarations/
std::string DeclarationBuilder::getDeclarator(std::string varName) {
    if (!validateDimensions()) {
        std::cerr << "Invalid dimensions. All dimensions must be a positive number or -1 to denote a pointer." << std::endl;
        return "";
    }

    std::stringstream ss;

    // Type goes first
    ss << typeSpec;

    if (varName != "") {
        ss << " ";
    }

    // Start from the varname and go out
    std::string curr_declarator = varName;

    enum Direction {LEFT, RIGHT};
    Direction dir = RIGHT;

    // Then go through the dimensions
    for (int dim : dims) {
        if (dim >= 1) {
            // Constrained dimension

            if (dir == LEFT) {
                // If we have changed directions, add a grouping
                curr_declarator = "(" + curr_declarator + ")";
            }
            curr_declarator = curr_declarator + "[" + std::to_string(dim) + "]";

            // Arrays are always to the right
            dir = RIGHT;
        }

        if (dim == -1) {
            // Pointer
            curr_declarator = "*" + curr_declarator;

            // Pointers are always to the left
            dir = LEFT;
        }

    }

    ss << curr_declarator;
    
    return ss.str();

}

std::string DeclarationBuilder::getAbstractDeclarator() {
    return getDeclarator("");
}

bool DeclarationBuilder::is_valid_dimension (int dim) {
    return dim == -1 || dim >= 1;
}

bool DeclarationBuilder::validateDimensions() {
    for (int dim : dims) {
        if (!is_valid_dimension(dim)) {
            return false;
        }
    }

    return true;
}





