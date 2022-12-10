#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "argparse.h"
#include "osl.h"
#include <assert.h>


static void print_arguments(osl_arguments_t * args)
{
  printf(" -- print arguments --\n");
  printf("NAME -- %s\n", args->name);
  osl_list_t * ptr = args->arg_list;
  for (; ptr; ptr = ptr->next)
  {
    osl_argument_t * arg = (osl_argument_t*)ptr->data;
    printf("  ARGUMENT -- %s = %s\n", arg->name, arg->value);
  }
  ptr = args->text_list;
  for (; ptr; ptr = ptr->next)
  {
    char * str = (char*)ptr->data;
    printf("  TEXT -- %s\n", str);
  }
}

void test_arguments(void)
{
  printf("== test arguments ==\n");
  osl_arguments_usage_t usage = {0,};
  {
    char * argv[] = {
	    "hello",
    };
    int argc = 1;
    osl_arguments_t * args = osl_arguments_parse(&usage, argc, argv);
    print_arguments(args);
    osl_arguments_free(args);
  }

  {
    char * argv[] = {
	    "hello",
	    "--debug", "yes",
	    "--single",
    };
    int argc = 4;
    osl_arguments_t * args = osl_arguments_parse(&usage, argc, argv);
    print_arguments(args);
    osl_arguments_free(args);
  }

  {
    char * argv[] = {
	    "hello",
	    "text1",
	    "--debug", "yes",
	    "--single",
	    "text2"
    };
    usage.flag_list = osl_list_append(usage.flag_list, osl_argument_flag_init(osl_argument_flag_new(), "--single", NULL, 1, NULL));
    int argc = 6;
    osl_arguments_t * args = osl_arguments_parse(&usage, argc, argv);
    print_arguments(args);
    osl_arguments_free(args);
  }

  osl_list_free(usage.flag_list, (osl_free_cb)osl_argument_flag_free);
}

int main()
{
  osl_init_once();
  osl_ignore_sigpipe();

  test_arguments();

  osl_finish();
    
  return 0;
}
