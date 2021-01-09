#include "pathname.h"
#include "string_buffer.h"
#include "str.h"

#if defined(USE_UNIX_STD)

static osl_bool s_is_root_path(const char * path)
{
    return OSL_BOOL(strcmp(path, "/") == 0);
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

static osl_bool s_is_separator(char c)
{
    return OSL_BOOL(c == '/');
}

static char * s_remove_last_separator(const char * path)
{
    const char * last = path + strlen(path) - 1;
    if (!osl_string_is_empty(path) && strlen(path) > 1 && s_is_separator(*last))
    {
	return osl_string_substr(path, 0, strlen(path) - 1);
    }
    return strdup(path);
}

static char * s_dirname(const char * path)
{
    if (osl_string_is_empty(path) || s_is_directory(path) || s_is_root_path(path))
    {
	return s_remove_last_separator(path);
    }
    const char * sep = osl_string_find_last_of(path, "/");
    if (sep == NULL)
    {
	return NULL;
    }
    return osl_string_substr(path, 0, sep - path + 1);
}

static char * s_basename(const char * path)
{
    if (osl_string_is_empty(path)) {
	return NULL;
    }
    const char * f = osl_string_find_last_of(path, "/");
    if (f == NULL) {
	return strdup(path);
    }
    return osl_string_substr(path, f - path + 1, strlen(path));
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

#elif defined(USE_MS_WIN)

#define STAT_STRUCT struct _stat64
#define STAT_FUNC __stat64
#define SEPARATORS "\\/"

static osl_bool s_is_directory(const char * path);

static osl_bool s_is_separator(char c) {
    return OSL_BOOL(c == '/' || c == '\\');
}

static osl_bool s_is_root_path(const char * path) 
{
    return OSL_BOOL(osl_string_is_empty(path) == 0 && strlen(path) == 1 && s_is_separator(path[0]));
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

static char * s_remove_last_separator(const char * path) {
    if (!osl_string_is_empty(path) && strlen(path) > 1 && s_is_separator(path[strlen(path) - 1])) {
	return osl_string_substr(path, 0, strlen(path) - 1); // trailing last / character
    }
    return strdup(path);
}

static char * s_dirname(const char * path) {

    if (osl_string_is_empty(path) || s_is_directory(path) || s_is_root_path(path)) {
	return s_remove_last_separator(path);
    }

    const char * f = osl_string_find_last_of(path, SEPARATORS);
    if (f == NULL) {
	return NULL;
    }

    return osl_string_substr(path, 0, f - path + 1);
}
static char * s_basename(const char * path)
{
    if (osl_string_is_empty(path)) {
	return NULL;
    }

    const char * f = osl_string_find_last_of(path, SEPARATORS);
    if (f == NULL) {
	return strdup(path);
    }

    return osl_string_substr(path, f - path + 1, strlen(path));
}
static char * s_get_ext(const char * path)
{
    char * name = s_basename(path);
    const char * f = osl_string_find_last_of(name, ".");
    if (f == NULL || f == name) {
	free(name);
	return NULL;
    }
    free(name);
    return osl_string_substr(name, f - name + 1, strlen(path));
}

#endif


char * osl_pathname_merge(const char * a, const char * b)
{
    if (b[0] == '\\')
    {
	return strdup(b);
    }
    osl_string_buffer_t * sb = osl_string_buffer_new();
    osl_string_buffer_append(sb, a);
    if (a[strlen(a)-1] != '\\')
    {
	osl_string_buffer_append(sb, "\\");
    }
    osl_string_buffer_append(sb, b);
    return osl_string_buffer_to_string_and_free(sb);
}

char * osl_pathname_basename(const char * path)
{
    return s_basename(path);
}

char * osl_pathname_dirname(const char * path)
{
    return s_dirname(path);
}

