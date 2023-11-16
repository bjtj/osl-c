#include "error.h"


size_t osl_get_last_error_message(char* buf, size_t size)
{

#if defined(USE_MS_WIN)
	DWORD err = GetLastError();
	DWORD   dwChars;  // Number of chars returned.

	// Try to get the message from the system errors.
	dwChars = FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM |
													 FORMAT_MESSAGE_IGNORE_INSERTS,
													 NULL,
													 err,
													 0,
													 buf,
													 (DWORD)size,
													 NULL );

	return dwChars;
#else
	return strerror_r(errno, buf, size);
#endif
}
