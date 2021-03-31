#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:       BEFE_FileWords - BEFE FileWords and related classes/code
#
# Generators: FileWords - Context for FileWords.word() generator
#
# Functions:  StateNULL      - Handle state 'null' - Determine next state
#             StateWHITE     - Handle state 'white' - " \t\n"
#             StateINT       - Handle state 'int' - Integer literals
#             StateHEX       - Handle state 'hex' - "0x..."
#             StateQUOTE     - Handle state 'quote' - "\'", "\"", etc.
#             StateCOMMENT   - Handle state 'comment' - "#...", "%..."
#             StateMLCOMMENT - Handle state 'mlcomment' - "/*...*/"
#             StateACTION    - Handle state 'action' - "{...}""
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

import sys
import os
from BEFE_errors import *
from BEFE_Struct import *
from BEFE_Enum   import *
from BEFE_ElmDef import *
from BEFE_Word   import *

# Rest of stuff...

def Lines(data):

  pos = 0
  datal = len(data)
  while pos < datal:
    nl = data.find('\n',pos)
    if nl < 0:
      nl = datal
    yield (pos,nl)
    pos = nl+1

  raise StopIteration

#===============================================================================
#
# Class: FileWords - Context for FileWords.Word() generator
#
# Note: The generated words include WHITESPACE and COMMENTs so you'll have to
#       ignore those if they're unwanted
#

def FileWords(filename,data):

  ctxmembers = ["filename",
                "data",
                "states",
                "state",
                "pos",
                "spos",
                "epos",
                "lineno",
                "col",
                "word",
                "fileseq",
                "lineseq",
                "quotechar",
               ]
  ctx = Struct(','.join(ctxmembers))
  
  ctx.filename   = filename
  ctx.data       = data
  ctx.fileseq    = 0 # Note: Indicates NEXT fileseq
  ctx.lineno     = -1
  statenames = ["NULL","NAME","INT","HEX","QUOTE","WHITE","COMMENT","MLCOMMENT"]
  ctx.states     = Enum(','.join(statenames))
  ctx.word       = Word()
  ctx.state      = ctx.states.NULL

  for line in Lines(data):
  
    ctx.lineno += 1
    ctx.spos,ctx.epos = line
    ctx.lineseq = 0 # Note: Indicates NEXT lineseq

    # Now, find and handle each word...
    ctx.pos = ctx.spos-1
    ctx.col = -1
    
    while ctx.pos+1 < ctx.epos:

      # Setup new character
      ctx.pos += 1
      ctx.col += 1
      c = ctx.data[ctx.pos]

      #
      # Handle the state...
      #
      
      funcs = [StateNULL,StateName,StateINT,StateHEX,StateQUOTE,StateWHITE,StateCOMMENT,StateMLCOMMENT]
      yword = funcs[ctx.state](c,ctx)
      if yword:
        ctx.word = Word()
        yword.filename=filename
        yield yword;
                
      continue          
                
    # Handle end of line...
    if ctx.state != ctx.states.MLCOMMENT:
      
      # If we've got a word, flush it...
      if ctx.word:
        if ctx.word.type in [Word.Type.DQUOTE, Word.Type.SQUOTE, Word.Type.BQUOTE]:
          raise SyntaxError(ctx.word,"Unterminated string")
        if ctx.state == ctx.states.NAME and ctx.word.value[0] == '<':
          raise SyntaxError(ctx.word,"Unterminated name")
        ctx.word.lineseq  = ctx.lineseq
        ctx.word.fileseq  = ctx.fileseq
        ctx.word.filename = ctx.filename
        ctx.word.eoff     = ctx.pos+1
        yield ctx.word
        ctx.fileseq += 1
        ctx.lineseq += 1
        ctx.word = Word()
      
      # Yield a whitespace '\n'
      ctx.word = Word()
      ctx.word.line    = ctx.lineno
      ctx.word.col     = ctx.col+1
      ctx.word.soff    = ctx.epos
      ctx.word.eoff    = ctx.epos+1
      ctx.word.len     = 1
      ctx.word.type    = Word.Type.WHITE
      ctx.word.value   = '\n'
      ctx.word.lineseq = ctx.lineseq
      ctx.word.fileseq = ctx.fileseq
      yield ctx.word
      # Note: Commented these out because, semantically, whitespace
      #       shouldn't count. Not sure about this
      #fileseq += 1
      #lineseq += 1
      ctx.word = Word()
      ctx.state = ctx.states.NULL

    continue

  # Finished  
  raise StopIteration

