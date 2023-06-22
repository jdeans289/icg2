#pragma once

#include "NewCheckpointAgentBase.hh"
#include "DataTypeInator.hh"

/**
 Base Class for dumping and restoring a checkpoint.
 */
class J_CheckpointAgent : public CheckpointAgentBase {

    public:

    J_CheckpointAgent(const DataTypeInator * inator);

    /**
     */
    virtual ~J_CheckpointAgent() {};


    /* ==================================================================== */
    /*                          VIRTUAL INTERFACE                           */
    /* ==================================================================== */

    /**
     * 
     Dump Checkpoint
        */
    virtual int dump ( std::ostream& checkpoint_out, std::vector<AllocInfo *> allocations_to_checkpoint) override;

    /**
     Restore Checkpoint.
        */
    virtual std::vector<AllocInfo *> restore( std::istream& checkpoint_in, std::vector<AllocInfo *> allocations_to_restore) override;


    /* ==================================================================== */
    /*                          CLASS INTERFACE                             */
    /* ==================================================================== */

    bool writeDeclaration( std::ostream& checkpoint_out, const AllocInfo * alloc);
    bool writeAssignment( std::ostream& checkpoint_out, const AllocInfo * alloc, const std::vector<AllocInfo *>& additional_search_allocs);

    AllocInfo * restoreDeclaration(std::string declaration);
    bool restoreAssignment(std::string assignment_string, const std::vector<AllocInfo *>& additional_search_allocs);


    private:

    const DataTypeInator * dataTypeInator;

    /**
     Write a declaration that describes the given ALLOC_INFO object.
    @return whether an error-condition has happened.
    */

    static std::string getCheckpointableName(const AllocInfo * alloc);
    static std::string resolvePointer(void * ptr_to_resolve, const DataType * expected_type, const std::vector<AllocInfo *>& allocs_to_search);
    static AllocInfo * findAllocByName(std::string name, std::vector<AllocInfo *> search_allocs);
    static void *  lookupPointer(std::string varname, const std::vector<AllocInfo *>& allocs_to_search);


    static const std::string error_str;
};
