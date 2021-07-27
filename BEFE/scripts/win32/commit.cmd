@echo off
echo Doing: svn commit -m %1
echo ...
if [%1] == [] goto expectarg
if not [%2] == [] goto expectonly
svn commit -m %1
goto :eof
:expectarg
echo ***Expected argument***
goto :eof
:expectonly
echo ***Expected only one argument, the commit message***