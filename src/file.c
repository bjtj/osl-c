#include "file.h"
#include "string_buffer.h"
#include "str.h"
#include "pathname.h"

#if defined(USE_UNIX_STD)

static osl_bool s_exists(const char * path)
{
    if (osl_string_is_empty(path)) {
	return osl_false;
    }
		
    // http://stackoverflow.com/questions/12774207/fastest-way-to-check-if-a-file-exist-using-standard-c-c11-c
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    return OSL_BOOL(stat(path, &st) == 0);
}

static osl_bool s_is_file(const char * path)
{
    if (osl_string_is_empty(path)) {
	return osl_false;
    }
    // http://stackoverflow.com/questions/3536781/accessing-directories-in-c/3536853#3536853
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    lstat(path, &st);
    return OSL_BOOL(S_ISDIR(st.st_mode) ? 0 : 1);
}

static osl_bool s_is_directory(const char * path)
{
    if (osl_string_is_empty(path)) {
	return osl_false;
    }
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    lstat(path, &st);
    return OSL_BOOL(S_ISDIR(st.st_mode) ? 1 : 0);
}

/* static char * s_get_ext(const char * path) */
/* { */
/*     char * name = s_basename(path); */
/*     const char * f = osl_string_find_last_of(name, "."); */
/*     if (f == NULL || f == name) { */
/* 	return NULL; */
/*     } */
/*     return osl_string_substr(name, f - name + 1, strlen(name)); */
/* } */

static osl_filesize_t s_get_file_size(const char * path)
{
    struct stat st;
    memset(&st, 0, sizeof(struct stat));
    lstat(path, &st);
    return st.st_size;
}

// http://stackoverflow.com/questions/2336242/recursive-mkdir-system-call-on-unix
static int s_mkdir(const char * dir, mode_t mode) {
	
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);

    if(tmp[len - 1] == '/') {
	tmp[len - 1] = 0;
    }

    for(p = tmp + 1; *p; p++) {
	if(*p == '/') {
	    *p = 0;
	    mkdir(tmp, mode); // ignore error (just try)
	    *p = '/';
	}
    }
	
    return mkdir(tmp, mode);
}

static osl_bool s_remove_file(const char * path) {
    return OSL_BOOL(remove(path) == 0);
}

static osl_list_t * s_list(const char * path) {
    osl_list_t * ret = NULL;
    struct dirent * ent = NULL;
    struct dirent ** list = NULL;
    int i;
    int cnt;
    cnt = scandir(path, &list, NULL, alphasort);
    if (cnt < 0) {
	return ret;
    }
    for (i = 0; i < cnt; i++) {
	ent = list[i];
	if (osl_strcmp(ent->d_name, ".") != 0 && osl_strcmp(ent->d_name, "..") != 0) {
	    ret = osl_list_append(ret, osl_pathname_merge(path, ent->d_name));
	}
	osl_safe_free(ent);
    }
    osl_safe_free(list);
    return ret;
}

#elif defined(USE_MS_WIN)

#define STAT_STRUCT struct _stat64
#define STAT_FUNC __stat64
#define SEPARATORS "\\/"

static osl_bool s_is_file(const char * path);
static osl_bool s_is_directory(const char * path);

static osl_bool s_exists(const char * path) 
{
    if (osl_string_is_empty(path)) 
    {
	return osl_false;
    }

    if (s_is_directory(path) || s_is_file(path))
    {
	return osl_true;
    }

    return osl_false;
}
static osl_bool s_is_file(const char * path)
{
    if (osl_string_is_empty(path)) {
	return osl_false;
    }

    STAT_STRUCT s;
    memset(&s, 0, sizeof(STAT_STRUCT));
    if (STAT_FUNC(path, &s) != 0) {
	// error
	return osl_false;
    }

    return OSL_BOOL(s.st_mode & S_IFREG ? 1 : 0);
}
static osl_bool s_is_directory(const char * path) {

    if (osl_string_is_empty(path)) {
	return osl_false;
    }

    STAT_STRUCT s;
    memset(&s, 0, sizeof(STAT_STRUCT));
    if (STAT_FUNC(path, &s) != 0) {
	// error
	return osl_false;
    }

    return OSL_BOOL(s.st_mode & S_IFDIR ? 1 : 0);
}


