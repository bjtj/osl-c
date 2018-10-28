#include "str.h"
#include "string_buffer.h"

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

int osl_string_starts_with(const char * str, const char * query)
{
    return (strstr(str, query) == str);
}

int osl_string_ends_with(const char * str, const char * query)
{
    if (strlen(str) < strlen(query))
    {
	return 0;
    }
    return (strcmp(str + strlen(str) - strlen(query), query) == 0);
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

const char * osl_string_find_first_not_of(const char * str, const char * tokens)
{
    for (; *str; str++)
    {
	if (s_contains_char(*str, tokens) == 0)
	{
	    return str;
	}
    }
    return NULL;
}

char * osl_string_replace_all(const char * str, const char * mat, const char * sub)
{
    osl_string_buffer_t * sb = osl_string_buffer_new();
    while (*str)
    {
	const char * ptr = strstr(str, mat);
	if (ptr)
	{
	    osl_string_buffer_append_buffer(sb, str, ptr - str);
	    osl_string_buffer_append(sb, sub);
	    str = (ptr + strlen(mat));
	}
	else
	{
	    osl_string_buffer_append(sb, str);
	    break;
	}
    }
    return osl_string_buffer_to_string_and_free(sb);
}

char * osl_string_uppercase(const char * str)
{
    char * ptr = osl_strdup(str);
    char * ret = ptr;
    for (; *ptr; ptr++)
    {
	if (*ptr >= 'a' && *ptr <= 'z')
	{
	    *ptr = *ptr - ('a' - 'A');
	}
    }
    return ret;
}

char * osl_string_lowercase(const char * str)
{
    char * ptr = osl_strdup(str);
    char * ret = ptr;
    for (; *ptr; ptr++)
    {
	if (*ptr >= 'A' && *ptr <= 'Z')
	{
	    *ptr = *ptr - ('A' - 'a');
	}
    }
    return ret;
}

char * osl_string_capital(const char * str)
{
    char * ptr = osl_strdup(str);
    char * ret = ptr;
    int first = 1;
    for (; *ptr; ptr++)
    {
	if (first && *ptr != ' ' && *ptr != '\t')
	{
	    if (*ptr >= 'a' && *ptr <= 'z')
	    {
		*ptr = *ptr - ('a' - 'A');
	    }
	    first = 0;
	    continue;
	}
	if (*ptr >= 'A' && *ptr <= 'Z')
	{
	    *ptr = *ptr - ('A' - 'a');
	}
    }
    return ret;
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
