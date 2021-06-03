#include <wdm.h>
#include <hvl.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    if (HVL_FAILURE(hvl_init()))
	    return STATUS_NOT_SUPPORTED;

    hvl_config_t* config = hvl_create_config();

    hvl_toggle_vmx_control(config, VMX_CTL_VMX_PREEMPTION_TIMER);
    hvl_toggle_vmx_control(config, VMX_CTL_EXT_INTERRUPT_EXITING);

    if (HVL_FAILURE(hvl_create_vm(config)))
        return STATUS_FATAL_APP_EXIT;

    return STATUS_SUCCESS;
}
