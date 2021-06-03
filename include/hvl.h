#ifndef HVL_H
#define HVL_H

#include "../hvl/include/config.h"
#include "../hvl/include/hvl.h"

hvl_status_t hvl_init();
hvl_status_t hvl_create_vm(hvl_config_t* config);

#endif