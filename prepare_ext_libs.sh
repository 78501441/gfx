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
git clone 'https://github.com/glfw/glfw.git' ||
    die "Can't clone glfw repo"

cd glfw
cmake -DGLFW_BUILD_TESTS=OFF -DBUILD_SHARED_LIBS=OFF . && make -j4
if [ ! $? ]; then
    echo "GLFW build failure"
    exit 3
fi

cd ..

git clone 'https://github.com/nigels-com/glew.git' ||
    die "Failed to clone glew"

cd glew/auto
make || die "Glew make auto rule failed"
cd ..
make -j4 glew.lib.static || die "Glew make failed"

cd ../..

test "$(basename $PWD)" == "gfx" || die "Wrong directory: $PWD"

if [ ! -d static_lib ]; then
    mkdir static_lib
fi

cp -v $(find ext -name '*.a') static_lib
