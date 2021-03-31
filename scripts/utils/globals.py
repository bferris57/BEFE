#!/usr/bin/python3
#coding=utf-8
#===============================================================================
#
# File: globals.py - Module to handle porridge.py globals
#
#===============================================================================

import os
import sys

import debug as deb

# For Python compatability tests...
python2 = sys.version[0:2] == '2.'
python3 = sys.version[0:2] == '3.'

# Note: Have to do this because of Python 2/3 incompatabilities...
#       Python2 module ConfigParser makes stuff of type 'unicode' which
#       doesn't exist in Python3.
if python3:
    unicode = str
else:
    unicode = unicode

#
# Globals
#

quiet       = False
verbose     = deb.verbose
veryverbose = deb.veryverbose
debug       = deb.debug
bash        = False           # ◄── A bit annoying, we'll turn it off for now

_types_ = {

  'quiet'       : bool,
  'verbose'     : bool,
  'veryverbose' : bool,
  'debug'       : bool,
  'bash'        : bool,
}

# OatConnections...
oats       = {}
currentOat = None

# Porridge input line queue...
g_lineQueue = []

# Stack of global values...
_stack_ = []

#------------------------------------------------------------------------------
#
# Function: push - Push current globals on stack
#           pop  - Pop current globals from stack
#

def push():

    save = {}
    for key in _types_.keys():
        save[key] = globals()[key]
    _stack_.append(save)

def pop():

    if not _stack_:
        raise RuntimeError("globals._stack_ is empty")
    save = _stack_[-1]
    del _stack_[-1]
    for key in save.keys():
        globals()[key] = save[key]

#------------------------------------------------------------------------------
#
# Function: members - Return list of named global members
#

def members():

    mem = list(_types_.keys())
    mem.sort()
    return mem

#------------------------------------------------------------------------------
#
# Function: set - Set a named global's value
#           get - Get a named global's value
#
# Usage:            set(name,value)
#           value = get(name)
#
# Where:    name  - str: Named value
#           value - ???: Value to set/or get
#
# Raises:   TypeError - Attempt to set the wrong type of value
#           NameError - Unknown global
#
#------------------------------------------------------------------------------

def set(name,value):

    if type(name) is not str:
        raise("Expected name to be a string")
    if name not in _types_:
        raise("Unknown global %s"%repr(name))
    t = _types_[name]
    if type(value) is not t:
        raise TypeError("Expected %s value to be %s, you passed %s"%(repr(name),repr(t.__name__),repr(type(value).__name__)))

    globals()[name] = value

def get(name):

    if type(name) is not str:
        raise("Expected name to be a string")
    return globals()[name]

#------------------------------------------------------------------------------
#
# Function: show - Display global values
#
#------------------------------------------------------------------------------

def show():

    globs = globals()
    maxLen = 0
    names = list(_types_.keys())
    names.sort()
    for name in names:
        maxLen = max(maxLen,len(name))
    print('')
    print("Settings...")
    print('')
    for name in names:
        print(name+' '*(maxLen-len(name))+" = "+repr(globals()[name]))
    print('')

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == "__main__":

    print("__main__: globals()['quiet'] = %s"%repr(globals()['quiet']))
