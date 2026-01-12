#ifndef PIPE_XLAT_H
#define PIPE_XLAT_H

#include "xlat.h"

#include <asm/fcntl.h>
#include <linux/watch_queue.h>

const t_xlat_data pipe2_flags_data[] = {
	XLAT(O_CLOEXEC),
	XLAT(O_DIRECT),
	XLAT(O_NONBLOCK),
	XLAT(O_NOTIFICATION_PIPE),
};
WXLAT(pipe2_flags);

#endif