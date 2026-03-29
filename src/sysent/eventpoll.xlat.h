#ifndef EVENTPOLL_XLAT_H
#define EVENTPOLL_XLAT_H

#include "xlat.h"

#include <linux/eventpoll.h>

const t_xlat_data epoll_create1_flags_data[] = {
	XLAT(EPOLL_CLOEXEC),
};
WXLAT(epoll_create1_flags);

const t_xlat_data epoll_event_flags_data[] = {
	XLAT(EPOLLIN),
	XLAT(EPOLLPRI),
	XLAT(EPOLLOUT),
	XLAT(EPOLLERR),
	XLAT(EPOLLHUP),
	XLAT(EPOLLNVAL),
	XLAT(EPOLLRDNORM),
	XLAT(EPOLLRDBAND),
	XLAT(EPOLLWRNORM),
	XLAT(EPOLLWRBAND),
	XLAT(EPOLLMSG),
	XLAT(EPOLLRDHUP),
	XLAT(EPOLL_URING_WAKE),
	XLAT(EPOLLEXCLUSIVE),
	XLAT(EPOLLWAKEUP),
	XLAT(EPOLLONESHOT),
	XLAT(EPOLLET),
};
WXLAT(epoll_event_flags);

const t_xlat_data epoll_ctl_ops_data[] = {
	XLAT(EPOLL_CTL_ADD),
	XLAT(EPOLL_CTL_DEL),
	XLAT(EPOLL_CTL_MOD),
};
WXLAT(epoll_ctl_ops);

#endif /* EVENTPOLL_XLAT_H */