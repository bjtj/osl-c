#include <assert.h>
#include "shared_memory.h"
#include "str.h"

#define NAME "MYMAP"
#define BUF_SIZE 256
#define MSG "Hello World"

static void s_write(void);
static void s_read(void);


static void s_write(void)
{
	char * buf;
	OSL_SHARED_MEMORY_ID id = osl_shared_memory_create(NAME, BUF_SIZE);
	assert(OSL_VALID_SHARED_MEMORY_ID(id));
	buf = osl_shared_memory_map(id, BUF_SIZE);
	assert(buf);
	osl_memcopy(buf, MSG, osl_strlen(MSG));

	s_read();

	osl_shared_memory_unmap(buf, BUF_SIZE);
	osl_shared_memory_unlink(NAME);
	osl_shared_memory_close(id);
}

static void s_read(void)
{
	char * buf;
	OSL_SHARED_MEMORY_ID id = osl_shared_memory_open(NAME);
	assert(OSL_VALID_SHARED_MEMORY_ID(id));
	buf = osl_shared_memory_map(id, BUF_SIZE);
	assert(buf);
	assert(osl_strncmp(buf, MSG, osl_strlen(MSG)) == 0);
	osl_shared_memory_unmap(buf, BUF_SIZE);
	osl_shared_memory_close(id);
}


int main()
{
	s_write();
	return 0;
}
