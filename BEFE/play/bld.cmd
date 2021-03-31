@echo off
rem ---------------------------------------------------------
rem File: bld.cmd - Build %1.cpp using BEFE
rem ---------------------------------------------------------

set BLD=c:/befe/builds/befe-0.4
set LIB=%BLD%/lib
set INC=%BLD%/inc
set MAP=-Xlinker -Map=main.map
g++ %1.cpp -std=c++0x -I %INC% -L%LIB% -lBEFE-0.4 -o %1.exe %MAP%