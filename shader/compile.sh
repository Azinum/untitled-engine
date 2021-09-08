#!/bin/bash

function string_replace() {
	local s="$1";
	echo ${s//./_}
}

HEADER=shader.h
SOURCE=shader.c

printf "// ${HEADER}\n\n#ifndef _SHADER_H\n#define _SHADER_H\n\n" > "${HEADER}"

printf "// shader.c\n\n" > "${SOURCE}"

for f in *.vert; do
	./shader_compile $f >> "${SOURCE}"
	printf "extern char $(string_replace $f)[];\n" >> "${HEADER}";
done

for f in *.frag; do
	./shader_compile $f >> "${SOURCE}"
	printf "extern char $(string_replace $f)[];\n" >> "${HEADER}";
done

printf "\n#endif\n" >> "${HEADER}"

