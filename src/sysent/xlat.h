#ifndef FT_XLAT
#define FT_XLAT

#include "../ft_utils.h"
#include <linux/fcntl.h>
#include <stdint.h>
#include <unistd.h>

typedef struct s_xlat_data
{
		uint64_t    val;
		const char *name;
} t_xlat_data;

typedef struct s_xlat
{
		unsigned int       size;
		const t_xlat_data *data;
} t_xlat;

#define XLAT(val_) {.val = (uint64_t) (val_), .name = #val_}

#define WXLAT(data_) {.data = data_, .size = ARRAY_SIZE(data_)}

static const t_xlat_data access_modes_data[] = {
	XLAT(F_OK),
	XLAT(R_OK),
	XLAT(W_OK),
	XLAT(X_OK)};
static const t_xlat_data faccessat2_flags_data[] = {
	XLAT(AT_EACCESS),
	XLAT(AT_EMPTY_PATH),
	XLAT(AT_SYMLINK_NOFOLLOW)};

static const t_xlat_data open_access_flags_data[] = {
	XLAT(O_RDWR),
	XLAT(O_RDONLY),
	XLAT(O_WRONLY),
	XLAT(O_ACCMODE),
};

static const t_xlat_data open_flags_data[] = {
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

static const t_xlat access_modes[] = {WXLAT(access_modes_data)};
static const t_xlat faccessat2_flags[] = {WXLAT(faccessat2_flags_data)};
static const t_xlat open_access_flags[] = {WXLAT(open_access_flags_data)};
static const t_xlat open_flags[] = {WXLAT(open_flags_data)};

// util functions
const char *search_xlat(const t_xlat *xlat, const uint64_t val);

#endif