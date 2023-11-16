#include "list.h"
#include "osl.h"
#include <assert.h>

void test_list(void)
{

  osl_list_t * list = NULL;

  printf("== test list ==\n");

  printf("count -- %zu\n", osl_list_size(list));
  printf("item of index 3 is %s\n", (char*)osl_list_get(list, 3));

  list = osl_list_append(list, strdup("hello"));
  list = osl_list_append(list, strdup("hi"));
  list = osl_list_append(list, strdup("ho"));
  list = osl_list_append(list, strdup("bye"));
  list = osl_list_prepend(list, strdup("!!!"));

  osl_list_t * ptr = list;
  for (; ptr; ptr = ptr->next)
  {
    printf("* %s\n", (char *)ptr->data);
  }

  printf("item of index 3 is %s\n", (char*)osl_list_get(list, 3));

  printf("count -- %zu\n", osl_list_size(list));

  list = osl_list_remove_idx(list, 2, free);
  ptr = list;
  for (; ptr; ptr = ptr->next)
  {
    printf("* %s\n", (char *)ptr->data);
  }

  printf("====\n");

  list = osl_list_remove_idx(list, 0, free);
  ptr = list;
  for (; ptr; ptr = ptr->next)
  {
    printf("* %s\n", (char *)ptr->data);
  }

  osl_list_free(list, free);
}

void test_list2(void)
{
  const char * s;
  osl_list_t * lst = NULL;
  
  printf("== test list2 ==\n");

  lst = osl_list_append(lst, "a");
  lst = osl_list_append(lst, "b");
  lst = osl_list_append(lst, "c");
  assert(lst != NULL);

  assert(osl_list_size(lst) == 3);
  s = osl_list_pop_first(&lst);
  assert(s != NULL);
  assert(strcmp(s, "a") == 0);
  assert(osl_list_size(lst) == 2);

  assert(strcmp(osl_list_pop_first(&lst), "b") == 0);
  assert(strcmp(osl_list_pop_first(&lst), "c") == 0);

  assert(osl_list_size(lst) == 0);

  lst = osl_list_append(lst, "x");
  lst = osl_list_append(lst, "y");
  lst = osl_list_append(lst, "z");

  assert(osl_list_size(lst) == 3);

  osl_list_free(lst, NULL);
}

void test_list3(void)
{
  osl_list_t * list = NULL;

  printf("== test list3 ==\n");
  
  list = osl_list_append(list, "a");
  list = osl_list_append(list, "b");
  list = osl_list_append(list, "c");

  assert(strcmp(osl_list_get(list, 0), "a") == 0);
  assert(strcmp(osl_list_get(list, 1), "b") == 0);
  assert(strcmp(osl_list_get(list, 2), "c") == 0);

  assert(strcmp(osl_list_offset(list, 0)->data, "a") == 0);
  assert(strcmp(osl_list_offset(list, 1)->data, "b") == 0);
  assert(strcmp(osl_list_offset(list, 2)->data, "c") == 0);

  assert(osl_list_insert(&list, 1, "x") == 0);

  printf("list[0]: %s\n", (char*)osl_list_get(list, 0));
  printf("list[1]: %s\n", (char*)osl_list_get(list, 1));
  printf("list[2]: %s\n", (char*)osl_list_get(list, 2));
  printf("list[3]: %s\n", (char*)osl_list_get(list, 3));
  fflush(stdout);

  assert(strcmp(osl_list_get(list, 0), "a") == 0);
  assert(strcmp(osl_list_get(list, 1), "x") == 0);
  assert(strcmp(osl_list_get(list, 2), "b") == 0);
  assert(strcmp(osl_list_get(list, 3), "c") == 0);

  assert(strcmp(osl_list_offset(list, 0)->data, "a") == 0);
  assert(osl_list_offset(list, -1) == NULL);
  assert(strcmp(osl_list_offset(list->next, -1)->data, "a") == 0);
  assert(strcmp(osl_list_offset(list->next->next, -1)->data, "x") == 0);
  assert(strcmp(osl_list_offset(list->next->next, -2)->data, "a") == 0);

  assert(osl_list_insert(&list, 0, "y") == 0);

  assert(strcmp(osl_list_get(list, 0), "y") == 0);
  assert(strcmp(osl_list_get(list, 1), "a") == 0);
  assert(strcmp(osl_list_get(list, 2), "x") == 0);
  assert(strcmp(osl_list_get(list, 3), "b") == 0);
  assert(strcmp(osl_list_get(list, 4), "c") == 0);

  osl_list_free(list, NULL);
}

int main(void)
{
  osl_init_once();

  test_list();
  test_list2();
  test_list3();
    
  osl_finish();
    
  return 0;
}
