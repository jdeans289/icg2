#pragma once

#include "NewCheckpointAgentBase.hh"

/**
 Base Class for dumping and restoring a checkpoint.
 */
class J_CheckpointAgent : public CheckpointAgentBase {

    public:

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
    virtual int restore( std::istream& checkpoint_in, std::vector<AllocInfo *> allocations_to_restore) override;


    /* ==================================================================== */
    /*                          CLASS INTERFACE                             */
    /* ==================================================================== */

    bool writeDeclaration( std::ostream& checkpoint_out, const AllocInfo * alloc);
    bool writeVariable( std::ostream& checkpoint_out, const AllocInfo * alloc, const std::vector<AllocInfo *>& additional_search_allocs);

    private:

    /**
     Write a declaration that describes the given ALLOC_INFO object.
    @return whether an error-condition has happened.
    */

    std::string getCheckpointableName(const AllocInfo * alloc);
    std::string resolvePointer(void * ptr_to_resolve, const DataType * expected_type, const std::vector<AllocInfo *>& allocs_to_search);

    static const std::string error_str;
};
