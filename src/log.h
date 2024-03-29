#ifndef __OSL_LOG_H__
#define __OSL_LOG_H__

#include "osl.h"

#define OSL_LOG_TRACE 0
#define OSL_LOG_DEBUG 1
#define OSL_LOG_INFO 2
#define OSL_LOG_WARN 3
#define OSL_LOG_ERROR 4
#define OSL_LOG_FATAL 5

typedef struct _osl_logger_t
{
  char prefix[10];
  int level;
} osl_logger_t;

#define osl_log_trace(...) osl_log_print(osl_log_default_logger(), OSL_LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define osl_log_debug(...) osl_log_print(osl_log_default_logger(), OSL_LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define osl_log_info(...) osl_log_print(osl_log_default_logger(), OSL_LOG_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define osl_log_warn(...) osl_log_print(osl_log_default_logger(), OSL_LOG_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define osl_log_error(...) osl_log_print(osl_log_default_logger(), OSL_LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define osl_log_fatal(...) osl_log_print(osl_log_default_logger(), OSL_LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT osl_logger_t * osl_log_default_logger(void);
  extern OSL_EXPORT void osl_log_print(osl_logger_t *, int, const char *, int, const char *, ...);

#ifdef __cplusplus
}
#endif

#endif
