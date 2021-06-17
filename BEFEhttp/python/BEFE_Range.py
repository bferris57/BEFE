#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:     BEFE_Range - Range class and related utility functions
#
# Contains: class    Range
#           function IntToHex
#           function QuoteStrToInt
#           function IntStrToInt
#           function HexStrToInt
#

# External dependencies...
from BEFE_Struct    import *
from BEFE_functions import *

#===============================================================================
#
# Class:    Range
#
# Purpose:  Represent an inclusive range of integers
#
# Usage:    range = Range()
#           range = Range(min,max[,step])
#
# Where:    min  - int/atr: Range minimum
#           max  - int/str: Range maximum
#           step - int/str: Range step
#
# Notes:    If min, max, or step are passed as strings they must be as follows:
#
#             min  - Integer, single byte hex (e.g. "0x35"), or single quoted char (e.g. "'a'")
#             max  - Integer, hex, or quoted char
#             step - Integer
#
#           If min > max they are swapped and step is forced negative, otherwise
#           step is forced positive.
#

class Range(Struct):

  def __init__(self,*args):
  
    super(Range,self).__init__('min,max,step')
    
    self.min  = None
    self.max  = None
    self.step = None
  
    if len(args) >= 1:
      self.min = args[0]
    if len(args) >= 2:
      self.max = args[1]
    if len(args) == 3:
      self.step = args[2]
    if len(args) > 3:
      raise RuntimeError("Expected maximum of 3 arguments (min, max, step), got "+str(len(args)))
      
    # If only one string passed, split '..' and optional ':'...
    if type(self.min) == str and '..' in self.min:
      parts = self.min.split('..')
      self.min = parts[0]
      self.max = parts[1]
      if ':' in self.max:
        parts = self.max.split(':')
        self.max  = parts[0]
        self.step = parts[1]
    
    # Convert strings to integers...
    if type(self.min) == str:
      #if len(self.min) == 1:
      #  self.min = '"' + self.min + '"'
      min = IntStrToInt(self.min)
      if min is None: min = QuoteStrToInt(self.min)
      if min is None: min = HexStrToInt(self.min)
      self.min = min
      
    if type(self.max) == str:
      #if len(self.max) == 1:
      #  self.max = '"' + self.max + '"'
      max = IntStrToInt(self.max)
      if max is None: max = QuoteStrToInt(self.max)
      if max is None: max = HexStrToInt(self.max)
      self.max = max
      
    if type(self.step) == str:
      self.step = IntStrToInt(self.step)
    
    if self.step is None:
      self.step = 1
    if self.min is None:
      self.min = 0
    if self.max is None:
      self.max = 0
    if self.min != None and self.max != None and self.min > self.max:
      if self.step != None and self.step > 0:
        self.step = -self.step
    
  def __str__(self):
  
    smin  = ''
    smax  = ''
    if self.min != None:
      smin = str(self.min)
    if self.max != None:
      smax = str(self.max)
    s = smin + ".." + smax
    
    if self.step != None and self.step != 1:
      s += ':' + str(self.step)
      
    return s
    
  def __repr__(self):
  
    return "Range("+str(self)+")"
    
#===============================================================================
#
# __main__
#
  
if __name__ == '__main__':

  if True:
    cases = [(1,10),
             (10,1,2),
             ('"a"','"c"'),
             ('a','c'),
             ("10","1"),
             ("0x0a","0x01"),
             ("'\\x0a'","'\\x1f'"),
             ("0..10",),
             ('0..10:2',),
             ('10..0:2',),
             ('0..',),
             ('..:3',),
            ]
            
    maxLen = 0
    for case in cases:
      maxLen = max(maxLen,len(repr(case))+1)
    for case in cases:
      a = Range(*case)
      c = repr(case) + ':'
      c += ' '*(maxLen-len(c))
      print("Case: %s Result = %s"%(c,str(a)))
      
  if False:
    cases = [0,1,10,255,65535,-1,-20,-65537]
    for case in cases:
      s = IntToHex(case)
      print("%s: %s"%(case,repr(s)))
