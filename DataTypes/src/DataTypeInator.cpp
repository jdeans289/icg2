#include "DataTypeInator.hh"
#include "Type/ArrayDataType.hh"
#include "Type/PointerDataType.hh"
#include "Utils/MutableDeclaration.hh"

DataTypeInator::DataTypeInator () : typeDictionary(new TypeDictionary) {
    // Probably shouldn't do this.
    typeDictionary->addBuiltinTypes();
}

DataTypeInator::DataTypeInator (TypeDictionary * dict) : typeDictionary(dict) {
}

const DataType * DataTypeInator::resolve(std::string name) const {

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
            result = new PointerDataType(this, next_level_decl);            
        } else {
            // Constrained dim case
            result = new ArrayDataType(this, next_level_decl, this_dim);
        }

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


