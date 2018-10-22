#ifndef __FILE_STREAM_H__
#define __FILE_STREAM_H__

#include "osl.h"

typedef struct _osl_file_stream_t
{
    void * handle;
    int eof;
    int (*read)(struct _osl_file_stream_t * stream);
    int (*write)(struct _osl_file_stream_t * stream, int ch);
    void (*close)(struct _osl_file_stream_t * stream);
} osl_file_stream_t;

#define osl_file_stream_read(stream) (stream).read(&stream)
#define osl_file_stream_write(stream, ch) (stream).write(&stream, ch)
#define osl_file_stream_close(stream) (stream).close(&stream)

#ifdef __cplusplus
extern "C" {
#endif

    osl_file_stream_t OSL_EXPORT osl_file_stream_open(const char * path, const char * flags);
    osl_file_stream_t OSL_EXPORT osl_file_stream_init(FILE * fp);
#if defined(USE_MS_WIN)
    osl_file_stream_t OSL_EXPORT osl_file_stream_init_win32(HANDLE handle);
#endif

    char OSL_EXPORT * osl_file_stream_dump(osl_file_stream_t * stream);


#ifdef __cplusplus
}
#endif

#endif
