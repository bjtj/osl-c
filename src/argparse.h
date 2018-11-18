#ifndef __OSL_ARGPARSE_H__
#define __OSL_ARGPARSE_H__

#include "osl.h"
#include "list.h"

typedef struct _osl_argument_flag_t
{
    char * name;
    char * defval;
    int single_flag;
    char * description;
} osl_argument_flag_t;

typedef struct _osl_arguments_usage_t
{
    osl_list_t * flag_list;
} osl_arguments_usage_t;

typedef struct _osl_argument_t
{
    char * name;
    char * value;
} osl_argument_t;

typedef struct _osl_arguments_t
{
    char * name;
    osl_list_t * arg_list;
    osl_list_t * text_list;
} osl_arguments_t;

#ifdef __cplusplus
extern "C" {
#endif

    osl_argument_flag_t OSL_EXPORT * osl_argument_flag_new(const char * name, const char * defval, int single, const char * desc);
    void OSL_EXPORT osl_argument_flag_free(osl_argument_flag_t * flag);
    int OSL_EXPORT osl_arguments_usage_is_single_flag(osl_arguments_usage_t * usage, const char * name);
    osl_argument_flag_t OSL_EXPORT * osl_arguments_usage_get_flag(osl_arguments_usage_t * usage, const char * name);
    osl_arguments_usage_t OSL_EXPORT * osl_arguments_usage_new(void);
    void OSL_EXPORT osl_arguments_usage_free(osl_arguments_usage_t * usage);
    osl_argument_t OSL_EXPORT * osl_argument_new(const char * name, const char * value);
    void OSL_EXPORT osl_argument_free(osl_argument_t * arg);
    osl_arguments_t OSL_EXPORT * osl_arguments_new(void);
    void OSL_EXPORT osl_arguments_free(osl_arguments_t * args);
    osl_arguments_t OSL_EXPORT * osl_arguments_parse(osl_arguments_usage_t * usage, int argc, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
