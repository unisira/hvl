wdk_add_library(hvl STATIC
    hvl.c
    config.c
    vcpu.c
    vm.c
    vmx.c
    platform/windows/hvl_alloc.c
    )

target_include_directories(hvl INTERFACE .)