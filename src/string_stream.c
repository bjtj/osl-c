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
  if (stream->eof) {
    return -1;
  }
  char ch = *(con->ptr++);
  if (ch == 0)
  {
    stream->eof = 1;
    return -1;
  }
  return ch;
}

static int s_write_cb(osl_stream_t * stream, int ch)
{
  (void)stream;
  (void)ch;
  /* todo: */
    
  return -1;
}

static void s_close_cb(osl_stream_t * stream)
{
  struct container * con = (struct container*)stream->handle;
  if (con)
  {
    osl_safe_free(con->str);
    osl_safe_free(con);
    stream->handle = NULL;
  }
}


osl_stream_t * osl_string_stream_init(osl_stream_t * stream, const char * str)
{
  struct container * con;
  if (str == NULL) {
    return NULL;
  }
  con = (struct container*)malloc(sizeof(struct container));
  OSL_HANDLE_MALLOC_ERROR(con);
  memset(con, 0, sizeof(struct container));
  con->str = osl_safe_strdup(str);
  con->ptr = con->str;
  stream->handle = con;
  stream->eof = 0;
  stream->read = s_read_cb;
  stream->write = s_write_cb;
  stream->close = s_close_cb;
  return stream;
}
