#include "file.h"
#include "str.h"
#include "string_buffer.h"

#if defined(USE_UNIX_STD)

static osl_filesize_t s_get_file_size(const char * path)
{
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    lstat(path, &st);
    return st.st_size;
}

#elif defined(USE_MS_WIN)

#define STAT_STRUCT struct _stat64
#define STAT_FUNC __stat64

static osl_filesize_t s_get_file_size(const char * path) {

    STAT_STRUCT st;
    memset(&st, 0, sizeof(STAT_STRUCT));
    int ret = STAT_FUNC(path, &st);
    if (ret != 0) {
	return 0;
    }

    return (osl_filesize_t)st.st_size;
}

#else

/* unknown */

#endif


FILE * osl_file_open(const char * path, const char * flags)
{
#if defined(USE_MS_WIN)
    FILE * fp = NULL;
    if (fopen_s(&fp, path, flags) != 0) {
	return NULL;
    }
    return fp;
#else
    return fopen(path, flags);
#endif
}

void osl_file_close(FILE * fp)
{
    if (fp)
    {
	fclose(fp);
    }
}

osl_filesize_t osl_file_size(const char * path)
{
    return s_get_file_size(path);
}

char * osl_file_dump(FILE * stream)
{
    osl_string_buffer_t * sb = osl_string_buffer_new();
    char * ret;

    while (1)
    {
	char buffer[1024] = {0,};
	/* Reference (IBM): https://www.ibm.com/support/knowledgecenter/ko/ssw_ibm_i_73/rtref/fread.htm */
	int num = fread(buffer, 1, sizeof(buffer) - 1, stream);
	if (num <= 0)
	{
	    if (ferror(stream))
	    {
		perror("Error reading file");
	    } else if (feof(stream))
	    {
		perror("EOF found");
	    }
	    break;
	}

	osl_string_buffer_append(sb, buffer);
    }

    ret = osl_string_buffer_to_string(sb);
    osl_string_buffer_free(sb);
    return ret;
}

