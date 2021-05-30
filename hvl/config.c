#include "include/config.h"

// Allocates and returns a pointer to a HVL VM config
struct hvl_config_t* hvl_create_config()
{
    struct hvl_config_t* config = (struct hvl_config_t*)hvl_alloc(sizeof(hvl_config_t));

    // By default, all exceptions cause VM-exits
    config->exception_bitmap = ~0UL;
    
    // Fill out default exit handlers, giving unsupported ones
    // a null handler which will panic
    config->exit_handlers[VMX_EXIT_REASON_CPUID] = cpuid_handler;

    some other shit
}
