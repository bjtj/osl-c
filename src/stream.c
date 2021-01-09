#include "stream.h"
#include "string_buffer.h"

static int s_read_cb(osl_stream_t * stream);
static size_t s_read_buffer_cb(osl_stream_t * stream, void * buf, size_t siz);
static int s_write_cb(osl_stream_t * stream, int ch);
static int s_write_string_cb(osl_stream_t * stream, const char * str);
static size_t s_write_buffer_cb(osl_stream_t * stream, const void * buf, size_t siz);
static void s_close_cb(osl_stream_t * stream);

static FILE * s_fopen(const char * path, const char * flags)
{
#if defined(USE_MS_WIN)
    FILE * fp = NULL;
    if (fopen_s(&fp, path, flags) != 0) {
	return NULL;
    }
    return fp;
#else
    return fopen(path, flags);
#endif
}

osl_stream_t * osl_stream_new(void)
{
    osl_stream_t * stream = (osl_stream_t*)malloc(sizeof(osl_stream_t));
    if (stream == NULL)
    {
        /* TODO: exception */
        return NULL;
    }
    memset(stream, 0, sizeof(osl_stream_t));
    return stream;
}

void osl_stream_free(osl_stream_t * stream)
{
    if (stream == NULL)
    {
	return;
    }
    osl_free(stream);
}

osl_stream_t * osl_stream_open(const char * path, const char * flags)
{
    return osl_stream_wrap_std_file(s_fopen(path, flags));
}

osl_stream_t * osl_stream_wrap_std_file(FILE * fp)
{
    if (fp == NULL)
    {
	return NULL;
    }
    osl_stream_t * stream = osl_stream_new();
    stream->handle = fp;
    stream->eof = 0;
    stream->read = s_read_cb;
    stream->read_buffer = s_read_buffer_cb;
    stream->write = s_write_cb;
    stream->write_string = s_write_string_cb;
    stream->write_buffer = s_write_buffer_cb;
    stream->close = s_close_cb;
    return stream;
}


osl_bool osl_stream_is_open(osl_stream_t * stream)
{
    return OSL_BOOL(stream != NULL && stream->handle != NULL);
}

char * osl_stream_dump(osl_stream_t * stream)
{
    osl_string_buffer_t * sb = osl_string_buffer_new();
    int ch;
    while ((ch = stream->read(stream)) > 0)
    {
	osl_string_buffer_append_buffer(sb, (char*)&ch, 1);
    }
    return osl_string_buffer_to_string_and_free(sb);
}

char * osl_stream_readline(osl_stream_t * stream)
{
    if (stream->eof)
    {
	return NULL;
    }
    osl_string_buffer_t * sb = osl_string_buffer_new();
    int ch;
    while ((ch = osl_stream_read(stream)) > 0 && ch != '\n')
    {
	char c = (char)ch;
	osl_string_buffer_append_buffer(sb, &c, 1);
    }
    return osl_string_buffer_to_string_and_free(sb);
}

void osl_stream_writeline(osl_stream_t * stream, const char * str)
{
    if (str == NULL)
    {
	return;
    }
    osl_stream_write_string(stream, str);
    osl_stream_write(stream, '\n');
}


static int s_read_cb(osl_stream_t * stream)
{
    FILE * fp = (FILE*)stream->handle;
    int ch = fgetc(fp);
    if (ch == EOF) {
	ch = -1;
    }
    stream->eof = feof(fp);
    return ch;
}

static size_t s_read_buffer_cb(osl_stream_t * stream, void * buf, size_t siz)
{
    FILE * fp = (FILE*)stream->handle;
    return fread(buf, 1, siz, fp);
}

static int s_write_cb(osl_stream_t * stream, int ch)
{
    FILE * fp = (FILE*)stream->handle;
    return fputc(ch, fp);
}

static int s_write_string_cb(osl_stream_t * stream, const char * str)
{
    FILE * fp = (FILE*)stream->handle;
    return fputs(str, fp);
}

static size_t s_write_buffer_cb(osl_stream_t * stream, const void * buf, size_t siz)
{
    FILE * fp = (FILE*)stream->handle;
    return fwrite(buf, 1, siz, fp);
}

static void s_close_cb(osl_stream_t * stream)
{
    FILE * fp = (FILE*)stream->handle;
    if (fp)
    {
	fclose(fp);
	stream->handle = NULL;
    }
}

#if defined(USE_MS_WIN)

static int s_win32_read_cb(osl_stream_t * stream);
static int s_win32_write_cb(osl_stream_t * stream, int ch);
static void s_win32_close_cb(osl_stream_t * stream);

osl_stream_t * osl_stream_wrap_win32_handle(HANDLE handle)
{
    if (handle == NULL)
    {
	return NULL;
    }
    osl_stream_t * stream = osl_stream_new();
    stream->handle = handle;
    stream->eof = 0;
    stream->read = s_win32_read_cb;
    stream->write = s_win32_write_cb;
    stream->close = s_win32_close_cb;
    return stream;
}


static int s_win32_read_cb(osl_stream_t * stream)
{
    HANDLE handle = (HANDLE)stream->handle;
    DWORD dwRead = 0;
	int ch;
    BOOL bSuccess = ReadFile(handle, &ch, 1, &dwRead, NULL);
    stream->eof = (!bSuccess || dwRead == 0);
    return (stream->eof ? -1 : ch);
}

static int s_win32_write_cb(osl_stream_t * stream, int ch)
{
    HANDLE handle = (HANDLE)stream->handle;
    DWORD dwWritten = 0;
    char c = (char)ch;
    BOOL bSuccess = WriteFile(handle, (const char *)&c, 1, &dwWritten, NULL);
    if (!bSuccess)
    {
        /* TODO: handling write error */
    }
    return (int)dwWritten;
}

static void s_win32_close_cb(osl_stream_t * stream)
{
    HANDLE handle = (HANDLE)stream->handle;
    if (handle)
    {
	CloseHandle(handle);
	stream->handle = NULL;
    }
}

#endif

