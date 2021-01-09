#include "file.h"
#include "stream.h"
#include "pathname.h"
#include <assert.h>

void test_file()
{
    osl_stream_t * stream;
    const char * filename = ".temp.txt";
    osl_file_unlink(filename);
    assert(osl_file_exists(filename) == osl_false);
    stream = osl_stream_open(filename, "w");
    osl_stream_write_string(stream, "hello");
    osl_stream_close_and_free(stream);

    assert(osl_file_exists(filename) == osl_true);
}

static void _touch(const char * path)
{
    osl_stream_t * stream = osl_stream_open(path, "w");
    osl_stream_close_and_free(stream);
    
}

void test_file2()
{
    char * path;
    osl_list_t * list;
    osl_list_t * ptr;
    
    osl_file_mkdir("temp");
    
    path = osl_pathname_merge("temp", "a");
    _touch(path);
    assert(osl_file_exists(path) == osl_true);
    assert(osl_file_is_file(path) == osl_true);
    osl_safe_free(path);
    
    path = osl_pathname_merge("temp", "b");
    _touch(path);
    assert(osl_file_exists(path) == osl_true);
    assert(osl_file_is_file(path) == osl_true);
    osl_safe_free(path);

    path = osl_pathname_merge("temp", "subdir");
    osl_file_mkdir(path);
    assert(osl_file_exists(path) == osl_true);
    assert(osl_file_is_dir(path) == osl_true);
    osl_safe_free(path);
    
    list = osl_file_listdir("temp");
    for (ptr = list; ptr; ptr = ptr->next) {
	char * name = (char*)ptr->data;
	printf("File name: %s (%s)\n", name, osl_file_is_dir(name) ? "DIR" : "FILE");
    }
    osl_list_free(list, osl_safe_free);

    osl_file_rmdir_recursive("temp");
    assert(osl_file_exists("temp") == osl_false);
}

int main()
{
    test_file();
    //test_file2();
    return 0;
}
