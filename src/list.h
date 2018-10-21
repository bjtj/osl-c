#ifndef __LIST_H__
#define __LIST_H__

#include "osl.h"

typedef void (*osl_free_cb)(void *);

typedef struct _osl_list_t
{
    void * data;
    struct _osl_list_t * prev;
    struct _osl_list_t * next;
} osl_list_t;

void SHARED_EXPORT osl_list_free(osl_list_t * list, osl_free_cb free_cb);
size_t SHARED_EXPORT osl_list_count(osl_list_t * list);
osl_list_t SHARED_EXPORT * osl_list_append(osl_list_t * list, void * data);
osl_list_t SHARED_EXPORT * osl_list_prepend(osl_list_t * list, void * data);
osl_list_t SHARED_EXPORT * osl_list_remove(osl_list_t * list, int idx, osl_free_cb free_cb);
osl_list_t SHARED_EXPORT * osl_list_append_list(osl_list_t * list, osl_list_t * other);
osl_list_t SHARED_EXPORT * osl_list_prepend_list(osl_list_t * list, osl_list_t * other);
osl_list_t SHARED_EXPORT * osl_list_last(osl_list_t * list);
osl_list_t SHARED_EXPORT * osl_list_offset(osl_list_t * list, int idx);
void SHARED_EXPORT * osl_list_get(osl_list_t * list, int idx);

#endif
