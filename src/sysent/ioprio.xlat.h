#ifndef IOPRIO_XLAT_H
#define IOPRIO_XLAT_H

#include "xlat.h"

#include <linux/ioprio.h>

const t_xlat_data ioprio_which_data[] = {
	XLAT(IOPRIO_WHO_PROCESS),
	XLAT(IOPRIO_WHO_PGRP),
	XLAT(IOPRIO_WHO_USER),
};
WXLAT(ioprio_which);

const t_xlat_data ioprio_classes_data[] = {
	XLAT(IOPRIO_CLASS_NONE),
	XLAT(IOPRIO_CLASS_RT),
	XLAT(IOPRIO_CLASS_BE),
	XLAT(IOPRIO_CLASS_IDLE),
	XLAT(IOPRIO_CLASS_INVALID),
};
WXLAT(ioprio_classes);

#endif /* IOPRIO_XLAT_H */