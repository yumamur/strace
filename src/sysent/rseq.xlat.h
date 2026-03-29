#ifndef RSEQ_XLAT_H
#define RSEQ_XLAT_H

#include "xlat.h"

#include <linux/rseq.h>

const t_xlat_data rseq_cpu_id_states_data[] = {
	XLAT(RSEQ_CPU_ID_UNINITIALIZED),
	XLAT(RSEQ_CPU_ID_REGISTRATION_FAILED),
};
WXLAT(rseq_cpu_id_states);

const t_xlat_data rseq_cs_flags_data[] = {
	XLAT(RSEQ_CS_FLAG_NO_RESTART_ON_PREEMPT),
	XLAT(RSEQ_CS_FLAG_NO_RESTART_ON_SIGNAL),
	XLAT(RSEQ_CS_FLAG_NO_RESTART_ON_MIGRATE),
#ifdef RSEQ_CS_FLAG_SLICE_EXT_AVAILABLE
	XLAT(RSEQ_CS_FLAG_SLICE_EXT_AVAILABLE),
#endif
#ifdef RSEQ_CS_FLAG_SLICE_EXT_ENABLED
	XLAT(RSEQ_CS_FLAG_SLICE_EXT_ENABLED),
#endif
};
WXLAT(rseq_cs_flags);

const t_xlat_data rseq_flags_data[] = {
	XLAT(RSEQ_FLAG_UNREGISTER),
#ifdef RSEQ_FLAG_SLICE_EXT_DEFAULT_ON
	XLAT(RSEQ_FLAG_SLICE_EXT_DEFAULT_ON),
#endif
};
WXLAT(rseq_flags);

#endif /* RSEQ_XLAT_H */