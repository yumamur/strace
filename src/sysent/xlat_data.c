#include "../ft_utils.h"
#include "xlat.h"
#include <linux/fcntl.h>
#include <stdint.h>
#include <unistd.h>

#define XLAT(val_) {.val = (uint64_t) (val_), .name = #val_}

#define WXLAT(data_) {.data = data_, .size = ARRAY_SIZE(data_)}

const t_xlat_data access_modes_data[] = {
	XLAT(F_OK),
	XLAT(R_OK),
	XLAT(W_OK),
	XLAT(X_OK)};
const t_xlat_data faccessat2_flags_data[] = {
	XLAT(AT_EACCESS),
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW)};

const t_xlat_data open_access_flags_data[] = {
	XLAT(O_RDWR),
	XLAT(O_RDONLY),
	XLAT(O_WRONLY),
	XLAT(O_ACCMODE),
};

const t_xlat_data open_flags_data[] = {
	XLAT(O_CREAT),
	XLAT(O_EXCL),
	XLAT(O_APPEND),
	XLAT(O_CLOEXEC),
	XLAT(O_DIRECT),
	XLAT(O_DIRECTORY),
	XLAT(O_DSYNC),
	XLAT(O_LARGEFILE),
	XLAT(O_NOATIME),
	XLAT(O_NOCTTY),
	XLAT(O_NOFOLLOW),
	XLAT(O_NONBLOCK),
	XLAT(O_PATH),
	XLAT(__O_SYNC),
	XLAT(__O_TMPFILE),
	XLAT(FASYNC),
	XLAT(O_SYNC),
	XLAT(O_TMPFILE),
	XLAT(O_TRUNC),
};

const t_xlat_data execveat_flags_data[] = {
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW),
};

const t_xlat access_modes[] = {WXLAT(access_modes_data)};
const t_xlat faccessat2_flags[] = {WXLAT(faccessat2_flags_data)};
const t_xlat open_access_flags[] = {WXLAT(open_access_flags_data)};
const t_xlat open_flags[] = {WXLAT(open_flags_data)};
const t_xlat execveat_flags[] = {WXLAT(execveat_flags_data)};