#define _CRT_SECURE_NO_WARNINGS

#include "str.h"
#include "string_buffer.h"

const char * osl_string_end_ptr(const char * str)
{
    while (*(str + 1))
    {
	str++;
    }
    return str;
}

osl_bool osl_string_is_empty(const char * str)
{
    return OSL_BOOL(str == NULL || strlen(str) == 0);
}

osl_bool osl_string_equals(const char * s1, const char * s2)
{
    return OSL_BOOL(strcmp(s1, s2) == 0);
}

osl_bool osl_string_equals_ignorecase(const char * s1, const char * s2)
{
    return OSL_BOOL(osl_strcmp_ignorecase(s1, s2) == 0);
}

char * osl_string_substr(const char * str, size_t start, size_t end)
{
    size_t size = end - start;
    char * ret = (char*)malloc(size + 1);
    OSL_HANDLE_MALLOC_ERROR(ret);
    memcpy(ret, str + start, size);
    ret[size] = '\0';
    return ret;
}

static osl_bool s_contains_char(int ch, const char * tokens)
{
    for (; *tokens; tokens++)
    {
	if (ch == *tokens)
	{
	    return osl_true;
	}
    }
    return osl_false;
}

osl_bool osl_string_starts_with(const char * str, const char * query)
{
    return OSL_BOOL(strstr(str, query) == str);
}

osl_bool osl_string_starts_with_ignorecase(const char * str, const char * query)
{
    if (strlen(str) < strlen(query))
    {
	return osl_false;
    }
    return OSL_BOOL(strncasecmp(str, query, strlen(query)) == 0);
}

osl_bool osl_string_ends_with(const char * str, const char * query)
{
    if (strlen(str) < strlen(query))
    {
	return osl_false;
    }
    return OSL_BOOL(strcmp(str + strlen(str) - strlen(query), query) == 0);
}

osl_bool osl_string_ends_with_ignorecase(const char * str, const char * query)
{
    if (strlen(str) < strlen(query))
    {
	return osl_false;
    }
    return OSL_BOOL(strcasecmp(str + strlen(str) - strlen(query), query) == 0);
}

const char * osl_string_find(const char * str, const char * match)
{
    return strstr(str, match);
}

const char * osl_string_find_last_of(const char * str, const char * tokens)
{
    const char * ptr = str + strlen(str) - 1;
    for (; ptr != str; ptr--)
    {
	if (s_contains_char(*ptr, tokens) == osl_true)
	{
	    return ptr;
	}
    }
    return NULL;
}

const char * osl_string_find_last_not_of(const char * str, const char * tokens)
{
    const char * ptr = str + strlen(str) - 1;
    for (; ptr != str; ptr--)
    {
	if (s_contains_char(*ptr, tokens) == osl_false)
	{
	    return ptr;
	}
    }
    return NULL;
}

const char * osl_string_find_first_of(const char * str, const char * tokens)
{
    for (; *str; str++)
    {
	if (s_contains_char(*str, tokens) == osl_true)
	{
	    return str;
	}
    }
    return NULL;
}

const char * osl_string_find_first_not_of(const char * str, const char * tokens)
{
    for (; *str; str++)
    {
	if (s_contains_char(*str, tokens) == osl_false)
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
	    osl_string_buffer_append_buffer(sb, str, (int)(ptr - str));
	    osl_string_buffer_append(sb, sub);
	    str = (ptr + strlen(mat));
	}
	else
	{
	    osl_string_buffer_append(sb, str);
	    break;
	}
    }
    return osl_string_buffer_to_str_and_free(sb);
}

char * osl_string_uppercase(const char * str)
{
    char * ptr = osl_safe_strdup(str);
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
    char * ptr = osl_safe_strdup(str);
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
    char * ptr = osl_safe_strdup(str);
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

size_t osl_strlen (const char * str)
{
    return strlen(str);
}

char * osl_strcat(char * s1, const char * s2)
{
    return strcat(s1, s2);
}

char * osl_safe_strdup(const char * str)
{
    if (str == NULL) {
	return NULL;
    }
    return osl_strdup(str);
}

char * osl_strdup(const char * str)
{
    return strdup(str);
}

char * osl_strndup(const char * str, size_t size)
{
    char * ret = (char*)malloc(size + 1);
    OSL_HANDLE_MALLOC_ERROR(ret);
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
	return osl_safe_strdup(str);
    }
}


const char * osl_string_safe(const char * str, const char * def)
{
    return (str ? str : def);
}

int osl_strcmp(const char * a, const char * b)
{
    return strcmp(a, b);
}

int osl_strcmp_ignorecase(const char * a, const char * b)
{
    return strcasecmp(a, b);
}

osl_list_t * osl_string_split(const char * str, const char * del)
{
    const char * ptr = str;
    osl_list_t * lst = NULL;

    do
    {
	const char * end_ptr = strstr(ptr, del);

	if (end_ptr)
	{
	    if (ptr != end_ptr)
	    {
		lst = osl_list_append(lst, osl_strdup_for(ptr, end_ptr));
	    }
	    ptr = end_ptr + strlen(del);
	}
	else
	{
	    if (strlen(ptr) > 0)
	    {
		lst = osl_list_append(lst, osl_safe_strdup(ptr));
	    }
	    ptr = NULL;
	}
    } while (ptr);

    return lst;
}


osl_list_t * osl_string_split_limit(const char * str, const char * del, int limit)
{
    const char * ptr = str;
    osl_list_t * lst = NULL;

    do
    {
	const char * end_ptr = strstr(ptr, del);

	if (end_ptr)
	{
	    if (ptr != end_ptr)
	    {
		if (--limit > 0)
		{
		    lst = osl_list_append(lst, osl_strdup_for(ptr, end_ptr));
		    ptr = end_ptr + strlen(del);
		}
		else
		{
		    lst = osl_list_append(lst, osl_safe_strdup(ptr));
		    ptr = NULL;
		}
	    }
	    else
	    {
		ptr = end_ptr + strlen(del);
	    }
	}
	else
	{
	    if (strlen(ptr) > 0)
	    {
		lst = osl_list_append(lst, osl_safe_strdup(ptr));
	    }
	    ptr = NULL;
	}
    } while (ptr);

    return lst;
}

char * osl_string_trim(const char * str)
{
    const char * start = NULL;
    const char * end = NULL;
    start = osl_string_find_first_not_of(str, " \t\n");
    if (start == NULL) {
	return NULL;
    }
    end = osl_string_find_last_not_of(str, " \t\n");
    if (end == NULL) {
	return NULL;
    }
    return osl_strndup(start, end + 1 - start);
}

char * osl_string_ltrim(const char * str)
{
    const char * ptr = osl_string_find_first_not_of(str, " \t\n");
    if (ptr == NULL) {
	return NULL;
    }
    return osl_safe_strdup(ptr);
}

char * osl_string_rtrim(const char * str)
{
    const char * ptr = osl_string_find_last_not_of(str, " \t\n");
    if (ptr == NULL) {
	return NULL;
    }
    return osl_strndup(str, ptr + 1 - str);
}

char * osl_string_append(const char * s1, const char * s2)
{
    size_t len = strlen(s1) + strlen(s2) + 1;
    char * ret = (char*)malloc(len);
    OSL_HANDLE_MALLOC_ERROR(ret);
    memset(ret, 0, len);
    osl_strcat(ret, s1);
    osl_strcat(ret, s2);
    return ret;
}
