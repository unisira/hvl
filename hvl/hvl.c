#include "include/hvl.h"

vm_t* hvl_create_vm(hvl_config_t* config)
{
    vm_t* vm = (vm_t*)hvl_alloc(sizeof(vm_t), HVL_MEM_NONPAGEABLE);

    if (!vm)
        return NULL;
    return 0;
}
