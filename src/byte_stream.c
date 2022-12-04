#include "byte_stream.h"
#include "str.h"

struct container
{
  uint8_t * data;
  uint8_t * ptr;
  size_t capacity;
};

static int s_read_cb(osl_stream_t * stream)
{
  struct container * con = (struct container*)stream->handle;
  if (con->ptr >= con->data + con->capacity) {
    stream->eof = 1;
    return -1;
  }
  return *(con->ptr++);
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
    osl_safe_free(con->data);
    osl_safe_free(con);
    stream->handle = NULL;
  }
}


osl_stream_t * osl_byte_stream_init(osl_stream_t * stream, uint8_t * data, size_t size)
{
  struct container * con;
  if (data == NULL) {
    return NULL;
  }
  con = (struct container*)malloc(sizeof(struct container));
  OSL_HANDLE_MALLOC_ERROR(con);
  memset(con, 0, sizeof(struct container));
  con->data = malloc(size);
  memcpy(con->data, data, size);
  con->capacity = size;
  con->ptr = con->data;
  stream->handle = con;
  stream->eof = 0;
  stream->read = s_read_cb;
  stream->write = s_write_cb;
  stream->close = s_close_cb;
  return stream;
}
