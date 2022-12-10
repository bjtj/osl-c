#include "process.h"
#include "string_buffer.h"
#include "str.h"

osl_process_t * osl_process_new(void)
{
  osl_process_t * process = (osl_process_t*)malloc(sizeof(osl_process_t));
  OSL_HANDLE_MALLOC_ERROR(process);
  memset(process, 0, sizeof(osl_process_t));
    
  return process;
}

osl_process_t * osl_process_init(osl_process_t * process, const char * cmd)
{
  process->cmd = osl_safe_strdup(cmd);
  return process;
}

void osl_process_free(osl_process_t * process)
{
  if (process == NULL)
  {
    return;
  }

  osl_process_close(process);

  if (process->cmd)
  {
    osl_safe_free(process->cmd);
  }

  osl_safe_free(process);
}

#if defined(USE_UNIX_STD)

int osl_process_start(osl_process_t * process)
{
  pid_t pid = 0;

  if (pipe(process->pipe_in) != 0 || pipe(process->pipe_out) != 0 || pipe(process->pipe_err) != 0) {
    perror("pipe() failed");
    return 1;
  }
	
  pid = fork();
  if (pid == 0) {

    // child

    close(process->pipe_in[1]);
    close(process->pipe_out[0]);
    close(process->pipe_err[0]);

    dup2(process->pipe_in[0], STDIN_FILENO);
    dup2(process->pipe_out[1], STDOUT_FILENO);
    dup2(process->pipe_err[1], STDERR_FILENO);

    close(process->pipe_in[0]);
    close(process->pipe_out[1]);
    close(process->pipe_err[1]);

    execlp("sh", "sh", "-c", process->cmd, (char*)NULL);
		
    // execl do not return if successful
    // -- http://stackoverflow.com/a/16089327/5676460

    perror("execl() error");
    exit(1);
			
  } else {

    // parent

    process->pid = pid;
			
    process->fdin = fdopen(process->pipe_in[1], "w");
    if (!process->fdin) {
	    perror("fdopen() failed");
	    return -1;
    }
    process->fdout = fdopen(process->pipe_out[0], "r");
    if (!process->fdout) {
	    perror("fdopen() failed");
	    return -1;
    }
    process->fderr = fdopen(process->pipe_err[0], "r");
    if (!process->fderr) {
	    perror("fdopen() failed");
	    return -1;
    }

    close(process->pipe_in[0]);
    close(process->pipe_out[1]);
    close(process->pipe_err[1]);

    process->opened = 1;
  }

  return 0;
}

#elif defined(USE_MS_WIN)

int osl_process_start(osl_process_t * process)
{
  /* make pipe */

  SECURITY_ATTRIBUTES saAttr = {0,};
  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;
  if (!CreatePipe(&process->out_read, &process->out_write, &saAttr, 0)) {
    fprintf(stderr, "CreatePipe() failed - stdout read\n");
    return -1;
  }	
  if (!SetHandleInformation(process->out_read, HANDLE_FLAG_INHERIT, 0)) {
    fprintf(stderr, "SetHandleInformation() failed - stdout\n");
    return -1;
  }
  if (!CreatePipe(&process->err_read, &process->err_write, &saAttr, 0)) {
    fprintf(stderr, "CreatePipe() failed - stderr read\n");
    return -1;
  }	
  if (!SetHandleInformation(process->err_read, HANDLE_FLAG_INHERIT, 0)) {
    fprintf(stderr, "SetHandleInformation() failed - stderr\n");
    return -1;
  }
  if (!CreatePipe(&process->in_read, &process->in_write, &saAttr, 0))  {
    fprintf(stderr, "CreatePipe() failed - stdin read\n");
    return -1;
  }
  if (!SetHandleInformation(process->in_write, HANDLE_FLAG_INHERIT, 0)) {
    fprintf(stderr, "SetHandleInformation() failed - stdin\n");
    return -1;
  }
		
  /* create process */
  BOOL bSuccess = FALSE;
  STARTUPINFO siStartInfo = {0,};

  siStartInfo.cb = sizeof(STARTUPINFO); 
  siStartInfo.hStdError = process->err_write;
  siStartInfo.hStdOutput = process->out_write;
  siStartInfo.hStdInput = process->in_read;
  siStartInfo.dwFlags |= STARTF_USESTDHANDLES;

  osl_string_buffer_t * sb = osl_string_buffer_new();
  osl_string_buffer_append(sb, "cmd /C ");
  osl_string_buffer_append(sb, process->cmd);
  char * cmd = osl_string_buffer_to_str_and_free(sb);
		
  bSuccess = CreateProcess(NULL, 
                           (LPSTR)cmd,
                           NULL,          // process security attributes 
                           NULL,          // primary thread security attributes 
                           TRUE,          // handles are inherited 
                           0,             // creation flags 
                           NULL,          // use parent's environment 
                           NULL,          // use parent's current directory 
                           &siStartInfo,  // STARTUPINFO pointer 
                           &process->pi_proc_info);

  osl_safe_free(cmd);

  if (!bSuccess) {
    fprintf(stderr, "CreateProcess() failed\n");
    return -1;
  }

  CloseHandle(process->err_write);
  CloseHandle(process->out_write);
  CloseHandle(process->in_read);    

  process->opened = 1;

  return 0;
}

