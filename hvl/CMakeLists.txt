enable_language(C ASM_MASM)

wdk_add_library(hvl STATIC
    config.c
    cpu.c
    cpu.asm
    hvl.c
    vcpu.c
    vm.c
    vmx.c
    platform/windows/hvl_alloc.c
    platform/windows/hvl_print.c
    )

target_include_directories(hvl INTERFACE .)
