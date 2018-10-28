#include "file_stream.h"
#include "string_buffer.h"

static int s_read_cb(osl_file_stream_t * stream);
static int s_write_cb(osl_file_stream_t * stream, int ch);
static void s_close_cb(osl_file_stream_t * stream);

static int s_win32_read_cb(osl_file_stream_t * stream);
static int s_win32_write_cb(osl_file_stream_t * stream, int ch);
static void s_win32_close_cb(osl_file_stream_t * stream);

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

osl_file_stream_t * osl_file_stream_new(void)
{
    osl_file_stream_t * stream = (osl_file_stream_t*)malloc(sizeof(osl_file_stream_t));
    memset(stream, 0, sizeof(osl_file_stream_t));
    return stream;
}

void osl_file_stream_free(osl_file_stream_t * stream)
{
    if (stream == NULL)
    {
	return;
    }
    osl_free(stream);
}

osl_file_stream_t * osl_file_stream_open(const char * path, const char * flags)
{
    return osl_file_stream_init(s_fopen(path, flags));
}

osl_file_stream_t * osl_file_stream_init(FILE * fp)
{
    if (fp == NULL)
    {
	return NULL;
    }
    osl_file_stream_t * stream = osl_file_stream_new();
    stream->handle = fp;
    stream->eof = 0;
    stream->read = s_read_cb;
    stream->write = s_write_cb;
    stream->close = s_close_cb;
    return stream;
}


int osl_file_stream_is_open(osl_file_stream_t * stream)
{
    return (stream != NULL && stream->handle != NULL);
}

char * osl_file_stream_dump(osl_file_stream_t * stream)
{
    osl_string_buffer_t * sb = osl_string_buffer_new();
    int ch;
    while ((ch = stream->read(stream)) > 0)
    {
	osl_string_buffer_append_buffer(sb, (char*)&ch, 1);
    }
    return osl_string_buffer_to_string_and_free(sb);
}

char * osl_file_stream_readline(osl_file_stream_t * stream)
{
    if (stream->eof)
    {
	return NULL;
    }
    osl_string_buffer_t * sb = osl_string_buffer_new();
    int ch;
    while ((ch = osl_file_stream_read(stream)) > 0 && ch != '\n')
    {
	char c = (char)ch;
	osl_string_buffer_append_buffer(sb, &c, 1);
    }
    return osl_string_buffer_to_string_and_free(sb);
}

void osl_file_stream_writeline(osl_file_stream_t * stream, const char * str)
{
    if (str == NULL)
    {
	return;
    }
    for (; *str; str++)
    {
	if (osl_file_stream_write(stream, *str) < 0)
	{
	    return;
	}
    }
    osl_file_stream_write(stream, '\n');
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
    if (fp)
    {
	fclose(fp);
	stream->handle = NULL;
    }
}

#if defined(USE_MS_WIN)

osl_file_stream_t * osl_file_stream_init_win32(HANDLE handle)
{
    if (handle == NULL)
    {
	return NULL;
    }
    osl_file_stream_t * stream = osl_file_stream_new();
    stream->handle = handle;
    stream->eof = 0;
    stream->read = s_win32_read_cb;
    stream->write = s_win32_write_cb;
    stream->close = s_win32_close_cb;
    return stream;
}


static int s_win32_read_cb(osl_file_stream_t * stream)
{
    HANDLE handle = (HANDLE)stream->handle;
    DWORD dwRead = 0;
	int ch;
    BOOL bSuccess = ReadFile(handle, &ch, 1, &dwRead, NULL);
    stream->eof = (!bSuccess || dwRead == 0);
    return (stream->eof ? -1 : ch);
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
    if (handle)
    {
	CloseHandle(handle);
	stream->handle = NULL;
    }
}

#endif

