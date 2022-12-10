#include "date.h"


#if defined(USE_MS_WIN)

static osl_date_t s_systemtime_to_date(SYSTEMTIME t) {
  osl_date_t date = {0,};
  date.year = t.wYear;
  date.month = t.wMonth - 1;
  date.day = t.wDay;
  date.wday = t.wDayOfWeek;
  date.hour = t.wHour;
  date.minute = t.wMinute;
  date.second = t.wSecond;
  date.millisecond = t.wMilliseconds;
  return date;
}

static SYSTEMTIME s_filetime_to_systemtime(FILETIME ftime) {
  SYSTEMTIME stime;
  FILETIME localtime;
  FileTimeToLocalFileTime(&ftime, &localtime);
  FileTimeToSystemTime(&localtime, &stime);
  return stime;
}

/**
 * @ref https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms724284(v=vs.85).aspx
 */
static osl_time_t s_filetime_to_osl_time(const FILETIME * ft) {
  osl_time_t t = {0,};
  ULARGE_INTEGER time;
  time.LowPart = ft->dwLowDateTime;
  time.HighPart = ft->dwHighDateTime;
  unsigned __int64 uit = time.QuadPart;
  t.sec = (uint64_t)(uit / 10000000);
  t.nano = (unsigned long)((uit % 10000000) * 100);

  return t;
}

/**
 * @ref https://support.microsoft.com/ko-kr/kb/167296
 */
static FILETIME s_osl_time_to_filetime(const osl_time_t * ot) {
  unsigned __int64 uit = 0;
  uit = ((unsigned __int64)ot->sec) * 10000000ULL;
  uit += ((unsigned __int64)ot->nano) / 100ULL;
  FILETIME t = {0,};
  t.dwLowDateTime = (DWORD)uit;
  t.dwHighDateTime= (DWORD)(uit >> 32);
  return t;
}

#endif	/* if win32 */



static int s_get_gmt_offset(void)
{
#if defined(USE_APPLE_STD) || defined(USE_POSIX_STD)
  struct tm info;
  time_t t = 0;
  localtime_r(&t, &info);
  time_t local = mktime(&info);
  gmtime_r(&t, &info);
  time_t gmt = mktime(&info);
  double offset = difftime(local, gmt);
  return (int)(offset / 60);
#elif defined(USE_MS_WIN)
  // http://stackoverflow.com/a/597562
  TIME_ZONE_INFORMATION TimeZoneInfo;
  GetTimeZoneInformation(&TimeZoneInfo);
  return -((int)TimeZoneInfo.Bias);
#else
  /* no implementation */
  return 0;
#endif
}

static osl_date_t s_get_localtime(void)
{
  osl_date_t date = {0,};

#if defined(USE_APPLE_STD)
        
  // @ref http://stackoverflow.com/a/11681069
        
  clock_serv_t cclock;
  mach_timespec_t mts;
  host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
  clock_get_time(cclock, &mts);
  mach_port_deallocate(mach_task_self(), cclock);
  time_t t = (time_t)mts.tv_sec;
  struct tm info;
  localtime_r(&t, &info);
  date.year = 1900 + info.tm_year;
  date.month = info.tm_mon;
  date.day = info.tm_mday;
  date.wday = info.tm_wday;
  date.hour = info.tm_hour;
  date.minute = info.tm_min;
  date.second = info.tm_sec;
  date.millisecond = mts.tv_nsec / 1000000;
  date.gmtoffset = s_get_gmt_offset();
        
#elif defined(USE_POSIX_STD)
        
  struct timespec spec;
  clock_gettime(CLOCK_REALTIME, &spec);
  time_t t = (time_t)spec.tv_sec;
  struct tm info;
  localtime_r(&t, &info);
  date.year = (1900 + info.tm_year);
  date.month = info.tm_mon;
  date.day = info.tm_mday;
  date.wday = info.tm_wday;
  date.hour = info.tm_hour;
  date.minute = info.tm_min;
  date.second = info.tm_sec;
  date.millisecond = spec.tv_nsec / 1000000;
  date.gmtoffset = s_get_gmt_offset();
        
#elif defined(USE_MS_WIN)
        
  SYSTEMTIME now;
  GetLocalTime(&now);
  date.year = now.wYear;
  date.month = now.wMonth - 1;
  date.day = now.wDay;
  date.wday = now.wDayOfWeek;
  date.hour = now.wHour;
  date.minute = now.wMinute;
  date.second = now.wSecond;
  date.millisecond = now.wMilliseconds;
  date.gmtoffset = s_get_gmt_offset();
        
#endif
  return date;
}

