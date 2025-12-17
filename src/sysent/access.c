#include "../ft_common.h"
#include "../ft_print.h"
#include "../ft_utils.h"
#include "xlat.h"
#include <unistd.h>

static const t_xlat_data access_modes_data[] = {
	XLAT(F_OK),
	XLAT(R_OK),
	XLAT(W_OK),
	XLAT(X_OK)};

static const t_xlat access_modes = {
	.data = access_modes_data,
	.size = ARRAY_SIZE(access_modes_data)};

SYS_FUNC(access)
{
	FIRST_ARG("pathname");
	printpath(td, td->sc_args[0]);

	NEXT_ARG("mode");
	printflags(&access_modes, td->sc_args[1]);
}

SYS_FUNC(faccessat)
{
	FIRST_ARG("dirfd");
	putnum(td->sc_args[0]);

	NEXT_ARG("mode");
	printpath(td, td->sc_args[1]);
}
