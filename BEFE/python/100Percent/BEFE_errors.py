#!/usr/bin/python
#coding=utf8
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

class Failed(Exception): pass

class Error(Exception):

  def __init__(line, col, err):
    self.filename = None
    self.line     = line
    self.col      = col
    self.error    = err
    
  def __init__(self, word, err):
    self.filename = word.filename
    self.line     = word.line
    self.col      = word.col
    self.error    = err
    
class SyntaxError(Exception):

  def __init__(self, line, col, err):
    self.filename = None
    self.line     = line
    self.col      = col
    self.error    = err

  def __init__(self, word, err):
    self.filename = word.filename
    self.line     = word.line
    self.col      = word.col
    self.error    = err
    
  def __str__(self):
    if self.line is None:
      s1 = "None"
    else:
      s1 = repr(self.line+1)
    if self.col is None:
      s2 = "None"
    else:
      s2 = repr(self.col+1)
    s = "***SyntaxError: (%s/%s): "%(s1,s2)+self.error
    return s
    
  def __repr__(self):
    return repr((self.line,self.col,self.error))

class InternalError(Exception):
  def __init__(self,*args):
    super(InternalError,self).__init__(*args)

  def __str__(self):
    s = "***Internal Error: "+self.message
    return s
    
    
class RuntimeError(Exception):
  def __init__(self,*args):
    super(RuntimeError,self).__init__(*args)

class Warning(Exception):

  def __init__(self, line, col, err):
    self.line  = line
    self.col   = col
    self.error = err

  def __init__(self, word, err):
    self.line  = word.line
    self.col   = word.col
    self.error = err
    
  def __init__(self, err):
  
    self.line  = None
    self.col   = None
    self.error = err
    
  def __str__(self):
  
    s = "***Warning "
    if self.line is not None and self.col is not None:
      s += '(%d/%d)'%(self.line,self.col)
    s += ': '+err
    return s