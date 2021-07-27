@echo off
rem ---------------------------------------------------------
rem File: compile.cmd - Compile a single source file
rem ---------------------------------------------------------
rem
rem NOTE: We expect the first argument (%1) to be the file name
rem       without any path in it.
rem
rem NOTE: We expect %OBJ% to be a g++ compatible (with '/' not '\')
rem       path name to place the objects
rem
rem NOTE: We expect %SRC% to be an NT compatible (with '\' not '/')
rem       path name where the source file is
rem
rem NOTE: We expect %CCOPTS% to be g++ compile options
rem

echo   %1 -^> %OBJ%/%1.o...
g++ -c %SRC%\%1 %CCOPTS% -o %OBJ%/%1.o