#endif

void osl_process_close(osl_process_t * process)
{
  if (process->opened == 0)
  {
    fprintf(stderr, "process not open\n");
    return;
  }

  process->opened = 0;
    
#if defined(USE_UNIX_STD)

  if (process->fdin) {
    fclose(process->fdin);
    process->fdin = NULL;
  }
  if (process->fdout) {
    fclose(process->fdout);
    process->fdout = NULL;
  }
  if (process->fderr) {
    fclose(process->fderr);
    process->fderr = NULL;
  }

  printf("close pipe\n");

  close(process->pipe_in[1]);
  close(process->pipe_out[0]);
  close(process->pipe_err[0]);
  process->pid = 0;
    
#elif defined(USE_MS_WIN)

  CloseHandle(process->pi_proc_info.hProcess);
  CloseHandle(process->pi_proc_info.hThread);    
  CloseHandle(process->in_write);
  CloseHandle(process->out_read);
  CloseHandle(process->err_read);

#else
  fputs(stderr, "not implemented");
#endif
}

int osl_process_exited(osl_process_t * process)
{
#if defined(USE_UNIX_STD)
  // http://linux.die.net/man/2/wait
  int ret = 0;
  int status;
  if (waitpid(process->pid, &status, WUNTRACED | WCONTINUED) == -1) {
    perror("waitpid() error");
    return -1;
  }

  if (WIFEXITED(status))
  {
    ret = 1;
    process->exit_code = WEXITSTATUS(status);
  }
  else if (WIFSIGNALED(status))
  {
    ret = 1;
  }
  else if (WIFSTOPPED(status))
  {
    // stopped
  }
  else if (WIFCONTINUED(status))
  {
    // continued
  }
  return ret;
#elif defined(USE_MS_WIN)
  DWORD code;
  if (!GetExitCodeProcess(process->pi_proc_info.hProcess, &code)) {
    fprintf(stderr, "GetExitCodeProcess() failed\n");
    return -1;
  }
  return ((int)code != STILL_ACTIVE);
#else
  fputs(stderr, "not implemented");
  return -1;
#endif
}

void osl_process_wait(osl_process_t * process)
{
  while (!osl_process_exited(process)) {
    osl_idle(10);
  }
}

osl_stream_t * osl_process_in_stream(osl_process_t * process)
{
#if defined(USE_UNIX_STD)
  return osl_stream_wrap_std_file(process->fdin);
#elif defined(USE_MS_WIN)
  return osl_stream_wrap_win32_handle(process->in_write);
#endif
}

osl_stream_t * osl_process_out_stream(osl_process_t * process)
{
#if defined(USE_UNIX_STD)
  return osl_stream_wrap_std_file(process->fdout);
#elif defined(USE_MS_WIN)
  return osl_stream_wrap_win32_handle(process->out_read);
#endif
}

osl_stream_t * osl_process_err_stream(osl_process_t * process)
{
#if defined(USE_UNIX_STD)
  return osl_stream_wrap_std_file(process->fderr);
#elif defined(USE_MS_WIN)
	return osl_stream_wrap_win32_handle(process->err_read);
#endif
}
