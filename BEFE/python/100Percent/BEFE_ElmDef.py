#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:      BNF_ElmDef.py
#
# Purpose:   Process/manage data element definitions
#
# Classes:   ElmDef
#
# Functions: IsDataType    - Is str a valid DataType name?
#            IsValidName   - Is str a valid DataElm/type name?
#            WhichDataType - Determine DataType enum value
#            ElmDefWords   - Generator: Sequence of (line,col,word) DataElm
#                                       definitions
#
# Globals:   datatypes  - Index: List of DataType names
#            modifiers  - Index: List of DataELm modifiers
#            namechars  - String: Valid characters in a DataElm name/type
#            specchars  - String: List of valid specification delimeters
#            whitespace - String: List of valid whitespace characters
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

# Dependencies
import sys
from BEFE_errors    import *
from BEFE_functions import *
from BEFE_Enum      import *

#
# Globals
#

# Note: These datatypes are 'nominal' and used throughout BNF to map on to 'real' datatypes
#       used by databases

datatypes = ["bit",
             "bool"
             "byte",
             "short",
             "int",
             "long",
             "float",
             "double",
             "char",
             "string",
             "number",
             ""
            ]

numbers = ["bit","bool","byte","short","int","long","float","double","number"]
strings = ["char","string"]            

modifiers = {"signed":["signed",True],
             "unsigned":["signed",False],
             "mutable":["mutable",True],
             "immutable":["mutable",False],
             "nullable":["nullable",True],
             "notnull":["nullable",False],
             "sensitive":["sensitive",True],
             "insensitive":["sensitive",False],
             }

namechars  = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz0123456789'
specchars  = '[](),'
whitespace = ' \t\n\r'


def IsDataType(s):
  return s in datatypes

def IsValidName(s):
  for char in s:
    if char not in namechars:
      return False
  return True

def WhichDataType(s):
  
  i = 0
  for elm in datatypes:
    if elm == s:
      return i
    i += 1
  return None

def ElmDefWords(stmt):

  word = ''
  specchars = '[](),;'
  whitespace = ' \t\n\r'
  
  offset = -1
  line = 0
  col  = -1
  wordStart = 0
  skipping = False
  
  for c in stmt:
  
    offset += 1
    col    += 1

    # If skipping (eol comment)    
    if skipping and c != '\n':
      continue
    if c == '#':
      skipping = True
      continue
    if c == '/':
      if offset < len(stmt) and stmt[offset+1] == '/':
        skipping = True
        continue
      
    # Normal characters...
    if c in namechars:
      word += c
    elif c in whitespace:
      if word:
        yield (line,col-len(word),word)
        word = ''
        offset += len(word)
        col += len(word)
      if c == '\n':
        yield (line,col,'<eol>')
        line += 1
        col = -1
        skipping = False
    elif c in specchars:
      if word:
        yield (line,col-len(word),word)
        word = ''
      yield (line,col,c)
    else:
      raise SyntaxError(line,col,"ElmDefWords: Invalid character '"+c+"'")
      
  if word:
    # Note: Have to add one because col incremented at beginning of loop so
    #       increment indicating end of string is missing. ;-)
    yield (line,col-len(word)+1,word)

  yield None
  
  raise StopIteration

#===============================================================================
#
# Class:   ElmDef - A single element definition
#
# Purpose: Describe a data element in terms of name, type, etc.
#
# Members:
#

class ElmDef(object):

  def __init__(self,definition=None):
    self.definition = definition
    self.name       = None
    self.datatype   = None
    self.signed     = None
    self.mutable    = None
    self.nullable   = None
    self.sensitive  = None
    self.precision  = None
    self.dimension  = None
    self.errors     = []
    if not definition:
      return
    
    # Pull definition apart...      
    errors,defs = ElmDefs(definition)
    if errors:
      self.errors = errors
      return
      
    # Make sure only one defined
    if len(defs) == 0:
      return
      
    if len(defs) > 1:
      raise AttributeError("Multiple definitions in '"+definition+"', expected only one")
    
    # Copy the members...
    d = defs[0]
    self.name      = d.name
    self.datatype  = d.datatype
    self.signed    = d.signed
    self.mutable   = d.mutable
    self.nullable  = d.nullable
    self.sensitive = d.sensitive
    self.precision = d.precision
    self.dimension = d.dimension

  def IsNull(self):
  
    s = repr(self)
    if s:
      return False
    return True
    
  def __str__(self):

    s = ''
    
    if self.name != None:
      s += self.name
    
    if self.datatype:
      if len(s):
        s += ' '
      s += self.datatype

    if self.dimension:
      d = ''
      first = True
      for dim in self.dimension:
        if not first:
          d += ','
        first = False
        if dim != None:
          d += str(dim)
      s += '['+d+']'        

    if self.precision:
      p = ''
      first = True
      for prec in self.precision:
        if not first:
          p += ','
        first = False
        if prec != None:
          p += str(prec)
      s += '('+p+')'

    if self.mutable != None:
      if s:
        s += ' '
      s += ['immutable','mutable'][self.mutable]
        
    if self.nullable != None:
      if s:
        s += ' '
      s += ['notnull','nullable'][self.nullable]
      
    if self.signed != None:
      if s:
        s += ' '
      s += ['unsigned','signed'][self.signed]
      
    if self.sensitive != None:
      if s:
        s += ' '
      s += ['insensitive','sensitive'][self.sensitive]
      
    return s
    
  def __repr__(self):

    return "ElmDef("+repr(str(self))+")"
    
