#include "osl.h"
#include "str.h"
#include <assert.h>

static void print_string_item(int i, const char * str, const char * user)
{
    printf("[%s] '%s'\n", user, str);
}

static void print_string_list(osl_list_t * lst)
{
    osl_list_each(lst, (osl_iter_cb)print_string_item, "print string");
}

void test_string(void)
{

    printf("== test string ==\n");
    
    assert(osl_string_starts_with("--hello", "--") == 1);
    assert(osl_string_ends_with("img.jpg", ".jpg") == 1);

    assert(osl_string_starts_with_ignorecase("hEllo world", "hell") == 1);
    assert(osl_string_ends_with_ignorecase("img.JPG", "pg") == 1);

    {
	char * str = osl_string_uppercase("abc123def");
	printf("%s\n", str);
	assert(strcmp(str, "ABC123DEF") == 0);
	osl_free(str);
    }

    {
	char * str = osl_string_lowercase("aBc123dEf");
	printf("%s\n", str);
	assert(strcmp(str, "abc123def") == 0);
	osl_free(str);
    }

    {
	char * str = osl_string_capital("aBc123dEf");
	printf("%s\n", str);
	assert(strcmp(str, "Abc123def") == 0);
	osl_free(str);
    }

    {
	osl_list_t * lst = osl_split("hello world bye", " ");
	assert(lst != NULL);
	assert(osl_list_count(lst) == 3);
	assert(strcmp((char*)osl_list_get(lst, 0), "hello") == 0);
	assert(strcmp((char*)osl_list_get(lst, 1), "world") == 0);
	assert(strcmp((char*)osl_list_get(lst, 2), "bye") == 0);
	osl_list_free(lst, osl_free);
    }

    {
	osl_list_t * lst = osl_split("hello    world  bye", " ");
	assert(lst != NULL);
	assert(osl_list_count(lst) == 3);
	assert(strcmp((char*)osl_list_get(lst, 0), "hello") == 0);
	assert(strcmp((char*)osl_list_get(lst, 1), "world") == 0);
	assert(strcmp((char*)osl_list_get(lst, 2), "bye") == 0);
	osl_list_free(lst, osl_free);
    }

    {
	osl_list_t * lst = osl_split("   hello    world  bye  ", " ");
	assert(lst != NULL);
	print_string_list(lst);
	assert(osl_list_count(lst) == 3);
	assert(strcmp((char*)osl_list_get(lst, 0), "hello") == 0);
	assert(strcmp((char*)osl_list_get(lst, 1), "world") == 0);
	assert(strcmp((char*)osl_list_get(lst, 2), "bye") == 0);
	osl_list_free(lst, osl_free);
    }


    {
	osl_list_t * lst = osl_split_limit("   hello    world  bye  ", " ", 2);
	assert(lst != NULL);
	print_string_list(lst);
	assert(osl_list_count(lst) == 2);
	assert(strcmp((char*)osl_list_get(lst, 0), "hello") == 0);
	assert(strcmp((char*)osl_list_get(lst, 1), "world  bye  ") == 0);
	osl_list_free(lst, osl_free);
    }
}

int main(int argc, char *argv[])
{
    osl_init_once();

    test_string();
    
    osl_finish();
    
    return 0;
}
