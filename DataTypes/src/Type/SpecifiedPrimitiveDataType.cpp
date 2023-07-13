#include <ctype.h>
#include <iostream>

#include "Type/SpecifiedPrimitiveDataType.hpp"


/*
 Template specializations for SpecifiedPrimitiveDataType<T>::getTypeSpecName()
 */
template <> std::string SpecifiedPrimitiveDataType<void>::getTypeSpecName() const {
    std::string s("void"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<char>::getTypeSpecName() const {
    std::string s("char"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<short>::getTypeSpecName() const {
    std::string s("short"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<int>::getTypeSpecName() const {
    std::string s("int"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<long>::getTypeSpecName() const {
    std::string s("long"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<wchar_t>::getTypeSpecName() const {
    std::string s("wchar_t"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<long long>::getTypeSpecName() const {
    std::string s("long long"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<unsigned char>::getTypeSpecName() const {
    std::string s("unsigned char"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<unsigned short>::getTypeSpecName() const {
    std::string s("unsigned short"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<unsigned int>::getTypeSpecName() const {
    std::string s("unsigned int"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<unsigned long>::getTypeSpecName() const {
    std::string s("unsigned long"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<unsigned long long>::getTypeSpecName() const {
    std::string s("unsigned long long"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<float>::getTypeSpecName() const {
    std::string s("float"); return s;
}
template <> std::string SpecifiedPrimitiveDataType<double>::getTypeSpecName() const {
    std::string s("double"); return s;
}

/*
 Template specializations for SpecifiedPrimitiveDataType<T>::isFloatingPoint()
 */
template <> bool SpecifiedPrimitiveDataType<float>::isFloatingPoint() const { return true; }
template <> bool SpecifiedPrimitiveDataType<double>::isFloatingPoint() const { return true; }

/*
 Template specializations for SpecifiedPrimitiveDataType<T>::isSigned()
 */
template <> bool SpecifiedPrimitiveDataType<char>::isSigned() const { return true; }
template <> bool SpecifiedPrimitiveDataType<short>::isSigned() const { return true; }
template <> bool SpecifiedPrimitiveDataType<int>::isSigned() const { return true; }
template <> bool SpecifiedPrimitiveDataType<long>::isSigned() const { return true; }
template <> bool SpecifiedPrimitiveDataType<long long>::isSigned() const { return true; }

/*
 Template specializations for SpecifiedPrimitiveDataType<T>::isVoid()
 */
template <> bool SpecifiedPrimitiveDataType<void>::isVoid() const { return true; }

/*
 Template specializations for SpecifiedPrimitiveDataType<char>::printValue()
*/
template <> void SpecifiedPrimitiveDataType<char>::printValue(std::ostream &s, void *address ) const {
    if (isprint( (int)*(char*)address) ) {
        s << "'" << *(char*)address << "'";
    } else {
        s << (int)*(char*)address;
    }
}

template <> void SpecifiedPrimitiveDataType<unsigned char>::printValue(std::ostream &s, void *address ) const {
    if (isprint( (int)*(unsigned char*)address) ) {
        s << "'" << *(unsigned char*)address << "'";
    } else {
        s << (int)*(unsigned char*)address;
    }
}

/*
 Template specializations for SpecifiedPrimitiveDataType<void>
 */
template <> size_t SpecifiedPrimitiveDataType<void>::getSize() const {
    std::cerr << "WARNING: Calling SpecifiedPrimitiveDataType<void>::getSize()"
              << ", i.e., attempting to get the size of void, indicates that"
              << " you may be up to no good and about to run out of luck." << std::endl;
    return 0;
}
template <> void SpecifiedPrimitiveDataType<void>::deleteInstance(void* address) const {
    std::cerr << "WARNING: Cannot delete the object, of type <void> at address ("
              << address << ")." << std::endl;
}
template <> void* SpecifiedPrimitiveDataType<void>::createInstance(unsigned int num) const {
    std::cerr << "WARNING: Cannot create an object, of type <void>." << std::endl;
    return NULL;
}
template <> void SpecifiedPrimitiveDataType<void>:: clearValue(void * address) const {
    std::cerr << "WARNING: Cannot clear the value of the object, of type <void> at address ("
              << address << ")." << std::endl;
}
template <> void SpecifiedPrimitiveDataType<void>:: assignValue(void * address, Value * value) const {
    std::cerr << "WARNING: Cannot assign a value to the object, of type <void> at address ("
              << address << ")." << std::endl;
}
template <> void SpecifiedPrimitiveDataType<void>::printValue(std::ostream &s, void *address ) const {
    std::cerr << "WARNING: Cannot print the value of the object, of type <void> at address (" << address
              << " of type void." << std::endl;
}

template <> Value * SpecifiedPrimitiveDataType<void>::getValue(void *address ) const {
    std::cerr << "WARNING: Cannot print the get the value of the object, of type <void> at address (" << address
              << " of type void." << std::endl;

    return NULL;
}
