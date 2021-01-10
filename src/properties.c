#include "properties.h"
#include "str.h"
#include "stream.h"
#include "string_buffer.h"

static osl_bool __property_cmp_name(osl_property_t * prop, const char * name)
{
    if (prop->comment)
    {
	return osl_false;
    }

    return osl_string_equals(prop->name, name);
}

osl_property_t * osl_property_new(const char * name, const char * value)
{
    osl_property_t * prop = (osl_property_t*)malloc(sizeof(osl_property_t));
    OSL_HANDLE_MALLOC_ERROR(prop);
    memset(prop, 0, sizeof(osl_property_t));
    prop->name = osl_safe_strdup(name);
    prop->value = osl_safe_strdup(value);
    return prop;
}

osl_property_t * osl_property_new_comment(const char * comment)
{
    osl_property_t * prop = (osl_property_t*)malloc(sizeof(osl_property_t));
    OSL_HANDLE_MALLOC_ERROR(prop);
    memset(prop, 0, sizeof(osl_property_t));
    prop->comment = osl_safe_strdup(comment);
    return prop;
}

void osl_property_free(osl_property_t * prop)
{
    if (prop == NULL)
    {
	return;
    }
    osl_safe_free(prop->comment);
    osl_safe_free(prop->name);
    osl_safe_free(prop->value);
    osl_safe_free(prop);
}

void osl_property_set_name(osl_property_t * prop, const char * name)
{
    osl_safe_free(prop->name);
    prop->name = osl_safe_strdup(name);
}

void osl_property_set_value(osl_property_t * prop, const char * value)
{
    osl_safe_free(prop->value);
    prop->name = osl_safe_strdup(value);
}

osl_properties_t * osl_properties_new(void)
{
    osl_properties_t * props = (osl_properties_t *)malloc(sizeof(osl_properties_t));
    OSL_HANDLE_MALLOC_ERROR(props);
    memset(props, 0, sizeof(osl_properties_t));
    return props;
}

osl_properties_t * osl_properties_load(const char * path)
{
    osl_stream_t * stream = osl_stream_open(path, "r");
    if (osl_stream_is_open(stream) == 0)
    {
	return NULL;
    }
    osl_properties_t * props = osl_properties_new();
    props->path = osl_safe_strdup(path);

    char * line;
    while ((line = osl_stream_readline(stream)) != NULL)
    {
	const char * start = osl_string_find_first_not_of(line, " \t");
	if (start == NULL)
	{
	    osl_safe_free(line);
	    continue;
	}
	if (start[0] == '#')
	{
	    props->properties = osl_list_append(props->properties, osl_property_new_comment(start));
	    osl_safe_free(line);
	    continue;
	}
	char * ptr = strstr(line, "=");
	if (ptr)
	{
	    char * name = osl_strdup_for(line, ptr);
	    char * value = osl_safe_strdup(ptr+1);
	    props->properties = osl_list_append(props->properties, osl_property_new(name, value));
	    osl_safe_free(name);
	    osl_safe_free(value);
	}
	else
	{
	    props->properties = osl_list_append(props->properties, osl_property_new(line, NULL));
	}
	osl_safe_free(line);
    }

    osl_stream_close_and_free(stream);
    return props;
}

int osl_properties_save(osl_properties_t * props)
{
    osl_stream_t * stream;
    osl_list_t * node;
    if (props == NULL) {
	return -1;
    }
    if (props->path == NULL)
    {
	return -1;
    }
    stream = osl_stream_open(props->path, "w");
    if (osl_stream_is_open(stream) == 0)
    {
	return -1;
    }
    node = props->properties;
    for (; node; node = node->next)
    {
	osl_property_t * prop = (osl_property_t*)node->data;
	osl_string_buffer_t * sb = osl_string_buffer_new();
	if (prop->comment)
	{
	    osl_string_buffer_append(sb, prop->comment);
	}
	else
	{
	    osl_string_buffer_append(sb, prop->name);
	    if (prop->value)
	    {
		osl_string_buffer_append(sb, "=");
		osl_string_buffer_append(sb, prop->value);
	    }
	}
	char * line = osl_string_buffer_to_string_and_free(sb);
	osl_stream_writeline(stream, line);
	osl_safe_free(line);
    }
    osl_stream_close_and_free(stream);
    return 0;
}

void osl_properties_free(osl_properties_t * props)
{
    if (props == NULL)
    {
	return;
    }

    osl_safe_free(props->path);
    osl_properties_clear(props);
    osl_safe_free(props);
}

void osl_properties_clear(osl_properties_t * props)
{
    osl_list_free(props->properties, (osl_free_cb)osl_property_free);
    props->properties = NULL;
}

char * osl_properties_get(osl_properties_t * props, const char * name)
{
    osl_list_t * node = props->properties;
    for (; node; node = node->next)
    {
	osl_property_t * prop =  (osl_property_t*)node->data;

	if (prop->comment)
	{
	    continue;
	}
	
	if (strcmp(prop->name, name) == 0)
	{
	    return prop->value;
	}
    }
    return NULL;
}

void osl_properties_set(osl_properties_t * props, const char * name, const char * value)
{
    osl_list_t * node = props->properties;
    for (; node; node = node->next)
    {
	osl_property_t * prop =  (osl_property_t*)node->data;
	if (prop->comment)
	{
	    continue;
	}
	
	if (strcmp(prop->name, name) == 0)
	{
	    osl_property_set_value(prop, value);
	}
    }
    props->properties = osl_list_append(props->properties, osl_property_new(name, value));
}

void osl_properties_remove(osl_properties_t * props, const char * name)
{
    props->properties = osl_list_remove_if(props->properties, (osl_compare_cb)__property_cmp_name, (void*)name, (osl_free_cb)osl_property_free);
}
