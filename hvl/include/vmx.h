#ifndef HVL_VMX_H
#define HVL_VMX_H

#include "hvl_types.h"

// Provides a nice method of representing any VMX control flag as a number
// The format is as follows:
// | VMX control:
// |    field : 3 (0) - The bitfield this control is contained in
// |    pos   : 6 (3) - The position (0-63) of the flag in the field 
// |
#define ENCODE_VMX_CONTROL(field, position) \
    (uint64_t)(((uint64_t)position << 3) | (uint64_t)field)

// MSR definitions for VMX related MSRs
#define IA32_VMX_BASIC 0x480
#define IA32_VMX_PINBASED_CTLS 0x481
#define IA32_VMX_PROCBASED_CTLS 0x482
#define IA32_VMX_PROCBASED_CTLS2 0x48B
#define IA32_VMX_EXIT_CTLS 0x483
#define IA32_VMX_ENTRY_CTLS 0x484
#define IA32_VMX_TRUE_PINBASED_CTLS 0x48D
#define IA32_VMX_TRUE_PROCBASED_CTLS 0x48E
#define IA32_VMX_TRUE_EXIT_CTLS 0x48F
#define IA32_VMX_TRUE_ENTRY_CTLS 0x490

// Type alias for the exit handler function pointers
typedef void(*exit_handler_t)(struct vcpu_t*);

typedef struct
{
    uint32_t pinbased_ctls;
    uint32_t exit_ctls;
    uint32_t entry_ctls;
    uint32_t primary_procbased_ctls; 
    uint32_t secondary_procbased_ctls;
    
    // States of required control fields 
    struct
    {
        uint64_t true_pinbased_ctls;
        uint64_t true_exit_ctls;
        uint64_t true_entry_ctls;
        uint64_t true_procbased_ctls;
        uint64_t true_procbased_ctls2;
    } cap;
} vmx_state_t;

typedef struct
{
    uint32_t vmcs_revision_id;
    uint32_t vmx_abort_indicator;
    uint8_t data[0x1000 - sizeof(uint64_t)];
} vmx_region_t;

typedef union
{
    uint32_t value;

    struct
    {
        uint16_t basic_exit_reason;
        uint32_t reserved : 11;
        uint32_t enclave_mode : 1;
        uint32_t pending_mtf : 1;
        uint32_t vmx_root_operation : 1;
        uint32_t reserved_2 : 1;
        uint32_t vm_entry_failure;
    };
} vmx_exit_reason_t;

typedef enum 
{
    VMX_EXIT_REASON_EXCEPTION_NMI = 0,
    VMX_EXIT_REASON_EXT_INTERRUPT = 1,
    VMX_EXIT_REASON_TRIPLE_FAULT = 2,
    VMX_EXIT_REASON_INIT_SIGNAL = 3,
    VMX_EXIT_REASON_STARTUP_SIPI = 4,
    VMX_EXIT_REASON_IO_SMI = 5,
    VMX_EXIT_REASON_OTHER_SMI = 6,
    VMX_EXIT_REASON_INTERRUPT_WINDOW = 7,
    VMX_EXIT_REASON_NMI_WINDOW = 8, 
    VMX_EXIT_REASON_MAX
} vmx_basic_exit_reason_t;

typedef enum
{
    VMX_PINBASED_CTLS,
    VMX_PRIM_PROCBASED_CTLS,
    VMX_SEC_PROCBASED_CTLS,
    VMX_EXIT_CTLS,
    VMX_ENTRY_CTLS
} vmx_control_field_t;

typedef enum
{
    VMX_CTL_EXT_INTERRUPT_EXITING       = ENCODE_VMX_CONTROL(VMX_PINBASED_CTLS, 0),
    VMX_CTL_NMI_EXITING                 = ENCODE_VMX_CONTROL(VMX_PINBASED_CTLS, 3),
    VMX_CTL_VIRTUAL_NMIS                = ENCODE_VMX_CONTROL(VMX_PINBASED_CTLS, 5),
    VMX_CTL_VMX_PREEMPTION_TIMER        = ENCODE_VMX_CONTROL(VMX_PINBASED_CTLS, 6),
    VMX_CTL_PROCESS_POSTED_INTERRUPTS   = ENCODE_VMX_CONTROL(VMX_PINBASED_CTLS, 7),
} vmx_control_t;

typedef enum
{
    CONTROL_VIRTUAL_PROCESSOR_ID = 0x00000000,
    CONTROL_POSTED_INERRUPT_NOTIF = 0x00000002,
    CONTROL_PINBASED_CONTROLS = 0x00004000,
    CONTROL_PRIMARY_PROCBASED_CONTROLS = 0x00004002,
    CONTROL_VMEXIT_CONTROLS = 0x0000400C,
    CONTROL_VMENTRY_CONTROLS = 0x00004012,
    CONTROL_SECONDARY_PROCBASED_CONTROLS = 0x0000401E
} vmcs_t;

void vmx_toggle_control(vmx_state_t*, vmx_control_t);
void vmx_setup_default_state(vmx_state_t*);
void vmx_commit_state(vmx_state_t*);

#endif
