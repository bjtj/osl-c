#include <assert.h>
#include "error.h"
#include "stream.h"

int main()
{
	osl_stream_t * stream = osl_stream_open("!@#!!@!!@#!#!@#", "rb");
	if (!stream) {
    char buf[256] = {0,};
		osl_get_last_error_message(buf, sizeof(buf));
		printf("%s\n", buf);
		return 0;
	}

	return 1;
}