#=============================================================================
#
# State Handlers...
#

#
# null state
#

def StateNULL(c,ctx):
  
  yword = None

  # If comment...
  iscomment = False
  if c in '#%/':
    iscomment = True
    if c == '/' and (ctx.pos >= ctx.epos-1 or ctx.data[ctx.pos+1] != '/'):
      iscomment = False
  if iscomment:
    ctx.word.line = ctx.lineno
    ctx.word.col  = ctx.col
    ctx.word.soff = ctx.pos
    ctx.word.eoff = ctx.epos
    ctx.word.len  = ctx.epos-ctx.pos
    ctx.word.type = Word.Type.COMMENT
    ctx.word.value = ctx.data[ctx.pos:ctx.epos]
    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    yword = ctx.word
    # Note: Commented these out because, semantically, comments
    #       shouldn't count. Not sure about this but if it matters
    #       you can always sort comments by Word.soff instead of
    #       Word.fileseq and Word.lineseq
    #fileseq += 1
    #lineseq += 1
    ctx.word = Word()
    ctx.pos = ctx.epos
    return yword # (But finished with line!!!)

  # Check for '/*...
  if c == '/' and ctx.pos < ctx.epos-1 and ctx.data[ctx.pos+1] == '*':
    ctx.state      = ctx.states.MLCOMMENT
    ctx.word.line  = ctx.lineno
    ctx.word.col   = ctx.col
    ctx.word.soff  = ctx.pos
    ctx.word.eoff  = ctx.epos
    ctx.word.len   = 2
    ctx.word.type  = Word.Type.COMMENT
    ctx.word.value = ctx.data[ctx.pos:ctx.pos+2]
    ctx.pos += 1
    
    return yword
    
  # If whitespace...
  if c in Word.whitespace:
    ctx.state = ctx.states.WHITE
    ctx.word.line  = ctx.lineno
    ctx.word.col   = ctx.col
    ctx.word.soff  = ctx.pos
    ctx.word.eoff  = ctx.pos
    ctx.word.len   = 1
    ctx.word.type  = Word.Type.WHITE
    ctx.word.value = c
    return yword

  # Expand c if matches Word.ops...
  if c in Word.ops:
    for op in Word.ops:
      if ctx.pos <= ctx.epos-len(op) and ctx.data[ctx.pos:ctx.pos+len(op)] == op:
        c = op
        break

  # If operator...
  if c in Word.ops:
  
    ctx.word.line = ctx.lineno
    ctx.word.col  = ctx.col
    ctx.word.soff = ctx.pos
    ctx.word.len  = len(c)
    ctx.word.eoff = ctx.pos + len(c)
    ctx.word.type = Word.Type.OPERATOR
    ctx.word.value = c
    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    yword = ctx.word
    ctx.fileseq += 1
    ctx.lineseq += 1
    ctx.word = Word()
    ctx.pos += yword.len-1
    ctx.col += yword.len-1
    return yword
    
  elif c in namechars:
    ctx.word.line = ctx.lineno
    ctx.word.col  = ctx.col
    ctx.word.soff = ctx.pos
    ctx.word.eoff = ctx.pos
    if c == '0' and ctx.pos < ctx.epos-2 and ctx.data[ctx.pos+1] in 'xX':
      ctx.word.type = Word.Type.HEX
      ctx.word.len = 2
      ctx.word.value = "0x"
      ctx.pos += 1
      ctx.col += 1
      ctx.state = ctx.states.HEX
    else:
      ctx.word.len  = 1
      ctx.word.value = c
      if c in Word.digits:
        ctx.word.type = Word.Type.INT
        ctx.state = ctx.states.INT
      else:
        ctx.word.type  = Word.Type.NAME
        ctx.state = ctx.states.NAME
    return yword

  elif c in "\"'`<":
    realquote = True if c in "\"'`" else False
    ctx.quotechar = c if not c == '<' else '>'
    ctx.word.line = ctx.lineno
    ctx.word.col  = ctx.col
    ctx.word.soff = ctx.pos
    ctx.word.eoff = ctx.pos
    ctx.word.len  = 1 if not realquote else 0
    ctx.word.type  = Word.Type.DQUOTE if ctx.quotechar == '"' else Word.Type.SQUOTE if ctx.quotechar == '\'' else Word.Type.BQUOTE
    ctx.word.value = c if not realquote else ''
    if (ctx.quotechar == '"' and ctx.pos <= ctx.epos-3 and ctx.data[ctx.pos:ctx.pos+3] == '"""') or \
       (ctx.quotechar == "'" and ctx.pos <= ctx.epos-3 and ctx.data[ctx.pos:ctx.pos+3] == "'''") or \
       (ctx.quotechar == "`" and ctx.pos <= ctx.epos-3 and ctx.data[ctx.pos:ctx.pos+3] == "```"):
      ctx.word.eoff += 3
      ctx.word.len = 3
      ctx.word.value = ctx.data[ctx.pos:ctx.pos+3]
      ctx.word.lineseq = ctx.lineseq
      ctx.word.fileseq = ctx.fileseq
      yword = ctx.word
      ctx.fileseq += 1
      ctx.lineseq += 1
      ctx.word = Word()
      ctx.state = ctx.states.NULL
      ctx.pos += 2
      return yword
    ctx.state = ctx.states.QUOTE
    return yword
    
  else:
    raise SyntaxError(ctx.word,"Unexpected character "+repr(c)+" encountered")

