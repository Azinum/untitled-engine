#!/bin/bash

[ $# -lt 2 ] && echo "not enough args" && exit 1

[ -f $1 ] || exit 1

printf "// pack_file.c\n\n"

bintoc $1 $2 && printf "\n#define HAS_PACK_FILE\n" 2> /dev/null
