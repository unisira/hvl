#include "include/vmx.h"
#include "include/cpu.h"

typedef union 
{
    uint64_t value;

    struct
    {
        uint32_t on_bits;
        uint32_t off_bits;
    };
} vmx_capability_msr_t;

typedef union
{
    uint64_t value;

    struct 
    {
        uint64_t vmcs_revision_id : 31;
        uint64_t reserved1 : 1;
        uint64_t vmx_region_size : 13;
        uint64_t reserved2 : 3;
        uint64_t vmxon_phys_addr_width : 1;
        uint64_t dual_monitor_smi : 1;
        uint64_t memory_type : 4;
        uint64_t io_instruction_reporting : 1;
        uint64_t true_controls : 1;
        uint64_t no_error_code_requirement : 1;
        uint64_t reserved3 : 7;
    };
} vmx_basic_msr_t;

static inline uint64_t cap_get_fixed_bits_mask(uint64_t value);

// Lookup table for the `field` ID in the encoded value
static const uint64_t s_vmx_control_field_lookup[5] = {
    CONTROL_PINBASED_CONTROLS,
    CONTROL_VMEXIT_CONTROLS,
    CONTROL_VMENTRY_CONTROLS,
    CONTROL_PRIMARY_PROCBASED_CONTROLS,
    CONTROL_SECONDARY_PROCBASED_CONTROLS
};

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

    const uint64_t default_pinbased_ctls = true_pinbased_ctls.on_bits & true_pinbased_ctls.off_bits;
    const uint64_t default_exit_ctls = true_exit_ctls.on_bits & true_exit_ctls.off_bits;
    const uint64_t default_entry_ctls = true_entry_ctls.on_bits & true_entry_ctls.off_bits;
    const uint64_t default_primary_procbased_ctls = 
        true_primary_procbased_ctls.on_bits & true_primary_procbased_ctls.off_bits;
    const uint64_t default_secondary_procbased_ctls = 
        true_secondary_procbased_ctls.on_bits & true_secondary_procbased_ctls.off_bits;

    // Update the state with the new bitmasks
    *state = (vmx_state_t){
        .pinbased_ctls = default_pinbased_ctls,
        .exit_ctls = default_exit_ctls,
        .entry_ctls = default_entry_ctls,
        .primary_procbased_ctls = default_primary_procbased_ctls,
        .secondary_procbased_ctls = default_secondary_procbased_ctls,
        .cap.true_pinbased_ctls = true_pinbased_ctls.value,
        .cap.true_exit_ctls = true_exit_ctls.value,
        .cap.true_entry_ctls = true_entry_ctls.value,
        .cap.true_procbased_ctls = true_primary_procbased_ctls.value,
        .cap.true_procbased_ctls2 = true_secondary_procbased_ctls.value
    };
}

// Gets the flexible bits mask from a capability MSR
static inline uint64_t cap_get_fixed_bits_mask(const uint64_t value)
{
    const vmx_capability_msr_t cap = {value}; 

    // ~(ON ^ OFF) returns all bits that are different, inverted meaning that all the 
    // 1 bits are actually the bits that cannot be changed
    return ~(cap.on_bits ^ cap.off_bits); 
}
