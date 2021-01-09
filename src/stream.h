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

    extern OSL_EXPORT osl_stream_t * osl_stream_new(void);
    extern OSL_EXPORT void osl_stream_free(osl_stream_t * stream);
    extern OSL_EXPORT osl_stream_t * osl_stream_open(const char * path, const char * flags);
    extern OSL_EXPORT osl_stream_t * osl_stream_wrap_std_file(FILE * fp);
#if defined(USE_MS_WIN)
    extern OSL_EXPORT osl_stream_t * osl_stream_wrap_win32_handle(HANDLE handle);
#endif

    extern OSL_EXPORT osl_bool osl_stream_is_open(osl_stream_t * stream);
    extern OSL_EXPORT char * osl_stream_dump(osl_stream_t * stream);
    extern OSL_EXPORT char * osl_stream_readline(osl_stream_t * stream);
    extern OSL_EXPORT void osl_stream_writeline(osl_stream_t * stream, const char * str);


#ifdef __cplusplus
}
#endif

#endif
