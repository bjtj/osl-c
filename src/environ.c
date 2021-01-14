#include "environ.h"
#include "str.h"
#include "string_buffer.h"

#if !defined(USE_MS_WIN)
extern char ** environ;
#endif

char * osl_environ_get_full(void)
{
#if defined(USE_MS_WIN)

    LPTSTR lpszVariable;
    LPTCH lpvEnv;
    lpvEnv = GetEnvironmentStrings();
    if (lpvEnv == NULL)
    {
	fprintf(stderr, "GetEnvironmentStrings() failed\n");
	return NULL;
    }
    osl_string_buffer_t * sb = osl_string_buffer_new();
    for (lpszVariable = (LPTSTR)lpvEnv; *lpszVariable; lpszVariable++)
    {
	osl_string_buffer_append(sb, (const char *)lpszVariable);
	osl_string_buffer_append(sb, "\n");
    }
    
    FreeEnvironmentStrings(lpvEnv);
    return osl_string_buffer_to_str_and_free(sb);
#else
    osl_string_buffer_t * sb = osl_string_buffer_new();
    char ** ptr = environ;
    for (; *ptr; ptr++)
    {
	osl_string_buffer_append(sb, *ptr);
	osl_string_buffer_append(sb, "\n");
    }
    return osl_string_buffer_to_str_and_free(sb);
#endif
}

char * osl_environ_get(const char * key)
{
#if defined(USE_MS_WIN)
    /* https://docs.microsoft.com/en-us/windows/desktop/api/winbase/nf-winbase-getenvironmentvariable */
    DWORD buffer_size = 32767;
    char * buffer = (char*)malloc(buffer_size);
    OSL_HANDLE_MALLOC_ERROR(buffer);
    memset(buffer, 0, buffer_size);
    GetEnvironmentVariable(key, buffer, buffer_size);
    char * ret = osl_safe_strdup(buffer);
    osl_safe_free(buffer);
    return ret;
#else
    return osl_safe_strdup(getenv(key));
#endif
}

void osl_environ_set(const char * key, const char * value)
{
#if defined(USE_MS_WIN)
    /* https://docs.microsoft.com/en-gb/windows/desktop/api/winbase/nf-winbase-setenvironmentvariable */
    SetEnvironmentVariable(key, value);
#else
    setenv(key, value, 1);
#endif
}


void osl_environ_unset(const char * key)
{
#if defined(USE_MS_WIN)
    /* https://docs.microsoft.com/en-gb/windows/desktop/api/winbase/nf-winbase-setenvironmentvariable */
    SetEnvironmentVariable(key, NULL);
#else
    unsetenv(key);
#endif
}
