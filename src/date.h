#ifndef __OSL_DATE_H__
#define __OSL_DATE_H__

#include "osl.h"

typedef struct _osl_time_t
{
    uint64_t sec;
    uint64_t nano;    
} osl_time_t;

typedef struct _osl_date_t
{
    int gmtoffset;
    int year;
    int month;
    int day;
    int wday;
    int hour;
    int minute;
    int second;
    int millisecond;
} osl_date_t;

#ifdef __cplusplus
extern "C" {
#endif

    osl_date_t OSL_EXPORT osl_date_from_tm(struct tm * t);
    osl_date_t OSL_EXPORT osl_date_from_time(osl_time_t * time, int gmtoffset);
    osl_date_t OSL_EXPORT osl_date_now(void);
    osl_date_t OSL_EXPORT osl_date_to_gmt(osl_date_t * date);
    osl_time_t OSL_EXPORT osl_date_to_time(osl_date_t * date);

#ifdef __cplusplus
}
#endif

#endif
