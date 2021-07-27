#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BEFE_MinExpDict - BEFE MinExp dictionary
#
# Classes: MinExpDict - Dictionary of MinExps, set any one by MinExp seed and
#                       get by any MinExp possibilities
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_MinExp import *
from BEFE_errors import *

class MinExpDict(dict):

  def __init__(self,that=None):
  
    self.map       = {}
    self.poss2seed = {}
    self.seed2poss = {}
    self.longmap   = {}
    self.shortmap  = {}
    
    if that is not None:
      if type(that) is not dict:
        raise InternalError("Expected 'from' initialiser to be a dict, got %s instead"%repr(type(that)))
      for key in that:
        self[key] = that[key]
        
  def __setitem__(self,key,value):

    if type(key) is not str:
      raise KeyError("Expected str key")
      
    if key in self.map:
      return self.map[key]

    # Create a new MinExp to get the possibilities and shortest/longest...      
    me = MinExp(key)
    
    # Make sure none of the possibilities are already decleared...
    for tkey in me:
      if tkey in self.poss2seed:
        raise InternalError("Key %s (%s) conflicts with previously declared %s"%(repr(me.inStr),repr(tkey),repr(self.poss2seed[tkey])))
        
    # Populate everything...
    #seed = me.Longest()
    seed = me.inStr
    self.map[seed] = value
    for poss in me:
      self.poss2seed[poss] = seed
      if not seed in self.seed2poss:
        self.seed2poss[seed] = []
      self.seed2poss[seed].append(poss)
    self.longmap[key] = me.Longest()
    self.shortmap[key] = me.Shortest()

  def __getitem__(self,key):

    if type(key) is not str:
      raise KeyError("Expected str key")
 
    # Note: We lowercase here since Python doesn't handle case insensitive strings like BEFE does...
    key = key.lower()
    
    if key in self.map:
      return self.map[key]

    if key in self.poss2seed:
      return self.map[self.poss2seed[key]]
      
    raise KeyError("Key %s not in dict"%repr(key))
      
  def __delitem__(self,key):
  
    if type(key) is not str:
      raise KeyError("Expected str key")
    
    # Note: We lowercase here since Python doesn't handle case insensitive strings like BEFE does...
    key = key.lower()
    
    if key in self.poss2seed:
      seed = self.poss2seed[key]
    elif key in self.map:
      seed = key
    else:
      raise KeyError("Key %s not in dict"%repr(key))
      
    del self.map[seed]
    for poss in self.seed2poss[seed]:
      del self.poss2seed[poss]
    del self.seed2poss[seed]
    del self.shortmap[seed]
    del self.longmap[seed]
    
  def shortest(self,key):
  
    key = key.lower()
    
    if key not in self.map:
      return None
    return self.shortmap[key]
    
  def longest(self,key):
  
    key = key.lower()
    
    if key not in self.map:
      return None
    return self.longmap[key]
  
  def __len__(self):
  
    return len(self.map)

  def __nonzero__(self):
  
    return True if self.map else False

  def __str__(self):

    return str(self.map)
    
  def __repr__(self):

    return repr(self.map)
    
  def __iter__(self):
  
    return iter(self.map)
    
  def __contains__(self,key):

    # Note: We lowercase here since Python doesn't handle case insensitive strings like BEFE does...
    key = key.lower()
    
    return key in self.map or key in self.poss2seed
    
  def keys(self):
  
    return self.map.keys()
    
  def resolve(self,key):
    if key in self.poss2seed:
      return self.poss2seed[key]
    if key in self.map:
      return key
    return None
    
if __name__ == "__main__":

  map = MinExpDict()
  map["E[xit]|Q[uit]|X"] = "Hi there!"
  map["Min[imal[ist]]Exp[r[ession]]"] = "Minimal"
  print("map['e'] = %s"%repr(map['e']))
  print("map['minexp'] = %s"%repr(map['minexp']))
  #del map['minexp']
  print("repr(map) = %s"%repr(map))
  print("map.keys = %s"%repr(map.keys()))
  
  # Make sure we get an exception when we try to declare an overlapping one again...
  print("map['X'] = %s..."%map['X'])
  wasCaught = False
  try:
    map["X"] = "Uh oh!"  
  except:
    wasCaught = True
  if not wasCaught:
    raise InternalError("Expected 'X' to mismatch with 'E[xit]|Q[uit]|X'")
    