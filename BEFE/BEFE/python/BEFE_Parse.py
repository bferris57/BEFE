#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BEFE_Parse - BEFE Parse and related classes
#
# Classes: Parse     - BEFE Parser/compiler
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_FileWords    import *
from BEFE_WordList     import *
from BEFE_Word         import *
from BEFE_errors       import *
from BEFE_Struct       import *
from BEFE_ParseTree    import *
from BEFE_MinExp       import *
from BEFE_Span         import *
from BEFE_ParseStatics import Style

# DEBUG...
from   BEFE_debug      import *
# ...DEBUG

import os
              
#===============================================================================
#
# Class: Parse - Parse of a single file
#

class Parse:

  from BEFE_ParseStatics import stmtHandlers
  from BEFE_ParseMethods import findEndOfLogicalLine
  from BEFE_ParseMethods import findEndOfExpression
  from BEFE_ParseMethods import parseFileNode 
  from BEFE_ParseMethods import breakNodeExpressionWords  
  from BEFE_ParseMethods import indentChildren  
  from BEFE_ParseMethods import parseExpression
  from BEFE_ParseMethods import parseExpressionPass1
  from BEFE_ParseMethods import parseExpressionPass2
  from BEFE_ParseMethods import parseExpressionPass3
  from BEFE_ParseMethods import parseExpressionPass4
  from BEFE_ParseMethods import parseExpressionPass5

  # For Parse processing...  
  lParens = ['(','{','[']
  rParens = [')','}',']']
  parens  = []
  parens.extend(lParens)
  parens.extend(rParens)

  def __init__(self,inFileName = None):
  
    self.initialise()
    if not inFileName:
      return
    if type(inFileName) != str:
      raise RuntimeError("Expected string input file name")
    self.file(inFileName)

  def initialise(self):

    self.inFileName  = None
    self.data        = None
    self.lines       = None
    self.outFileName = None
    self.errors      = None
    self.warnings    = None
    self.tree        = ParseTree()
    self.styleStack  = Stack()
    self.defer5count = 0
    
    #self.style = Style.Freeform

  #===============================================================================
  #
  # Methods: Parse.postError     - Post a single error
  #          Parse.postWarning   - Post a single warning
  #          Parse.hasErrors     - Does this Parse have any errors?
  #          Parse.hasWarnings   - Does this Parse have any warnings?
  #          Parse.printErrors   - Format and print Parse errors (if any)
  #          Parse.printWarnings - Format and print Parse warnings (if any)
  #
    
  def postError(self,error):

    # Sanity checks...
    if not isinstance(error,Exception):
      raise ValueError("Expected error argument to be an Exception")

    if type(error) in [SyntaxError,RuntimeError,InternalError]:
      error.filename = self.inFileName
    else:
      error.filename = None
      
    if self.errors == None:
      self.errors = []

    self.errors.append(error)
  
  def postWarning(self,warning):
  
    # Sanity checks...
    if not isinstance(warning,Exception):
      raise ValueError("Expected warmimg argument to be an Exception")

    if type(warning) in [SyntaxError,RuntimeError,InternalError]:
      warning.filename = self.inFileName
      
    if self.warnings == None:
      self.warnings = []

    self.warnings.append(warning)
    
  def hasErrors(self):
    return True if self.errors else False
    
  def hasWarnings(self):
    return True if self.warnings else False
    
  def numErrors(self):
    return 0 if not self.errors else len(self.errors)
  
  def numWarnings(self):
    return 0 if not self.warnings else len(self.warnings)
  
  def printErrors(self,indent=''):

    if type(indent) == int:
      indent = ' '*indent
    elif type(indent) != str:
      indent = ''
      
    errors = self.errors
    if not errors:
      if self.inFileName:
        print(indent+self.inFileName+': ***No Errors***')
      else:
        print(indent+"***No Errors***")
      return

    # Get the file contents...
    if self.lines is None:
      self.inFileName = self.errors[0].filename
      self.lines = open(self.inFileName).read().split('\n')
      # DEBUG...
      if debug and verbose0:
        print("printErrors: self.inFileName = %s, len(self.lines)= %d"%(repr(self.inFileName),len(self.lines)))
      # ...DEBUG
      for i in range(0,len(self.lines)):
        if self.lines[i] and self.lines[i][-1] == '\n':
          self.lines[i] = self.lines[i][:-1]
    
    # Process the errors...
    for error in errors:
      pType = {SyntaxError:"Syntax",
               InternalError:"Internal",
               RuntimeError:"Runtime",
              }[type(error)]
      if type(error) in [SyntaxError]:
        filename = error.filename
        line = error.line
        col  = error.col
      else:
        filename = None
        line     = None
        col      = None
        
      if line is not None and line >= 0 and line < len(self.lines):
        lineStr = self.lines[line] if self.lines[line] else ''
        prefix = "Line %d: "%(line+1)
        print("")
        print(prefix+lineStr)
        print(' '*(col+len(prefix))+'^')
      #s = pType + ": " + str(error)
      s = "Error: " + str(error)
      if filename:
        s = "File "+repr(filename)+': '+s
      print(indent+s)
      
    return

  def printWarnings(self,indent=''):

    if type(indent) == int:
      indent = ' '*indent
    elif type(indent) != str:
      indent = ''
      
    warnings = self.warnings
    if not warnings:
      if self.inFileName:
        print(indent+self.inFileName+': ***No Warnings***')
      else:
        print(indent+"***No Warnings***")
      return

    # Get the file contents...
    if self.lines is None:
      self.inFileName = self.warnings[0].filename
      for i in range(0,len(self.lines)):
        if self.lines[i][-1] == '\n':
          self.lines[i] = self.lines[i][:-1]
      # DEBUG...
      if (debug):
        print("parse.printErrors: %d lines total"%len(self.lines))
      # ...DEBUG
    
    # Process the errors...
    for warning in warnings:
      pType = {SyntaxError:"Syntax",
               InternalError:"Internal",
               RuntimeError:"Runtime",
              }[type(warning)]
      if type(warning) in [SyntaxError]:
        filename = warning.filename
        line = warning.line
        col  = warning.col
      else:
        filename = None
        line     = None
        col      = None
        
      if line is not None and line >= 0 and line < len(self.lines):
        lineStr = self.lines[line] if self.lines[line] else ''
        prefix = "Line %d: "%(line+1)
        print("")
        print(prefix+lineStr)
        print(' '*(col+len(prefix))+'^')
      s = "Warning: " + str(warning)
      if filename:
        s = filename+': '+s
      print(indent+s)
      
    return

  #===============================================================================
  #
  # Method: Parse.file  - Attach a single file to parse tree
  #

  def file(self,filename=None):

    self.errors   = []
    self.warnings = []
    
    if filename and not self.inFileName:
      self.initialise()
      self.inFileName = filename
      
    if not filename:
      return
      
    if not os.path.exists(filename):
      self.postError(RuntimeError("File \""+filename+"\" does not exist"))
      return
    if not os.path.isfile(filename):
      self.postError(RuntimeError("File \""+filename+"\" exists but is not a file"))
      return
      
    # Add file to parse tree
    file = ParseNode(name=filename,type='file')
    tree = self.tree
    tree.insert(file)
    
    return
    
  #===============================================================================
  #
  # Method: Parse.parse  - Parse all file nodes
  #

  def parse(self):
  
    heads = []
    for node in self.tree.heads:
      heads.append(node)
      
    for node in heads:

      # Ignore if not a file node
      if not node.testType('file'):
        continue
              
      self.parseFileNode(node)
      if self.errors:
        return
        
