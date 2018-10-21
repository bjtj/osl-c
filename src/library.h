#ifndef __LIBRARY_H__
#define __LIBRARY_H__

#include "osl.h"

#if defined(USE_UNIX_STD)

typedef void * osl_lib_handle;
typedef void * osl_lib_symbol;

#elif defined(USE_MS_WIN)

typedef HMODULE osl_lib_handle;
typedef FARPROC osl_lib_symbol;

#endif


osl_lib_handle SHARED_EXPORT osl_library_load(const char * path, const char * name);
void SHARED_EXPORT osl_library_close(osl_lib_handle handle);
osl_lib_symbol SHARED_EXPORT osl_library_get_symbol(osl_lib_handle handle, const char * name);

#endif
