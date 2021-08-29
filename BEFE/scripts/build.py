#!/usr/bin/env python3
# coding=utf-8
#===============================================================================
#
# File: build.py
#
# Purpose: Build BEFE executable
#
# Usage:   python build.py [args]
#
# Where:   args - Optional...
#
#===============================================================================
# Copyright (©) 2021, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

# External modules
import                    os.path
import                    string
import                    sys
import                    shutil
import                    colr
from   utils.funcs import execute
from   utils.funcs import Prefix
from   utils.funcs import red,green

# Default configurables
here = os.path.dirname(os.path.abspath(__file__))
root = os.path.abspath(here+'/..')
BEFE_SrcRoot  = root + '/BEFE-Core'
BEFE_BuildDir = root + '/temp'
BEFE_Src      = BEFE_BuildDir + '/_src'
BEFE_Inc      = BEFE_BuildDir + '/_inc'
BEFE_Obj      = BEFE_BuildDir + '/_obj'
BEFE_Bin      = BEFE_BuildDir + '/_bin'
#print("DEBUG: BEFE_SrcRoot  = %s"%repr(BEFE_SrcRoot))
#print("       BEFE_BuildDir = %s"%repr(BEFE_BuildDir))

# Handy flags
isWin32 = sys.platform == 'win32'

#-------------------------------------------------------------------------------
#
# Function: PathSplit
#
# Purpose:  Split an OS file path into constituent parts. Takes both Windows
#           (e.g. "c:\dir\dir...") and Linux/Unix (e.g. "/usr/bin") formats.
#
# Args:     path - String: Path to split
#
# Returns:  List of path parts.
#
# Notes:    Doesn't currently handle Windows network pathc (e.g. '\\mypc\...')
#
#           Leading '/' '\\' etc. will return empty strings in the list.
#

def PathSplit(path):
  if path.find('/') >= 0:
    return path.split('/')
  elif path.find('\\') >= 0:
    return path.split('\\')
  return path

#-------------------------------------------------------------------------------
#
# Function: PathNormalise
#
# Purpose:  Ensure an OS path is Linux/Unix instead of Windows format
#
# Args:     path - String: The path to normalise
#
# Returns:  Normalised path (e.g. "c:\SVN\befe" becomes "c:/SVN/befe"
#

def PathNormalise(thePath):
  if thePath.find('\\') >= 0:
    return string.join(thePath.split('\\'),'/')
  return thePath

#-------------------------------------------------------------------------------
#
# Generator: PathWalker
#
# Purpose:   Walk and visit every entry, deep, in a given OS path
#
# Args:      path - String: The path to walk
#
# Notes:     If the path doesn't exist or isn't a directory, PathWalker doesn't
#            generate anything
#
#            If you send this generator a False, it will skip going deep. This
#            is handy if you want to ignore certain directories. Example...
#
#              walker = PathWalker(path)
#              for path in walker:
#                # Skip if unwanted directory
#                if os.path.isdir(path) and dontWant(path):
#                  walker.send(false)
#                  continue
#                # We want this one...
#                ...
#
#             The paths generated by PathWalker are in normalised form
#             (see PathNormalise above)
#

def PathWalker(path):

  # See if anything to do
  path = PathNormalise(path)
  if type(path) != str or not os.path.isdir(path):
    return

  # Populate initial stack
  stack = PathSplit(path)
  for i in range(0,len(stack)):
    stack[i] = [stack[i]]

  # Until Finished...
  while stack:

    # Get current path
    path = '/'
    for level in stack:
      if path and path[-1] != '/':
        path += '/'
      path += level[0]

    # Give it to the caller...
    if os.path.isdir(path) or os.path.isfile(path):
      wanted = yield path
      if wanted == None:
        wanted = True
    else:
      wanted = False

    # If dir: Go deeper if wanted
    if wanted and os.path.isdir(path):
      contents = os.listdir(path)
      if contents:
        contents.sort()
        stack.append(contents)
        continue

    # Was either a file or an empty directory, get rid of it cascading...
    while stack:
      if not stack[-1]:
        del stack[-1]
        continue
      del stack[-1][0]
      if stack[-1]:
        break;

#-------------------------------------------------------------------------------
#
# Function: Clean
#
# Purpose:  Clean the build directory
#
# Args:     dir - String: Build directory (None means use BEFE_BuildDir)
#
# Returns:  None
#

def Clean(directory = None):

  if directory == None:
    directory = BEFE_BuildDir

  if not os.path.exists(directory) or not os.path.isdir(directory):
    return

  shutil.rmtree(directory)

#-------------------------------------------------------------------------------
#
# Function: Empty
#
# Purpose:  Create structered but empty build directory
#
# Args:     directory - String: Build directory (None means use BEFE_BuildDir)
#
# Returns:  None
#

