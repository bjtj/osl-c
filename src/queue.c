#include "queue.h"


osl_queue_node_t * osl_queue_node_new(void)
{
  osl_queue_node_t * node = (osl_queue_node_t *)malloc(sizeof(osl_queue_node_t));
  OSL_HANDLE_MALLOC_ERROR(node);
  memset(node, 0, sizeof(osl_queue_node_t));
  return node;
}

osl_queue_node_t * osl_queue_node_init(osl_queue_node_t * node, void * data)
{
  node->data = data;
  return node;
}

void osl_queue_node_free(osl_queue_node_t * node)
{
  osl_safe_free(node);
}

osl_queue_t * osl_queue_new(void)
{
  osl_queue_t * queue = (osl_queue_t *)malloc(sizeof(osl_queue_t));
  OSL_HANDLE_MALLOC_ERROR(queue);
  memset(queue, 0, sizeof(osl_queue_t));
  return queue;
}

osl_queue_t * osl_queue_init(osl_queue_t * queue)
{
  return queue;
}

osl_queue_t * osl_queue_init_with_limit(osl_queue_t * queue, size_t limit)
{
  queue->limit = limit;
  return queue;
}

osl_bool osl_queue_is_empty(osl_queue_t * queue)
{
  return OSL_BOOL(queue->count == 0);
}

void osl_queue_clear(osl_queue_t * queue, osl_free_cb freecb)
{
  osl_queue_node_t * node_ptr = queue->front;
  while (node_ptr)
  {
    osl_queue_node_t * next = node_ptr->next;
    if (freecb)
    {
	    freecb(node_ptr->data);
    }
    osl_safe_free(node_ptr);
    node_ptr = next;
  }
}

int osl_queue_push(osl_queue_t * queue, void * data)
{
  osl_queue_node_t * node = NULL;

  if (queue->limit > 0 && queue->count >= queue->limit)
  {
    return -1;
  }
  
  node = osl_queue_node_init(osl_queue_node_new(), data);

  if (osl_queue_is_empty(queue))
  {
    queue->front = node;
    queue->rear = node;
  }
  else
  {
    queue->rear->next = node;
    queue->rear = node;
  }
  queue->count++;
  return 0;
}

void * osl_queue_pop(osl_queue_t * queue)
{
  void * ret;
  osl_queue_node_t * front;
  if (osl_queue_is_empty(queue))
  {
    return NULL;
  } 
  ret = queue->front->data;
  front = queue->front;
  queue->front = queue->front->next;
  if (queue->front == NULL)
  {
    queue->rear = NULL;
  }
  osl_queue_node_free(front);
  queue->count--;
  return ret;
}

void osl_queue_free(osl_queue_t * queue, osl_free_cb freecb)
{
  osl_queue_clear(queue, freecb);
  osl_safe_free(queue);
}
