#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File: BNF_MinExp.py - MinExp and related classes
#
# Description: The MinExp (Minimimalist Expression)Comis responsible for handling
#              "name expressions".  Think of a MinExp as a "Minimalist RegEx"...
#
#              RegEx's have repitition, MinExp's don't. So, as it turns out,
#              instead of computing matches on the fly, as RegEx's do, a MinExp
#              can have the possible matches performed ahead of time.
#
#              A MinExp has the following special symbols (which can be
#              escaped using '\'...
#
#                '('...')' - A "grouping", useful only with '|' inside
#                '['...']' - An "optional" sub-expression (0 or 1 occurence)
#                '|'       - Indicating "or", or an "alternative"
#
#              Examples include:
#
#                "[e]x[it]|q[uit]" produces the following possible matches...
#               
#                   "exit", "xit", "ex", "x", "quit", and "q"
#
# Notes: This class is used haavily by the BEFE Name class.
#
#        This differs slightly from the VarTerm class mainly because it was
#        reworked AFTER VarTerm in order to allow '|' syntax. Both are used
#        when appropriate.
#
#        We've added some, what appear to be, "private" members and functions
#        to the MinExp class to allow it to be used to resolve and compute
#        itself. These members/functions are all preceeded with '_' so DO NOT
#        use or change them, okay?
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_functions import FindChars
from BEFE_Stack     import Stack
from BEFE_Struct    import Struct
from BEFE_errors    import *

specialChars = '()[]|'

#===============================================================================
#
# Class: MinExp - Minimalise Expression
#

class MinExp:

  #
  # MinExp Class Methods
  #
  
  #
  # MinExp Instance Methods (internal use only)
  #  

  def __init__(self,inStr):
  
    # Sanity checks...
    if type(inStr) is not str: raise ValueError("MinExp() expected a single string argument")
    
    # Do it...
    self.inStr    = _MinStack.Normalise(inStr)
    self.fullIdx  = None
    self.shortIdx = None
    self._parse()
    
    return
    
  # Notes: Since self.inStr has been normalised we can be certain(ish) that
  #        parens are balanced so we'll not do any special checks below.
  #
  
  def _parse(self):
  
    self.strings = []
    stack = _MinStack()
    self.strings = stack.ProcessChars(self.inStr)
    return
    
  def Longest(self):
  
    stringsLen = len(self.strings)
    if stringsLen == 0:
      return None
      
    if self.fullIdx is None:
      self.fullIdx = 0
      for idx in range(1,stringsLen):      
        this = self.strings[idx]
        that = self.strings[self.fullIdx]
        if len(this) > len(that):
          self.fullIdx = idx
          continue
        if len(this) < len(that):
          continue
        if this < that:
          self.fullIdx = idx
          continue
          
    return self.strings[self.fullIdx]
    
  def Shortest(self):
  
    stringsLen = len(self.strings)
    if stringsLen == 0:
      return None
      
    if self.shortIdx is None:
      self.shortIdx = 0
      for idx in range(1,stringsLen):      
        this = self.strings[idx]
        that = self.strings[self.shortIdx]
        if this == "":
          continue
        if len(this) < len(that):
          self.shortIdx = idx
          continue
        if len(this) > len(that):
          continue
        if this < that:
          self.shortIdx = idx
          continue
          
    return self.strings[self.shortIdx]

  def __iter__(self):
  
    return iter(self.strings)
    
  def __contains__(self, key):
  
    return key in self.strings
    
  def __len__(self):
  
    return len(self.strings)

  def __getitem__(self,index):

    if type(index) is not int:
      raise TypeError("Expected int index")
    
    if not len(self.strings) \
       or (index < 0 and -index > len(self.strings)) \
       or (index >= 0 and index >= len(self.strings)):
      return None

    return self.strings[index]      
    
  @staticmethod    
  def Normalise(str):
  
    return _MinStack.Normalise(str)
    
#-------------------------------------------------------------------------------
#
# Internal use only...
#
#-------------------------------------------------------------------------------

