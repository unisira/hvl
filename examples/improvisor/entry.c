#include <wdm.h>
#include <hvl.h>


VOID
DriverUnload(
    IN PDRIVER_OBJECT DriverObject
)
{
    return;
}

NTSTATUS 
DriverEntry(
    IN PDRIVER_OBJECT DriverObject, 
    IN PUNICODE_STRING RegistryPath
)
{
    UNREFERENCED_PARAMETER(RegistryPath);

    DriverObject->DriverUnload = DriverUnload;

    if (HVL_FAILURE(hvl_init()))
	    return STATUS_NOT_SUPPORTED;

    hvl_config_t* config = hvl_create_config();

    hvl_toggle_vmx_control(config, VMX_CTL_VMX_PREEMPTION_TIMER);
    hvl_toggle_vmx_control(config, VMX_CTL_EXT_INTERRUPT_EXITING);

    if (HVL_FAILURE(hvl_create_vm(config)))
        return STATUS_FATAL_APP_EXIT;

    return STATUS_SUCCESS;
}
