
#pragma once

#include "DataType.hh"
#include "TypeDictionary.hh"

class DataTypeInator {
    public:
        DataTypeInator ();

        DataTypeInator (TypeDictionary * dict);

        const DataType * resolve(std::string name);
        void addToDictionary(std::string name, BaseType * typeSpec);

        bool validateDictionary();


    private:
        TypeDictionary * typeDictionary;
};