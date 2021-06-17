#!/usr/bin/python
#coding=utf8
'''
import hashlib

fn = "c:/svn/python/t.befe"
data = bytearray(open(fn,'rb').read())
print("type(data) = "+repr(type(data)))
print("len(data)  = "+repr(len(data)))
dig = hashlib.md5()
dig.update(data)
md5 = bytearray(dig.digest())
print("len(md5)   = "+repr(len(md5)))
print("type(md5)  = "+repr(type(md5)))
print("md5        = "+repr(md5))
s = '0x'
for b in md5:
  s += '%02x'%b
print("mdt        = "+s)

from BEFE_ParseTree import *

if __name__ == '__main__':

  fn = 'c:/svn/python/t.befe'
  fnode = ParseFile(fn)
  if fnode.data is not None:
    print("type(fnode.data) = "+repr(type(fnode.data)))
    print("len(fnode.data)  = "+repr(len(fnode.data)))
    s = '0x'
    for b in fnode.md5: s += '%02x'%b
    print("md5              = "+s)
  else:
    print("File %s does not exist"%repr(fn))
'''

a = '◄──'
b = bytes(a)
print("type(a) = "+repr(type(a)))
print("type(b) = "+repr(type(b)))
print("len(a)  = "+repr(len(a)))
print("len(b)  = "+repr(len(b)))
print("a = %s"%repr(a))
print("b = %s"%repr(b))