def Empty(directory = None):

  # Default parameters
  if directory == None:
    directory = BEFE_BuildDir
  directory = PathNormalise(directory)

  # List of dirs to build
  dirs = ['bin',
          'doc',
          'home',
          'inc',
          'lib',
          'obj',
          'src',
          '_src',
          '_inc',
          '_obj',
          '_lib',
          '_bin',
         ]

  # Remove it
  Clean(directory)

  # Make them
  if not os.path.exists(directory):
    os.mkdir(directory)
  for sdir in dirs:
    os.makedirs(directory + '/' + sdir)

  return

#-------------------------------------------------------------------------------
#
# Function: copySource
#
# Purpose:  Copy source to build directory
#
# Notes:    If the destination file already exists (e.g. if the directories
#           haven't been cleaned or emptied), it will only be copied if
#           the destination file is older than the source file.
#

def copySource():

  # Destinations
  dests = {
            '.c':BEFE_BuildDir+'/_src',
          '.cpp':BEFE_BuildDir+'/_src',
            '.h':BEFE_BuildDir+'/_inc',
          }

  # Do it...
  walker = PathWalker(BEFE_SrcRoot)
  total = 0
  copied = 0
  for path in walker:
    WantIt = True
    if isWin32:
      dontWant = 'linux'
    else:
      dontWant = 'win32'
    if path.find('/.') >= 0: WantIt = False
    if dontWant in path:    WantIt = False
    if not WantIt:
      continue
    if not os.path.isfile(path):
      continue
    ext = os.path.splitext(path)[1].lower()
    if not ext in dests:
      continue

    total += 1

    # Copy it...
    dest = dests[ext]
    _,fileName = os.path.split(path)
    dest += '/'+fileName
    doit = True
    if os.path.exists(dest) and os.path.isfile(dest) and os.path.getmtime(path) <= os.path.getmtime(dest):
      doit = False
    if doit:
      if os.path.exists(dest):
        os.remove(dest)
      shutil.copy2(path,dest)
      #shutil.copystat(path,dest)
      copied += 1

  print("  Total Files  =",total)
  print("  Copied Files =",copied)

  return

#-------------------------------------------------------------------------------
#
# Function: compileSource - Compile everything
#
# Usage:    errCount = compileSource()
#
# Where:    errCount = Number of errors encountered
#

def compileSource():

  cmd = 'g++ -Wall -c %s -o %s -I %s -std=c++0x -fno-exceptions ' \
        '-finline-functions -nodefaultlibs -fno-rtti'
  # TEMP...
  cmd += ' -m32 -fno-pic -fno-threadsafe-statics -fno-use-cxa-atexit'
  # ...TEMP

  fileCount = 0
  errCount  = 0

  sys.stdout.flush()

  for path in PathWalker(BEFE_Src):

    spath = path
    sfile = os.path.basename(path)
    fname,ext = os.path.splitext(sfile)
    if ext not in ('.c','.cpp'):
      continue

    dpath = BEFE_Obj+'/'+fname+'.o'
    if os.path.exists(dpath) and os.path.getmtime(spath) < os.path.getmtime(dpath):
      continue

    tcmd = cmd%(spath,dpath,BEFE_Inc)
    #green('DEBUG: tcmd = %s'%repr(tcmd))
    print('  '+sfile+'...')
    sys.stdout.flush()

    rc,out,err = execute(tcmd)
    sys.stdout.flush()
    if rc:
      errCount += 1
      print("Error...")
      Prefix(err,prefix='...',samePrefix=True,fore='red',style='bright')
      break;

    fileCount += 1

  return errCount

#-------------------------------------------------------------------------------
#
# Block:   __main__
#
# Purpose:  Python Main
#

# NOTE: ld temp/_obj/*.o -o temp/bin/befe -m elf_i386 -fno-use-cxa-atexit 2>ld.err

def usage():

  red('usage() not implemented yet')

if __name__ == "__main__":

  args = sys.argv[1:]
  clean   = False
  copy    = True
  compile = True
  link    = True
  for arg in args:
    if arg == '+clean':
      clean = True
    elif arg == '-clean':
      clean = False
    elif arg == '-copy':
      copy = False
    elif arg == '+copy':
      copy = True
    elif arg in ('-c','-compile'):
      compile = False
    elif arg in ('+c','+compile'):
      compile = True
    elif arg == '-link':
      link = False
    elif arg == '+link':
      link = True
    else:
      usage()
      sys.exit(1)

  if clean:
    print('Cleaning...')
    Clean()
    print('Creating empty build structure...')
    Empty()

  if copy:
    print('Copying files...')
    copySource()

  if compile:

    print('Compiling...')
    errCount = compileSource()
    if errCount:
      print('  %d errors'%errCount)

  if link or True:

    print('Linking...')
    print('os.getcwd() = %s'%os.getcwd())
