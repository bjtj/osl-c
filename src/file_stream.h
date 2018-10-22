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


#ifdef __cplusplus
extern "C" {
#endif

    osl_file_stream_t OSL_EXPORT osl_file_stream_init(FILE * fp);
#if defined(USE_MS_WIN)
    osl_file_stream_t OSL_EXPORT osl_file_stream_init_with_handle(HANDLE handle);
#endif
    int osl_file_stream_read(osl_file_stream_t * stream);
    void osl_file_stream_write(osl_file_stream_t * stream, int ch);
    void osl_file_stream_close(osl_file_stream_t * stream);

#ifdef __cplusplus
}
#endif

#endif
