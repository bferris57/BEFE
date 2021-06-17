#!/usr/bin/env python
from build import *

#-------------------------------------------------------------------------------
# Build Testers
#-------------------------------------------------------------------------------

def testPathwalker():

  total  = 0
  wanted = 0
  walker = PathWalker(BEFE_SrcRoot)
  files  = 0
  dirs   = 0

  want = 'c,cpp,h,py,txt,doc,docx,pdf,cmd'
  exts = []
  cnts = []
  for ext in want.split(','):
    exts.append('.'+ext)
    cnts.append(0)
    
  for path in walker:
    total += 1
    wantIt = True
    if isWin32:
      dontWant = 'linux'
    else:
      dontWant = 'win32'
    wantIt = dontWant not in path
    if not wantIt:
      walker.send(False)
      continue
    wanted += 1
    if os.path.isdir(path): dirs += 1
    if os.path.isfile(path): files += 1
    ext = os.path.splitext(path)[1].lower()
    if ext in exts:
      cnts[exts.index(ext)] += 1
      # DEBUG...
      if 'Port.h' in path:
        print('DEBUG: TestPathWalker: path = %s'%repr(path))
      # ...DEBUB
    
  print("Total: ",total)
  print("Wanted:",wanted)
  print("Files: ",files)
  print("Dirs:  ",dirs)
  for ext in exts:
    print(ext+':',cnts[exts.index(ext)])
    
#-------------------------------------------------------------------------------
# Main
#-------------------------------------------------------------------------------

if __name__ == '__main__':

  print('DEBUG: BEFE_SrcRoot = %s'%repr(BEFE_SrcRoot))
  testPathwalker()
