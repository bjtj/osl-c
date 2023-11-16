#include "shared_memory.h"
#include "error.h"


OSL_SHARED_MEMORY_ID osl_shared_memory_create(const char * name, size_t bufsize)
{
#if defined(USE_WIN_SHARED_MEMORY)
  (void)bufsize;
  OSL_SHARED_MEMORY_ID hMapFile;
  hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		(DWORD)bufsize,                 // maximum object size (low-order DWORD)
		name);                   // name of mapping object
  if (hMapFile == NULL)
  {
		char errbuf[512] = {0,};
		osl_get_last_error_message(errbuf, sizeof(errbuf));
    fprintf(stderr, "Could not open file mapping object (%d:%s).\n", GetLastError(), errbuf);
    return NULL;
  }
  return hMapFile;
#elif defined(USE_POSIX_SHARED_MEMORY)
  int fd;
  int ret;
  fd = shm_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("shm_open failed");
    return -1;
  }
  ret = ftruncate(fd, bufsize);
  if (ret == -1) {
    perror("ftruncate failed");
    return -1;
  }
  return fd;
#endif
}

OSL_SHARED_MEMORY_ID osl_shared_memory_open(const char * name)
{
#if defined(USE_WIN_SHARED_MEMORY)
	OSL_SHARED_MEMORY_ID hMapFile;
  hMapFile = OpenFileMapping(
    FILE_MAP_ALL_ACCESS, // read/write access
    FALSE,               // do not inherit the name
    name);               // name of mapping object
  if (hMapFile == NULL)
  {
		char errbuf[512] = {0,};
		osl_get_last_error_message(errbuf, sizeof(errbuf));
    fprintf(stderr, "Could not open file mapping object (%d:%s).\n", GetLastError(), errbuf);
    return NULL;
  }
  return hMapFile;
#elif defined(USE_POSIX_SHARED_MEMORY)
  int fd;
  fd = shm_open(name, O_RDWR, S_IRUSR | S_IWUSR);
  if (fd == -1) {
    perror("shm_open failed");
    return -1;
  }
  return fd;
#endif
}

char * osl_shared_memory_map(OSL_SHARED_MEMORY_ID id, size_t size)
{
#if defined(USE_WIN_SHARED_MEMORY)
  LPTSTR pBuf;
  pBuf = (LPTSTR)MapViewOfFile(id,   // handle to map object
                               FILE_MAP_ALL_ACCESS, // read/write permission
                               0,
                               0,
                               size);
  if (pBuf == NULL)
  {
		char errbuf[512] = {0,};
		osl_get_last_error_message(errbuf, sizeof(errbuf));
    fprintf(stderr, "Could not map view of file (%d:%s).\n", GetLastError(), errbuf);
    return NULL;
  }
  return pBuf;
#elif defined(USE_POSIX_SHARED_MEMORY)
	char * addr;
  addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, id, 0);
  if (addr == MAP_FAILED) {
    perror("mmap failed");
    return NULL;
  }
  return addr;
#endif
}

void osl_shared_memory_unmap(char * addr, size_t size)
{
#if defined(USE_WIN_SHARED_MEMORY)
	(void)size;
  UnmapViewOfFile(addr);
#elif defined(USE_POSIX_SHARED_MEMORY)
	int ret;
  ret = munmap(addr, size);
  if (ret == -1) {
    perror("munmap failed");
  }
#endif
}

void osl_shared_memory_unlink(const char * name)
{
#if defined(USE_WIN_SHARED_MEMORY)
	(void)name;
#elif defined(USE_POSIX_SHARED_MEMORY)
	int fd;
  fd = shm_unlink(name);
  if (fd == -1) {
    perror("shm_unlink failed");
  }
#endif
}

void osl_shared_memory_close(OSL_SHARED_MEMORY_ID id)
{
#if defined(USE_WIN_SHARED_MEMORY)
	CloseHandle(id);
#elif defined(USE_POSIX_SHARED_MEMORY)
	close(id);
#endif
}
