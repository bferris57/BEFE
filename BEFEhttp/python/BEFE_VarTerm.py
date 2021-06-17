#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File: BEFE_VarTerm.py
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_errors import *
from BEFE_Stack  import *

class VarTerm:

  def __init__(self):
  
    self.StartUp()
    
  def __init__(self,iString):
  
    if type(iString) != str:
      raise RuntimeError("Expected string parameter")
    self.theSpec = iString
    self.Parse()
    
  def StartUp(self):
  
    self.theSpec    = None
    self.theStrings = []
    
  def ShutDown(self):
  
    del theSpec
    del theStrings
    
  def Copy(self):
  
    that = VarTerm()
    that.theSpec    = self.theSpec
    that.theStrings = self.theStrings
    
  def Parse(self):
  
    self.theStrings = [""]
    if self.theSpec == None or len(self.theSpec) == 0:
      return
    stack = Stack()
    
    for pos in range(0,len(self.theSpec)):
    
      c = self.theSpec[pos]
      prevIsBackSlash = False if pos == 0 else self.theSpec[pos-1] == '\\'
      
      if len(stack): top = stack[-1]
      else:          top = 0
      numStrings = len(self.theStrings)
        
      # Handle '['...
      if c == '[' and not prevIsBackSlash:
      
        # We're now in another level
        stack.append(numStrings)
        
        # Push copies of appripriate strings...
        for i in range(top,numStrings):
          tString = self.theStrings[i]
          self.theStrings.append(tString)
          
        continue
      
      # Handle ']'...
      if c == ']' and not prevIsBackSlash:
      
        if len(stack) == 0:
          raise RuntimeError("Mismatched ']' in '%s'"%self.theSpec)
        del stack[-1]

        continue

      # Normal character...
      for i in range(top,numStrings):
        tString = self.theStrings[i]
        if c != '\\':
          tString += c
        self.theStrings[i] = tString
        
      continue
      
    # Remove duplicates...
    j = 0
    while j < len(self.theStrings):
      i = 0
      while i < j and self.theStrings[i] != self.theStrings[j]:
        i += 1
      if i < j:
        del self.theStrings[i]
      else:
        j += 1

    # Reverse them so longest one is first and shortest one is last...
    i = 0
    numStrings = len(self.theStrings)
    while i < len(self.theStrings)/2:
      t = self.theStrings[i]
      self.theStrings[i] = self.theStrings[-1-i]
      self.theStrings[-1-i] = t
      i += 1
    return

def VarTermMap(stringList):

  # Sanity checks...
  if type(stringList) != list:
    raise ValueError("Expected list of strings")
    
  theMap = {}
  for idx in range(0,len(stringList)):
    string = stringList[idx]
    if type(string) != str:
      raise ValueError("Expected list of strings (stringList[%d] isn't a string)"%idx)
    vt = VarTerm(string)
    strings = vt.theStrings
    key = strings[0]
    for string in strings:
      theMap[string] = key
    
  return theMap
    
if __name__ == "__main__":

  cases = ["[e]x[clusive]or",
           #"e]x[clusive]or",
           "[e]x[it]",
           "bool[ean]",
           "[[[[[a]b]c]d]e]f",
           "enum[eration]",
           "char[acter]",
           "int[eger]",
           "long",
           "float[ing][point]",
           "float\\[ing\]\\[point\\]",
           ]
  for case in cases:
    a = VarTerm(case)
    print("case = `"+a.theSpec+"`")
    for i in range(0,len(a.theStrings)):
      print("  %d: `%s`"%(i,a.theStrings[i]))
    print("")
    
  mapCases = ['if','else','then','dir[ectory]']
  theMap = VarTermMap(mapCases)
  s = ''
  for case in mapCases:
    if s: s += ','
  print("mapCases = "+s)
  maxLen = 0
  for key in theMap: maxLen = max(maxLen,len(key))
  print("Map...")
  for key in theMap:
    print('  '+repr(key)+':'+' '*(maxLen-len(key))+" = "+repr(theMap[key]))