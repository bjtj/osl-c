#include "library.h"
#include "string_buffer.h"
#include "pathname.h"


static char * s_get_lib_filename(const char * name)
{
#if defined(USE_APPLE_STD)
    osl_string_buffer_t * sb = osl_string_buffer_new();
    osl_string_buffer_append(sb, "lib");
    osl_string_buffer_append(sb, name);
    osl_string_buffer_append(sb, ".dylib");
    return osl_string_buffer_to_string_and_free(sb);
#elif defined(USE_UNIX_STD)
    osl_string_buffer_t * sb = osl_string_buffer_new();
    osl_string_buffer_append(sb, "lib");
    osl_string_buffer_append(sb, name);
    osl_string_buffer_append(sb, ".so");
    return osl_string_buffer_to_string_and_free(sb);
#elif defined(USE_MS_WIN)
    osl_string_buffer_t * sb = osl_string_buffer_new();
    osl_string_buffer_append(sb, name);
    osl_string_buffer_append(sb, ".dll");
    return osl_string_buffer_to_string_and_free(sb);
#else
    return NULL;
#endif
}

osl_lib_handle osl_library_load(const char * path, const char * name)
{
    char * filename = s_get_lib_filename(name);
    char * fullpath = osl_pathname_merge(path, filename);
    osl_lib_handle handle;
    free(filename);
#if defined(USE_UNIX_STD)
    handle = dlopen(fullpath, RTLD_LAZY);
    free(fullpath);
    if (handle == NULL) {
	fprintf(stderr, "Failed: dlopen() - %s", dlerror());
	return NULL;
    }
    dlerror();			/* clear any existing error */
    return handle;
    
#elif defined(USE_MS_WIN)
    handle = LoadLibrary(fullpath);
    free(fullpath);
    if (!handle) {
	fprintf(stderr, "LoadLibrary() failed");
	return NULL;
    }
    return handle;
#else
    return NULL;
#endif
}

void osl_library_close(osl_lib_handle handle)
{
#if defined(USE_UNIX_STD)
    if (handle) {
	dlclose(handle);
    }
#elif defined(USE_MS_WIN)
    if (handle) {
	FreeLibrary(handle);
    }
#endif
}

osl_lib_symbol osl_library_get_symbol(osl_lib_handle handle, const char * name)
{
#if defined(USE_UNIX_STD)
    osl_lib_symbol ret = dlsym(handle, name);
    if (!ret)
    {
	return NULL;
    }
    return ret;
#elif defined(USE_MS_WIN)
    osl_lib_symbol ret = GetProcAddress(handle, name);
    if (!ret)
    {
	return NULL;
    }
    return ret;
#endif
}
