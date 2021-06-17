#!/bin/bash
#-----------------------------------------------------------------------
# File: main.bash - Ubuntu example compile/link using BEFE
#-----------------------------------------------------------------------
#
# This sample bash script demonstrates a build using the installed BEFE
# source 'main.cpp' as the main program and creating an gdb debuggable
# executable named 'main' in the current directory.
#
# Note: You'll probably have to adjust the settings below so that the
#       directories make sense in your installation.
#
# Note: You'll need to have the uuid library installed.  If you don't
#       have this on your version of Ubuntu, use the following command
#       to get it, because we can't generate proper random UUIDs without
#       that library...
#
#         sudo apt-get install uuid-dev
#
#-----------------------------------------------------------------------
# Copyright (©) 2011-2012, Bruce Ferris (UK) Email: befe@bferris.co.uk
#-----------------------------------------------------------------------

# Note: Change this to reflect where you unzipped your copy of BEFE
base=./BEFE-0.3

# Note: Change these to reflect the BEFE version number which we
#       embed in the library name...
copts="-g -O0 -Wall -std=c++0x -fno-exceptions -finline-functions -fno-rtti"
lopts="-o main -L./BEFE-0.3/lib -lBEFE-0.3Debug -lrt -luuid"

# Right, do it...
g++ $copts $base/src/main.cpp -I $base/inc $lopts

#
# There, that was quite simple, wasn't it?
#
#-----------------------------------------------------------------------
