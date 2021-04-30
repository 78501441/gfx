#!/bin/sh

ERROR_CODE=3
die() {
    echo "$1" 1>&2
    exit $ERROR_CODE
}

if [ ! -d ext ]; then
    mkdir ext
fi

cd ext
if [ ! -d glfw ]; then
    git clone 'https://github.com/glfw/glfw.git' ||
        die "Can't clone glfw repo"
fi

cd glfw
cmake -DGLFW_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF . && make -j4 || die 'Failed to compile glfw'

cd deps
tcc -Wall -O2 -c -o glad_gl.o -I. glad_gl.c && ar rcs libglad_gl.a glad_gl.o && rm glad_gl.o ||
    die 'Failed to compile glade ar'

