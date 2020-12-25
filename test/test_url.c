#include "osl.h"
#include "url.h"
#include <assert.h>

void test_url(void)
{
    printf("== test url ==\n");
    {
	osl_url_t * url = osl_url_from_string("http://localhost:5000");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost:5000/resource");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost/resource");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost/resource?query");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://localhost/resource?a=A&b=B");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {
	osl_url_t * url = osl_url_from_string("http://username:password@localhost/resource");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
    }

    {

	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {

	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c/");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {
	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c");
	osl_url_t * url = osl_url_merge(base_url, "/d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(base_url);
	osl_url_free(url);
    }


    {
	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c?query");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {

	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c/?query");
	osl_url_t * url = osl_url_merge(base_url, "d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(url);
	osl_url_free(base_url);
    }

    {
	osl_url_t * base_url = osl_url_from_string("http://127.0.0.1/a/b/c?query");
	osl_url_t * url = osl_url_merge(base_url, "/d");
	char * url_str = osl_url_to_string(url);
	printf("url -- %s\n", url_str);
	free(url_str);
	osl_url_free(base_url);
	osl_url_free(url);
    }
}

int main(int argc, char *argv[])
{
    osl_init_once();

    test_url();

    osl_finish();
    
    return 0;
}
