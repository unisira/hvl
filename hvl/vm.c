#include "include/vm.h"
#include "include/hvl_print.h"
#include "include/hvl_alloc.h"
#include "include/hvl_smp.h"

// Structure for field passed to setup_vmx_region_ipi
typedef struct
{
    vm_t* vm; 
    hvl_config_t* config;
    hvl_status_t status;
} vcpu_vmx_setup_t;

static uint8_t am_count_online_cpus(uint64_t mask);
static uint8_t am_core_index_to_id(uint64_t mask, uint8_t idx);
static void setup_vmx_op_ipi(vcpu_vmx_setup_t* params);

hvl_status_t vm_prepare_vcpu_resources(vm_t* vm, target_cpus_t target, uint8_t affinity_mask)
{
    hvl_status_t status = HVL_OK;

    switch (target)
    {
    case HVL_TARGET_ALL_PROCESSORS:
        {
            vm->ncpu = hvl_core_count();
        }
    case HVL_TARGET_CURRENT_PROCESSOR:
        {
            vm->ncpu = 1;
        }
    case HVL_USE_POSITIONAL_TARGETING:
        {
            vm->vcpu_affinity = affinity_mask;
            vm->ncpu = am_count_online_cpus(affinity_mask);
        }
    default: 
        {
            hvl_print("vm_prepare_vcpu_resources: Unknown target\n");
            return HVL_INVALID_PARAMETER;
        }
    }
    
    vm->vcpus = (vcpu_t*)hvl_alloc(sizeof(vcpu_t) * vm->ncpu, HVL_MEM_NONPAGEABLE);
    if (vm->vcpus == NULL)
    {
        hvl_print("vm_prepare_vcpu_resources: Couldn't allocate VCPU list\n");
        return HVL_NO_RESOURCES;
    }

    for (uint8_t i = 0; i < vm->ncpu; i++)
    {
        vcpu_t* vcpu = &vm->vcpus[i]; 

        status = vcpu_setup(vcpu, am_core_index_to_id(affinity_mask, i + 1));
        if (HVL_FAILURE(status))
        {
            hvl_print("vm_prepare_vcpu_resources: Failed to setup VCPU (%x)\n", status);
            goto panic;
        }
    }
    
    return HVL_OK;

panic:
    hvl_free(vm->vcpus);
    return status;
}

// Prepares resources for the virtualisation target
hvl_status_t vm_prepare_target_resources(vm_t* vm, virt_target_t target)
{
    return HVL_OK;
}

// Delegates work to each CPU which needs to be virtualised, setting up VMX regions
// and starting VMX operation
hvl_status_t vm_setup_vmx_operation(vm_t* vm, hvl_config_t* config)
{
    hvl_status_t status = HVL_OK;
    if (config->target_cpus == HVL_TARGET_CURRENT_PROCESSOR)
    {
        status = vcpu_setup_vmx_operation(&vm->vcpus[0], &config->vmx);
        if (HVL_FAILURE(status))
        {
            hvl_print("Failed to virtualise current processor (%x)\n", status);
            goto panic;
        }
        
        return status;
    }

    const volatile vcpu_vmx_setup_t params = (vcpu_vmx_setup_t){
        .vm = vm,
        .config = config,
        .status = HVL_OK
    };

    if (vm->vcpu_affinity)
        hvl_ipi_call_targeted(vm->vcpu_affinity, setup_vmx_op_ipi, &params);
    else
        hvl_ipi_call_generic(setup_vmx_op_ipi, &params);

    status = params.status;
    if (HVL_FAILURE(status))
        goto panic;

    return HVL_OK;

panic:
    for (uint8_t i = 0; i < vm->ncpu; i++)
        vcpu_destroy(&vm->vcpus[i]);

    hvl_free(vm->vcpus);
    return status;
}

// Counts the amount of online CPUs from an affinity mask
static uint8_t am_count_online_cpus(uint64_t mask)
{
    uint8_t idx = 0, res = 0;
    while (idx != sizeof(uint64_t) * 8)
        res += ((mask >> idx++) & 1);
    return res;
}

// Converts a core index in a VCPU list to a core ID
static uint8_t am_core_index_to_id(uint64_t mask, uint8_t index)
{
    uint8_t count = 0;
    while (
            // Remove one from count if we found an online CPU
            index -= ((mask >> count) & 1),
            // Check if we have passed enough VCPUs yet, if not advance 
            index != 0
          )
        count++;

    return count;
}

// Proxy function which unpacks the parameters and calls vcpu_setup_vmx_regions 
static void setup_vmx_op_ipi(vcpu_vmx_setup_t* params)
{
    uint8_t core = hvl_core_id();
    vcpu_t* vcpu = &params->vm->vcpus[core];

    params->status = vcpu_setup_vmx_operation(vcpu, &params->config->vmx);
    if (HVL_FAILURE(params->status))
        hvl_print("setup_vmx_op_ipi: vcpu_setup_vmx_operation failed on VCPU #%i\n", core);
}
