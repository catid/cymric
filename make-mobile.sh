#!/bin/sh

echo "This script is supposed to run on a Mac/Linux desktop/laptop to generate a folder containing portable C code that can be dropped into an iOS/Android project.  You can just follow the steps in this script manually if it does not work for some reason.  It is basically just copying files.  When you use the cymric API be sure to check cymric_init() == 0 on startup, and handle failures gracefully.  Using assert() is a bad idea because code in asserts may not be compiled."

# Make sure we have the submodules checked out
git submodule update --init

rm -rf cymric-mobile
mkdir cymric-mobile

cp src/cymric.cpp ./cymric-mobile
cp include/cymric.h ./cymric-mobile

cp libcat/Clock.cpp ./cymric-mobile
cp libcat/Clock.hpp ./cymric-mobile
cp libcat/Config.hpp ./cymric-mobile
cp libcat/Platform.hpp ./cymric-mobile
cp libcat/Atomic.hpp ./cymric-mobile

cp blake2/ref/blake2-impl.h ./cymric-mobile
cp blake2/ref/blake2.h ./cymric-mobile
cp blake2/ref/blake2b-ref.c ./cymric-mobile

cp chacha-opt/chacha.c ./cymric-mobile
cp chacha-opt/chacha.h ./cymric-mobile
cp chacha-opt/chacha_blocks_ref.c ./cymric-mobile
cp README.md ./cymric-mobile

sed -ie "s/chacha_blocks_impl/chacha_blocks_ref/g" "./cymric-mobile/chacha.c"
sed -ie "s/hchacha_impl/hchacha_ref/g" "./cymric-mobile/chacha.c"
rm ./cymric-mobile/chacha.ce

echo "Done."

