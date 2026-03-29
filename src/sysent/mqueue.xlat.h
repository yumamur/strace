#ifndef MQUEUE_XLAT_H
#define MQUEUE_XLAT_H

#include "xlat.h"

#include <fcntl.h>

const t_xlat_data mq_attr_flags_data[] = {
	XLAT(O_NONBLOCK),
};
WXLAT(mq_attr_flags);

#endif /* MQUEUE_XLAT_H */