class _MinStack(Stack):

  #
  # Class methods...
  #
  
  @staticmethod
  def appendStringToList(lst,string):
    i = 0
    while i < len(lst):
      lst[i] = lst[i] + string    
      i += 1
    if not lst:
      lst.append(string)
    
  @staticmethod
  def JoinLists(lst1,lst2):

    # Handle cases where one is empty...
    result = []
    if not lst1 or not lst2:
      if not lst1:
        result = list(lst2)
      else:
        result = list(lst1)
      return result
      
    # Both have contents, join them...
    else:
      for x in lst1:
        for y in lst2:
          if x+y not in result:
            result.append(x+y)
            
    return result

  @staticmethod
  def _sortCompare(val1,val2):
    if len(val1) == len(val2):
      return -1 if val1 < val2 else 0 if val1 == val2 else 1
    return -1 if len(val1) > len(val2) else 1

  @staticmethod
  def Normalise(inStr):

    # Note: We lowercase here since Python doesn't handle case insensitive strings like BEFE does...
    inStr = inStr.lower()
    
    stack = Stack()
    pos = -1
    
    while True:
    
      # Find next special character...
      pos += 1
      pos = FindChars(inStr,specialChars,pos)
      if pos < 0:
        break
      
      # Ignore if preceeded by '\'...
      if pos and inStr[pos-1] == '\\':
        continue
        
      # Handle cases...
      c = inStr[pos]
      if c in '([':
        stack.push(pos)
        continue
      if c == '|': # |...
        continue
      else:        # []()...
        if c == ')': shouldBe = '('
        else:        shouldBe = '['
        if stack and inStr[stack.TOS()] != shouldBe:
          tPos = stack.TOS()
          inStr = inStr[0:tPos+1] + shouldBe + inStr[tPos+1:]
          pos += 1
        elif stack:
          tPos = stack.pop()
        else:
          inStr = shouldBe+inStr
          pos += 1 
     
    # Put missing cases on end
    endCases = {"(":")","[":"]"}
    while stack:
      tPos = stack.pop()
      inStr = inStr + endCases[inStr[tPos]]
      
    return inStr
  
  #
  # Instance Methods...
  #
  
  def __init__(self):

    self[:] = []
    self.prevChar = ''

  def ProcessChars(self, str):

    self.__init__()

    self.inStr = _MinStack.Normalise(str)
    firstEntry = _MinStackEntry()
    firstEntry.isGrp = True
    firstEntry.isOpen = True
    self.push(firstEntry)
    
    pos = -1
    c = ''
    
    while pos < len(self.inStr)-1:
    
      self.prevChar = c
      pos += 1
      c = self.inStr[pos]

      # If previus char wasn't '\' then handle special ones...
      if self.prevChar != '\\':
      
        # Handle '[' and '('...
        if c in '[(':

          isOptional = (c == '[')
          self.ProcessBegin(isOptional)
          continue

        # Handle ']' and ')'...      
        elif c in '])':

          self.ProcessEnd()      
          continue
            
        # Handle '|'...
        elif c == '|':

          self.ProcessOr()
          continue

        # Handle '\'...
        elif c == '\\':
        
          continue
        
      # Handle normal characters...
      self.ProcessChar(c)
        
    # Finish processing...
    if not self[-1]:
      del self[-1]
    self.strings = self.ProcessEOS()
    self.strings.sort()

    return self.strings
  
  def ProcessChar(self,char):
    
    # Sanity checks...
    if len(self) < 1:
      self.push(_MinStackEntry())
    
    # Do it...
    tos = self.TOS()
    if tos.value == None or not tos.value:
      tos.value = char
    else:
      tos.value += char
    self.prevChar = char
        
  def ProcessBegin(self,isOptional=False):

    self.FlushValue()
    entry = _MinStackEntry()
    entry.isOpt  = isOptional
    entry.isGrp  = True
    entry.isOpen = True
    entry.list   = []
    self.push(entry)
    
  def ProcessEnd(self):

    # Sanity checks...
    if len(self) < 1: raise InternalError("Expected stack to have at least 1 entry")
    
    # Flush any open value
    self.FlushValue()
    tos = self[-1]
    if not tos:
      del self[-1]
            
    # Find last open one...
    open = len(self)
    while open > 0:
      open -= 1
      if self[open].isOpen: break
      self.Combine()
    if open < 0: raise InternalError("Expected stack to have at least 1 \"Open Group\"")
        
    # Close and combine...
    that = self[open]
    that.isOpen = False
    if that.isOpt and not '' in that.list:
      that.list.append('')
    self.Combine()
     
    # If previous character was '|' (and not '\|'), put '' in list if not already...
    tos = self[-1]
    if self.prevChar == '|' and '' not in tos.list:
      tos.list.append('')
      
    # Push empty entry for follow-on stuff...
    self.push(_MinStackEntry())
    
  def ProcessEOS(self):
  
    # Before we start, handle dangling '|'...
    tos = self.TOS()
    if tos.value == None and self.prevChar == '|' and '' not in tos.list:
      tos.list.append('')
      
    # Close/End entire MinExp...
    self.ProcessEnd()
    
    # Remove NULL entry ProcessEnd puts there...
    if not self[-1]:
      del self[-1]
      
    # Sanity checks...
    if len(self) != 1:     raise InternalError("Expected stack to have at 1 entry, has %d"%len(self))
    if not self[-1].isGrp: raise InternalError("Expected TOS.isGrp == True")

    result = self[0].list
    self[:] = []
    
    return result
    
  def FlushValue(self):
  
    # Nothing to do if empty
    if len(self) < 1:
      return
    
    # Do it...
    tos = self.TOS()
    if tos.value == None:
      return
    _MinStack.appendStringToList(tos.list,tos.value)
        
    tos.value = None
    
  def ProcessOr(self):
  
    # Sanity checks
    if not self: raise InternalError("Stack Empty")
    
    # Find target Open Group...
    idx = len(self)
    while idx > 0:
      idx -= 1
      if self[idx].isGrp and self[idx].isOpen:
        break
    
    # If no value, we'll add '' because it's a leading/trailing '|'...
    tos = self.TOS()
    if not tos.value:
      tos.value = ''
    self.FlushValue()
      
    # Combine everything until that's at TOS then add empty entry...
    while len(self) > 1 and len(self)-1 > idx:
      self.Combine()
    self.push(_MinStackEntry())
      
    # Do It...
    self[idx].isOr = True
    self.prevChar = '|'
        
  def Combine(self):

    # If nothing to combine, nothing to do...
    if len(self) < 2:
      return
      
    # Do it...
    tos1 = self[-1]
    tos2 = self[-2]
    #if tos1.isOpen or tos2.isOpen:
    #  return

    if tos2.isOr:
    
      lst1 = tos1.list
      lst2 = tos2.list
      result = list(lst2)
      for x in lst1:
        result.append(x)
      tos2.list  = result

    elif len(tos1.list) == 0:
      
      pass

    elif len(tos2.list) == 0:

      tos2.list = list(tos1.list)

    elif not tos2.isOr:

      tos2.list = _MinStack.JoinLists(tos2.list,tos1.list)

    else:
    
      tos2.list.extend(tos1.list)

    del self[-1]
    
  def PrintContents(self,indent=''):
  
    for i in range(0,len(self)):
      print(indent+"[%d]: "%i+str(self[i]))
    if len(self) == 0:
      print(indent+"***EMPTY***")
    
