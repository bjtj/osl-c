#include "file_stream.h"

static int s_read_cb(osl_file_stream_t * stream);
static int s_write_cb(osl_file_stream_t * stream, int ch);
static void s_close_cb(osl_file_stream_t * stream);

static int s_win32_read_cb(osl_file_stream_t * stream);
static int s_win32_write_cb(osl_file_stream_t * stream, int ch);
static void s_win32_close_cb(osl_file_stream_t * stream);

osl_file_stream_t OSL_EXPORT osl_file_stream_init(FILE * fp)
{
    osl_file_stream_t stream = {
	.handle = fp,
	.eof = 0,
	.read = s_read_cb,
	.write = s_write_cb,
	.close = s_close_cb
    };
    return stream;
}

static int s_read_cb(osl_file_stream_t * stream)
{
    FILE * fp = (FILE*)stream->handle;
    int ch = fgetc(fp);
    if (ch == EOF) {
	ch = -1;
    }
    stream->eof = feof(fp);
    return ch;
}

static int s_write_cb(osl_file_stream_t * stream, int ch)
{
    FILE * fp = (FILE*)stream->handle;
    return fputc(ch, fp);
}

static void s_close_cb(osl_file_stream_t * stream)
{
    FILE * fp = (FILE*)stream->handle;
    fclose(fp);
    stream->handle = NULL;
}

#if defined(USE_MS_WIN)

osl_file_stream_t OSL_EXPORT osl_file_stream_init_with_handle(HANDLE handle)
{
    osl_file_stream_t stream = {
	.handle = handle,
	.eof = 0,
	.read = s_win32_read_cb,
	.write = s_win32_write_cb,
	.close = s_win32_close_cb
    };
    return stream;
}


static int s_win32_read_cb(osl_file_stream_t * stream)
{
    HANDLE handle = (HANDLE)stream->handle;
    DWORD dwRead = 0;
    BOOL bSuccess = ReadFile(handle, buffer, size, &dwRead, NULL);
    stream->eof = (!bSuccess || dwRead == 0);
    return (stream->eof ? 0 : dwRead);
}

static int s_win32_write_cb(osl_file_stream_t * stream, int ch)
{
    HANDLE handle = (HANDLE)stream->handle;
    DWORD dwWritten = 0;
    char c = (char)ch;
    BOOL bSuccess = WriteFile(handle, (const char *)&c, 1, &dwWritten, NULL);
    return (int)dwWritten;
}

static void s_win32_close_cb(osl_file_stream_t * stream)
{
    HANDLE handle = (HANDLE)stream->handle;
    CloseHandle(handle);
    stream->handle = NULL;
}

#endif

