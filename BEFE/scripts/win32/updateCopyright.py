# coding=utf-8
#----------------------------------------------------------------------
#
# This script walks through the BEFE SVN tree and changes the copyright
# notices.
#
#----------------------------------------------------------------------

import os

def processFile(fname):

  changed = False

  print 'FIL: '+fname+'...',

  # Get the lines
  file = open(fname,'r')
  lines = file.readlines()
  file.close()
  
  # Find the copyright lines
  searchFor = '// Copyright'
  changeTo  = '// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk\n'
  for lineNo in range(0,len(lines)):
    if lines[lineNo][:len(searchFor)] == searchFor:
      lines[lineNo] = changeTo
      changed = True
      break;

  # If changed, rewrite it
  if changed:
    file = open(fname,'w')
    file.writelines(lines)
    file.close()
    #print '- Changed'
  else:
    print '- Not Changed',
    
  print '\n',

  return changed

def processDirs(startDir):

  dirs = [startDir]
  dirPos = 0
  numFiles = 0
  numChanged = 0

  while dirPos < len(dirs):
    curdir = dirs[dirPos]
    contents = os.listdir(curdir)
    for fildir in contents:
      fildir = curdir+'/'+fildir
      if os.path.isdir(fildir):
        dirs.append(fildir)
      else:
        ext = os.path.splitext(fildir)[-1]
        if ext in ['.py','.c','.cpp','.h']:
          numFiles = numFiles + 1
          didIt = processFile(fildir)
          if didIt:
            numChanged = numChanged + 1
    dirPos = dirPos + 1

  print 'Total Files:      ',numFiles
  print 'Changed Files:    ',numChanged
  print 'Total Directories:',len(dirs)
  
if __name__ == '__main__':
 
  dir = 'c:/SVN/BEFE-Core'
  processDirs(dir)