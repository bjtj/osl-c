#include "heap.h"
#include "osl.h"
#include <assert.h>

void test_heap(void)
{
  printf("== test heap ==\n");
  osl_heap_t * heap = osl_heap_new();
  osl_heap_alloc(heap, sizeof(int));
  osl_heap_alloc(heap, sizeof(int));
  osl_heap_alloc(heap, sizeof(int));
  osl_heap_alloc(heap, sizeof(int));
  osl_heap_alloc(heap, sizeof(int));
  osl_heap_free(heap);
}

int main(void)
{
  osl_init_once();

  test_heap();
    
  osl_finish();
    
  return 0;
}
