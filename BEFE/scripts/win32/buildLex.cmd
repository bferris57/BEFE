@echo off
rem ---------------------------------------------------------
rem File: buildLex.cmd - Build BEFE-Lex executable
rem ---------------------------------------------------------

echo ***
echo *** BuildLex - Build BEFE-Lex.EXE
echo ***
echo.

pushd C:\BEFE\play

set BLD=C:/befe/builds/befe-0.4
set SRC=C:\SVN\BEFE-Lex
set LIB=%BLD%/lib
set INC=%BLD%/_inc
rem set MAP=-Xlinker -Map=befe-lex.map
set MAP=
rem set CCOPTS=-g -O0 -Wall -std=c++0x -I %INC%
set CCOPTS=-O0 -Wall -std=c++0x -I %INC%
set OBJ=C:/befe/play/obj
set EXE=./befe-lex.exe

rem
rem Start Clean...
rem

echo Clean start...
rm -f obj/*
rm -f %EXE%

rem
rem Build the Sources...
rem

echo Compiling...
pushd %SRC%
for /F %%f in ('dir /B *.cpp') do call compile %%f
popd

rem
rem Build the Executable...
rem

echo Building Executable...

g++ %OBJ%/*.o -L%LIB% -lBEFE-0.4 -o %EXE% %MAP%

:BUILDEXE

popd
