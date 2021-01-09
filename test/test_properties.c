#include "osl.h"
#include "properties.h"
#include "str.h"
#include "process.h"
#include <assert.h>

static void dump_file(const char * path)
{
    osl_stream_t * stream = osl_stream_open(path, "r");
    char * dump = osl_stream_dump(stream);
    printf("DUMP -- %s\n", path);
    printf("%s\n", dump);
    osl_safe_free(dump);
    osl_stream_close(stream);
    osl_stream_free(stream);
}

void test_properties(void)
{
    printf("== test properties  ==\n");

    osl_process_t * proc = osl_process_new("rm ./person.properties");
    osl_process_start(proc);
    osl_process_wait(proc);
    printf("exit code: %d\n", proc->exit_code);
    osl_process_close(proc);
    osl_process_free(proc);

    osl_properties_t * props = osl_properties_load("./person.properties");
    assert(props == NULL);

    osl_stream_t * stream = osl_stream_open("./person.properties", "w");
    osl_stream_writeline(stream, "# Person property");
    osl_stream_writeline(stream, "name=person1");
    osl_stream_writeline(stream, "age=35");
    osl_stream_close_and_free(stream);

    props = osl_properties_load("./person.properties");
    assert(props != NULL);

    assert(strcmp(osl_properties_get(props, "age"), "35") == 0);
    assert(strcmp(osl_properties_get(props, "name"), "person1") == 0);

    osl_properties_set(props, "msg", "xyz");

    osl_properties_save(props);
    dump_file(props->path);

    osl_properties_remove(props, "age");

    osl_properties_save(props);
    dump_file(props->path);

    osl_properties_free(props);
}

int main()
{
    osl_init_once();

    test_properties();
    
    osl_finish();
    
    return 0;
}
