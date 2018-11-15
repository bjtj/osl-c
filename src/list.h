#ifndef __LIST_H__
#define __LIST_H__

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

    void OSL_EXPORT osl_list_free(osl_list_t * list, osl_free_cb free_cb);
    size_t OSL_EXPORT osl_list_count(osl_list_t * list);
    osl_list_t OSL_EXPORT * osl_list_append(osl_list_t * list, void * data);
    osl_list_t OSL_EXPORT * osl_list_prepend(osl_list_t * list, void * data);
    osl_list_t OSL_EXPORT * osl_list_find(osl_list_t * list, osl_compare_cb compare_cb, void * user);
    osl_list_t OSL_EXPORT * osl_list_find_by_ptr(osl_list_t * list, void * data);
    osl_list_t OSL_EXPORT * osl_list_remove(osl_list_t * list, osl_list_t * ptr, osl_free_cb free_cb);
    osl_list_t OSL_EXPORT * osl_list_remove_idx(osl_list_t * list, int idx, osl_free_cb free_cb);
        osl_list_t OSL_EXPORT * osl_list_remove_if(osl_list_t * list, osl_compare_cb compare_cb, void * user, osl_free_cb free_cb);
    osl_list_t OSL_EXPORT * osl_list_append_list(osl_list_t * list, osl_list_t * other);
    osl_list_t OSL_EXPORT * osl_list_prepend_list(osl_list_t * list, osl_list_t * other);
    osl_list_t OSL_EXPORT * osl_list_first(osl_list_t * list);
    osl_list_t OSL_EXPORT * osl_list_last(osl_list_t * list);
    osl_list_t OSL_EXPORT * osl_list_offset(osl_list_t * list, int idx);
    void OSL_EXPORT * osl_list_get(osl_list_t * list, int idx);
    void OSL_EXPORT * osl_list_pop_first(osl_list_t ** list);
    void OSL_EXPORT * osl_list_each(osl_list_t * list, osl_iter_cb iter_cb, void * user);

#ifdef __cplusplus
}
#endif

#endif
