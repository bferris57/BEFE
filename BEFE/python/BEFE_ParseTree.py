#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:       BEFE_ParseTree - BEFE ParseTree and related classes/code
#
# Classes:    ParseTree  - Parse Tree (sub-class of Tree)
#             ParseNode  - BEFE Parsergeneral named Tree Node. Parse Tree is
#                            build of this class or sub-classes of it
#             ParseFile  - BEFE Parser File Tree Node (has filename, data,
#                            errors, warnings, etc. in it)
#

import copy
from   BEFE_Tree    import *
from   BEFE_Struct  import *
from   BEFE_Word    import *
from   BEFE_VarTerm import *
# DEBUG...
from BEFE_debug     import *
# ...DEBUG
import os
import hashlib

#
# BEFE_ParseTree Globals...
#

#-------------------------------------------------------------------------------
#
# Class: ParseTree - BEFE Parse Tree, contains hierarchical set of ParseNodes
#

class ParseTree(Tree):

  members = Tree.members
  members.append("dateTime")
  
  def __init__(self):
  
    Struct.__init__(self,';'.join(ParseTree.members))

    self.nodes    = []
    self.heads    = []
    self.dateTime = None

  #=============================================================================
  #
  # Method: newNode     - Create a new, unparented, ParseNode in the tree
  #
  
  def newNode(self,that=None):
  
    named = ParseNode(name=None,type=None)
    self.insert(named)
    
    # Copy all members if that specified...
    if that is not None:
      named.name         = that.name
      named.type         = that.type
      named.resolvedType = that.resolvedType
      named.value        = that.value
      named.word0        = that.word0
      named.wordN        = that.wordN
      named.words        = that.words
      named.breaker      = that.breaker
      named.moreWords    = that.moreWords
      
    return named

  #=============================================================================
  #
  # Method: newFile - Create a new, unparented, ParseFile node in the tree?
  #
  
  def newFile(self,filename):
  
    file = ParseFile(filename)
    file.setType('file')
    self.insert(file)
    return file
    
  #=============================================================================
  #
  # Method: printNodes - Print all nodes in the Parse Tree
  #
  
  def printNodes(self,indent=None,maxDepth=10):

    if type(indent) == int:
      ind = ' '*indent
    elif type(indent) != str:
      ind = ''
    else:
      ind = indent
    
    numNodes = 0
    depth = -1
    firstOverMax = True
    for node in TopDownLeftRight(self):
      numNodes += 1
      prevDepth = depth
      depth = node.depth()
      thisInd = ind+'  '*(node.depth()+1)
      if maxDepth is not None and depth <= maxDepth:      
        print(thisInd+"[%d]: %s"%(self.find(node),str(node)))
      else:
        if maxDepth is not None and firstOverMax:
          firstOverMax = False
          print(thisInd+'  ...')
        firstOverMax = False
        
    if numNodes == 0:
      print(ind+"***Empty***")

#-------------------------------------------------------------------------------
#
# Class: ParseNode - Type of TreeNode indicating something named and/or typed
#

