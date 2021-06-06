#include "include/vmx.h"
#include "include/cpu.h"
#include "include/vcpu.h"

typedef union 
{
    uint64_t value;

    struct
    {
        uint32_t on_bits;
        uint32_t off_bits;
    };
} vmx_capability_msr_t;

static inline uint64_t cap_get_fixed_bits_mask(const uint64_t value);

// Defined in vmx.asm
static uint64_t asm_vmwrite(uint64_t encoding, uint64_t value);
static uint64_t asm_vmread(uint64_t encoding, uint64_t* value);

// Lookup table for the `field` ID in the encoded value
static const uint64_t s_vmx_control_field_lookup[5] = {
    CONTROL_PINBASED_CONTROLS,
    CONTROL_VMEXIT_CONTROLS,
    CONTROL_VMENTRY_CONTROLS,
    CONTROL_PRIMARY_PROCBASED_CONTROLS,
    CONTROL_SECONDARY_PROCBASED_CONTROLS
};

// Enable/disable a VM-execution control in a VMX state using an encoded VM control
void vmx_toggle_control(vmx_state_t* state, vmx_control_t control)
{
    const uint8_t control_field = (uint8_t)control & 0b111;

    // Hacky lookup of the target control field (could use a switch instead?) 
    uint64_t* target_controls = &((uint64_t*)state)[control_field];
    uint64_t target_capability = ((uint64_t*)&state->cap)[control_field];

    const uint8_t control_bit = (uint8_t)((control & 0b111111000) >> 3);

    // Check if the target control is allowed to be modified
    if (cap_get_fixed_bits_mask(target_capability) & control_bit)
        return;

    // Flip the specified control bit
    *target_controls |= !(*target_controls & control_bit);
}

// Fill out a VMX state with default controls and values
void vmx_setup_default_state(vmx_state_t* state)
{
    const vmx_basic_msr_t vmx_cap = {asm_rdmsr(IA32_VMX_BASIC)};

    // Get the VMX controls' capability MSRs and work out the default controls
    const vmx_capability_msr_t true_pinbased_ctls = (vmx_capability_msr_t){
        asm_rdmsr(vmx_cap.true_controls ? IA32_VMX_TRUE_PINBASED_CTLS : IA32_VMX_PINBASED_CTLS)
    };

    const vmx_capability_msr_t true_exit_ctls = (vmx_capability_msr_t){
        asm_rdmsr(vmx_cap.true_controls ? IA32_VMX_TRUE_EXIT_CTLS : IA32_VMX_EXIT_CTLS)
    };

    const vmx_capability_msr_t true_entry_ctls = (vmx_capability_msr_t){
        asm_rdmsr(vmx_cap.true_controls ? IA32_VMX_TRUE_ENTRY_CTLS : IA32_VMX_ENTRY_CTLS)
    };

    const vmx_capability_msr_t true_primary_procbased_ctls = (vmx_capability_msr_t){
        asm_rdmsr(vmx_cap.true_controls ? IA32_VMX_TRUE_PROCBASED_CTLS : IA32_VMX_PROCBASED_CTLS)
    };

    const vmx_capability_msr_t true_secondary_procbased_ctls = (vmx_capability_msr_t){
        asm_rdmsr(IA32_VMX_PROCBASED_CTLS2)
    };

    // Update the state with the new bitmasks
    *state = (vmx_state_t){
        .pinbased_ctls = true_pinbased_ctls.on_bits,
        .exit_ctls = true_exit_ctls.on_bits,
        .entry_ctls = true_entry_ctls.on_bits,
        .primary_procbased_ctls = true_primary_procbased_ctls.on_bits,
        .secondary_procbased_ctls = true_secondary_procbased_ctls.on_bits,
        .cap.true_pinbased_ctls = true_pinbased_ctls.value,
        .cap.true_exit_ctls = true_exit_ctls.value,
        .cap.true_entry_ctls = true_entry_ctls.value,
        .cap.true_procbased_ctls = true_primary_procbased_ctls.value,
        .cap.true_procbased_ctls2 = true_secondary_procbased_ctls.value
    };
}

void vmx_vmwrite(vmcs_t component, uint64_t value)
{
    // TODO: Check flags
    asm_vmwrite((uint64_t)component, value);
}

uint64_t vmx_vmread(vmcs_t component)
{
    uint64_t result = 0;
    asm_vmread((uint64_t)component, &result);
    return result;
}

// Update the actual VMX state in the VMCS using a `vmx_state_t*`
void vmx_commit_state(const vmx_state_t* state)
{
    // NOTE: We should check if we are actually in VMX operation
    vmx_vmwrite(CONTROL_PINBASED_CONTROLS, state->pinbased_ctls); 
    vmx_vmwrite(CONTROL_PRIMARY_PROCBASED_CONTROLS, state->primary_procbased_ctls); 
    vmx_vmwrite(CONTROL_SECONDARY_PROCBASED_CONTROLS, state->secondary_procbased_ctls); 
    vmx_vmwrite(CONTROL_VMEXIT_CONTROLS, state->exit_ctls); 
    vmx_vmwrite(CONTROL_VMENTRY_CONTROLS, state->entry_ctls); 
}

// Gets the flexible bits mask from a capability MSR
static inline uint64_t cap_get_fixed_bits_mask(const uint64_t value)
{
    const vmx_capability_msr_t cap = {value}; 

    // ~(ON ^ OFF) returns all bits that are different, inverted meaning that all the 
    // 1 bits are actually the bits that cannot be changed
    return ~(cap.on_bits ^ cap.off_bits); 
}
