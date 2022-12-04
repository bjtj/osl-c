#include "osl.h"
#include "byte_buffer.h"
#include <assert.h>

void test_byte_buffer(void)
{
    printf("== test byte buffer ==\n");
    osl_byte_buffer_t * bb = osl_byte_buffer_init(osl_byte_buffer_new());
    osl_byte_buffer_append(bb, (uint8_t*)"hello", 5);
    osl_byte_buffer_append(bb, (uint8_t*)" ", 1);
    osl_byte_buffer_append(bb, (uint8_t*)"world", 5);
    uint8_t * str = osl_byte_buffer_to_array(bb);
    printf("str is '%.*s'\n", (int)bb->len, (char*)str);
    assert(bb->len == 11);
    assert((char)str[0] == 'h');
    assert((char)str[1] == 'e');
    assert((char)str[2] == 'l');
    assert((char)str[3] == 'l');
    assert((char)str[4] == 'o');
    assert((char)str[5] == ' ');
    assert((char)str[6] == 'w');
    assert((char)str[7] == 'o');
    assert((char)str[8] == 'r');
    assert((char)str[9] == 'l');
    assert((char)str[10] == 'd');
    osl_safe_free(str);
    osl_byte_buffer_free(bb);
}

int main()
{
    osl_init_once();
    test_byte_buffer();
    osl_finish();
    return 0;
}
