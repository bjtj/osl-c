#include "heap.h"


osl_heap_t * osl_heap_new(void)
{
    osl_heap_t * heap = (osl_heap_t*)malloc(sizeof(osl_heap_t));
    OSL_HANDLE_MALLOC_ERROR(heap);
    memset(heap, 0, sizeof(osl_heap_t));
    return heap;
}

osl_heap_t * osl_heap_new_shared(void)
{
    osl_heap_t * heap = (osl_heap_t*)malloc(sizeof(osl_heap_t));
    OSL_HANDLE_MALLOC_ERROR(heap);
    memset(heap, 0, sizeof(osl_heap_t));
    heap->mutex = osl_mutex_new();
    return heap;
}

osl_heap_t * osl_heap_new_with_callback(osl_heap_on_alloc_cb on_alloc, osl_heap_on_dealloc_cb on_dealloc)
{
    osl_heap_t * heap = osl_heap_new();
    heap->on_alloc = on_alloc;
    heap->on_dealloc = on_dealloc;
    return heap;
}

void osl_heap_free(osl_heap_t * heap)
{
    if (heap == NULL)
    {
	return;
    }
    osl_list_free(heap->allocs, osl_safe_free);
    osl_safe_free(heap);
}

void * osl_heap_alloc(osl_heap_t * heap, size_t size)
{
    void * alloc = malloc(size);
    OSL_HANDLE_MALLOC_ERROR(alloc);
    if (heap->mutex)
    {
	osl_mutex_lock(heap->mutex);
    }
    if (alloc)
    {
	heap->allocs = osl_list_append(heap->allocs, alloc);
    }
    if (heap->mutex)
    {
	osl_mutex_unlock(heap->mutex);
    }
    if (heap->on_alloc)
    {
	heap->on_alloc(heap, alloc);
    }
    return alloc;
}

osl_bool osl_heap_contains(osl_heap_t * heap, void * alloc)
{
    return OSL_BOOL(osl_list_find_by_ptr(heap->allocs, alloc) != NULL);
}

void osl_heap_dealloc(osl_heap_t * heap, void * alloc)
{
    if (heap->mutex)
    {
	osl_mutex_lock(heap->mutex);
    }
    osl_list_t * ptr = heap->allocs;
    for (; ptr; ptr = ptr->next)
    {
	if (ptr->data == alloc)
	{
	    if (heap->on_dealloc)
	    {
		heap->on_dealloc(heap, alloc);
	    }
	    osl_safe_free(alloc);
	    break;
	}
    }
    if (heap->mutex)
    {
	osl_mutex_unlock(heap->mutex);
    }
}
