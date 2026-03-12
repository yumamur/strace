#!/bin/sh

PROJECT_DIR="/home/ymamur/my_projects/ccc/ft_strace/"
XLAT_DIR="src/sysent/"

TDIR="$PROJECT_DIR$XLAT_DIR"

if [[ -z $1 ]]; then
	echo Invalid file name
	exit 1;
fi

filename="$1.xlat.h"
hdrname="$(tr a-z A-Z <<< "$1")_XLAT_H"

cat > "$TDIR$filename" << EOF
#ifndef $hdrname
#define $hdrname

#include "xlat.h"

#endif /* $hdrname */
EOF