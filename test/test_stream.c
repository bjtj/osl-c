#include "osl.h"
#include "stream.h"
#include "pathname.h"
#include "str.h"
#include "file.h"
#include <assert.h>

void test_stream(void)
{

  printf("== test stream ==\n");
    
  char * dump;
  osl_stream_t * stream = osl_stream_open("hello", "w");
  osl_stream_write(stream, 'h');
  osl_stream_write(stream, 'e');
  osl_stream_write(stream, 'l');
  osl_stream_write(stream, 'l');
  osl_stream_write(stream, 'o');
  osl_stream_close_and_free(stream);

  assert(osl_file_exists("hello"));
  assert(osl_file_is_file("hello"));
  assert(osl_file_is_dir("hello") == 0);

  stream = osl_stream_open("hello", "r");
  dump = osl_stream_dump(stream);
  printf("dump: '%s'\n", dump);
  assert(strcmp(dump, "hello") == 0);
  osl_safe_free(dump);
  osl_stream_close_and_free(stream);

  printf("file size: %d\n", (int)osl_file_filesize("hello"));
}

void test_stream2(void)
{
  char * dump;
  const char * filename = ".test_stream2";
  osl_stream_t * stream = osl_stream_open(filename, "w");
  osl_stream_write_string(stream, "hello world");
  osl_stream_close_and_free(stream);

  stream = osl_stream_open(filename, "r");
  dump = osl_stream_dump(stream);
  assert(osl_strcmp(dump, "hello world") == 0);
  osl_safe_free(dump);
  osl_stream_close_and_free(stream);
}

void test_stream3(void)
{
  const char * filename = ".test_stream3";
  osl_stream_t * stream = osl_stream_open(filename, "wb");
  size_t len;
  int arr[4] = {1,2,3,4};
  int buf[4] = {0,};
  len = osl_stream_write_buffer(stream, arr, sizeof(arr));
  assert(len == sizeof(arr));
  osl_stream_close_and_free(stream);

  stream = osl_stream_open(filename, "rb");
  len = osl_stream_read_buffer(stream, buf, sizeof(buf));
  assert(buf[0] == arr[0]);
  assert(buf[1] == arr[1]);
  assert(buf[2] == arr[2]);
  assert(buf[3] == arr[3]);
  osl_stream_close_and_free(stream);
}

int main()
{
  test_stream();
  test_stream2();
  return 0;
}
