cmake_minimum_required(VERSION 3.1)

set(DATATYPES_SRC
    Value/CompositeValue.cpp
    Value/IntegerValue.cpp
    Value/FloatingPointValue.cpp
    Value/PointerValue.cpp
    Value/StringValue.cpp
    Value/ArrayValue.cpp
    Type/StructMember.cpp
    Type/TypedStructMember.cpp
    Type/NormalStructMember.cpp
    Type/StaticStructMember.cpp
    Type/BitfieldStructMember.cpp
    Type/CompositeDataType.cpp
    Type/EnumDataType.cpp
    Type/ArrayDataType.cpp
    Type/PointerDataType.cpp
    Type/PrimitiveDataType.cpp
    Type/StringDataType.cpp
    Type/DataType.cpp
    Type/Enumerator.cpp
    Type/EnumDictionary.cpp
    MemoryManagement/AllocInfo.cpp
    MemoryManagement/MemoryManager.cpp
    Utils/LexicalAnalyzer.cpp
    Utils/ParsedDeclaration.cpp
    Utils/MutableDeclaration.cpp
    Utils/ParsedAssignment.cpp
    Algorithm/DataTypeVisitor.cpp
    Algorithm/CheckpointVisitor.cpp
    Algorithm/LookupNameByAddressVisitor.cpp
    Algorithm/LookupAddressByNameVisitor.cpp
    CheckpointAgent/J_CheckpointAgent.cpp
    TypeDictionary.cpp
    DataTypeInator.cpp
)

list(TRANSFORM DATATYPES_SRC PREPEND src/)

add_library(decl STATIC ${DATATYPES_SRC})

target_include_directories(decl PUBLIC include)

enable_testing()

add_subdirectory(test)