#
# white State
#

def StateWHITE(c,ctx):

  if c not in Word.whitespace:
    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    yword = ctx.word
    # Note: Commented these out because, semantically, whitespace
    #       shouldn't count. Not sure about this
    #fileseq += 1
    #lineseq += 1
    ctx.word = Word()
    ctx.pos -= 1
    ctx.col -= 1
    ctx.state = ctx.states.NULL
    return yword

  ctx.word.eoff += 1          
  ctx.word.len += 1
  ctx.word.value += c
  return None
  
#
# name state
#
    
def StateName(c,ctx):

  yword = None
  if c not in namechars:
    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    ctx.word.eoff    = ctx.pos
    yword = ctx.word
    ctx.fileseq += 1
    ctx.lineseq += 1
    ctx.word = Word()
    if c != '>':
      ctx.pos -= 1
      ctx.col -= 1
    ctx.state = ctx.states.NULL
    if c != '>':
      return yword
    else:
      yword.len += 1
      yword.eoff += 1
      yword.value += c
      return yword
  ctx.word.len += 1
  ctx.word.eoff += 1
  ctx.word.value += c
  return yword

#
# int State - Integers
#

def StateINT(c,ctx):

  if c not in Word.digits:
    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    ctx.word.eoff    = ctx.pos
    yword = ctx.word
    ctx.fileseq += 1
    ctx.lineseq += 1
    ctx.word = Word()
    ctx.state = ctx.states.NULL
    ctx.pos -= 1
    ctx.col -= 1
    return yword
  ctx.word.value += c
  ctx.word.len   += 1
  ctx.word.eoff  += 1
  return None
  
  
#
# hex State - Hexidecimal Integers
#

def StateHEX(c,ctx):

  if c not in Word.hexdigits:
    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    ctx.word.eoff    = ctx.pos
    yword = ctx.word
    ctx.fileseq += 1
    ctx.lineseq += 1
    ctx.word = Word()
    ctx.state = ctx.states.NULL
    ctx.pos -= 1
    ctx.col -= 1
    return yword
  ctx.word.value += c
  ctx.word.len   += 1
  ctx.word.eoff  += 1
  return None
  
