#include <iostream>

#include "StructMember.hh"

StructMember::StructMember(std::string name) : name(name){}

// COPY CONSTRUCTOR
StructMember::StructMember ( const StructMember & original) {
    this->name = original.name;
}

std::string StructMember::getName() const {
    return name;
}

