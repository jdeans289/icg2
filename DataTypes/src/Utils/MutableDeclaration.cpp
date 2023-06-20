#include <cstdlib>

#include "Utils/MutableDeclaration.hh"
#include "Utils/DeclarationParser.hh"


// CONSTRUCTOR
MutableDeclaration::MutableDeclaration( std::string s ) : MutableDeclaration(s, std::vector<int>()) {}

MutableDeclaration::MutableDeclaration( std::string base_type, std::vector<int> dimensions) : dims(dimensions) {
    DeclarationParser parser(base_type);
    this->typeSpec = parser.getTypeSpecifier();
    this->varName = parser.getVariableName();

    std::vector<int> other_dims = parser.getDims();

    this->dims.insert(dims.end(), other_dims.begin(), other_dims.end());
}

std::string MutableDeclaration::getTypeSpecifier() const {
    return typeSpec;
}

std::string MutableDeclaration::getVariableName() const {
    return varName;
}

unsigned int MutableDeclaration::getNumberOfDimensions() const {
    return dims.size();
}

int MutableDeclaration::getDimensionSize( unsigned int index) const {
    // FIXME: needs bounds checking.
    return dims[index];
}

std::vector<int> MutableDeclaration::getDims() const {
    return dims;
}

// Reference for this algorithm: https://eli.thegreenplace.net/2008/07/18/reading-c-type-declarations/
std::string MutableDeclaration::getDeclarator(std::string varName) {
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

std::string MutableDeclaration::getAbstractDeclarator() {
    return getDeclarator("");
}

bool MutableDeclaration::is_valid_dimension (int dim) {
    return dim == -1 || dim >= 1;
}

bool MutableDeclaration::validateDimensions() {
    for (int dim : dims) {
        if (!is_valid_dimension(dim)) {
            return false;
        }
    }

    return true;
}

int MutableDeclaration::popDimension() {
    int ret = dims.front();

    // Maybe we should use a deque or something
    dims.erase(dims.begin());

    return ret;
}

void MutableDeclaration::pushDimension(int dim) {

    dims.insert(dims.begin(), dim);

}