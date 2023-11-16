#ifndef __ERROR_H__
#define __ERROR_H__

#include "osl.h"

#ifdef __cplusplus
extern "C" {
#endif

	extern OSL_EXPORT size_t osl_get_last_error_message(char*, size_t);

#ifdef __cplusplus
}
#endif

#endif /* __ERROR_H__ */
