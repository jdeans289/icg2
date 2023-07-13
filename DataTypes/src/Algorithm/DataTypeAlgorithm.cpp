#include "Algorithm/DataTypeAlgorithm.hpp"

// #include "Algorithm/ContainsPointer.hpp"
// #include "Algorithm/FindLeaves.hpp"
// #include "Algorithm/LookupAddressAndTypeByName.hpp"
// // #include "Algorithm/LookupNameByAddressVisitor.hpp"

namespace DataTypeAlgorithm {

    ContainsPointer::Result containsPointer(const DataType * node) {
        ContainsPointer::ContainsPointerVisitor visitor;
        return visitor.go(node);
    }

    FindLeaves::Result findLeaves(const DataType * node, std::string starting_name, void * starting_address) {
        FindLeaves::FindLeavesVisitor visitor(starting_name, starting_address);
        bool status = visitor.go(node);
        
        if (!status) {
            FindLeaves::Result empty_result;
            return empty_result;
        }
        
        return visitor.getResult();
    }


    LookupAddressAndTypeByName::Result _lookupAddressAndTypeByName (const DataType * node, LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor& visitor) {
        bool status = visitor.go(node);
        if (!status) {
            LookupAddressAndTypeByName::Result empty_result;
            return empty_result;
        }

        return visitor.getResult();
    }

    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (const DataType * node, void * starting_address, std::string full_name) {
        LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor visitor(starting_address, full_name);
        return _lookupAddressAndTypeByName(node, visitor);
    }

    LookupAddressAndTypeByName::Result lookupAddressAndTypeByName (const DataType * node, void * starting_address, MutableVariableName name_elems) {
        LookupAddressAndTypeByName::LookupAddressAndTypeByNameVisitor visitor(starting_address, name_elems);
        return _lookupAddressAndTypeByName(node, visitor);
    }

    LookupNameByAddressAndType::Result lookupNameByAddressAndType (const DataType * node, std::string starting_name, void * starting_address, void * lookup_address, const DataType * const search_type) {
        try {
            LookupNameByAddressAndType::LookupNameByAddressVisitor visitor(starting_name, starting_address, lookup_address, search_type);
            bool status = visitor.go(node);
            if (!status) {
                LookupNameByAddressAndType::Result empty_result;
                return empty_result;
            }

            LookupNameByAddressAndType::Result result;
            result.success = true;
            result.name = visitor.getResult();
            return result;

        } catch (std::exception& e) {
            std::cerr << "LookupNameByAddressVisitor threw an exception: " << e.what() << std::endl;
            LookupNameByAddressAndType::Result empty_result;
            return empty_result;
        }
    }

}