# Change your compiler settings here

# Clang seems to produce faster code
#CCPP = g++
#CC = gcc
#OPTFLAGS = -O3 -fomit-frame-pointer -funroll-loops
CCPP = C:\LLVM\bin\clang++
CC = C:\LLVM\bin\clang
OPTFLAGS = -O3 -m64
DBGFLAGS = -g -O0 -DDEBUG
CFLAGS = -Wall -fstrict-aliasing -I./blake2/sse -I./chacha-opt -I./libcat -I./include \
		 -Dchacha_blocks_impl=chacha_blocks_ssse3 -Dhchacha_impl=hchacha
LIBNAME = libcymric.lib
LIBS =


# Object files

shared_test_o =

cymric_o = cymric.o Clock.o \
		   blake2b.o chacha.o chacha_blocks_ssse3-64.o

cymric_test_o = cymric_test.o $(shared_test_o)


# Release target (default)

release : CFLAGS += $(OPTFLAGS)
release : library


# Debug target

debug : CFLAGS += $(DBGFLAGS)
debug : LIBNAME = libcymric_debug.lib
debug : library


# Library.ARM target

library.arm : CCPP = /Volumes/casedisk/prefix/bin/arm-unknown-eabi-g++
library.arm : CPLUS_INCLUDE_PATH = /Volumes/casedisk/prefix/arm-unknown-eabi/include
library.arm : CC = /Volumes/casedisk/prefix/bin/arm-unknown-eabi-gcc
library.arm : C_INCLUDE_PATH = /Volumes/casedisk/prefix/arm-unknown-eabi/include
library.arm : library


# Library target

library : CFLAGS += $(OPTFLAGS)
library : $(cymric_o)
	ar rcs $(LIBNAME) $(cymric_o)


# tester executables

test : CFLAGS += -DUNIT_TEST $(OPTFLAGS)
test : clean $(cymric_test_o) library
	$(CCPP) $(cymric_test_o) $(LIBS) -L. -lcymric -o test.exe
	./test.exe


# Shared objects

Clock.o : libcat/Clock.cpp
	$(CCPP) $(CFLAGS) -c libcat/Clock.cpp


# Library objects

cymric.o : src/cymric.cpp
	$(CCPP) $(CFLAGS) -c src/cymric.cpp

blake2b.o : blake2/sse/blake2b.c
	$(CC) $(CFLAGS) -c blake2/sse/blake2b.c

chacha.o : chacha-opt/chacha.c
	$(CC) $(CFLAGS) -c chacha-opt/chacha.c

chacha_blocks_ssse3-64.o : chacha-opt/chacha_blocks_ssse3-64.S
	$(CC) $(CFLAGS) -c chacha-opt/chacha_blocks_ssse3-64.S


# Executable objects

cymric_test.o : tests/cymric_test.cpp
	$(CCPP) $(CFLAGS) -c tests/cymric_test.cpp


# Cleanup

.PHONY : clean

clean :
	-del test libcymric.a $(shared_test_o) $(cymric_test_o) $(cymric_o)


