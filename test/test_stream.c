#include "osl.h"
#include "stream.h"
#include "pathname.h"
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

    assert(osl_pathname_exists("hello"));
    assert(osl_pathname_is_file("hello"));
    assert(osl_pathname_is_dir("hello") == 0);

    stream = osl_stream_open("hello", "r");
    dump = osl_stream_dump(stream);
    printf("dump: '%s'\n", dump);
    assert(strcmp(dump, "hello") == 0);
    free(dump);
    osl_stream_close_and_free(stream);

    printf("file size: %d\n", (int)osl_pathname_filesize("hello"));
}

int main(int argc, char *argv[])
{
    osl_init_once();

    test_stream();
    
    osl_finish();
    
    return 0;
}
