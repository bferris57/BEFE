#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File: BEFE_WordList - WordList and related classes
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

import sys
import os
from BEFE_errors    import *
from BEFE_Struct    import *
from BEFE_Enum      import *
from BEFE_ElmDef    import *
from BEFE_FileWords import *

# DEBUG...
from BEFE_debug     import *
# ...DEBUG

# Python 2.7 has type long, Python 3.x doesn't...
if sys.version_info.major >= 3:
  long = int
  
#===============================================================================
#
# Class:   WordList
#
# Purpose: Pesent a "lazy list" of BEFE Word instances present in some input.
#
# Notes:   The word list can be index normally except...
#
#            o Negative indices return None
#            o Indices > len(list) return None
#
#          We do this to avoid raising errors, plus it's more handy when pulling
#          the words apart into BNF Rules.
#
#          Because of this the only exceptions raised are Failed() and
#          SyntaxError. Any other exception indicates a bug that probably needs
#          fixing.
#          

class WordList:

  def __init__(self,filename,data,ignwhite=True,igncomment=True):
  
    self.filename   = filename
    self.data       = data
    self.list       = []
    self.finished   = False
    self.input      = FileWords(filename,data)
    self.iter       = _WordListIter(self)
    self.ignwhite   = ignwhite
    self.igncomment = igncomment
    self.tokens     = None
    
    # If it's a .py input file, it's befe output...
    if len(filename) > 3 and filename[-3:] == ".py":
      file,words,data = ProcessBefeOutput(filename)
      self.filename = file
      self.data     = data
      self.list     = words
    
  def __iter__(self):
  
    return _WordListIter(self)
    
  def __len__(self):

    try:
      while not self.finished:
        self.iter.next()
    except StopIteration:
      pass
      
    return len(self.list)
    
  def __getitem__(self,idx):
  
    if type(idx) not in [int,long]:
      return super(WordList,self).__getitem__(idx)
      
    if idx < 0:
      return None

    try:
      while not self.finished and len(self.list) <= idx:
        self.iter.next()
    except StopIteration:
      return None
    
    if idx >= len(self.list):
      return None

    return self.list[idx]
    
#===============================================================================
#
# Iterator: _WordListIter - Iterate through WordList words
#
# Notes:    This iterator will ignore whitespace and comment words, if indicated
#           on WordList creation, and can be used to fully populate the
#           associated WordList's list of words.
#
#           A WordList's list of words is a "lazy list" so this class is used
#           populate it as needed.
#

class _WordListIter:

  def __init__(self,wordlist):

    self.wordlist = wordlist
    self.pos      = 0
    
  def __iter__(self):
  
    self.pos = 0
    return self

  def __getitem__(self,idx):

    if type(idx) in [int,long]:
      return self.wordlist[idx]    
    else:
      raise TypeError("Expected idx to be an int or long")
      
  def next(self):

    # If already got it...
    if self.pos < len(self.wordlist.list):
      self.pos += 1
      return self.wordlist.list[self.pos-1]
      
    # Not got it yet...
    if self.wordlist.finished:
      raise StopIteration
      
    # Fill out the WordList's list...
    tpos = self.pos
    try:
      while tpos >= len(self.wordlist.list):
        if sys.version_info.major >= 3:
          word = self.wordlist.input.__next__()
        else:
          word = self.wordlist.input.next()
        if word.col == 0:
          eol = Word(word=word)
          eol.type = Word.Type.EOL
          self.wordlist.list.append(eol)
        if word.type not in [Word.Type.WHITE,Word.Type.COMMENT] or \
           (word.type == Word.Type.WHITE and not self.wordlist.ignwhite) or  \
           (word.type == Word.Type.COMMENT and not self.wordlist.igncomment):
          self.wordlist.list.append(word)
    except StopIteration:
      self.wordlist.finished = True
      self.wordlist.input = None
      
    if self.pos >= len(self.wordlist.list):
      raise StopIteration    
      
    self.pos += 1
    return self.wordlist.list[self.pos-1]
    
  def __next__(self):
    return self.next()
    
#===============================================================================
#
# Generator: LineWords
#
# Purpose:   Present a line by line list of BEFE Words
#
# Notes:     Each line (iteration) processes a list of BNF Words on the same line.
#

def LineWords(filename,data,ignwhite=True,igncomment=True):

  curline = []
  words = WordList(filename,data,ignwhite=ignwhite,igncomment=igncomment)
  idx = 0
  
  while True:
  
    # Get next word
    next = words[idx]
    idx += 1
    
    # If at end...
    if not next:
      if curline:
        yield curline
      raise StopIteration
    
    # If different line, got a line
    if curline and next.line != curline[0].line:
      yield curline
      curline = []
      
    # Same or new line...
    curline.append(next)
    
#===============================================================================
#
# Function: ProcessBefeOutput
#
# Purpose:  Hack to read C++ output from befe when the following is done...
#
#             befe +vv test trythis;x >filename
#
#               or...
#
#             befe lang;curlang .befe;lex +pyout c:/svn/befe.befe/BEFE.primitives.befe >filename
#
#           When this output is saved in a '.py' file since trythis() outputs
#           Python executable source code. So, we can load it into
#           and produce our list of our own Word instances vs C++ LexTokens.
#
# Usage:    words,data = ProcessPythonInput(fileName)
#
# Where:    words    = Out: list: List of Word() instances in file
#           data     = Out: str:  Contents of the file specified in the token set read
#           fileName = In:  str:  Name of file to read (expect '.py' file)
#
# Notes:    Each LexToken produced by trythis() contains the following keys/value pairs...
#
#             "type" - See typeMap below and LexToken.h
#             "lineCount" - Number of '\n' in token (dodgy at the moment)
#             "status"    - Ignored
#             "value"     - Ignored (we populate Word with real value)
#             "lineNo"    - Token line number (0..N)
#             "colNo"     - Token column number (0..N)
#             "span"      - File data span (populate Word members soff, eoff, and value)
#

