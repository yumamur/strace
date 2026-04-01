#ifndef OPEN_XLAT_H
#define OPEN_XLAT_H

#include "xlat.h"
#include <linux/fcntl.h>
#include <linux/openat2.h>
#include <unistd.h>

const t_xlat_data open_access_flags_data[] = {
	XLAT(O_RDWR),
	XLAT(O_RDONLY),
	XLAT(O_WRONLY),
};
WXLAT(open_access_flags);

const t_xlat_data open_flags_data[] = {
	XLAT(O_CREAT),
	XLAT(O_EXCL),
	XLAT(O_NOCTTY),
	XLAT(O_TRUNC),
	XLAT(O_APPEND),
	XLAT(O_NONBLOCK),
	XLAT(O_DSYNC),
	XLAT(FASYNC),
	XLAT(O_DIRECT),
	XLAT(O_LARGEFILE),
	XLAT(O_DIRECTORY),
	XLAT(O_NOFOLLOW),
	XLAT(O_NOATIME),
	XLAT(O_CLOEXEC),
	XLAT_NAMED(__O_SYNC, "O_SYNC"),
	XLAT(O_PATH),
	XLAT_NAMED(__O_TMPFILE, "O_TMPFILE"),
};
WXLAT(open_flags);

const t_xlat_data access_modes_data[] = {
	XLAT(F_OK),
	XLAT(R_OK),
	XLAT(W_OK),
	XLAT(X_OK),
};
WXLAT(access_modes);

const t_xlat_data faccessat2_flags_data[] = {
	XLAT(AT_EACCESS),
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW),
};
WXLAT(faccessat2_flags);

const t_xlat_data fchownat_flags_data[] = {
	XLAT(AT_SYMLINK_NOFOLLOW),
	XLAT(AT_REMOVEDIR),
	XLAT(AT_SYMLINK_FOLLOW),
	XLAT(AT_NO_AUTOMOUNT),
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_RECURSIVE),
};
WXLAT(fchownat_flags);

const t_xlat_data name_to_handle_at_flags_data[] = {
	XLAT(AT_HANDLE_MNT_ID_UNIQUE),
	XLAT(AT_HANDLE_CONNECTABLE),
	XLAT(AT_HANDLE_FID),
	XLAT(AT_SYMLINK_FOLLOW),
	XLAT(AT_EMPTY_PATH),
};
WXLAT(name_to_handle_at_flags);

const t_xlat_data open_how_resolve_flags_data[] = {
	XLAT(RESOLVE_NO_XDEV),
	XLAT(RESOLVE_NO_MAGICLINKS),
	XLAT(RESOLVE_NO_SYMLINKS),
	XLAT(RESOLVE_BENEATH),
	XLAT(RESOLVE_IN_ROOT),
	XLAT(RESOLVE_CACHED),
};
WXLAT(open_how_resolve_flags);

const t_xlat_data fchmodat2_flags_data[] = {
	XLAT(AT_SYMLINK_NOFOLLOW),
	XLAT(AT_EMPTY_PATH),
};
WXLAT(fchmodat2_flags);

#endif