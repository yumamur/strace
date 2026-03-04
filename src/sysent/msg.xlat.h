#ifndef MSG_XLAT_H
#define MSG_XLAT_H

#include "xlat.h"

#ifndef __USE_GNU
#  define __USE_GNU
#endif
#ifndef __USE_MISC
#  define __USE_MISC
#endif
#include <sys/msg.h>

const t_xlat_data ipc_msg_flags_data[] = {
	XLAT(IPC_NOWAIT),
	XLAT(MSG_NOERROR),
	XLAT(MSG_EXCEPT),
	XLAT(MSG_COPY),
};
WXLAT(ipc_msg_flags);

const t_xlat_data msgctl_flags_data[] = {
	XLAT(IPC_RMID),
	XLAT(IPC_SET),
	XLAT(IPC_STAT),
	XLAT(IPC_INFO),
	XLAT(MSG_STAT),
	XLAT(MSG_INFO),
	XLAT(MSG_STAT_ANY),
};
WXLAT(msgctl_flags);

extern const t_xlat *ipc_key;
extern const t_xlat *ipc_mode_flags;
extern const t_xlat *ipc_cmd_flags;

#endif