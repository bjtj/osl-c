#include "osl.h"
#include "byte_stream.h"
#include <assert.h>

void test_byte_stream(void)
{
  printf("== test string stream ==\n");
  uint8_t arr[6] = {0, 1, 2, 3, 4, 5};
  osl_stream_t * stream = osl_byte_stream_init(osl_stream_new(), arr, sizeof(arr));
  assert(0 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(1 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(2 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(3 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(4 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(5 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(-1 == osl_stream_read(stream));
  assert(stream->eof == 1);
  osl_stream_close_and_free(stream);
}

void test_byte_stream2(void)
{
  printf("== test string stream ==\n");
  uint8_t arr[6] = {0, 1, 2, 3, 4, 5};
  osl_stream_t * stream = osl_byte_stream_init(osl_stream_new(), arr, sizeof(arr));
  assert(0 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(1 == osl_stream_read(stream));
  assert(stream->eof == 0);
  assert(2 == osl_stream_read(stream));
  assert(stream->eof == 0);

  uint8_t buf[6] = {0,};
  size_t read_size = osl_stream_read_buffer(stream, buf, sizeof(buf));
  assert(3 == read_size);
  assert(3 == buf[0]);
  assert(4 == buf[1]);
  assert(5 == buf[2]);
  assert(0 == buf[3]);

  assert(-1 == osl_stream_read(stream));
  assert(stream->eof == 1);
  osl_stream_close_and_free(stream);
}

int main(void)
{
  osl_init_once();
  test_byte_stream();
  osl_finish();
    
  return 0;
}