# Map LexToken.h token type to Word.type...  typenames  = ["WHITE","COMMENT","OPERATOR","DQUOTE","SQUOTE","BQUOTE","INT","HEX","NAME","ACTION","RANGE","BOL"]
#  typenames  = ["WHITE","COMMENT","OPERATOR","DQUOTE","SQUOTE","BQUOTE","INT","HEX","NAME","ACTION","RANGE","EOL"]

typeMap = {
  "Unknown":     None,
  "EndOfInput":  None,
  "EndOfLine":   Word.typenames.index("EOL"),
  "Error":       None,
  "Unexpected":  None,
  "Warning":     None,
  "WhiteSpace":  Word.typenames.index("WHITE"),
  "Operator":    Word.typenames.index("OPERATOR"),
  "BracketLeft": Word.typenames.index("OPERATOR"),
  "BracketRight":Word.typenames.index("OPERATOR"),
  "Separator":   Word.typenames.index("OPERATOR"),
  "Comment":     Word.typenames.index("COMMENT"),
  "Name":        Word.typenames.index("NAME"),
  "Number":      Word.typenames.index("INT"),
  "DLiteral":    Word.typenames.index("DQUOTE"),
  "SLiteral":    Word.typenames.index("SQUOTE"),
  "BLiteral":    Word.typenames.index("BQUOTE"),
  "MultiQuote":  Word.typenames.index("DQUOTE"),
  "NameLiteral": Word.typenames.index("NAME"),
  "Preprocessor":None,
}

def ProcessBefeOutput(fileName,ignorewhite=True,ignorecomment=True):

  import os.path as path

  globals = {}
  locals  = {}
  execfile(fileName,globals,locals)
  befeTokens = {}
  if 'befeTokens' in globals:
    befeTokens = globals['befeTokens']
  else:
    befeTokens = locals['befeTokens']
  if len(befeTokens) < 2:
    raise RuntimeError("Malformed 'befeTokens', len(befeTokens) = %d"%len(befeTokens))
  
  file = befeTokens['file']
  if not path.exists(file):
    raise RuntimeError("File %s does not exist, can't go on"%repr(file))
  if not path.isfile(file):
    raise RuntimeError("Input %s exists but isn't a file, can't go on"%repr(file))
  data = bytes(open(file,'rb').read())
  tokens = befeTokens['tokens']

  #print("File = %s"%repr(file))
  #print("Contains %d characters"%len(data))
  #print("Processing %d tokens..."%len(tokens))
  fileseq = -1
  lineseq = -1
  prevLine = -1
  curLine = -1
  words = []
  numIn = 0
  numOut = 0
  for token in tokens:
  
    numIn += 1
    
    # Get starting info...
    word    = Word()
    tType   = token["type"]
    tLineNo = token["lineNo"]
    tColNo  = token["colNo"]
    tSpan   = token["span"]
    
    # Ignore if not in typeMap...
    if tType not in typeMap or typeMap[tType] is None: 
      continue
    
    # Adjust fileseq and lineseq...
    fileseq += 1
    if tLineNo != curLine:
      lineseq = -1
      curLine = tLineNo
    lineseq += 1
    
    # Populate the word...
    word.filename = file
    word.fileseq  = fileseq
    word.lineseq  = lineseq
    word.line     = tLineNo
    word.col      = tColNo
    parts = tSpan.split(':')
    word.soff     = int(parts[0])
    word.eoff     = int(parts[1])
    word.type     = typeMap[tType]
    word.value    = str(data[word.soff:word.eoff])
    
    if (word.type == Word.Type.WHITE and ignorewhite) or (word.type == Word.Type.COMMENT and ignorecomment):
      continue
      
    words.append(word)
    numOut += 1
      
  return file,words,data
  
#===============================================================================
#
# Code: __main__
#

if __name__ == "__main__":

    import sys
    import os.path as path
    
    pythonInput = False
    if len(sys.argv) > 1:
      name = sys.argv[1]
      if not path.exists(name):
        raise RuntimeError("File '%s' does not exist"%name)
      if not path.isfile(name):
        raise RuntimeError("'%s' is not a file"%name)
      root,ext = path.splitext(name)
      pythonInput = ext == '.py'
    else:
      name = 'c:/SVN/befe.befe/BEFE.primitives.befe'

    if pythonInput: # Hack to read 'befe test trythis;x >...' output...
    
      words,data = ProcessBefeOutput(name)
      print("__main__: len(words) = %d, len(data) = %d"%(len(words),len(data)))
      
      tokenNo = -1
      for word in words:
        tokenNo += 1
        out = ''
        if word.type in [Word.Type.NAME,Word.Type.BQUOTE]:
          out += '`'+word.value+'`'
        elif word.type == Word.Type.SQUOTE:
          out += "'"+word.value+"'"
        elif word.type == Word.Type.DQUOTE:
          out += '"'+word.value+'"'
        elif word.type == Word.Type.OPERATOR:
          out += 'OPERATOR:`'+word.value+'`'
        else:
          out += word.value
        print("[%d]: %s"%(tokenNo,str(word)))
      
    
    else:           # Normal python processing...
    
      data = open(name).read()

      lineList = LineWords(name,data,True,True)

      for line in lineList:
        wds = ''
        wordNo = -1
        lineNo = -1
        indent = -1
        for word in line:
          wordNo += 1
          if wordNo == 0:
            lineNo = word.line+1
            indent = word.col
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
        print("[%d]: indent=%d: %s"%(lineNo,indent,wds))