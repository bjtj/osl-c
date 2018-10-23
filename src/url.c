#include "url.h"
#include "str.h"
#include "string_buffer.h"

osl_url_t * osl_url_from_string(const char * url_str)
{
    osl_url_t * url = (osl_url_t*)malloc(sizeof(osl_url_t));
    memset(url, 0, sizeof(osl_url_t));
    char * sep = strstr(url_str, "://");
    char * address_part = NULL;
    char * auth_end = NULL;
    char * auth_sep = NULL;
    char * auth_part = NULL;
    char * path_start = NULL;
    char * address_sep = NULL;
    char * query_start = NULL;
    if (sep == NULL)
    {
	goto err;
    }
    url->protocol = osl_strdup_for(url_str, sep);
    auth_end = strstr(sep + 3, "@");
    if (auth_end)
    {
	auth_part = osl_strdup_for(sep + 3, auth_end);
	auth_sep = strstr(auth_part, ":");
	url->username = osl_strdup_for(auth_part, auth_sep);
	if (auth_sep)
	{
	    url->passphase = osl_strdup(auth_sep + 1);
	}
	
	path_start = strstr(auth_end + 1, "/");
	address_part = osl_strdup_for(auth_end + 1, path_start);
    }
    else
    {
	path_start = strstr(sep + 3, "/");
	address_part = osl_strdup_for(sep + 3, path_start);
    }

    address_sep = strstr(address_part, ":");
    url->hostname = osl_strdup_for(address_part, address_sep);
    if (address_sep)
    {
	url->port = osl_strdup(address_sep + 1);
    }

    if (path_start)
    {
	query_start = strstr(path_start, "?");
	url->path = osl_strdup_for(path_start, query_start);
	if (query_start)
	{
	    url->query = osl_strdup(query_start + 1);
	}
    }

    goto done;

err:
    osl_url_free(url);
    url = NULL;
done:
    osl_free(auth_part);
    osl_free(address_part);

    return url;
}

void osl_url_free(osl_url_t * url)
{
    if (url == NULL)
    {
	return;
    }

    osl_free(url->protocol);
    osl_free(url->username);
    osl_free(url->passphase);
    osl_free(url->hostname);
    osl_free(url->port);
    osl_free(url->path);
    osl_free(url->query);
    osl_free(url);
}

osl_url_t * osl_url_merge(osl_url_t * base_url, const char * relative)
{
    osl_url_t * url = (osl_url_t*)malloc(sizeof(osl_url_t));
    memset(url, 0, sizeof(osl_url_t));
    if (relative[0] == '/')
    {
	url->protocol = osl_strdup(base_url->protocol);
	url->username = osl_strdup(base_url->username);
	url->passphase = osl_strdup(base_url->passphase);
	url->hostname = osl_strdup(base_url->hostname);
	url->query = osl_strdup(base_url->query);
	url->port = osl_strdup(base_url->port);
	url->path = osl_strdup(relative);	
    }
    else
    {
	url->protocol = osl_strdup(base_url->protocol);
	url->username = osl_strdup(base_url->username);
	url->passphase = osl_strdup(base_url->passphase);
	url->hostname = osl_strdup(base_url->hostname);
	url->port = osl_strdup(base_url->port);
	url->query = osl_strdup(base_url->query);
	if (base_url->path)
	{
	    osl_string_buffer_t * sb = osl_string_buffer_new();
	    osl_string_buffer_append(sb, base_url->path);
	    if (base_url->path[strlen(base_url->path) - 1] != '/')
	    {
		osl_string_buffer_append(sb, "/");
	    }
	    osl_string_buffer_append(sb, relative);
	    url->path = osl_string_buffer_to_string_and_free(sb);
	}
	else
	{
	    osl_string_buffer_t * sb = osl_string_buffer_new();
	    osl_string_buffer_append(sb, "/");
	    osl_string_buffer_append(sb, relative);
	    url->path = osl_string_buffer_to_string_and_free(sb);
	}
    }
    return url;
}


char * osl_url_to_string(osl_url_t * url)
{
    osl_string_buffer_t * sb = osl_string_buffer_new();
    osl_string_buffer_append(sb, url->protocol);
    osl_string_buffer_append(sb, "://");
    if (url->username)
    {
	osl_string_buffer_append(sb, url->username);
	if (url->passphase)
	{
	    osl_string_buffer_append(sb, ":");
	    osl_string_buffer_append(sb, url->passphase);
	}
	osl_string_buffer_append(sb, "@");
    }
    osl_string_buffer_append(sb, url->hostname);
    if (url->port)
    {
	osl_string_buffer_append(sb, ":");
	osl_string_buffer_append(sb, url->port);
    }
    if (url->path)
    {
	osl_string_buffer_append(sb, url->path);
    }
    if (url->query)
    {
	osl_string_buffer_append(sb, "?");
	osl_string_buffer_append(sb, url->query);
    }
    return osl_string_buffer_to_string_and_free(sb);
}
