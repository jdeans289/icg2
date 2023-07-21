#pragma once

#include "Type/DataType.hpp"

/**
 * @brief Abstract class to represent a class that can go in the TypeDictionary (aka not an array or pointer)
 * 
 */
class BaseType : public DataType {};