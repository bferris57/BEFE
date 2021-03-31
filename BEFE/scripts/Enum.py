#!/usr/bin/env python
# coding=utf-8
#===============================================================================
#
# File: enum.py
#
# Purpose: Enum and other datatype classes
#
# Usage:   python enum.py
#
#===============================================================================

# External modules

#===============================================================================
#
# Class:   Enum
#
# Purpose: Maintain an object with member names/value pairs equivalent to a
#          C/C++ enum.
#
# Usage:   a = Enum(membs)
#
# Where:   mems -   List: List of memdefs
#                   Str:  Comma separated string of memdefs
#
#          memdef - Str:  name[=value] name definition
#          value  - Int:  Value for member
#
# Example: a = Enum('a,b,c,d=100,e,f,g')
#          print a.e
#
#          This displays the value 101
#

class Enum(object):

  def __init__(self, members = [], start=0):

    cur  = start
    last = None
    cnt  = 0
    
    # Need some members
    if not members:
      raise AttributeError("Enum expects at least one member name")
      
    # Allow ',' separated list of names in a string...
    if type(members) == str:
      members = members.split(',')
    
    # Create the enum members...
    finalMembers = []
    for memb in members:
      
      name = memb.strip()

      # Handle '=' declarations
      parts = name.split('=')
      numParts = len(parts)
      if numParts < 1:
        if type(cur) is int:
          cur += 1
        continue
      elif numParts > 2:
        raise AttributeError("Enum expected single '=' in member '%s'"%name)
      elif numParts == 2:
        name,value = parts
        try:
          value = int(value)
        except:
          value = value.strip()
        name = name.strip()
        cur = value
        if not name:
          continue
      else:
        name = parts[0].strip()
      
      # Set it
      if name:
        finalMembers.append(name)
        self.__dict__[name] = cur
        last = cur
        cnt += 1
      if type(cur) is int:
        cur += 1
      
    # Save special members...
    self.__dict__['__members__'] = finalMembers
    self.__dict__['__min__']     = start
    self.__dict__['__max__']     = last
    self.__dict__['__count__']   = cnt
  
  def __setattr__(self, name, value):
  
    if name == '__members__':
      self.__dict__['__members__'] = value
      return
      
    if name in ['__min__', '__max__','__count__','__members__']:
      raise AttributeError("Enum attribute '%s' is immutable"%name)
      
    if not self.__dict__.has_key(name):
      raise AttributeError("Enum has no member named '%s'"%name)
      
  def __getattr__(self, name):
  
    if name in ['__min__', '__max__','__count__','__members__']:
      return self.__dict__[name]
      
    if name in ['__min__', '__max__','__count__']:
      raise AttributeError("Enum attribute '%s' is immutable"%name)
      
    if not self.__dict__.has_key(name):
      raise AttributeError("Enum has no member named '%s'"%name)
  
  def __getitem__(self, index):
  
    typ = type(index)
    if typ == int or typ == long:
      if index < 0 or index >= self.__dict__['__count__']:
        raise IndexError("Enum index %s is invalid "%index)
      return self.__dict__['__members__'][index]
      
    elif typ == str:
      if len(index) >= 2 and index[0:2] == '__':
        raise IndexError("Enum index '%s' is hidden"%index)
      if not self.__dict__.has_key(index):
        raise IndexError("Enum has no member '%s'"%index)
      return self.__dict__[index]
    else:
      raise IndexError("Enum can only be indexed by a string or integer")
    
  def __str__(self):

    result = ''
    for memb in self.__members__:
      if result:
        result += ', '
      result = result + memb + '=' + str(self[memb])
    return result
    
  def __repr__(self):
    return "Enum('" + str(self) + "')"
    
  def Members(self):
  
    for member in self.__dict__['__members__']:
      yield member
    
  def Min(self):
  
    return self.__dict__['__min__']
    
  def Max(self):
  
    return self.__dict__['__max__']
    
  def Count(self):
  
    return self.__dict__['__count__']
    
#===============================================================================
#
# Function: NoneList
#
# Purpose:  Create a list of N None values
#

def NoneList(numNones):

  theList = []
  for i in range(0,numNones):
    theList.append(None)
  return theList

#===============================================================================
#
# Code:    Main
#
# Purpose: Invoked when passed to Python on the command line (e.g. "python enum.py")
#

if __name__ == '__main__':

  a = Enum(',,,,ee = 3,ff,ggg')
  
  # Display it
  print("1...")
  membs = ''
  indent = ' '*2
  for memb in a.Members():
    if membs: membs += ','
    membs += memb+'='+str(a[memb])
  print(indent+'a.Members() =',membs)
  print(indent+'a.Min()     =',a.Min())
  print(indent+'a.Max()     =',a.Max())
  print(indent+'a.Count()   =',a.Count())
  print('')
  a.ggg = 'Hi there!'
  print(indent+'a.ggg       =',repr(a.ggg))
  
  # Access it and print result
  struct = NoneList(a.Max()+1)
  struct[a.ee] = 'Hi there'
  struct[a.ggg] = 'Hello'
  print("2...")
  print(indent+"struct =",repr(struct))
  
