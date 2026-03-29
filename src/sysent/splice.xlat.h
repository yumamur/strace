#ifndef SPLICE_XLAT_H
#define SPLICE_XLAT_H

#include "xlat.h"

#ifndef __USE_GNU
#  define __USE_GNU
#endif
#include <fcntl.h>

const t_xlat_data splice_flags_data[] = {
	XLAT(SPLICE_F_MOVE),
	XLAT(SPLICE_F_NONBLOCK),
	XLAT(SPLICE_F_MORE),
	XLAT(SPLICE_F_GIFT),
};
WXLAT(splice_flags);

#endif /* SPLICE_XLAT_H */