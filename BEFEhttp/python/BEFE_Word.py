#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BNF_Word.py
#
# Purpose: Class Word and related definitions
#
#   Notes: An instance of class Word indicates the occurance of a single
#          BNF Lexical word in a BNF declaration.
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_Struct import *

#===============================================================================
#
# Class:    Word
#
# Purpose:  Represent a single Lexical BNF word
#

class Word(Struct):

  # Class members
  typenames  = ["WHITE","COMMENT","OPERATOR","DQUOTE","SQUOTE","BQUOTE","INT","HEX","OCT","BIN","NAME","ACTION","RANGE","EOL","EOS"]
  Type       = Enum(','.join(typenames))
  namechars    = "_abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"

  ops = [
    '[', ']', '(', ')', '{','}',
    '|', '||', '&', '&&', '!', '~',
    '==',
    '|=', '||=', '&=', '&&=', '!=', '~=',
    '**','++','--',
    '*=','+=','-=',
    '++','--',
    '*','/','%',
    '+','-',
    '->','<-',
    '?',
    '<','>','=','<=','>=', '<>',
    '<<','>>','<<<','>>>',
    ':', ':=', '=', '::=', ';' ,',','.', '..', '...',
    '+++++','++---','--+++','---++','-----', # <-- Aggregate Operators'
    '+++','---',                             # <-- Aggregate Operators
    '++++','----',                           # <-- Aggregate Operators
    '\\',
  ]
  #ops = sorted(ops,cmp=lambda k1,k2: cmp(k1,k2) if len(k1) == len(k2) else -cmp(len(k1),len(k2)))
  ops = sorted(ops,reverse=True)
  opchars = ""
  for op in ops:
    c = op[0]
    if c not in opchars:
      opchars += op[0]
  
  whitespace = " \t"
  hexsubst   = {'0':0,'1':1,'2':2,'3':3,'4':4,'5':5,'6':6,'7':7,'8':8,'9':9,
                'a':10,'b':11,'c':12,'d':13,'e':14,'f':15,
                'A':10,'B':11,'C':12,'D':13,'E':14,'F':15
               }
  digits    = "0123456789"
  hexdigits = "0123456789abcdef"
  members = ["filename string", # File name
             "lineseq  int",    # Sequence (index) in line (0..N)
             "fileseq  int",    # Sequence (index) in file (0..N)
             "line     int",    # Line number in file (0..N)
             "col      int",    # Column number in file (0..N)
             "soff     int",    # File start offset (0..N)
             "eoff     int",    # File end offset (0..N) + 1 for slicing
             "len      int",    # Length of word
             "type     int",    # Type of word (Word.type Enum)
             "value    string", # Word value
            ]


  def __init__(self,filename=None,lineseq=None,fileseq=None,line=None,col=None,soff=None,eoff=None,len=None,type=None,value=None,word=None):
  
              
    Struct.__init__(self,';'.join(Word.members))

    if word != None:
      self.filename = word.filename
      self.fileseq  = word.fileseq
      self.lineseq  = word.lineseq
      self.line     = word.line
      self.col      = word.col
      self.soff     = word.soff
      self.eoff     = word.eoff
      self.type     = word.type
      self.value    = word.value

    else:
      self.filename = filename    
      self.fileseq  = fileseq
      self.lineseq  = lineseq
      self.line     = line
      self.col      = col
      self.soff     = soff
      self.eoff     = eoff
      self.type     = type
      self.value    = value
    
  def __str__(self):
  
    if True:
      s = '['
      s += 'f='+repr(self.filename)
      s += ',fl=('+repr(self.fileseq)+','+repr(self.lineseq)+')'
      s += ','+repr(self.line+1)+'/'+repr(self.col+1)
      s += ','+repr(self.soff)+':'+repr(self.eoff)
      s += ',len='+repr(self.len)
      if self.type is None:
        s += ',type=None'
      else:
        s += ',type='+repr(Word.Type[self.type])
      s += ',val='+repr(self.value)
      s += ']'
      
    if False:
    
      if self.type in [Word.Type.NAME, Word.Type.BQUOTE]:
        s = '`' + self.value + '`'
      elif self.type == Word.Type.SQUOTE:
        s = "'" + self.value + "'"
      elif self.type == Word.Type.DQUOTE:
        s = '"' + self.value + '"'
      else:
        s = self.value
        
    return s

  def toShortString(self):
  
    maxLen = 0
    for name in Word.typenames: maxLen = max(maxLen,len(name))
    
    if self.type is not None:
      s = Word.typenames[self.type]+':'+' '*(maxLen-len(Word.typenames[self.type]))
    else:
      s = 'None' + ' '*maxLen-4
      
    s += ' '+repr(self.filename) + ','
    s += '('+repr(self.line+1)+','+repr(self.col+1)+'),'
    s += repr(self.soff)+':'+repr(self.eoff) + ','
    s += repr(self.len)+': '
    s += repr(self.value)

    return s
    