class ParseNode(TreeNode):

  # For setting ParseNode.flags member...
  Flag = Enum("expPass1=1,expPass2,expPass3,expPass4,expPass5,expPass6")

  members = copy.copy(TreeNode.members)
  members.append('flags')
  members.append('name')
  members.append('type')
  members.append('resolvedType')
  members.append('value')
  members.append('indent')      
  members.append('word0')
  members.append('wordN')
  members.append('words')
  members.append('breaker')
  members.append('moreWords')

  def initialise(self):
  
    Struct.__init__(self,';'.join(ParseNode.members))
    TreeNode.initialise(self)
    self.flags        = []
    self.name         = None
    self.type         = None
    self.resolvedType = None
    self.value        = None
    self.indent       = None
    self.word0        = None
    self.wordN        = None
    self.words        = None
    self.breaker      = None
    self.moreWords    = None
    
  def __init__(self,words=None):

    # Do it
    ParseNode.initialise(self)
    
  def __init__(self,name,type):
    ParseNode.initialise(self)
    self.name = name
    self.type = type

  def __nonzero__(self):
  
    return True if self.flags or self.name  or self.type    or self.value or self.word0     \
                              or self.wordN or self.words or self.breaker or self.moreWords \
                else False
                      
  def __str__(self):

    s = ''
    
    s += "type="+repr(self.type)
    resType = self.resolvedType
    if resType and (not self.type or resType != self.type):
      s += " resolved=%s"%repr(resType)
    s = s + " name="+repr(self.name)
    if self.word0 is not None:
      lc = self.word0
      s += ' '+repr(lc.line+1) + '/' + repr(lc.col+1)
    if self.value is not None:
      s += " value="+repr(self.value)
    if debug and self.indent is not None:
      s += ' indent=' + repr(self.indent)
    if self.word0 is not None and not self.words:
      s += ' word0=' + repr(self.word0.value)
    if self.wordN is not None:
      s += ' wordN = ' + repr(self.wordN.value)
    if self.breaker is not None:
      s += ' breaker='+repr(self.breaker.value)
    if debug and self.flags:
      s += ' flags='+repr(self.flags)
    if self.words is not None and len(self.words) > 0:
      ss = ''
      for word in self.words:
        if ss: ss += ' '
        ss += word.value
      if s: s += ' '
      s += ' words=' + repr(ss)
    if self.moreWords:
      ss = ''
      for word in self.moreWords:
        if ss: ss += ' '
        ss += word.value
      if s: s += ' '
      s += ' moreWords=' + repr(ss)
      
    return s
  
  #
  # self.flags - Used to control parsing: Tells us "we've already done that here"
  #
  
  def setFlag(self,flag):
  
    if self.flags is None:
      self.flags = [flag]
    else:
      if flag not in self.flags:
        self.flags.append(flag)
        
  def setFlagDeep(self,flag):
  
    for node in TopDownLeftRight(self):
      node.setFlag(flag)
        
  def clearFlag(node,flag):
  
    if node.flags and flag in node.flags:
      where = node.flags.index(flag)
      del node.flags[where]
      if not node.flags:
        node.flags = None
      
  def clearFlags(self):
  
    if self.flags is not None:
      self.flags = None
      
  def clearFlagsDeep(self):
  
    for node in TopDownLeftRight(self):
      node.clearFlags()
     
  def testFlag(self,flag):
  
    if not self.flags:
      return False
    return flag in self.flags

  #
  # self.type - Tells us multiple things about the ParseNode's type
  #

  def testType(self,theType):
  
    return False if self.type is None else (theType in self.type.split(':'))
    
  def setType(self,theType):
  
    if self.type is None:
      self.type = ''
    parts = self.type.split(':')
    try:
      pos = parts.index('')
    except:
      pos = -1
    if pos >= 0:
      del parts[pos]
    if theType not in parts:
      parts.insert(0,theType)
      self.type = ':'.join(parts)
      
  def unsetType(self,theType):
  
    parts = self.type.split(':')
    if theType in parts:
      where = parts.index(theType)
      del parts[where]
      self.type = ':'.join(parts)
      
  def oneOfTypes(self,types):
  
    parts = self.type.split(':')
    for theType in types:
      if theType in parts:
        return True
    return False
 
  #
  # self.qual - Qualifier testing (applies to our children, if any)
  #
  
  def isQualified(self):
  
    answer = False
    if not self.children: return answer
    for child in self.children:
      if child.testType('qual'):
        answer = True; break
    return answer
    
  def isQualified(self,name):
  
    answer = False
    if not self.children: return answer
    for child in self.children:
      if child.testType('qual') and child.name == name:
        answer = True; break
    return answer

#-------------------------------------------------------------------------------
#
# Class: ParseNode - Type of TreeNode indicating something named and/or typed
#

class ParseFile(ParseNode):
  
  # For setting ParseNode.flags member...
  members = copy.copy(ParseNode.members)
  members.append('filename')
  members.append('data')
  members.append('md5')

  def initialise(self):
  
    Struct.__init__(self,';'.join(ParseFile.members))
    TreeNode.initialise(self)
    self.filename = None
    self.data     = None
    self.md5      = None
    self.words    = None

  def __init__(self,filename):

    self.initialise()
    self.filename = filename
    if os.path.isfile(filename):
      self.data = bytearray(open(filename,'rb').read())
      dig = hashlib.md5()
      dig.update(self.data)
      self.md5 = bytearray(dig.digest())
      
#-------------------------------------------------------------------------------
#
# Main
#

if __name__ == "__main__":

  tree = ParseTree()
  file = ParseNode(name="BEFE_ParseTree.py",type='file')
  tree.insert(file)
  ns = ParseNode(name='BEFE',type='namespace')
  tree.insert(ns)
  ns.setParent(file)
  
  tree.printNodes()
