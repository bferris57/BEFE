@echo off
del t.exe 2>nul
rem g++ t.cpp -fpermissive -w -o t.exe
g++ t.cpp -o t.exe
if exist t.exe t