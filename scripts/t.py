#!/usr/bin/env python3
import                     os
import                     sys
from   utils.walker import walk
from   utils.funcs  import preEllipse
from   utils.funcs  import postEllipse
from   utils.funcs  import green,red

#------------------------------------------------------------------------------
#
# Function: expand - Expand file/path to full path
#

def expand(path):

  return os.path.abspath(os.path.expanduser(path))

#---
#
# Main
#

if __name__ == '__main__':

  srcdir  = expand('~/usb_src/bb')
  dstdir  = expand('~/sd_dst/bb')

  print('DEBUG: srcdir = %s'%repr(srcdir))
  print('DEBUG: dstdir = %s'%repr(dstdir))

  green('Getting files to copy...')
  files = list(walk(srcdir))
  files.sort()
  print('%d files...'%len(files))
  for file in files[:10]:
    print('  '+file)
  print('')

  print('Building dict...')
  src = {}
  for file in files:
    fname = os.path.basename(file)
    dname = file[:-len(fname)]
    if dname not in src:
      src[dname] = [] 
    src[dname].append(fname)
  minfiles = 0x7fffffff
  maxfiles = 0
  dirs = list(src.keys())
  dirs.sort()
  print('  %d directories'%len(dirs))
  for key in dirs:
    nfiles = len(src[key])
    minfiles = min(minfiles,nfiles)
    maxfiles = max(maxfiles,nfiles)
    print('  '+postEllipse(key,80,fill='.')+' %d files'%nfiles)
    todir = dstdir+key[len(srcdir):]
    print('    -> %s'%postEllipse(todir,78))
    if nfiles < 10:
      for file in src[key]:
        print('    '+file)
    src[key].sort()
  print('')
  print('  Minimum files: %d'%minfiles)
  print('  Maximum files: %d'%maxfiles)
