#ifndef __OSL_MEM_H__
#define __OSL_MEM_H__

#include "osl.h"
#include "list.h"
#include "mutex.h"

struct _osl_heap_t;

typedef void (*osl_heap_on_alloc_cb)(struct _osl_heap_t *, void *);
typedef void (*osl_heap_on_dealloc_cb)(struct _osl_heap_t *, void *);

typedef struct _osl_heap_t
{
    osl_mutex_t * mutex;
    osl_heap_on_alloc_cb on_alloc;
    osl_heap_on_dealloc_cb on_dealloc;
    osl_list_t * allocs;
} osl_heap_t;


#ifdef __cplusplus
extern "C" {
#endif

    osl_heap_t OSL_EXPORT * osl_heap_new(void);
    osl_heap_t OSL_EXPORT * osl_heap_new_shread(void);
    osl_heap_t OSL_EXPORT * osl_heap_new_with_callback(osl_heap_on_alloc_cb on_alloc, osl_heap_on_dealloc_cb on_dealloc);
    
    void OSL_EXPORT osl_heap_free(osl_heap_t * heap);
    void OSL_EXPORT * osl_heap_alloc(osl_heap_t * heap, size_t size);
    int OSL_EXPORT osl_heap_contains(osl_heap_t * heap, void * alloc);
    void OSL_EXPORT osl_heap_dealloc(osl_heap_t * heap, void * alloc);

#ifdef __cplusplus
}
#endif    

#endif
