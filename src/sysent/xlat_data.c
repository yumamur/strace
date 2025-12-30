#include "../ft_utils.h"
#include "xlat.h"
#include <linux/fcntl.h>
#include <stdint.h>
#include <sys/stat.h>
#include <unistd.h>

#define XLAT(val_)              {.val = (uint64_t) (val_), .name = #val_}
#define XLAT_NAMED(val_, name_) {.val = (uint64_t) (val_), .name = name_}

#define WXLAT(data_)                                 \
	{                                                \
		{                                            \
			.data = data_, .size = ARRAY_SIZE(data_) \
		}                                            \
	}

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
	XLAT_NAMED(__O_SYNC, "O_SYNC"),
	XLAT_NAMED(__O_TMPFILE, "O_TMPFILE"),
	XLAT(FASYNC),
	XLAT(O_SYNC),
	XLAT(O_TMPFILE),
	XLAT(O_TRUNC),
};

const t_xlat_data execveat_flags_data[] = {
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW),
};

const t_xlat_data mode_file_types_data[] = {
	XLAT_NAMED(__S_IFDIR, "S_IFDIR"),
	XLAT_NAMED(__S_IFCHR, "S_IFCHR"),
	XLAT_NAMED(__S_IFBLK, "S_IFBLK"),
	XLAT_NAMED(__S_IFREG, "S_IFREG"),
	XLAT_NAMED(__S_IFIFO, "S_IFIFO"),
	XLAT_NAMED(__S_IFLNK, "S_IFLNK"),
	XLAT_NAMED(__S_IFSOCK, "S_IFSOCK"),
};

const t_xlat_data mode_protection_bits_data[] = {
	XLAT_NAMED(__S_ISUID, "S_ISUID"),
	XLAT_NAMED(__S_ISGID, "S_ISGID"),
	XLAT_NAMED(__S_ISVTX, "S_ISVTX"),
	// not sure why these are even defined in 'stat.h' in the first place
	// XLAT_NAMED(__S_IREAD, "S_IREAD"),
	// XLAT_NAMED(__S_IWRITE, "S_IWRITE"),
	// XLAT_NAMED(__S_IEXEC, "S_IEXEC"),
};

const t_xlat_data fstatat_flags_data[] = {
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_NO_AUTOMOUNT),
	XLAT(AT_SYMLINK_NOFOLLOW),
};

const t_xlat access_modes[] = WXLAT(access_modes_data);
const t_xlat faccessat2_flags[] = WXLAT(faccessat2_flags_data);
const t_xlat open_access_flags[] = WXLAT(open_access_flags_data);
const t_xlat open_flags[] = WXLAT(open_flags_data);
const t_xlat execveat_flags[] = WXLAT(execveat_flags_data);
const t_xlat mode_file_types[] = WXLAT(mode_file_types_data);
const t_xlat mode_protection_bits[] = WXLAT(mode_protection_bits_data);
const t_xlat fstatat_flags[] = WXLAT(fstatat_flags_data);