osl_date_t osl_date_from_tm(struct tm * t)
{
  osl_date_t date = {0,};
  date.year = 1900 + t->tm_year;
  date.month = t->tm_mon;
  date.day = t->tm_mday;
  date.wday = t->tm_wday;
  date.hour = t->tm_hour;
  date.minute = t->tm_min;
  date.second = t->tm_sec;
  return date;
}

osl_date_t osl_date_from_time(osl_time_t * time, int gmtoffset)
{
  /* todo */
#if defined(USE_MS_WIN)
  osl_date_t date = s_systemtime_to_date(s_filetime_to_systemtime(s_osl_time_to_filetime(time)));
  date.gmtoffset = gmtoffset;
  return date;
#else
  osl_date_t date = {0,};
  time_t t = (time_t)time->sec;
  struct tm info;
  localtime_r(&t, &info);
  date.year = 1900 + info.tm_year;
  date.month = info.tm_mon;
  date.day = info.tm_mday;
  date.wday = info.tm_wday;
  date.hour = info.tm_hour;
  date.minute = info.tm_min;
  date.second = info.tm_sec;
  date.millisecond = (int)(time->nano / (1000 * 1000));
  date.gmtoffset = gmtoffset;
  return date;
#endif
}

osl_date_t osl_date_now(void)
{
  return s_get_localtime();
}

osl_date_t osl_date_to_gmt(osl_date_t * date)
{
  if (date->gmtoffset == 0) {
    return *date;
  }
		
#if defined(USE_MS_WIN)
  int defaultOffset = s_get_gmt_offset();
  osl_time_t t = osl_date_to_time(date);
  t.sec -= ((uint64_t)defaultOffset * 60);
  return osl_date_from_time(&t, 0);
#else
  int defaultOffset = s_get_gmt_offset();
  int offset = date->gmtoffset;
  osl_time_t time = osl_date_to_time(date);
  uint64_t seconds = time.sec;
  time_t x = (time_t)(seconds + ((defaultOffset - offset) * 60));
  struct tm info;
  gmtime_r(&x, &info);
  osl_date_t ret = osl_date_from_tm(&info);
  ret.millisecond = date->millisecond;
  return ret;
#endif
}

osl_time_t osl_date_to_time(osl_date_t * date)
{
#if defined(USE_MS_WIN)
  SYSTEMTIME st = {0,};
  FILETIME ft = {0,};
  st.wYear = (WORD)date->year;
  st.wMonth = (WORD)date->month + 1;
  st.wDay = (WORD)date->day;
  st.wHour = (WORD)date->hour;
  st.wMinute = (WORD)date->minute;
  st.wSecond = (WORD)date->second;
  st.wMilliseconds = (WORD)date->millisecond;
  SystemTimeToFileTime(&st, &ft);
  osl_time_t ret = s_filetime_to_osl_time(&ft);
  ret.sec -= ((uint64_t)date->gmtoffset * 60);
  return ret;
#else
  struct tm info = {0,};
  info.tm_year = date->year - 1900;
  info.tm_mon = date->month;
  info.tm_mday = date->day;
  info.tm_hour = date->hour;
  info.tm_min = date->minute;
  info.tm_sec = date->second;
  time_t t = mktime(&info);
  time_t base = {0,};
  double seconds = difftime(t, base);
  osl_time_t ret;
  ret.sec = (uint64_t)seconds;
  ret.nano = date->millisecond * (1000 * 1000);
  return ret;
#endif
}
