#include "str.h"


int osl_string_is_empty(const char * str)
{
    return str == NULL || strlen(str) == 0;
}

char * osl_string_substr(const char * str, int start, int end)
{
    int size = end - start;
    char * ret = (char*)malloc(size + 1);
    memcpy(ret, str + start, size);
    ret[size] = '\0';
    return ret;
}

static int s_contains_char(int ch, const char * tokens)
{
    for (; *tokens; tokens++)
    {
	if (ch == *tokens)
	{
	    return 1;
	}
    }
    return 0;
}

const char * osl_string_find_last_of(const char * str, const char * tokens)
{
    const char * ptr = str + strlen(str) - 1;
    for (; ptr != str; ptr--)
    {
	if (s_contains_char(*ptr, tokens))
	{
	    return ptr;
	}
    }
    return NULL;
}

char * osl_strdup(const char * str)
{
    if (str == NULL)
    {
	return NULL;
    }
    return strdup(str);
}

char * osl_strndup(const char * str, size_t size)
{
    char * ret = (char*)malloc(size + 1);
    memcpy(ret, str, size);
    ret[size] = '\0';
    return ret;
}

char * osl_strdup_for(const char * str, const char * end_ptr)
{
    if (end_ptr)
    {
	return osl_strndup(str, end_ptr - str);
    }
    else
    {
	return osl_strdup(str);
    }
}
