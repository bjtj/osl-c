#ifndef __OSL_PROCESS_H__
#define __OSL_PROCESS_H__

#include "osl.h"
#include "stream.h"

typedef struct _osl_process_t
{
  int exit_code;
  char * cmd;
  int opened;
#if defined(USE_UNIX_STD)
  pid_t pid;
  FILE * fdin;
  FILE * fdout;
  FILE * fderr;
  int pipe_in[2];
  int pipe_out[2];
  int pipe_err[2];
#elif defined(USE_MS_WIN)
  PROCESS_INFORMATION pi_proc_info;
  HANDLE in_read;
  HANDLE in_write;
  HANDLE out_read;
  HANDLE out_write;
  HANDLE err_read;
  HANDLE err_write;
#endif
    
} osl_process_t;

#ifdef __cplusplus
extern "C" {
#endif

  extern OSL_EXPORT osl_process_t * osl_process_new(void);
  extern OSL_EXPORT osl_process_t * osl_process_init(osl_process_t *, const char *);
  extern OSL_EXPORT void osl_process_free(osl_process_t *);
  extern OSL_EXPORT int osl_process_start(osl_process_t *);
  extern OSL_EXPORT void osl_process_close(osl_process_t *);
  extern OSL_EXPORT int osl_process_exited(osl_process_t *);
  extern OSL_EXPORT void osl_process_wait(osl_process_t *);
  extern OSL_EXPORT osl_stream_t * osl_process_in_stream(osl_process_t *);
  extern OSL_EXPORT osl_stream_t * osl_process_out_stream(osl_process_t *);
  extern OSL_EXPORT osl_stream_t * osl_process_err_stream(osl_process_t *);

#ifdef __cplusplus
}
#endif

#endif
