#include <osl/osl.h>
#include <osl/thread.h>
#include <osl/list.h>
#include <osl/string_buffer.h>
#include <osl/file.h>
#include <osl/network.h>
#include <osl/date.h>
#include <osl/sem.h>

void * worker(void * arg)
{
    int id = *(int*)arg;
    int i = 0;
    for (; i < 10; i++) {
	printf("[%d] idx -- %d\n", id, i);
	idle(50);
    }
    return 0;
}

void test_thread(void)
{
    int id1 = 1;
    int id2 = 2;
    osl_thread_t * thread1 = osl_thread_new(worker, &id1);
    osl_thread_t * thread2 = osl_thread_new(worker, &id2);
    osl_thread_start(thread1);
    osl_thread_start(thread2);
    osl_thread_join(thread1);
    osl_thread_join(thread2);
    osl_thread_free(thread1);
    osl_thread_free(thread2);
}

void test_list(void)
{

    osl_list_t * list = NULL;

    printf("count -- %lu\n", osl_list_count(list));
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

    printf("count -- %lu\n", osl_list_count(list));

    list = osl_list_remove(list, 2, free);
    ptr = list;
    for (; ptr; ptr = ptr->next)
    {
	printf("* %s\n", (char *)ptr->data);
    }

    printf("====\n");

    list = osl_list_remove(list, 0, free);
    ptr = list;
    for (; ptr; ptr = ptr->next)
    {
	printf("* %s\n", (char *)ptr->data);
    }

    osl_list_free(list, free);
}

void test_date(void)
{
    printf(" == test date ==\n");
    int i = 0;
    for (; i < 3; i++)
    {
	osl_date_t now = osl_date_now();
	printf("%04d-%02d-%02d %02d:%02d:%02d.%03d\n",
	       now.year, now.month+1, now.day, now.hour, now.minute, now.second, now.millisecond);
	idle(1000*2);
    }
}

void test_string_buffer(void)
{
    printf(" == test string buffer ==\n");
    osl_string_buffer_t sb = {0,};
    osl_string_buffer_append(&sb, "hello");
    osl_string_buffer_append(&sb, " ");
    osl_string_buffer_append(&sb, "world");

    char * str = osl_string_buffer_to_string(&sb);
    printf("str is '%s'\n", str);
    free(str);

    osl_string_buffer_clear(&sb);
}

void test_file(void)
{
    char * dump;
    FILE * fp = fopen("hello", "w");
    fwrite("hello", 1, 5, fp);
    fclose(fp);

    fp = fopen("hello", "r");
    dump = osl_file_dump(fp);
    printf("dump: '%s'\n", dump);
    free(dump);
    fclose(fp);

    printf("file size: %lu\n", osl_file_size("hello"));
}

void test_network(void)
{
    osl_list_t * ifaces = osl_network_all_interfaces();
    osl_list_t * ptr = ifaces;
    for (; ptr; ptr = ptr->next)
    {
	osl_network_interface_t * iface = (osl_network_interface_t*)ptr->data;
	printf("* iface name -- %s\n", iface->name);
    }

    osl_list_free(ifaces, (void (*)(void*))osl_network_interface_free);
}

int main(int argc, char *argv[])
{
    test_thread();
    test_list();
    test_string_buffer();
    test_file();
    test_network();
    test_date();
    
    return 0;
}
