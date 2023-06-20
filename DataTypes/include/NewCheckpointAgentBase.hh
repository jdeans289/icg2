#ifndef CHECKPOINT_AGENT_BASE_HH
#define CHECKPOINT_AGENT_BASE_HH

#include <stdio.h>
#include <iostream>
#include <vector>

#include "MemoryManagement/AllocInfo.hh"

/**
 Base Class for dumping and restoring a checkpoint.
 */
class CheckpointAgentBase {

    public:

    /**
     */
    virtual ~CheckpointAgentBase() {};

    /**
     Dump Checkpoint
        */
    virtual int dump ( std::ostream& checkpoint_out, std::vector<AllocInfo *> allocations_to_checkpoint) = 0;

    /**
     Restore Checkpoint.
        */
    virtual int restore( std::istream& checkpoint_in, std::vector<AllocInfo *> allocations_to_restore) = 0;
};
#endif
