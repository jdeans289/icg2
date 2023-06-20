#include "J_CheckpointAgent.hh"

#include "Type/DataType.hh"
#include "Value/PointerValue.hh"
#include "Algorithm/CheckpointVisitor.hh"
#include "Algorithm/LookupAddressVisitor.hh"

const std::string J_CheckpointAgent::error_str = "<UNDEFINED>";

/* ==================================================================== */
/*                          DUMP CHECKPOINT                             */
/* ==================================================================== */

int J_CheckpointAgent::dump ( std::ostream& checkpoint_out, std::vector<AllocInfo *> allocations_to_checkpoint) {
    
    // First go through and check that the allocations are well formed
    auto it = allocations_to_checkpoint.begin();
    while(it != allocations_to_checkpoint.end()) {
        if((*it)->getDataType() == NULL) {
            std::cerr << "Alloc info named " << getCheckpointableName(*it) << " does not have a data type associated with it. Cannot checkpoint." << std::endl;
            it = allocations_to_checkpoint.erase(it);
        }
        else {
            ++it;
        }
    }

    // Write out declarations
    checkpoint_out << "// Variable Declarations." << std::endl;
    checkpoint_out.flush();

    for (AllocInfo * allocInfo : allocations_to_checkpoint) {
        StorageClass::e storageClass = allocInfo->getStorageClass();
        // Only write the declaration if it's a local alloc
        if ( storageClass == StorageClass::LOCAL) {
            writeDeclaration(checkpoint_out, allocInfo);
        }
    }

    // 3) Write a "clear_all_vars" command.
    checkpoint_out << std::endl << std::endl << "// Clear all allocations to 0." << std::endl;
    checkpoint_out << "clear_all_vars();" << std::endl;

    // 4) Dump the contents of each of the local and extern allocations.
    checkpoint_out << std::endl << std::endl << "// Variable Assignments." << std::endl;
    checkpoint_out.flush();

    for (AllocInfo * allocInfo : allocations_to_checkpoint) {
        writeVariable(checkpoint_out, allocInfo, allocations_to_checkpoint);
    }

    return 0;
}

bool J_CheckpointAgent::writeDeclaration( std::ostream& checkpoint_out, const AllocInfo * allocInfo) {
    std::string name = getCheckpointableName(allocInfo);

    // Write the declaration
    checkpoint_out << allocInfo->getDataType()->makeDeclaration(name) << " ;\n";

    return true;
}

bool J_CheckpointAgent::writeVariable( std::ostream& checkpoint_out, const AllocInfo * alloc_to_write, const std::vector<AllocInfo *>& additional_search_allocs) {
    std::string name = getCheckpointableName(alloc_to_write);

    CheckpointVisitor visitor (name, alloc_to_write->getStart());
    bool status = visitor.go(alloc_to_write->getDataType());
    if (!status) {
        std::cerr << "Something when wrong while trying to find the leaves of AllocInfo named " << name << " of type " << alloc_to_write->getDataType()->toString() << ". Skipping." << std::endl;
        return false;
    }
    
    // This is all of the leaf values in this alloc info
    // Write out an assignment for each
    for (auto leaf : visitor.getResults()) {
        std::string value;
        if (leaf.is_pointer) {
            // If it's a pointer, we have to resolve the value to the name of another variable in the set of allocations.
            // Do a sketch cast but it's ok because we know that Value is definitely a PointerValue
            void * ptr_value = ((PointerValue *) leaf.value )->getPointer();
            std::string ptr_name = resolvePointer(ptr_value, leaf.pointer_subtype, additional_search_allocs);
            if (ptr_name == error_str) {
                std::cerr << "Warning: Pointer " << leaf.name_stack.toString() << " was not found in managed memory. Skipping." << std::endl;
                continue;
            }

            value = "&" + ptr_name;

        } else {
            value = leaf.value->toString();
        }
        checkpoint_out << leaf.name_stack.toString() << " = " << value << " ;" << std::endl;
    }

    return true;
}


std::string J_CheckpointAgent::resolvePointer(void * ptr_to_resolve, const DataType * expected_type, const std::vector<AllocInfo *>& allocs_to_search) {
    for (auto allocInfo : allocs_to_search) {
        if (allocInfo->contains(ptr_to_resolve)) {
            LookupAddressVisitor visitor (getCheckpointableName(allocInfo), allocInfo->getStart(), ptr_to_resolve, expected_type);
            bool status = visitor.go(allocInfo->getDataType());

            // If something went wrong, just continue.
            // Although we should probably be able to just return an error
            // assuming that the AllocInfos never overlap, which ~should~ be true
            if (!status) continue;

            return visitor.getResult().toString();
        }
    }

    // Oh no we didn't find it :(
    return error_str;
}

std::string J_CheckpointAgent::getCheckpointableName(const AllocInfo * allocInfo) {
    std::string name = allocInfo->getName();

    if ( name.empty() ) {
        name = allocInfo->getSerialName();
    }

    return name;
}

/* ==================================================================== */
/*                          RESTORE CHECKPOINT                          */
/* ==================================================================== */

int J_CheckpointAgent::restore( std::istream& checkpoint_in, std::vector<AllocInfo *> allocations_to_restore) {
    return 0;
}