#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "osl.h"

typedef struct _osl_queue_node_t
{
  void * data;
  struct _osl_queue_node_t * next;
} osl_queue_node_t;

typedef struct _osl_queue_t
{
  osl_queue_node_t * front;
  osl_queue_node_t * rear;
  osl_free_cb freecb;
  size_t count;
  size_t limit;
} osl_queue_t;

#ifdef __cplusplus
extern "C" {
#endif

  // QUEUE NODE
  extern OSL_EXPORT osl_queue_node_t * osl_queue_node_new(void);
  extern OSL_EXPORT osl_queue_node_t * osl_queue_node_init(osl_queue_node_t *, void *);
  extern OSL_EXPORT void osl_queue_node_free(osl_queue_node_t *);
  extern OSL_EXPORT osl_queue_t * osl_queue_new(void);
  // QUEUE
  extern OSL_EXPORT osl_queue_t * osl_queue_init(osl_queue_t *);
  extern OSL_EXPORT osl_queue_t * osl_queue_init_with_limit(osl_queue_t *, size_t);
  extern OSL_EXPORT osl_bool osl_queue_is_empty(osl_queue_t *);
  extern OSL_EXPORT void osl_queue_clear(osl_queue_t *, osl_free_cb);
  extern OSL_EXPORT int osl_queue_push(osl_queue_t *, void *);
  extern OSL_EXPORT void * osl_queue_pop(osl_queue_t *);
  extern OSL_EXPORT void osl_queue_free(osl_queue_t *, osl_free_cb);

#ifdef __cplusplus
}
#endif

#endif /* __QUEUE_H__ */