#===============================================================================
#
# Function: lineToString - Convert Line Words to displayable string
#

def lineToString(line):

  wds = ''
  wordNo = -1
  for word in line:
    wordNo += 1
    if wds: wds += ' '
    if word.type in [Word.Type.NAME,Word.Type.BQUOTE]:
      wds += '`'+word.value+'`'
    elif word.type == Word.Type.SQUOTE:
      wds += "'"+word.value+"'"
    elif word.type == Word.Type.DQUOTE:
      wds += '"'+word.value+'"'
    elif word.type == Word.Type.OPERATOR:
      wds += 'OPERATOR:`'+word.value+'`'
    else:
      wds += word.value
  
  return wds

#===============================================================================
#
# Function: removeWhite - Remove all whitespace and comments, returning indentation
#
# Usage: removeWhite(words)
#
# Where: words  - List: Words
#

def removeWhite(words):

  if type(words) is not list:
    raise ValueError("Expected list of Words")
  
  if len(words) < 1:
    return
    
  pos = 0
  while pos < len(words):
  
    word = words[pos]
    if word.type == Word.Type.WHITE or word.type == Word.Type.COMMENT:
      del words[pos]
      continue
    pos += 1
    
  return
  
#===============================================================================
#
# __main__ - Python standalone main
#

if __name__ == "__main__":
  
  args = sys.argv[1:]
  errors = []

  if not args:
    args = ['__dict.py']

  if True:
  
    for arg in args:
    
      parse = Parse()
      parse.file(arg)
      parse.parse()
      parse.printErrors(indent=0)
      parse.printWarnings(indent=0)
      #if not parse.numErrors():
      parse.tree.printNodes(maxDepth=10)