class _MinStackEntry(Struct):

  #          ┌─────────────────────────────── Possibilities (list)
  #          │    ┌────────────────────────── Current value being built (char by char)
  #          │    │     ┌──────────────────── True  = "Is OR"
  #          │    │     │                     False = "Is not OR"
  #          │    │     │    ┌─────────────── True  = "Is Optional (0..1)"
  #          │    │     │    │                False = "Not Optional (1..1)"
  #          │    │     │    │     ┌───────── True  = "Is a Grouping"
  #          │    │     │    │     │          False = "Is not a Grouping"
  #          │    │     │    │     │     ┌─── True  = "Is Open"
  #          │    │     │    │     │     │    False = "Is Closed"
  #          │    │     │    │     │     │
  #          ▼    ▼     ▼    ▼     ▼     ▼
  members = "list,value,isOr,isOpt,isGrp,isOpen"
    
  def __init__(self):
    super(_MinStackEntry,self).__init__(_MinStackEntry.members)
    self.list  = []
    self.value = None
    self.isOr  = False
    self.isOpt = False
    self.isGrp = False
    self.isOpen  = False
    
  def __nonzero__(self):
  
    return True if self.list or self.value or self.isOpen else False
    
  def __bool__(self): # For python 3...
  
    return self.__nonzero__()
  
  def __str__(self):

    list  = self.list
    value = self.value
    isOr  = self.isOr
    isOpt = self.isOpt
    isGrp = self.isGrp
    isOpen  = self.isOpen
    s =  "context: "
    s += ", list = %s"%repr(list)
    s += ", value = %s"%repr(value)
    s += ", isOr = %s"%repr(isOr)
    s += ", isOpt = %s"%repr(isOpt)
    s += ", isGrp = %s"%repr(isGrp) 
    s += ", isOpen = %s"%repr(isOpen)
    
    return s

#===============================================================================
#
# Class: MinExpMap - Minimalise Expression Map
#
# Usage: dict = MinExpMap(stringList,longest=True,shortest=False)
#
# Where: stringList - List: List of target MinExp specs (string)
#        longest    - Bool: "Use longest possibility"
#        shortest   - Bool: "Use shortest possibility"
#        
# Notes: This function can be used to product a dictionary of MinExp
#        possibilities mapped to longest or shortest possibility.
#
#        If longest == shortest, we'll use the original string spec as the
#        mapping target.
#

