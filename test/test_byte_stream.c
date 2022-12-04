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

int main()
{
    osl_init_once();
    test_byte_stream();
    osl_finish();
    
    return 0;
}
