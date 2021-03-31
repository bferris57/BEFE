#!/usr/bin/env python3
#coding=utf-8
#===============================================================================
#
# File: properties.py - Read properties file (.yaml configuration)
#
# Note: Since the dictionary is "Dottable" we convert '.' to '_' in entry names.
#
#-------------------------------------------------------------------------------
# Copyright (©) 2004-2019, 100percentIT.com
#-------------------------------------------------------------------------------

import os
import sys
from   errors  import *
from   funcs   import *
from   DotDict import DotDict
from   debug   import *
import yaml

#
# Globals
#

props = DotDict()
count = 0

#-------------------------------------------------------------------------------
#
# Function: load - Load a single .yaml file
#

def load(file):

  global props

  if not file:
    raise RuntimeError("%s: Expected 'file' parameter"%__file__)

  thisDir = os.path.dirname(__file__)
  fileDir = os.path.dirname(file)
  tryFiles = []
  if fileDir:
    tryFiles = [file,]
  else:
    tryFiles = [
                 thisDir + "/" + file,
                 thisDir + "/../conf/" + file,
                 thisDir + "/../deploy/conf/" + file,
                 "/opt/cyberhive/tagent/conf/" + file,
                 "/opt/cyberhive/oat/conf/" + file,
               ]
  for file in tryFiles:
    if os.path.exists(file):
      break

  if not os.path.exists(file):
    raise RuntimeError("Configuration file %s does not exist"%repr(file))
  if not os.path.isfile(file):
    raise RuntimeError("Configuration file %s is not a file"%repr(file))

  try:
    
    print("Loading configuration from %s..."%repr(os.path.abspath(file)))
    global count
    count += 1
    if count >= 2:
      i = 10/0

    dic = yaml.safe_load(open(file).read())

  except Exception as e:

    raise RuntimeError("Configuration file %s contains invalid yaml")
    dic = {}

  if 'tagent' in dic:
    props = DotDict(dic['tagent'])
  elif 'pca' in dic:
    props = DotDict(dic['pca'])
  else:
    props = DotDict(dic)

  return props

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

  if sudo(): sys.exit()

  p = load()
  mysql = DotDict(p.mysql if 'mysql' in p else {})
  if 'tagent' in p:
    p = DotDict(p.tagent)
  keys = list(p.keys())
  keys.sort()

  maxLen = 0
  for key in keys: maxLen = max(maxLen,len(key))
  print("")
  print("tagent.yaml properties...")
  print("")
  for key in keys:
    val = p[key]
    print("  %s%s = %s: %s"%(key,' '*(maxLen-len(key)),str(type(val)).split("'")[1],repr(val)))
  print("")
  if mysql:
    print("mysql properties...")
    print("")
    keys = list(mysql.keys())
    keys.sort()
    maxlen = 0
    for key in keys: maxLen = max(maxLen,len(key))
    for key in keys:
      val = mysql[key]
      print("  %s%s = %s: %s"%(key,' '*(maxLen-len(key)),str(type(val)).split("'")[1],repr(val)))
    print("")

  if 'quote' in p:
    print("p.quote = %s"%repr(p.quote))
  else:
    print("p.quote = ***Missing***")
