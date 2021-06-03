#ifndef HVL_VCPU_H
#define HVL_VCPU_H

#include "hvl_types.h"
#include "config.h"
#include "vmx.h"

typedef struct
{
    // Debug Registers
    uint64_t dr0; 

    // General purpose registers
    uint64_t rax;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rbx;
    uint64_t rsp;
    uint64_t rbp;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} vcpu_state_t;

typedef struct
{
    uint8_t id;

    // The most recent state of registers from this VCPU
    vcpu_state_t state;

    // The current state of the VMX controls
    vmx_state_t vmx;
} vcpu_t;

hvl_status_t vcpu_toggle_control(vcpu_t*, vmx_control_t);

#endif
