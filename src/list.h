#ifndef __OSL_LIST_H__
#define __OSL_LIST_H__

#include "osl.h"

typedef struct _osl_list_t
{
  void * data;
  struct _osl_list_t * prev;
  struct _osl_list_t * next;
} osl_list_t;


#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT void osl_list_free(osl_list_t *, osl_free_cb);
  extern OSL_EXPORT size_t osl_list_size(osl_list_t *);
  extern OSL_EXPORT osl_list_t * osl_list_append(osl_list_t *, void *);
  extern OSL_EXPORT osl_list_t * osl_list_prepend(osl_list_t *, void *);
  extern OSL_EXPORT void * osl_list_find(osl_list_t *, osl_compare_cb, void *);
  extern OSL_EXPORT osl_list_t * osl_list_find_by_ptr(osl_list_t *, void *);
  extern OSL_EXPORT osl_list_t * osl_list_remove(osl_list_t *, osl_list_t *, osl_free_cb);
  extern OSL_EXPORT osl_list_t * osl_list_remove_by_ptr(osl_list_t *, void *, osl_free_cb);
  extern OSL_EXPORT osl_list_t * osl_list_remove_idx(osl_list_t *, int, osl_free_cb);
  extern OSL_EXPORT osl_list_t * osl_list_remove_if(osl_list_t *, osl_compare_cb, void *, osl_free_cb);
  extern OSL_EXPORT osl_list_t * osl_list_append_list(osl_list_t *, osl_list_t *);
  extern OSL_EXPORT osl_list_t * osl_list_prepend_list(osl_list_t *, osl_list_t *);
  extern OSL_EXPORT osl_list_t * osl_list_first(osl_list_t *);
  extern OSL_EXPORT osl_list_t * osl_list_last(osl_list_t *);
  extern OSL_EXPORT osl_list_t * osl_list_offset(osl_list_t *, int);
  extern OSL_EXPORT void * osl_list_get(osl_list_t *, int);
  extern OSL_EXPORT void * osl_list_pop_first(osl_list_t **);
  extern OSL_EXPORT void * osl_list_each(osl_list_t *, osl_iter_cb, void *);

#ifdef __cplusplus
}
#endif

#endif
