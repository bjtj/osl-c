#include "log.h"
#include "date.h"

static osl_logger_t s_default_logger = {
.prefix = {0,},
.level = OSL_LOG_DEBUG,
};

static char s_get_level_char(int level)
{
	switch (level)
	{
	case OSL_LOG_TRACE:
		return 'T';
	case OSL_LOG_DEBUG:
		return 'D';
	case OSL_LOG_INFO:
		return 'I';
	case OSL_LOG_WARN:
		return 'W';
	case OSL_LOG_ERROR:
		return 'E';
	case OSL_LOG_FATAL:
		return 'F';
	default:
		return 'U';
	}
}

osl_logger_t * osl_log_default_logger(void)
{
	return &s_default_logger;
}

void osl_log_print(osl_logger_t * logger, int level, const char * file, int line, const char * fmt, ...)
{
	if (level < logger->level)
	{
		return;
	}

	osl_date_t now = osl_date_now();
	va_list args;
	printf("[%04d-%02d-%02d %02d:%02d:%02d] ",
		   now.year, now.month+1, now.day, now.hour, now.minute, now.second);
	printf("%c ", s_get_level_char(level));
	printf("'%s:%d' ", file, line);
	if (logger->prefix[0])
	{
		printf("%s ", logger->prefix);
	}
	printf("- ");
	va_start(args, fmt);
	vprintf(fmt, args);
	va_end(args);
	puts("");
	fflush(stdin);
	
}
