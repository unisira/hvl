#ifndef HVL_CONFIG_H
#define HVL_CONFIG_H

#include "hvl_types.h" 
#include "vmx.h"

typedef enum 
{
    // Virtualise all present processors, useful for virtualising a whole system
    HVL_TARGET_ALL_PROCESSORS,
    
    // Only virtualise the current processor, useful for quick native code emulation 
    // or micro VMs on specific threads
    HVL_TARGET_CURRENT_PROCESSOR,

    // UNIMPLEMENTED: Use the positional targeting fields in the configuation
    HVL_USE_POSITIONAL_TARGETING
} target_processors_t;

// The structure describing the features and architecture of a VM (and subsequent VCPU's)
typedef struct 
{
    // Controls the processors that will be targeted (see enum target_processors_t)
    uint32_t target_processors;

    // The exception bitmap used to determine which exceptions will cause a VM-exit 
    uint32_t exception_bitmap;

    // Array of exit handlers which can be overriden to allow for custom functionality,
    // the base exit handlers do the bare minimum to allow for stable execution
    exit_handler_t exit_handlers[VMX_EXIT_REASON_MAX];
} hvl_config_t;

#endif
