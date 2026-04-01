#ifndef MEMPOLICY_XLAT_H
#define MEMPOLICY_XLAT_H

#include "xlat.h"

#include <linux/mempolicy.h>

const t_xlat_data memory_policies_data[] = {
	XLAT(MPOL_DEFAULT),
	XLAT(MPOL_PREFERRED),
	XLAT(MPOL_BIND),
	XLAT(MPOL_INTERLEAVE),
	XLAT(MPOL_LOCAL),
	XLAT(MPOL_PREFERRED_MANY),
};
WXLAT(memory_policies);

const t_xlat_data memory_policy_flags_data[] = {
	XLAT(MPOL_F_STATIC_NODES),
	XLAT(MPOL_F_RELATIVE_NODES),
	XLAT(MPOL_F_NUMA_BALANCING),
};
WXLAT(memory_policy_flags);

const t_xlat_data mbind_flags_data[] = {
	XLAT(MPOL_MF_STRICT),
	XLAT(MPOL_MF_MOVE),
	XLAT(MPOL_MF_MOVE_ALL),
	XLAT(MPOL_MF_LAZY),
	XLAT(MPOL_MF_INTERNAL),
};
WXLAT(mbind_flags);

const t_xlat_data getmempolicy_flags_data[] = {
	XLAT(MPOL_F_NODE),
	XLAT(MPOL_F_ADDR),
	XLAT(MPOL_F_MEMS_ALLOWED),
};
WXLAT(getmempolicy_flags);

const t_xlat_data move_pages_flags_data[] = {
	XLAT(MPOL_MF_MOVE),
	XLAT(MPOL_MF_MOVE_ALL),
};
WXLAT(move_pages_flags);

#endif /* MEMPOLICY_XLAT_H */