#===============================================================================
#
# Function:   ElmDefs
#
# Purpose:    Parse and produce a list of ElmDefs
#
# Usage:      errors, elmdefs = ElmDefs(stmt)
#
# Parameters: stmt - String: Element definition (see below)
#
# Returns:    errors  - List of error exceptions (see BNF_errors)
#             elmdefs - List of produced ElmDefs
#
# Element definitions...
#
#   An element definition is a comma and/or newline separated definitions in
#   the following format...
#
#     <name> <type> [<structure>][<modifier>...]
#
# Note: <structure> and <modifier> aren't handled yet
#

def ElmDefs(expr):

  errors  = []
  elmdefs = []
  elmdef  = None

  states = Enum('name,type,modifiers,precision,dimension,skipping')
  state = states.name
  depth = []
  error = None
  dimension = None
  precision = None
    
  try:
    words = list(ElmDefWords(expr))
  except SyntaxError as err:
    return ([err],[])

  word = None
  
  for thisWord in words:

    prevword = word
    
    if thisWord is None:
      line = col = word = None    
    else:
      line, col, word = thisWord
      
    try:

      if word == ')' and (not depth or depth[-1][2] != '('):
        raise SyntaxError(line,col,"Misplaced ')'")
      if word == ']' and (not depth or depth[-1][2] != '['):
        raise SyntaxError(line,col,"Misplaced ']'")
      if word == ';' and depth:
        raise SyntaxError(line,col,"Seperator ';' only valid on Element Definition level")

      if   state == states.name:
        if word is None or not IsValidName(word):
          error = (line, col, "Expected element name here")
          datatype = None
          if word in ',;':
            state = states.name
          else:
            state = states.skipping
          continue         
        elmdef = ElmDef()        
        elmdef.name = word
        state = states.type
        continue
        
      # If skipping...
      if state == states.skipping:
        if word is None:
          break
        if word == '<eol>' or (not depth and word in (',',';')):
          state = states.name
        elif len(word) == 1:
          if word in '[(':
            depth.append((line,col,word))
          elif word in '])':
            depth.pop()
        continue

      # Handle truncated definition...
      atend = word in (',',';','<eol>',None) and len(depth) == 0
      if atend:
        elmdefs.append(elmdef)
        elmdef = None
        state = states.name
        if word is None:
          break
        continue

      # Handle optional states      
      if state == states.type:
      
        if not word in datatypes:
          raise SyntaxError(line, col, "Expected Type here")
        elmdef.datatype = word
        state = states.modifiers
      
      elif state == states.modifiers:
      
        if word in '([':
          depth.append((line,col,word))
          if word == '(':
            if len(depth) > 1:
              raise SyntaxError(line,col,"Nested precision")
            if elmdef.precision != None:
              raise SyntaxError(line,col,"Precision already specified")
            state = states.precision
            if elmdef.precision is None:
              elmdef.precision = []
            elmdef.precision.append(None)
          else:
            if len(depth) > 1:
              raise SyntaxError(line,col,"Nested dimension")
            #if elmdef.dimension != None:
            #  raise SyntaxError(line,col,"Dimension already specified")
            state = states.dimension
            if elmdef.dimension is None:
              elmdef.dimension = []
            elmdef.dimension.append(None)
          continue
          
        else:
        
          # Make sure modifier's known
          if not word in modifiers:
            raise SyntaxError(line,col,"Expected modifier here")
            
          # Get member/value
          member,value = modifiers[word]
          
          # Make sure member not already set...
          if elmdef.__dict__[member] != None and elmdef.__dict__[member] != value:
            raise SyntaxError(line,col,"Conflicting modifier")
            
          # Set it
          elmdef.__dict__[member] = value
  
        if not atend and not word in modifiers:
          raise SyntaxError(line, col, "Expected modifier here")
      
      elif state == states.precision or state == states.dimension:

        if word == ',':
          if state == states.precision:
            if len(elmdef.precision) >= 2:
              raise SyntaxError(line, col, "Only <precision>/<scale> accepted")
            elmdef.precision.append(None)
          else:
            elmdef.dimension.append(None)
          continue

        if word == ']':
          if state != states.dimension:
            raise SyntaxError(line,col,"Misplaced ']'")
          state = states.modifiers
          depth.pop()
          continue
        if word == ')':
          if state != states.precision:
            raise SyntaxError(line,col,"Misplaced ')'")
          while len(elmdef.precision) < 2:
            elmdef.precision.append(None)
          state = states.modifiers
          depth.pop()
          continue
          
        if not is_int(word):
          raise SyntaxError(line,col,"Expected integer here")

        if state == states.precision:
          lst = elmdef.precision
        else:
          lst = elmdef.dimension

        if lst and prevword not in "([,":
          raise SyntaxError(line,col,"Expected ',' here")
          
        x = lst.pop()
        if x != None:
          raise InternalError("Expected lst.pop() to be None")
        lst.append(int(word))
                
      continue
      
    except SyntaxError as err:
    
      elmdef = None
      errors.append(err)
      if word in (',','<eol>') and not depth:
        state = states.name
      else:
        state = states.skipping
      continue
      
  if elmdef:
    elmdefs.append(elmdef)
    
  return errors,elmdefs

#===============================================================================
#
# __main__
#

if __name__ == "__main__":

  strs = ["a int[,10] immutable",
          "b string(1,) nullable sensitive",
          "c int unsigned",
          "d int[,,,]",
          "e int notnull nullable",
          "f char notnull notnull",
          "g int(1,2)(3,4)",
          "h char[1][2][3]",
          "i char[1 2 3]"
         ]
         
  for s in strs:
    print("String: "+s)
    e = ElmDef(s)
    if e.errors:
      print('')
      print("***Error(s) in "+repr(s)+"...")
      for error in e.errors:
        print('Line: '+s)
        print('      '+(' '*error.col)+'^')
        print('      '+error.error)
      print(' ')
    else:
      print("ElmDef: "+repr(e))
