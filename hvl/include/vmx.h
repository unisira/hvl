#ifndef HVL_VMX_H
#define HVL_VMX_H

// Type alias for the exit handler function pointers
typedef void(*exit_handler_t)(struct vcpu_t*);

enum vmx_exit_reason_t
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
};

#endif
