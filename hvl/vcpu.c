#include "include/vcpu.h"
#include "include/hvl_print.h"

vcpu_t* vcpu_create(hvl_config_t* config)
{
    return NULL;
}

// Toggles a VMX control inside of this VCPUs VMX state
hvl_status_t vcpu_toggle_control(vcpu_t* vcpu, vmx_control_t control)
{
    if (vcpu == NULL)
    {
        hvl_print("vcpu_toggle_control: Null VCPU pointer\n");
        return HVL_INVALID_PARAMETER;
    }

    vmx_toggle_control(&vcpu->vmx, control);

    return HVL_OK;
}