def MinExpMap(stringList,longest=True,shortest=False):

  # Sanity checks...
  if type(stringList) != list:
    raise ValueError("Expected list of strings")
    
  theMap = {}
  for idx in range(0,len(stringList)):
    string = stringList[idx]
    if type(string) != str:
      raise ValueError("Expected list of strings (stringList[%d] isn't a string)"%idx)
    minexp = MinExp(string)
    if shortest == longest:
      target = string
    elif longest:
      target = minexp.Longest()
    elif shortest:
      target = minexp.Shortest()
    for possibility in minexp.theStrings:
      theMap[possibility] = target
    
  return theMap
    

#===============================================================================
#
# __main__
#

if __name__ == '__main__':

  cases = [
           "abc)def",
           "(def",
           "abc)",
           "(abc|def)ghi(jkl|mno)",
           "[(abc|def)ghi(jkl|mno)]",
           ")\)\[)([(",
           "e]x[it",
           "[e]x[it]",
           "[[e]x[it]]",
           "q[uit]",
           "[e]x[it]|q[uit]",
           "[a]b[c]|"
           "[a]b[c]|d[e]"
           "[e]x[it]|q[uit]|",
           "a|(bc)",
           "a|b|c",
           "a|b|c|",
           "[e]x",
           "a|b|c",
           "|a|b|c",
           "a|b|c|",
           "|a|b|c",
           "(a|b|c|)",
           "[a|b|c]",
           "norm[al(ise|ize)]",
           "normal(ise|ize)",
           "empty[[]]braces",
           "\\xhi there\\",
           "e\[xit\]|q\[uit\]",
           "[def[ghi]]xxx|jkl",
           "[def[ghi]]xxx|",
           "[def[ghi]]xxx",
           "abc[def|ghi]xxx[jkl|mno]",
           "abcxxx[jkl|mno]",
           "abc[def|ghi]xxx[jkl|mno]|def",
           "abc(def|ghi)|jkl(mno|pqr)",
           "abc(def|ghi)",
           "abc[def|ghi]",
           "h[elp]",
           "G[lobal[y]]U[nique]ID[[ent[ifier]]]",
           "U[niversal]T[ext]F[ormat][[-]8]",
           "Enc[rypt][F[ile]]",
           "a[[[b][c]][d][[e][f]]]",
           "A[b[c[d]]]E[f][g]",
           "[[a][b][[c][d]]]",
           "a[b[c[d[e[f[g][h]]]]]]",
           "a[[[[[[b]c]d]e]f]g]",
           "[[abc]d[efg]]TOP[[h[i]]]",
           "[[[[[[[a]b]c]d]e]f]g]h",
           "cross[-]ref([erenc][ed])",
          ]
  
  from sys    import argv
  #from string import lower
  
  global debug
  
  if len(argv) > 1:
    yn = {'y':True,  'yes':True, 't':True,  'true':True,
          'n':False, 'no':False, 'f':False, 'false':False
         }
    if len(argv) > 2: raise Warning("Expected single Y/N argument")
    s = argv[1].lower()
    if s not in yn:  raise Warning("Unknown 'Debug (Y/N)?' argument '%s'"%argv[1])
    debug = yn[s]
  else:
    debug = False
  
  maxLen1 = 0
  maxLen2 = 0
  maxLen3 = 0
  maxLen4 = 0

  for case in cases:
    maxLen1 = max(maxLen1,len(repr(case)))
    norm = _MinStack.Normalise(case)
    maxLen2 = max(maxLen2,len(repr(norm)))
    minExp = MinExp(case)
    maxLen3 = max(maxLen3,len(repr(minExp.Shortest())))
    maxLen4 = max(maxLen4,len(repr(minExp.Longest())))
    
  for case in cases:

    minExp = MinExp(case)
    lv = minExp.Longest()
    sv = minExp.Shortest()
    print("normalise: "+repr(case) + ' '*(maxLen1-len(repr(case)))+         \
          " = "+repr(minExp.inStr) + ' '*(maxLen2-len(repr(minExp.inStr)))+ \
          ", Shortest = "+repr(sv) + ' '*(maxLen3-len(repr(sv))) +          \
          ", Longest = "+repr(lv)  + ' '*(maxLen4-len(repr(lv))) +          \
          " = "+repr(minExp.strings)[1:-1]
         )
         