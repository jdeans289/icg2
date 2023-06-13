#include "DataTypeInator.hh"
#include "Type/ArrayDataType.hh"
#include "Type/PointerDataType.hh"
#include "Utils/MutableDeclaration.hh"

DataTypeInator::DataTypeInator () : DataTypeInator(new TypeDictionary){}
DataTypeInator::DataTypeInator (TypeDictionary * dict) : typeDictionary(dict) {}

const DataType * DataTypeInator::resolve(std::string name) {

    MutableDeclaration decl(name);

    if (decl.getNumberOfDimensions() == 0) {
        // This is technically a base case
        return typeDictionary->lookup(decl.getTypeSpecifier());

    } else {
        // Pop off the last dimension, make an array/pointer, let the nested recursive structure do the rest

        // Pop first elem
        int this_dim = decl.popDimension();

        // Make a new declaration string for the subtype
        std::string next_level_decl = decl.getDeclarator(decl.getVariableName());

        DataType * result = NULL;

        if (this_dim == -1) {
            // Pointer case
            // std::cout << "Creating nested pointer type with subtype " << next_level_decl << std::endl;
            result = new PointerDataType(this, next_level_decl);            
        } else {
            // Constrained dim case
            // std::cout << "Creating nested array type with subtype " << next_level_decl << " and dimension " << this_dim << std::endl;
            result = new ArrayDataType(this, next_level_decl, this_dim);
        }

        // Should probably validate or something -jackie
        if (!result->validate()) {
            std::cerr << "Validate failed for " << result->toString() << std::endl;
        }
        return result;

    }
}

void DataTypeInator::addToDictionary(std::string name, BaseType * typeSpec) {
    typeDictionary->addTypeDefinition(name, typeSpec);
}

bool DataTypeInator::validateDictionary() {
    return typeDictionary->validate();
}


