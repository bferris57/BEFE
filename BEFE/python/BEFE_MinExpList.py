#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BEFE_MinExpList - BEFE MinExp list
#
# Classes: MinExpList - List of MinExps, append a MinExp, act as it contains
#                       all possible combinations
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_MinExp import *
from BEFE_errors import *

class MinExpList(list):

  def __init__(self):
  
    self.minExpSeeds = []
    self.minExpValues = []
  
  def append(self,what):
  
    if isinstance(what,str):
    
      what = MinExp.Normalise(what)
      if what in self.minExpSeeds:
        return
        
      minexp = MinExp(what)
      
      if minexp.inStr in self.minExpSeeds:
        return
      
      self.minExpSeeds.append(minexp.inStr)      
      for poss in minexp:
        if poss not in self.minExpValues:
          self.minExpValues.append(poss)

    elif isinstance(what,MinExp):
    
      if what.inStr in self.minExpSeeds:
        return
    
      self.minExpSeeds.append(what.inStr)
      for poss in what:
        if poss not in self.minExpValues:
          self.minExpValues.append(poss)
          
    else:
    
      raise InternalError("Expected str or Minexp, got %s instead"%repr(type(what)))
  
  def __iter__(self):
  
    return self.minExpValues.__iter__()
    
  def __len__(self):

    return len(self.minExpValues)
    
  def __getitem__(self,idx):
  
    if type(idx) not in [int,long]:
      return self.minExpValues.__getitem__(idx)
      
    return self.minExpValues[idx]
    
  def __str__(self):
  
    return str(self.minExpValues)

  def __repr__(self):
  
    return repr(self.minExpValues)
    
  def __contains__(self, key):
    
    return key in self.minExpValues

if __name__ == "__main__":

  cases = {"e[xit]|q[uit]|x","hi[there]"}
  check = ["exit","x","xxx"]
  
  lst = MinExpList()
  for case in cases:
    lst.append(case)
  print("List contents: %s"%repr(lst))
  for case in check:
    print("%s %s in list"%(repr(case),"is" if case in lst else "not"))
