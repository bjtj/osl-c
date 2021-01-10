#include "string_stream.h"
#include "str.h"

struct container
{
    char * str;
    char * ptr;
};

static int s_read_cb(osl_stream_t * stream)
{
    struct container * con = (struct container*)stream->handle;
    char ch = *(con->ptr++);
    if (ch == 0)
    {
	stream->eof = 1;
    }
    return ch;
}

static int s_write_cb(osl_stream_t * stream, int ch)
{
    (void)stream;
    (void)ch;
    /* todo: */
    
    return 0;
}

static void s_close_cb(osl_stream_t * stream)
{
    struct container * con = (struct container*)stream->handle;
    if (con)
    {
	osl_free(con->str);
	osl_free(con);
	stream->handle = NULL;
    }
}

osl_stream_t * osl_string_stream_new(const char * str)
{
    if (str == NULL)
    {
	return NULL;
    }
    osl_stream_t * stream = osl_stream_new();
    struct container * con = (struct container*)malloc(sizeof(struct container));
    OSL_HANDLE_MALLOC_ERROR(con);
    memset(con, 0, sizeof(struct container));
    con->str = osl_strdup(str);
    con->ptr = con->str;
    stream->handle = con;
    stream->eof = 0;
    stream->read = s_read_cb;
    stream->write = s_write_cb;
    stream->close = s_close_cb;
    return stream;
}
