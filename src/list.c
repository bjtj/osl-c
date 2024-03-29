#include "list.h"

static osl_list_t * __list_new(void)
{
  osl_list_t * node = (osl_list_t*)malloc(sizeof(osl_list_t));
  OSL_HANDLE_MALLOC_ERROR(node);
  memset(node, 0, sizeof(osl_list_t));
  return node;
}

static osl_list_t * __list_new_with_data(void * data)
{
  osl_list_t * node = __list_new();
  node->data = data;
  return node;
}

static void __list_free_single(osl_list_t * list, osl_free_cb free_cb)
{
  if (list == NULL)
  {
    return;
  }
  if (free_cb)
  {
    free_cb(list->data);
  }
  osl_safe_free(list);
}

void osl_list_free(osl_list_t * list, osl_free_cb free_cb)
{
  while (list)
  {
    osl_list_t * next = list->next;
    __list_free_single(list, free_cb);
    list = next;
  }
}

size_t osl_list_size(osl_list_t * list)
{
  int count = 0;
  for (; list; list = list->next)
  {
    count++;
  }
  return count;
}

osl_list_t * osl_list_append(osl_list_t * list, void * data)
{
  osl_list_t * tail = osl_list_last(list);
  if (tail)
  {
    tail->next = __list_new_with_data(data);
    tail->next->prev = tail;
    return list;
  }
  return __list_new_with_data(data);
}

osl_list_t * osl_list_prepend(osl_list_t * list, void * data)
{
  if (list == NULL)
  {
    return __list_new_with_data(data);
  }

  osl_list_t * node = __list_new_with_data(data);
  node->next = list;
  list->prev = node;
  return node;
}

int osl_list_insert(osl_list_t ** list, int idx, void * data)
{
  osl_list_t * new_node = NULL;
  osl_list_t * node = osl_list_offset(*list, idx);
  if (!node)
  {
    return -1;
  }

  if (*list == node) {
    *list = osl_list_prepend(*list, data);
    return 0;
  }

  new_node = __list_new_with_data(data);
  new_node->next = node;
  new_node->prev = node->prev;
  node->prev->next = new_node;
  node->prev = new_node;

  return 0;
}

osl_list_t * osl_list_remove_if(osl_list_t * list, osl_compare_cb compare_cb, void * user, osl_free_cb free_cb)
{
  osl_list_t * node = list;
  while (node)
  {
    osl_list_t * next = node->next;
    if (compare_cb(node->data, user))
    {
	    if (node->prev)
	    {
        node->prev->next = node->next;
	    }
	    if (node->next)
	    {
        node->next->prev = node->prev;
	    }
	    __list_free_single(node, free_cb);
	    if (node == list)
	    {
        list = next;
	    }
    }
    node = next;
  }
  return list;
}

void * osl_list_find(osl_list_t * list, osl_compare_cb compare_cb, void * user)
{
  for (; list; list = list->next)
  {
    if (compare_cb(list->data, user))
    {
	    return list->data;
    }
  }
  return NULL;
}

osl_list_t * osl_list_find_by_ptr(osl_list_t * list, void * data)
{
  for (; list; list = list->next)
  {
    if (list->data == data)
    {
	    return list;
    }
  }
  return NULL;
}

osl_list_t * osl_list_remove(osl_list_t * list, osl_list_t * ptr, osl_free_cb free_cb)
{
  osl_list_t * node = list;
  while (node)
  {
    osl_list_t * next = node->next;
    if (node == ptr)
    {
	    if (node->prev)
	    {
        node->prev->next = node->next;
	    }
	    if (node->next)
	    {
        node->next->prev = node->prev;
	    }
	    __list_free_single(node, free_cb);
	    if (node == list)
	    {
        list = next;
	    }
	    break;
    }
    node = next;
  }
  return list;
}

osl_bool _compare_ptr(void * p1, void * p2)
{
  return OSL_BOOL(p1 == p2);
}

osl_list_t * osl_list_remove_by_ptr(osl_list_t * list, void * ptr, osl_free_cb free_cb)
{
  return osl_list_remove_if(list, _compare_ptr, ptr, free_cb);
}

osl_list_t * osl_list_remove_idx(osl_list_t * list, int idx, osl_free_cb free_cb)
{
  if (idx == 0)
  {
    osl_list_t * ret = NULL;
    if (list)
    {
	    ret = list->next;
    }
    __list_free_single(list, free_cb);
    return ret;
  }

  osl_list_t * node = osl_list_offset(list, idx);
  if (node)
  {
    node->prev->next = node->next;
    if (node->next)
    {
	    node->next->prev = node->prev;
    }
    __list_free_single(node, free_cb);
  }
  return list;
}

osl_list_t * osl_list_append_list(osl_list_t * list, osl_list_t * other)
{
  osl_list_t * tail = osl_list_last(list);
  if (tail)
  {
    tail->next = other;
    other->prev = tail;
    return list;
  }
  return other;
}

osl_list_t * osl_list_prepend_list(osl_list_t * list, osl_list_t * other)
{
  return osl_list_append_list(other, list);
}

osl_list_t * osl_list_first(osl_list_t * list)
{
  for (; list && list->prev; list = list->prev) {}
  return list;
}

osl_list_t * osl_list_last(osl_list_t * list)
{
  for (; list && list->next; list = list->next) {}
  return list;
}

osl_list_t * osl_list_offset(osl_list_t * list, int idx)
{
  if (idx > 0)
  {
    for (; list && idx--; list = list->next) {}
  }
  else
  {
    for (; list && idx++; list = list->prev) {}
  }
  return list;
}

void * osl_list_get(osl_list_t * list, int idx)
{
  osl_list_t * node = osl_list_offset(list, idx);
  if (node)
  {
    return node->data;
  }
  return NULL;
}

void * osl_list_get_last(osl_list_t * list)
{
  return osl_list_last(list)->data;
}

void * osl_list_pop_first(osl_list_t ** list)
{
  void * data;
  if (*list == NULL) {
    return NULL;
  }
  data = (*list)->data;
  *list = osl_list_remove(*list, *list, NULL);
  return data;
}

void * osl_list_each(osl_list_t * list, osl_iter_cb iter_cb, void * user)
{
  int i = 0;
  for (; list; list = list->next, i++) {
    iter_cb(i, list->data, user);
  }
  return user;
}
