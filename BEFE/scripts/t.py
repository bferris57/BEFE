#!/usr/bin/env python
# coding=utf-8

from Enum import Enum
import sys
 
python2 = sys.version.startswith('2.')
python3 = sys.version.startswith('3.')

if python3:
  print('DEBUG: python3 = True')
  long = int
print('DEBUG: type(long) = %s'%repr(type(long)))

if __name__ == '__main__':

  print('type(Enum) = %s'%repr(type((Enum))))
  a = Enum(',,,,ee = 100,ff,ggg')
  membs = ''
  for memb in a.Members():
    if membs: membs += ','
    membs += memb+'='+str(a[memb])
  print('a.Members()   =',membs)
  print('a.Min()       =',a.Min())
  print('a.Max()       =',a.Max())
  print('a.Count()     =',a.Count())
  print('')
  a.ggg = 'Hi there!'
  print('a.ggg         =',repr(a.ggg))
  print('a.__members__ =',a.__members__)
  print('str(a)        =',str(a))
  print('repr(a)       =',repr(a))
