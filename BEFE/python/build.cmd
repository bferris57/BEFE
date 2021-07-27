@echo off
rem
rem This script demonstrates how to used BEFE to build itself...
rem

befe build -debug +vv init+copyin+svn+compile+release+lib+exe+publib+pubexe+pubheaders+pubhome+zip;x