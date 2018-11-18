#ifndef __OSL_DATA_H__
#define __OSL_DATA_H__

#include "osl.h"

typedef struct _osl_data_t
{
    void * data;
    size_t size;
    osl_free_cb free_cb;
} osl_data_t;

#endif
