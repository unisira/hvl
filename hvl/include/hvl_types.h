#ifndef HVL_TYPES_H
#define HVL_TYPES_H

#define true 1
#define false 0
typedef unsigned char bool;

typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

// Used for reporting error codes/statuses
typedef uint32_t hvl_status_t;
// Strongly typed handle alias for VMs
typedef uint32_t hvl_handle_t;

#ifndef NULL
#define NULL (void*)0
#endif

#define HVL_OK 0x00000000
#define HVL_INVALID_PARAMETER 0x00000001
#define HVL_NO_RESOURCES 0x00000002

#define HVL_FAILURE(status) \
    (status > HVL_OK)

#endif
