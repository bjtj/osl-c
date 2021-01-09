#ifndef __OSL_PROPERTIES_H__
#define __OSL_PROPERTIES_H__

#include "osl.h"
#include "list.h"

typedef struct _osl_property_t
{
    char * comment;
    char * name;
    char * value;
} osl_property_t;

typedef struct _osl_properties_t
{
    char * path;
    osl_list_t * properties;
} osl_properties_t;

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_property_t * osl_property_new(const char * name, const char * value);
    extern OSL_EXPORT osl_property_t * osl_property_new_comment(const char * comment);
    extern OSL_EXPORT void osl_property_free(osl_property_t * prop);
    extern OSL_EXPORT void osl_property_set_name(osl_property_t * prop, const char * name);
    extern OSL_EXPORT void osl_property_set_value(osl_property_t * prop, const char * value);
    extern OSL_EXPORT osl_properties_t * osl_properties_new(void);
    extern OSL_EXPORT osl_properties_t * osl_properties_load(const char * path);
    extern OSL_EXPORT int osl_properties_save(osl_properties_t * props);
    extern OSL_EXPORT void osl_properties_free(osl_properties_t * props);
    extern OSL_EXPORT void osl_properties_clear(osl_properties_t * props);
    extern OSL_EXPORT char * osl_properties_get(osl_properties_t * props, const char * name);
    extern OSL_EXPORT void osl_properties_set(osl_properties_t * props, const char * name, const char * value);
    extern OSL_EXPORT void osl_properties_remove(osl_properties_t * props, const char * name);

#ifdef __cplusplus
}
#endif

#endif
