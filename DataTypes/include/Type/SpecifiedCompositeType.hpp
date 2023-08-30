#pragma once

#include "Type/CompositeDataType.hpp"

// An Allocator
template <typename T>
void* construct_composite (int num) {
    T* temp = (T*)calloc( num, sizeof(T));
    for (int ii=0 ; ii<num ; ii++) { new( &temp[ii]) T(); }
    return ((void *)temp);
}

// A Deallocator
// Object constructed with placement new must call the destructor explicitly (https://isocpp.org/wiki/faq/dtors#placement-new)
template <typename T>
void destruct_composite (void* address) {
    T* temp = static_cast<T*> (address);
    temp->~T();
}

/**
 * @brief This is a class template that can be used to represent a composite type.
 * This version can ONLY be used to represent an empty class, for the ease of testing. 
 * Normally, they are defined through template specialization.
 * 
 * @tparam T class that is represented 
 */
template <typename T>
class SpecifiedCompositeType : public CompositeDataType {

    public:
    SpecifiedCompositeType(std::string name) : CompositeDataType(name, sizeof(T), &construct_composite<T>, &destruct_composite<T>) {}

    MemberMap& getMemberMap () {
        static MemberMap memberMap;
        return memberMap;
    }

    // This is just a const overload of above
    const MemberMap& getMemberMap () const override {
        return (const_cast<SpecifiedCompositeType<T>*> (this))->getMemberMap();
    }

    template<typename Derived>
    static MemberMap applyMembersToDerived () {
        return getMemberMap();
    }
};