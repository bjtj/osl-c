#include "argparse.h"
#include "str.h"

osl_argument_flag_t * osl_argument_flag_new(const char * name, const char * defval, osl_bool single, const char * desc)
{
    osl_argument_flag_t * flag = (osl_argument_flag_t*)malloc(sizeof(osl_argument_flag_t));
    OSL_HANDLE_MALLOC_ERROR(flag);
    memset(flag, 0, sizeof(osl_argument_flag_t));
    flag->name = osl_safe_strdup(name);
    flag->defval = osl_safe_strdup(defval);
    flag->single_flag = single;
    flag->description = osl_safe_strdup(desc);
    return flag;
}

void osl_argument_flag_free(osl_argument_flag_t * flag)
{
    if (flag == NULL)
    {
	return;
    }
    osl_safe_free(flag->name);
    osl_safe_free(flag->defval);
    osl_safe_free(flag->description);
    osl_safe_free(flag);
}

osl_bool osl_arguments_usage_is_single_flag(osl_arguments_usage_t * usage, const char * name)
{
    osl_argument_flag_t * flag = osl_arguments_usage_get_flag(usage, name);
    if (flag)
    {
	return flag->single_flag;
    }
    return osl_false;
}

osl_argument_flag_t * osl_arguments_usage_get_flag(osl_arguments_usage_t * usage, const char * name)
{
    osl_list_t * ptr = usage->flag_list;
    for (; ptr; ptr = ptr->next)
    {
	osl_argument_flag_t * flag = (osl_argument_flag_t*)ptr->data;
	if (strcmp(flag->name, name) == 0)
	{
	    return flag;
	}
    }
    return NULL;
}

osl_arguments_usage_t * osl_arguments_usage_new(void)
{
    osl_arguments_usage_t * usage = (osl_arguments_usage_t*)malloc(sizeof(osl_arguments_usage_t));
    OSL_HANDLE_MALLOC_ERROR(usage);
    memset(usage, 0, sizeof(osl_arguments_usage_t));
    return usage;
}

void osl_arguments_usage_free(osl_arguments_usage_t * usage)
{
    if (usage == NULL)
    {
	return;
    }
    osl_list_free(usage->flag_list, (osl_free_cb)osl_argument_flag_free);
}

osl_argument_t * osl_argument_new(const char * name, const char * value)
{
    osl_argument_t * arg = (osl_argument_t*)malloc(sizeof(osl_argument_t));
    OSL_HANDLE_MALLOC_ERROR(arg);
    memset(arg, 0, sizeof(osl_argument_t));
    arg->name = osl_safe_strdup(name);
    arg->value = osl_safe_strdup(value);
    return arg;
}

void osl_argument_free(osl_argument_t * arg)
{
    if (arg == NULL)
    {
	return;
    }
    osl_safe_free(arg->name);
    osl_safe_free(arg->value);
    osl_safe_free(arg);
}

osl_arguments_t * osl_arguments_new(void)
{
    osl_arguments_t * args = (osl_arguments_t*)malloc(sizeof(osl_arguments_t));
    OSL_HANDLE_MALLOC_ERROR(args);
    memset(args, 0, sizeof(osl_arguments_t));
    return args;
}

void osl_arguments_free(osl_arguments_t * args)
{
    if (args == NULL)
    {
	return;
    }
    osl_safe_free(args->name);
    osl_list_free(args->arg_list, (osl_free_cb)osl_argument_free);
    osl_list_free(args->text_list, free);
    osl_safe_free(args);
}

osl_arguments_t * osl_arguments_parse(osl_arguments_usage_t * usage, int argc, char * argv[])
{
    osl_arguments_t * args = osl_arguments_new();
    int i = 0;
    for (; i < argc; i++)
    {
	char * arg = argv[i];
	if (i == 0)
	{
	    args->name = osl_safe_strdup(arg);
	    continue;
	}
	char * next_arg = (i < argc - 1) ? argv[i+1] : NULL;
	if (osl_string_starts_with(arg, "--"))
	{
	    osl_bool single = osl_arguments_usage_is_single_flag(usage, arg);
	    if (single || next_arg == NULL || next_arg[0] == '-')
	    {
		args->arg_list = osl_list_append(args->arg_list, osl_argument_new(arg + 2, NULL));
	    }
	    else
	    {
		args->arg_list = osl_list_append(args->arg_list, osl_argument_new(arg + 2, next_arg));
		i++;
	    }
	}
	else if (osl_string_starts_with(arg, "-"))
	{
	    osl_bool single = osl_arguments_usage_is_single_flag(usage, arg);
	    if (single || next_arg == NULL || next_arg[0] == '-')
	    {
		args->arg_list = osl_list_append(args->arg_list, osl_argument_new(arg + 1, NULL));
	    }
	    else
	    {
		args->arg_list = osl_list_append(args->arg_list, osl_argument_new(arg + 1, next_arg));
		i++;
	    }
	}
	else
	{
	    args->text_list = osl_list_append(args->text_list, osl_safe_strdup(arg));
	}
    }
    return args;
}
