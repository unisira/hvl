#ifndef HVL_ALLOC_H
#define HVL_ALLOC_H

#include "hvl_types.h"

typedef enum 
{
    HVL_MEM_PAGEABLE    = 1 << 0,
    HVL_MEM_NONPAGEABLE = 1 << 1,
} hvl_memory_flags_t;

// Allocate memory
void* hvl_alloc(uint32_t size, uint32_t flags);
void hvl_free(void* mem);
#endif
