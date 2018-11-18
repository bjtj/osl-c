#ifndef __OSL_STREAM_H__
#define __OSL_STREAM_H__

#include "osl.h"

typedef struct _osl_stream_t
{
    void * handle;
    int eof;
    int (*read)(struct _osl_stream_t * stream);
    int (*write)(struct _osl_stream_t * stream, int ch);
    void (*close)(struct _osl_stream_t * stream);
} osl_stream_t;

#define osl_stream_read(stream) (stream)->read(stream)
#define osl_stream_write(stream, ch) (stream)->write(stream, ch)
#define osl_stream_close(stream) (stream)->close(stream)
#define osl_stream_close_and_free(stream) do { (stream)->close(stream); osl_stream_free(stream); }while(0)

#ifdef __cplusplus
extern "C" {
#endif

    osl_stream_t OSL_EXPORT * osl_stream_new(void);
    void OSL_EXPORT osl_stream_free(osl_stream_t * stream);
    osl_stream_t OSL_EXPORT * osl_stream_open(const char * path, const char * flags);
    osl_stream_t OSL_EXPORT * osl_stream_wrap_std_file(FILE * fp);
#if defined(USE_MS_WIN)
    osl_stream_t OSL_EXPORT * osl_stream_wrap_win32_handle(HANDLE handle);
#endif

    int OSL_EXPORT osl_stream_is_open(osl_stream_t * stream);
    char OSL_EXPORT * osl_stream_dump(osl_stream_t * stream);
    char OSL_EXPORT * osl_stream_readline(osl_stream_t * stream);
    void OSL_EXPORT osl_stream_writeline(osl_stream_t * stream, const char * str);


#ifdef __cplusplus
}
#endif

#endif
