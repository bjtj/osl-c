#include "list.h"
#include "osl.h"
#include <assert.h>

void test_list(void)
{

    osl_list_t * list = NULL;

    printf("count -- %zu\n", osl_list_count(list));
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

    printf("count -- %zu\n", osl_list_count(list));

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

int main()
{
    osl_init_once();

    test_list();
    
    osl_finish();
    
    return 0;
}
