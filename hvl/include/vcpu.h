#ifndef HVL_VCPU_H
#define HVL_VCPU_H

#include "hvl_types.h"
#include "config.h"

struct vcpu_t
{
    uint8_t id;
    hvl_config_t* config;
};

#endif
