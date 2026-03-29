#ifndef RANDOM_XLAT_H
#define RANDOM_XLAT_H

#include "xlat.h"

#include <sys/random.h>

const t_xlat_data getrandom_flags_data[] = {
	XLAT(GRND_NONBLOCK),
	XLAT(GRND_RANDOM),
	XLAT(GRND_INSECURE),
};
WXLAT(getrandom_flags);

#endif /* RANDOM_XLAT_H */