static osl_filesize_t s_get_file_size(const char * path)
{
    STAT_STRUCT st;
    memset(&st, 0, sizeof(STAT_STRUCT));
    int ret = STAT_FUNC(path, &st);
    if (ret != 0) {
	return 0;
    }

    return (osl_filesize_t)st.st_size;
}

static osl_bool s_is_separator(char c) {
    const char* ptr = SEPARATORS;
    while (*ptr) {
        if (c == *ptr)
        {
            return osl_true;
        }
        ptr++;
    }
    return osl_false;
}

// http://stackoverflow.com/questions/2336242/recursive-mkdir-system-call-on-unix
static int s_mkdir(const char *dir, int mode) {
	
    // https://msdn.microsoft.com/en-us/library/2fkk4dzw.aspx
    (void)mode;

    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp),"%s",dir);
    len = strlen(tmp);

    if(s_is_separator(tmp[len - 1])) {
	tmp[len - 1] = 0;
    }

    for(p = tmp + 1; *p; p++) {
	if(s_is_separator(*p)) {
	    *p = 0;
        if (_mkdir(tmp) != 0)
        {
            /* TODO: handling */
        }
	    *p = SEPARATORS[0];
	}
    }
	
    return _mkdir(tmp);
}

static osl_bool s_remove_file(const char * path) {
    return OSL_BOOL(DeleteFile(path) == TRUE);
}

static osl_list_t * s_list(const char * path) {

    osl_list_t * ret = NULL;

    char * dir = osl_string_append(path, "\\*");

    WIN32_FIND_DATAA ffd;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    
    hFind = FindFirstFileA(dir, &ffd);
    if (hFind == INVALID_HANDLE_VALUE) {
	/* TODO: exception */
	goto end;
    }

    do {
        if (osl_strcmp(ffd.cFileName, ".") != 0 && osl_strcmp(ffd.cFileName, "..") != 0) {
            ret = osl_list_append(ret, osl_pathname_merge(path, ffd.cFileName));
        }
    } while (FindNextFileA(hFind, &ffd) != 0);

    FindClose(hFind);

end:
    osl_safe_free(dir);
    return ret;
}

#endif


osl_list_t * osl_file_listdir(const char * path)
{
    return s_list(path);
}

osl_bool osl_file_mkdir(const char * path)
{
    return OSL_BOOL(s_mkdir(path, 0755) == 0);
}

osl_bool osl_file_rmdir(const char * path)
{
#if defined(USE_MS_WIN)
    return OSL_BOOL(_rmdir(path) == 0);
#else
    return OSL_BOOL(rmdir(path) == 0);
#endif
}

osl_bool osl_file_unlink(const char * path)
{
    return s_remove_file(path);
}

osl_bool osl_file_rmdir_recursive(const char * path)
{
    osl_list_t * list;
    osl_list_t * ptr;
    if (osl_file_is_file(path)) {
	    return osl_file_unlink(path);
    }
    list = osl_file_listdir(path);
    ptr = list;
    for (; ptr; ptr = ptr->next) {
	char * subpath = (char*)ptr->data;
	if (osl_file_is_file(subpath)) {
	    osl_file_unlink(subpath);
	} else {
	    osl_file_rmdir_recursive(subpath);
	}
    }
    osl_list_free(list, osl_safe_free);
    return osl_file_rmdir(path);
}

osl_filesize_t osl_file_filesize(const char * path)
{
    return s_get_file_size(path);
}

osl_bool osl_file_exists(const char * path)
{
    return s_exists(path);
}

osl_bool osl_file_is_dir(const char * path)
{
    return s_is_directory(path);
}

osl_bool osl_file_is_file(const char * path)
{
    return s_is_file(path);
}
