#ifndef __OSL_ARGPARSE_H__
#define __OSL_ARGPARSE_H__

#include "osl.h"
#include "list.h"

typedef struct _osl_argument_flag_t
{
    char * name;
    char * defval;
    osl_bool single_flag;
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

    extern OSL_EXPORT osl_argument_flag_t * osl_argument_flag_new(void);
    extern OSL_EXPORT osl_argument_flag_t * osl_argument_flag_init(osl_argument_flag_t *, const char *, const char *, osl_bool, const char *);
    extern OSL_EXPORT void osl_argument_flag_free(osl_argument_flag_t *);
    extern OSL_EXPORT osl_bool osl_arguments_usage_is_single_flag(osl_arguments_usage_t *, const char *);
    extern OSL_EXPORT osl_argument_flag_t * osl_arguments_usage_get_flag(osl_arguments_usage_t *, const char *);
    extern OSL_EXPORT osl_arguments_usage_t * osl_arguments_usage_new(void);
    extern OSL_EXPORT osl_arguments_usage_t * osl_arguments_usage_init(osl_arguments_usage_t *);
    extern OSL_EXPORT void osl_arguments_usage_free(osl_arguments_usage_t *);
    extern OSL_EXPORT osl_argument_t * osl_argument_new(void);
    extern OSL_EXPORT osl_argument_t * osl_argument_init(osl_argument_t *, const char *, const char *);
    extern OSL_EXPORT void osl_argument_free(osl_argument_t *);
    extern OSL_EXPORT osl_arguments_t * osl_arguments_new(void);
    extern OSL_EXPORT void osl_arguments_free(osl_arguments_t *);
    extern OSL_EXPORT osl_arguments_t * osl_arguments_parse(osl_arguments_usage_t *, int, char * argv[]);

#ifdef __cplusplus
}
#endif

#endif
