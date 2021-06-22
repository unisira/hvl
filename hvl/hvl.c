#include "include/cpu.h"
#include "include/vm.h"
#include "include/hvl_alloc.h"

// Create important HVL structures
hvl_status_t hvl_init()
{
    cpu_check_vtx_support();

    return HVL_OK;
}

// Create, initialise and launch a VM from a HVL config
hvl_status_t hvl_create_vm(hvl_config_t* config)
{
    hvl_status_t status = HVL_OK;

    if (config == NULL)
        return HVL_INVALID_PARAMETER;

    vm_t* vm = (vm_t*)hvl_alloc(sizeof(vm_t), HVL_MEM_NONPAGEABLE);

    if (vm == NULL)
        return HVL_NO_RESOURCES;
   
    status = vm_prepare_vcpu_resources(vm, config->target_cpus, config->affinity_mask);
    if (HVL_FAILURE(status))
        goto panic;

    status = vm_prepare_target_resources(vm, config->target);
    if (HVL_FAILURE(status))

    status = vm_setup_vmx_operation(vm, config);
    if (HVL_FAILURE(status))
        goto panic;

    return HVL_OK;

panic:
    hvl_free(vm);
    return status; 
}

hvl_status_t hvl_terminate_active_vms()
{
    return HVL_OK;
}
