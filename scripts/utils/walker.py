#!/usr/bin/env python3
#coding=utf-8
#===============================================================================
#
# File: walker - Full directory walker
#
# Usage: walker <path>
#
# Where: <path> root directory to walk
#
# Notes: This script deep walks the specified directory.
#
#        This is intended to be entirely standalone so we've cribbed some
#        functions from funcs.py
#

import sys
import os
import sys
import json
import time
import datetime

errors = 0
walkCount = 0
fileCount = 0

def dtNow(): # ◄── Cribbed from funcs.py

    result = datetime.datetime.fromtimestamp(time.time())

    return result

def deltaSeconds(delta): # ◄── Cribbed from funcs.py

    diff = float(delta.days*(60*60*24)+delta.seconds) + delta.microseconds/1000000.0

    return diff

def ignore(oserr):
    global errors
    errors += 1
    pass

def walk(rootDir,prefixes=[],dirsonly=False):

  global walkCount
  global fileCount

  walkCount = 0
  fileCount = 0

  def ignore(oserr):
    global errors
    errors += 1

  if os.path.isfile(rootDir):
    return [rootDir]

  items = []
  files = []
  numOpen = 0
  sys.stdout.flush()
  for entry in os.walk(rootDir,onerror=ignore):
    dirpath, dirnames, filenames = entry
    if dirsonly:
      for dir in dirnames:
        files.append(os.path.join(dirpath,dir))
    else:
      for filename in filenames:
        filename = os.path.join(dirpath,filename)
        skip = False
        for prefix in prefixes:
          if filename[:len(prefix)] == prefix:
            skip = True
            break
        if skip: continue
        if not os.path.isfile(filename):
          continue
        files.append(filename)
        walkCount += 1
      fileCount += len(filenames)

  return files

#-------------------------------------------------------------------------------
#
#  __main__
#

if __name__ == '__main__':

    args = sys.argv
    while len(args) < 2: args.append(os.path.abspath('.'))
    start = dtNow()
    files = walk(args[1])
    end = dtNow()
    print("walkCount  = %d"%walkCount)
    print("fileCount  = %d"%fileCount)
    print("len(files) = %d"%len(files))
    print("Took %.3f seconds"%deltaSeconds(end-start))
    if errors:
        print("%d errors"%errors)
    else:
        print(args[1]+' files...')
        for file in files:
          print('  '+file)
