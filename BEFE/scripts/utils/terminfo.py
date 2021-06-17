#!/usr/bin/env python3
#  coding=utf-8
#==============================================================================
#
# Module:    terminfo - Retrieve Linux terminal configuration
#
# Functions: terminfo - Get dictionary of configuration (linux only)
#
#=============================================================================
# Copyright (C) 2021 Bruce Ferris (befe@bferris.co.uk)
#=============================================================================

import                os
import                sys
from   errors  import InternalError
from   errors  import Error
from   funcs   import isInt
from   DotDict import DotDict

def terminfo(termname=os.environ['TERM']):

  lines = os.popen('infocmp -1 -E %s'%repr(termname)).read().split('\n')

  if 'infocmp: ' in lines[0]:
    raise InternalError(lines[0])
  
  # Strip leading and trailing spaces from lines...
  for lineno in range(0,len(lines)):
    line = lines[lineno].strip()
    if line.endswith(','):
      line = line[:-1]
    lines[lineno] = line

  tinfo = {}
  c_name = ''

  for lineno in range(0,len(lines)):

    line = lines[lineno]

    # Handle ..._alias_data
    if '_alias_data[]' in line:
      lookfor = 'static char '
      pos1 = line.find(lookfor) + len(lookfor)
      pos2 = line.find('_alias_data')
      c_name = line[pos1:pos2]
      pos1 = line.find('[] = "') + 6
      pos2 = line.find('";')
      alias_data = line[pos1:pos2]
      tinfo['alias_data'] = alias_data
      tinfo['termname'] = termname
      tinfo['c_name']   = c_name
      continue

    if not c_name:
      continue

    # Handle escape formats...
    lookfor = 'static char '+c_name+'_s_'
    pos1 = line.find(lookfor)
    if pos1 >= 0:
      line = line[pos1+len(lookfor):]
      pos2 = line.find('[] =')
      key  = line[:pos2]
      pos3 = pos2 + 5
      pos4 = line.find('";')
      evalstr = line[pos3:pos4+1]
      val  = eval(evalstr)
      tinfo[key] = val
      continue

    # Handle boolean and numeric values...
    if '/*' in line and '*/' in line:
      pos1 = line.find(': ') + 2
      pos2 = line[pos1:].find(' ')+pos1
      key = line[pos1:pos2]
      pos1 = line.find('*/') + 2
      line = line[pos1:]
      pos2 = line.find(',')+pos1
      val = line[:pos2]
      while val and val[0] in ' \t':
        val = val[1:]
      if val == 'ABSENT_NUMERIC':
        continue
      elif val == 'FALSE':
        val = False
      elif val == 'TRUE':
        val = True
      elif isInt(val):
        val = int(val)
      else:
        raise InternalError('Unexpected format at line %d: %s'%(lineno+1,repr(line)))
      tinfo[key] = val
      continue

    # Finished if we've got to the pointers...
    if 'static char * ' in line:
      break

  return DotDict(tinfo)

#---
#
# __main__
#
#---

if __name__ == '__main__':

  if 1:

    if len(sys.argv) == 2:
      tinfo = terminfo(sys.argv[1])
    elif len(sys.argv) == 1:  
      tinfo = terminfo()
    else:
      print(InternalError("Expected only 1 argument, got %d"%(len(sys.argv)-1)))
      sys.exit(1)

    # Display tinfo...
    print('\n\nterminfo for %s...'%tinfo['termname'])
    keys = list(tinfo.keys())
    keys.sort()

    maxkey = 0
    for key in keys:
      maxkey = max(maxkey,len(key))

    for key in keys:
      val = tinfo[key]
      vallen = 0 if type(val) != str else len(val)
      print('  %s:%s (%d) = %s'%(key,' '*(maxkey-len(key)),vallen,repr(val)))
  
