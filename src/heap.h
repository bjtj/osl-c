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

  extern OSL_EXPORT osl_heap_t * osl_heap_new(void);
  extern OSL_EXPORT osl_heap_t * osl_heap_init_shread(osl_heap_t *);
  extern OSL_EXPORT osl_heap_t * osl_heap_init_with_callback(osl_heap_t *, osl_heap_on_alloc_cb, osl_heap_on_dealloc_cb);
  extern OSL_EXPORT void osl_heap_free(osl_heap_t *);
  extern OSL_EXPORT void * osl_heap_alloc(osl_heap_t *, size_t);
  extern OSL_EXPORT osl_bool osl_heap_contains(osl_heap_t *, void *);
  extern OSL_EXPORT void osl_heap_dealloc(osl_heap_t *, void *);

#ifdef __cplusplus
}
#endif    

#endif
