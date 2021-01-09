#ifndef __FILE_H__
#define __FILE_H__

#include "osl.h"
#include "list.h"

#ifdef __cplusplus
extern "C" {
#endif

    extern OSL_EXPORT osl_list_t * osl_file_listdir(const char * path);
    extern OSL_EXPORT osl_bool osl_file_mkdir(const char * path);
    extern OSL_EXPORT osl_bool osl_file_rmdir(const char * path);
    extern OSL_EXPORT osl_bool osl_file_unlink(const char * path);
    extern OSL_EXPORT osl_bool osl_file_rmdir_recursive(const char * path);
    extern OSL_EXPORT osl_filesize_t osl_file_filesize(const char * path);
    extern OSL_EXPORT osl_bool osl_file_exists(const char * path);
    extern OSL_EXPORT osl_bool osl_file_is_dir(const char * path);
    extern OSL_EXPORT osl_bool osl_file_is_file(const char * path);

#ifdef __cplusplus
}
#endif

#endif /* __FILE_H__ */
