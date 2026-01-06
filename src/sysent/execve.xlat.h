#ifndef EXECVE_XLAT_H
#define EXECVE_XLAT_H

#include "xlat.h"
#include <linux/fcntl.h>

const t_xlat_data execveat_flags_data[] = {
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW),
};
WXLAT(execveat_flags);

#endif