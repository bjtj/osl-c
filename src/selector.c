#include "selector.h"

osl_selector_t * osl_selector_new(void)
{
    osl_selector_t * selector = (osl_selector_t *)malloc(sizeof(osl_selector_t));
    OSL_HANDLE_MALLOC_ERROR(selector);
    memset(selector, 0, sizeof(osl_selector_t));
    return selector;
}

osl_selector_t * osl_selector_init(osl_selector_t * selector)
{
    osl_selector_clear(selector);
    return selector;
}

void osl_selector_free(osl_selector_t * selector)
{
    osl_safe_free(selector);
}

void osl_selector_clear(osl_selector_t * selector)
{
    memset(selector, 0, sizeof(osl_selector_t));
    FD_ZERO(&(selector->readfds));
    FD_ZERO(&(selector->writefds));
    FD_ZERO(&(selector->exceptfds));
}

void osl_selector_register(osl_selector_t * selector, osl_socket fd, int flags)
{
    if (fd > selector->maxfds)
    {
	selector->maxfds = fd;
    }

    if (flags & OSL_FLAG_READ)
    {
	FD_SET(fd, &(selector->readfds));
    }
    if (flags & OSL_FLAG_WRITE)
    {
	FD_SET(fd, &(selector->writefds));
    }
    if (flags & OSL_FLAG_EXCEPT)
    {
	FD_SET(fd, &(selector->exceptfds));
    }
}

void osl_selector_unregister(osl_selector_t * selector, osl_socket fd, int flags)
{
    if (flags & OSL_FLAG_READ)
    {
	FD_CLR(fd, &(selector->readfds));
    }
    if (flags & OSL_FLAG_WRITE)
    {
	FD_CLR(fd, &(selector->writefds));
    }
    if (flags & OSL_FLAG_EXCEPT)
    {
	FD_CLR(fd, &(selector->exceptfds));
    }
}

int osl_selector_select(osl_selector_t * selector, int timeout_milli)
{
    int ret;
    struct timeval timeout;
    timeout.tv_sec = timeout_milli / 1000;
    timeout.tv_usec = (timeout_milli % 1000) * 1000;

    selector->cur_readfds = selector->readfds;
    selector->cur_writefds = selector->writefds;
    selector->cur_exceptfds = selector->exceptfds;

    ret = select((int)selector->maxfds + 1, &(selector->cur_readfds), &(selector->cur_writefds), &(selector->cur_exceptfds), &timeout);
    if (ret < 0)
    {
	perror("select() failed");
    }
    selector->count = ret;
    return ret;
}

osl_bool osl_selector_is_readable(osl_selector_t * selector, osl_socket fd)
{
    return OSL_BOOL(FD_ISSET(fd, &(selector->cur_readfds)));
}

osl_bool osl_selector_is_writable(osl_selector_t * selector, osl_socket fd)
{
    return OSL_BOOL(FD_ISSET(fd, &(selector->cur_writefds)));
}

osl_bool osl_selector_is_except(osl_selector_t * selector, osl_socket fd)
{
    return OSL_BOOL(FD_ISSET(fd, &(selector->cur_exceptfds)));
}
