#include "../../include/hvl_alloc.h"
#include <Wdm.h>
#include <ntddk.h>

#define HVL_POOL_TAG ' LVH' // "HVL\0"

void* hvl_alloc(uint32_t size, uint32_t flags)
{
    if (size == 0)
        return NULL;

    void *buf = NULL;

    if (flags & HVL_MEM_PAGEABLE)
        buf = ExAllocatePoolWithTag(PagedPool, size, HVL_POOL_TAG);
    else if (flags & HVL_MEM_NONPAGEABLE)
        buf = ExAllocatePoolWithTag(NonPagedPool, size, HVL_POOL_TAG);

    // Invalid flags were passed, or the allocation failed
    if (buf == NULL)
        return NULL;

    memset(buf, 0, size);

    return buf;
}

void hvl_free(void* buf)
{
    ExFreePoolWithTag(buf, HVL_POOL_TAG);
}
