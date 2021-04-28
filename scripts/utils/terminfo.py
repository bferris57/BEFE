#!/usr/bin/env python3
#  coding=utf-8

import os
from   errors import *
from   funcs  import isInt

if __name__ == '__main__':

  def stripList(that):
    for entryno in range(0,len(that)):
      entry = that[entryno].strip()
      that[entryno] = entry
    return that

  termname = 'xterm'

  lines = os.popen('infocmp %s'%repr(termname)).read().split('\n')
  print('type(lines) = %s'%repr(type(lines)))
  print('len(lines)  = %d'%len(lines))

  if 'infocmp: ' in lines[0]:
    raise InternalError(lines[0])
  
  # Strip leading and trailing spaces and ',' from lines...
  for lineno in range(0,len(lines)):
    line = lines[lineno].strip()
    if line.endswith(','):
      line = line[:-1]
    lines[lineno] = line

  # DEBUG...
  for lineno in range(0,min(10,len(lines))):
    line = lines[lineno]
    print('%2d: %s'%(lineno+1,line))
  # ...DEBUG

  tinfo = {}
  for lineno in range(0,len(lines)):
    line = lines[lineno]
    if line.startswith('#'):
      continue
    parts = stripList(line.split(','))
    while parts:
      part = parts[0]
      del parts[0]

      key = ''
      val = ''
      if '=' in part:
        subparts = stripList(part.split('='))
        key = subparts[0]
        val = '='.join(subparts[1:])
      elif '#' in part:
        subparts = stripList(part.split('#'))
        key = subparts[0]
        val = '#'.join(subparts[1:])
        if isInt(val):
          val = int(val)
      else:
        key = part
      if key in tinfo:
        raise InternalError('Key %s already in terminfo at line %d'%(repr(key),lineno+1))
      tinfo[key] = val
  
  subvals = {
    }

  # Display tinfo...
  print('\n\nterminfo for %s...'%termname)
  keys = list(tinfo.keys())
  keys.sort()

  maxkey = 0
  for key in keys:
    maxkey = max(maxkey,len(key))

  for key in keys:
    val = tinfo[key]
    vallen = 0 if type(val) != str else len(val)
    print('  %s:%s (%d) = %s'%(key,' '*(maxkey-len(key)),vallen,repr(val)))
