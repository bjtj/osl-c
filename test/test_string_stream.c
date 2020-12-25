#include "osl.h"
#include "string_stream.h"
#include <assert.h>

void test_string_stream(void)
{

    printf("== test string stream ==\n");
    
    const char * str = "hello world\nbye.";
    osl_stream_t * stream = osl_string_stream_new(str);
    char * line = osl_stream_readline(stream);
    assert(strcmp(line, "hello world") == 0);
    osl_free(line);
    line = osl_stream_readline(stream);
    assert(strcmp(line, "bye.") == 0);
    osl_free(line);
    assert(stream->eof);
    osl_stream_close_and_free(stream);
}

int main(int argc, char *argv[])
{
    osl_init_once();

    test_string_stream();
    
    osl_finish();
    
    return 0;
}
