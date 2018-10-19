#ifndef __SELECTOR_H__
#define __SELECTOR_H__

#include "osl.h"

typedef struct _osl_selector_t
{
    int maxfds;
    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    fd_set cur_readfds;
    fd_set cur_writefds;
    fd_set cur_exceptfds;
    int count;
} osl_selector_t;

#define osl_read 0x0001
#define osl_write 0x0010
#define osl_except 0x0100

void osl_selector_clear(osl_selector_t * selector);
void osl_selector_register(osl_selector_t * selector, int fd, int flag);
void osl_selector_unregister(osl_selector_t * selector, int fd, int flag);
int osl_selector_select(osl_selector_t * selector, int timeout_milli);
int osl_selector_is_readable(osl_selector_t * selector, int fd);
int osl_selector_is_writable(osl_selector_t * selector, int fd);
int osl_selector_is_except(osl_selector_t * selector, int fd);

#endif
