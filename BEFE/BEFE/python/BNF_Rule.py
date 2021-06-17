#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:     BNF_Rule - Rule class
#
# Contains: class    Rule
#

# External dependencies...
from BNF_Struct import *
from BNF_Word   import *
from BNF_Rules  import *

#===============================================================================
#
# Class:    Rule
#
# Purpose:  Represent a single named BNF rule
#
# Usage:    rule = Rule(filename=None,rules=None)
#
# Where:    filename - str:   Name of file rule is defined in
#           rules    - Rules: Parent list of rules (e.g. context)
#

class Rule(Struct):

  members = ["name","type","minoccur","maxoccur","filename","words","rules","recursion","referencedby"]

  def __init__(self,filename=None,rules=None):
  
    Struct.__init__(self,','.join(Rule.members))
    
    self.filename = filename
    self.rules    = rules
    self.words    = []
    self.type     = None
  
  # Note: We've made this function act a teeny bit different to the one
  #       Provided by Struct... For semantic reasons, it returns True
  #       only when the name member is set AND something's in the words
  #       member.
  def __nonzero__(self):

    return True if self.name and self.words else False
    
  def IsRecursive(self):
  
    return self.recursion != None and self.recursion
    
  def IsHeadRecursive(self):
  
    return self.IsRecursive() and self.recursion[0] == 0
    
  def References(self):
  
    refs = []
    for word in self.words:
      if word.type == Word.Type.NAME:
        refs.append(word.value)
    return refs
