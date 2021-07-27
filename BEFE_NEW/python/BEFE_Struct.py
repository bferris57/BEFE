#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BNF_Struct.py
#
# Purpose: Class Struct and related definitions
#
#          Struct is an attempt to handle structured data in Python in a way
#          similar to SQL, C, C++, Java, etc.
#
#          We do this by using ElmDefs and ordering struct members.  But...
#          we also provide the mechanism to access struct members by name and
#          index as well as imposing type value restructions based on the
#          information described in the member's ElmDef (see BNF_ElmDef)
#
#===============================================================================#===============================================================================

# External dependencies
from BEFE_Enum   import *
from BEFE_ElmDef import *
import copy

#===============================================================================
#
# Class:   Struct
#
# Purpose: Maintain an object with an ordered set of named values.
#
# Usage:   a = Struct(enum)
#
# Where:   enum - Enum: Member enumeration
#                 str:  Enum specification
#
# Example: a = Struct('a,b,c,d,e int,f,g')
#          a.b = "Hi there"
#          a.e = 10
#          print(a)
#
#          This display: [None,'Hi there',None,None,10,None,None]
#
# Notes:   The enum must not have negative values
#

class Struct(object):

  def __init__(self, stmt=None):

    self.Define(stmt)
      
  # Note: We've added this so we can to "if struct:..."
  #       Basically, we return the count of non-None values  
  def __nonzero__(self):
  
    vals = self.__dict__['__vals__']
    for val in vals:
      if val != None:
        return True
    return False

  def Define(self, stmt=None):
   
    dict = self.__dict__
    dict['__defs__'] = []
    dict['__vals__'] = []
    dict['__elmidx__'] = {}
    
    if not stmt:
      return
      
    if type(stmt) != str:
      raise AttributeError('Struct.Define: Expected string')
      
    # Parse and validate
    errors, elmdefs = ElmDefs(stmt)
    if errors:
      raise AttributeError('Struct.Define: Element declarations have errors')
      
    elmidx = {}
    defs = []
    vals = []
    
    for elm in elmdefs:
      if elm.name in elmidx:
        raise AttributeError("Struct.Define: Element '"+elm.name+"' declared multiple times")
      elmidx[elm.name] = len(defs)
      defs.append(elm)
      vals.append(None)
      
    dict['__elmidx__'] = elmidx
    dict['__defs__']   = defs
    dict['__vals__']   = vals
      
    return
        
  def AddElmDef(self, elmdef):
  
    dict = self.__dict__
    elmidx = dict['__elmidx__']
    defs   = dict['__defs__']
    vals   = dict['__vals__']
    
    if elmdef.name in elmidx:
      raise AttributeError("Element '"+elmdef.name+"' already defined")

    # Create it...
    elmidx[elmdef.name] = len(defs)
    defs.append(elmdef)
    vals.append(None)
    
  def GetElmDef(self,name):
  
    dict = self.__dict__
    elmidx = dict['__elmidx__']
    vals = dict['__vals__']
    defs = dict['__defs__']
    if type(name) not in [int,str]:
      raise AttributeError("Expected name to be a string or integer")
      
    if type(name) in [int,long]:
      if name < 0 or name >= len(defs):
        return None
      elm = copy.copy(defs[name])
    else:
      if name not in elmidx:
        return None
      elm = copy.copy(defs[elmidx[name]])
      
    return elm
          
  def ElementNames(self):
  
    dict = self.__dict__
    elmidx = dict['__elmidx__']
  
    names = []
    for key in elmidx:
      names.append((elmidx[key],key))
    names.sort()
    
    names = [n[1] for n in names]
    
    return names
    
  def __setattr__(self, name, value):
  
    if name[:2] == '__':
      raise AttributeError("Immutable member "+repr(name))
      
    dict = self.__dict__
    elmidx = dict['__elmidx__']
    defs   = dict['__defs__']
    vals   = dict['__vals__']

    if name not in elmidx:
      raise AttributeError("Unknown member "+repr(name))
    
    vals[elmidx[name]] = value
    
    return
      
  def __getattr__(self, name):
  
    dict = self.__dict__
    elmidx = dict['__elmidx__']
    defs   = dict['__defs__']
    vals   = dict['__vals__']

    if name not in elmidx:
      raise AttributeError("Unknown member "+repr(name))
    
    return vals[elmidx[name]]

  def __getitem__(self, index):
  
    dict = self.__dict__
    elmidx = dict['__elmidx__']
    vals   = dict['__vals__']
    
    typ = type(index)
    
    if typ in [int,long]:
    
      if index < -len(vals) or index >= len(vals):
        raise IndexError("Struct index %s out of range"%index)
      return vals[index]
      
    elif typ == str:
    
      return self.__getattr__(self, index)
      
    else:
    
      raise IndexError("Struct can only be indexed by a string or integer")

  def GetValues(self):
  
    # Return a copy
    return list(self.__vals__)
    
  def SetValues(self, value):

    dict = self.__dict__
    elmidx = dict['__elmidx__']
    vals   = dict['__vals__']

    typ = type(value)
    if typ not in (list,tuple):
      raise AttributeError("Struct: Expected assignment to be a list or a tuple")
    if len(value) < len(vals):
      raise AttributeError("Struct: Expected value to have "+str(self.__enum__.Max()+1)+" or more entries")
      
    # Take a copy...
    dict['__vals__'] = list(value)
    
  def __len__(self):
  
    return len(self.__dict__['__vals__'])
        
  def __str__(self):

    dict = self.__dict__

    return str(dict['__vals__'])
    
  def __repr__(self):

    dict = self.__dict__
    defs   = dict['__defs__']
    elmidx = dict['__elmidx__']
    vals   = dict['__vals__']

    # Build list of (idx,name) values...
    elms = []
    for key in elmidx:
      elms.append((elmidx[key],key))
    elms.sort()
    
    # Build the repr...
    s = ''
    prev = -1
    for elm in elms:
      idx,name = elm
      if s:
        s += ','
      s += repr(defs[idx])
    s = self.__class__.__name__+"("+s+")"
    
    return s
    
#===============================================================================
#
# Code:    Main
#
# Purpose: Invoked when passed to Python on the command line (e.g. "python enum.py")
#

if __name__ == '__main__':

  a = Struct('a,b,c,d,e,type')
  a.a = "Hi there"
  a.e = "Hello"
  
  # Display it
  print("Struct a =",a)
  print("Struct a.a =",repr(a.a))
  print("Struct a.e =",repr(a.e))
  print("Struct a.b =",repr(a.b))
  print("Struct a.type =",repr(a.type))
