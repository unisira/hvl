#include <wdm.h>
#include <hvl.h>

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    hvl_handle_t vcpu = hvl_create_vcpu()

    return STATUS_SUCCESS;
}