#
# quote state
#

def StateQUOTE(c,ctx):

  realquote = True if ctx.quotechar in "\"'`" else False
  ctx.word.len += 1
  ctx.word.value += c
  
  if c != ctx.quotechar:
    return None
    
  # May be end quote... Count number of preceeding '\' characters...
  cpos = ctx.pos-1
  cnt = 0
  while cpos >= ctx.word.soff+1:
    if ctx.data[cpos] != '\\':
      break
    cnt  += 1
    cpos -= 1
  
  # If preceeding '\\' is EVEN, it's an endquote
  if (ctx.pos-cpos-1)%2 != 0:
    return None
    
  # Is an endquote...

  if realquote:
    ctx.word.len -= 1
    ctx.word.value = ctx.word.value[:-1]
  ctx.word.eoff = ctx.pos+1
  ctx.word.lineseq = ctx.lineseq
  ctx.word.fileseq = ctx.fileseq
  if ctx.quotechar == '>':
    ctx.word.type = Word.Type.NAME
  yword = ctx.word
  ctx.fileseq += 1
  ctx.lineseq += 1
  ctx.word = Word()
  ctx.state = ctx.states.NULL
  
  # Fiddle with yword resolving embedded '\'...
  i = -1
  numdel = 0
  while i < yword.len-1:
    i += 1
    if yword.value[i] != '\\':
      continue
    # Found '\'...
    if i >= yword.len-1:
      raise SyntaxError(yword.line,yword.col+i+numdel,"Found trailing '\\', shouldn't happen")
    c = yword.value[i+1]
    
    # Figure out substitution digit
    if c == 'x':
      # Handle \x<hexdigit><hexdigit>
      if i+3 >= yword.len or yword.value[i+2] not in Word.hexsubst or yword.value[i+3] not in Word.hexsubst:
        raise SyntaxError(yword.line,yword.col+i+numdel,"Invalid \\x escape sequence")
      subst = chr( Word.hexsubst[yword.value[i+2]]*16 + Word.hexsubst[yword.value[i+3]] )
      thisdel = 3
    else:      
      # Handle other ones...
      pos = 'ntb'.find(c)
      if pos >= 0:
        subst = '\n\t\b'[pos]
      else:
        subst = yword.value[i+1]
      thisdel = 1
    yword.value = yword.value[0:i]+subst+yword.value[(i+thisdel+1):]
    yword.len -= thisdel
    numdel += thisdel-1

  return yword

#
# comment State - Single line comments
#

def StateCOMMENT(c,ctx):

  ctx.word.value += c
  ctx.word.len   += 1
  ctx.word.eoff  += 1
  
  return None
     
   
#
# mlcommeent state - Multi-line comments
#
    
def StateMLCOMMENT(c,ctx):

  ctx.word.eoff += 1
  ctx.word.value += c

  yword = None
  
  if c == '/' and ctx.data[ctx.pos-1] == '*':

    ctx.word.lineseq = ctx.lineseq
    ctx.word.fileseq = ctx.fileseq
    yword = ctx.word
    # Note: Commented these out because, semantically, whitespace
    #       shouldn't count. Not sure about this
    #fileseq += 1
    #lineseq += 1
    ctx.word = Word()
    ctx.state = ctx.states.NULL

  return yword

if __name__ == "__main__":
  
  args = sys.argv[1:]
  errors = []

  if args:
  
    for arg in args:
    
      print("File %s..."%repr(arg))
      data = open(arg).read()
      for word in FileWords(arg,data):
        if word.type not in [Word.Type.COMMENT,Word.Type.WHITE]:
          print('  '+word.toShortString())
        
  else:

    data = open('../befe.befe/BEFE.befe').read()  
    for line in Lines(data):
      print("Line = "+repr(line))
