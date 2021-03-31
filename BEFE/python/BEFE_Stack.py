#!/usr/bin/pytho///n
#coding=utf8
#===============================================================================
#
# File: BNF_Stack - Implement Stack class (list with push method)
#

class Stack(list):

  def __init__(self,*args):
    list.__init__(self)
    for arg in args:
      self.push(arg)
      
  def push(self,*args):
    for arg in args:
      self.append(arg)
  
  def pop(self):
    if len(self):
      that = self[-1]
      del self[-1]
      return that
    else:
      return None
      
  def TOS(self):
    if len(self):
      return self[-1]
    else:
      return None

  def BOS(self):
    if len(self):
      return self[0]
    else:
      return None