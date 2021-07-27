#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    IANA.py
#
# Purpose: Manage IANA timezone info
#

import gzip
import tarfile
import os
import shutil
import sys
from DBIO_errors import *

gzfiles = ['tzcode2016f.tar.gz','tzdata2016f.tar.gz']

#===============================================================================
#
# Function: WhiteSplit
#
# Purpose:  Split a string by any valid whitespace
#
# Usage:    parts = WhiteSplit(string)
#
# Where:    string - str:   String to split
#           parts  - index: Split string parts
#

def WhiteSplit(s):

  if type(s) != str or not str:
    return []
    
  start  = 0
  slen   = len(s)
  result = []
  
  while start < slen:
    pos1 = s.find(' ',start)
    if pos1 < 0:
      pos1 = slen
    pos2 = s.find('\t',start)
    if pos2 < 0:
      pos2 = slen
    pos = min(pos1,pos2)
    part = s[start:pos]
    result.append(s[start:pos])
    start = pos+1
    
  if start < slen:
    result.append(s[start:])
  
  return result
  

#===============================================================================
#
# Class:   Line
#
# Purpose: Describe a line in a file as a tuple of filename, line number,
#          start, and end.
#

class Line():

  members = ['filename','lineno','start','end']
  memrep  = "filename, members, lineno, start and end"

  def __init__(self,filename=None,lineno=None,start=None,end=None,data=None):

    self.filename = filename
    self.lineno   = lineno
    self.start    = start
    self.end      = end
    self.__dict__['data'] = data
    
    
  def __repr__(self):
  
    result = "Line"+repr(self.Tuple())
    
    return result
    
  def __getattr__(self,name):
  
    if name not in self.members:
      raise AttributeError("Line: Only %s are accessible. Name %s attempted"%(self.memrep,repr(name)))
    return self.__dict__[name]
    
  def __setattr__(self,name,value):
  
    if name not in self.members:
      raise AttributeError("Line: Only %s are accessible. Name %s attempted"%(self.memrep,repr(name)))
    if name == "filename":
      if value != None and type(value) != str:
        raise AttributeError("Line: Expected a string or None")
    else:
      if value != None and type(value) not in [int,long]:
        raise AttributeError("Line: Expected an int, long, or None value")
      if value != None and value < 0:
        raise AttributeError("Line: Negative values not valid")
      
    self.__dict__[name] = value
    
  def __getitem__(self,index):
  
    if type(index) not in [str,int,long]:
      raise AttributeError("Line: Expected str, int, or long index")
      
    if type(index) == str:
      if index not in self.members:
        raise AttributeError("Line: Only %s are accessible. Name %s attempted"%(self.memrep,repr(index)))
      return self.__dict__[index]

    if index < 0 or index > 2:
      raise AttributeError("Line: Index %d invalid, must be in (0,1,2)"%index)
      
    if   index == 0: return self.lineno
    elif index == 1: return self.start
    else:            return self.end
    
  def Tuple(self):
  
    return (self.filename,self.lineno,self.start,self.end)
    
  def Get(self):

    data   = self.__dict__['data']  
    lineno = self.lineno
    start  = self.start
    end    = self.end
  
    if data  is None      or                \
       start is None      or end is None or \
       start <  0         or end <  0    or \
       start >  len(data) or end >  len(data):
      return None

    return data[start:end]

  def ReleaseData(self):
  
    self.__dict__['data'] = None
    
  
#===============================================================================
#
# Function: UnzipUntar
#
# Purpose:  Open a .gz file, unzip it, tar extract it, and put each file
#           contents in a dictionary keyed by it's name.
#
# Usage:    result = UnzipUntar(fname[,result])
#
# Where:    fname  - str:  Name of .tar.gz file
#           result - dict: Dictionary of file name/contents pairs
#
# Notes:    Arguments are validated and exceptions raised if anything's wrong
#

def UnzipUntar(fname,result={}):

  # Sanity checks
  if not type(fname) == str:
    Failed("UnzipUntar: Expected string file name")
  if type(result) != dict:
    Failed("UnzipUntar: Expected passed result to be a dictionary")

  # Get a gz stream open
  gz = gzip.open(fname,'rb')

  # Get the .tar contents...
  print("UnzipUntar: Opening %s...\n"%repr(fname))
  tar = tarfile.TarFile(fileobj=gz)
  members = tar.getmembers()
  for member in members:
    if result.has_key(member.name):
      Failed("UnzipUntar: file %s already in result set"%repr(member.name))
    # Read it...
    contents = tar.extractfile(member).read()
    result[member.name] = contents
  
  return result

#===============================================================================
#
# Generator: LinePositions
#
# Purpose:   Generate text line demarkation tuples for block of text
#            (typically an entire file's contents)
#
# Usage:     for line in LinePositions(filename):
#              # Process line (filename,lineno,start,end) here...
#
# Where:    filename - str: File name
#           start    - int: Starting position (0..)
#           end      - int: Ending position (not inclusive)
#
#           line  - Line: Class Line instance
#
# Notes:    Any beginning or trailing single '\r' charactersare skipped and
#           ignored making this compatable with some of the silly Microsoft
#           text files.
#

def LinePositions(filename,data,start=0,end=None):

  if end is None or end > len(data):
    end = len(data)

  if not end or type(data) != str or type(start) != int or type(end) != int:
    raise StopIteration
      
  lineno = 0
  
  while start < end:
  
    if data[start] == '\r':
      start += 1
      continue
      
    nl = data.find('\n',start)
    if nl < 0:
      if data[end] == '\r':
        end -= 1
      yield Line(filename,lineno,start,end,data)
      raise StopIteration
      
    lineend = nl
    if data[lineend] == '\r':
      lineend -= 1
    
    yield Line(filename,lineno,start,lineend,data)
    lineno += 1
    start = nl+1
    
  raise StopIteration
  
#===============================================================================
#
# Generator: BlockLines
#
# Purpose:   Generate list of Line instances for IANA tz files, each list
#            is a single
#
# Usage:     for blocklines in Blocks(data):
#              blktyp,lines = blocklines
#              for line in lines:
#                 ...
#
# Where:    filename - str: Input file name
#           data     - str: Input text
#           fn       - str: File name (optional)
#

def BlockLines(filename,data):

  start  = 0
  end    = len(data)
  blktyp = ''
  lines  = []
  
  for line in LinePositions(filename,data):
  
    text = line.Get()
    if not text.strip():
      continue
    
    if text[0] == '#':
      newtyp = '#'
    else:
      tabpos = text.find('\t')
      if tabpos < 0:
        tabpos = len(text)
      spcpos = text.find(' ')
      if spcpos < 0:
        spcpos = len(text)
      pos = min(tabpos,spcpos)
      newtyp = text[0:pos].strip()
      if not newtyp:
        newtyp = blktyp
      
    if newtyp != blktyp:
      if blktyp and lines:
        yield blktyp,lines
      lines = []
      blktyp = newtyp
      
    lines.append(line)
    
  if blktyp and lines:
    yield blktyp,lines
    
  raise StopIteration
  
#===============================================================================
#
# Function:  TypeComments
#
# Purpose:   Given type '#', search for comments '# ...<type>...'
#
# Usage:     lines = TypeComments(cmtlines,typ)
#
# Where:     cmtlines - list: List of comment Line instances
#            typ      - str: Type name ('Rule', etc.)
#            lines    - list: List of comment Line instances containing typ
#
# Notes:     What we're really looking for is lines like "# Rule..." etc.
#            but their's some lines like "# Rule\tSwiss..." that we don't
#            want so we cheat, we rule out ones with '-' or ':' in them.
#

def TypeComments(cmtlines,typ):

  lines = []
  for line in cmtlines:
    text = line.Get()
    pos = text.find(typ)
    if pos >= 0 and not ('-' in text or ':' in text):
      lines.append(line)
  return lines
  
#===============================================================================
#
# Function:  ProcessLines
#
# Purpose:   Process type specific Lines
#
# Usage:     result = ProcessLines(typname,lines)
#
# Where:     typname - str: Name of type ('Zone', etc.)
#            lines   - list: List of Line instances
#            result  - dict: Dictionary of lines indexed by name
#
# Notes:     For now, all we do is put all lines with the same name under
#            the same key in a list
#

def ProcessLines(typname,lines):

  result = {}
  prvline = Line()
  
  for line in lines:
  
    if prvline.lineno is None or line.lineno != prvline.lineno+1:
      prvkey = ''
    text = line.Get()
    
    # Truncate on embedded comment
    pos = text.find('#')
    if pos >= 0:
      line.end = pos
      
    # If first part has ' ' in it, turn into '\t'...
    parts = text.split('\t')
    if len(parts) < 2: continue  # Hack for sort of empty lines
    if ' ' in parts[0]:
      p0 = parts[0].split(' ')
      del parts[0]
      for i in range(0,len(p0)):
        parts.insert(i,p0[i])
        
    # Get on with it...
    name = parts[1].strip()
    if not name:
      name = prvkey
    if not result.has_key(name):
      result[name] = []
    result[name].append(line)
    
    prvkey = name
    prvline = line

  #if result.has_key('') and not result['']:
  #  del result['']
    
  return result
  
if __name__ == "__main__":

  # Make sure .gz files exist...
  for gz in gzfiles:
    if not os.path.exists(gz):
      raise Failed("File %s does not exist"%repr(gz))
    if not os.path.isfile(gz):
      raise Failed("File %s exists, but isn't a file"%repr(gz))

  # Remove IANA directory and create an empty one...
  #path = 'IANA'
  #if os.path.exists(path):
  #  if not os.path.isdir(path):
  #    raise Failed("Path %s exists but isn't a directory"%repr(path))
  #  shutil.rmtree(path)
  #  os.mkdir(path)
  
  # Get the files...
  files = {}
  print("Loading files...",)
  for gz in gzfiles:
    files = UnzipUntar(gz,files)
  print("%d files loaded"%len(files))
    
  # Remove ones with a '.' in it...
  keys = files.keys()
  for key in keys:
    if '.' in key or \
      key in ['CONTRIBUTING','LICENSE','Makefile','NEWS','README','Theory']:
      del files[key]  
  
  # Pull one of them apart...
  print("Processing %d files..."%len(files),)
  blktypes = {}
  filenames = files.keys()
  filenames.sort()
  for filename in filenames:
    data = files[filename]
    for blkdef in BlockLines(filename,data):
      typ,lines = blkdef
      if not blktypes.has_key(typ):
        blktypes[typ] = []
      blktypes[typ].extend(lines)
  print("Finished Processing")

  # Show the different block types...
  blkkeys = blktypes.keys()
  blkkeys.sort()
  maxlen = max(len(key) for key in blkkeys)
  for key in blkkeys:
    print(key+' '*(maxlen-len(key))+': %4d lines'%len(blktypes[key]))

  # Process Block types...
  print("DEBUG: blkkeys =",repr(blkkeys))
  for blktyp in blkkeys:
    if blktyp == '#':
      continue
    lines = blktypes[blktyp]
    result = ProcessLines(blktyp,lines)
    keys = result.keys()
    keys.sort()
    print('')
    print(blktyp+'s...')
    print('')
    maxlen = max(len(key) for key in keys)
    special = "America/Indiana"
    speclen = len(special)
    for key in keys:
      print('  '+key+' '*(maxlen-len(key))+' %d lines'%len(result[key])    )
      if blktyp == 'Leap' or \
        (blktyp in ['Zone','Link'] and key == 'Europe/London') or \
        (len(key) >= speclen and key[:speclen] == special) or \
        'indianapolis' in key.lower():
        for line in result[key]:
          #print('    '+line.filename+'('+str(line.lineno+1)+'): '+repr(line.Get()))
          print('    '+repr(WhiteSplit(line.Get())))
  
  # TEMP...
  if True:
    sys.exit(0)
    
  # Show us the Zone Europe/London lines...
  lines = blktypes['Zone']  
  result = ProcessLines('Zone',lines)
  londonLines = result['Europe/London']
  print('')
  print('Zone Europe/London...')
  mintabs = 99999
  maxtabs = 0
  for line in londonLines:
    text = line.Get()
    parts = text.split('\t')
    mintabs = min(mintabs,len(parts))
    maxtabs = max(maxtabs,len(parts))
    print(("%5d: "%(line.lineno+1))+repr(text))
  print("--mintabs =",mintabs,", maxtabs =",maxtabs)
  
  # Show us the Link Europe/London lines...
  lines = blktypes['Link']
  result = ProcessLines('Link',lines)
  londonLines = result['Europe/London']   
  print('')
  print('Link Europe/London...')
  mintabs = 99999
  maxtabs = 0
  for line in londonLines:
    text = line.Get()
    parts = text.split('\t')
    mintabs = min(mintabs,len(parts))
    maxtabs = max(maxtabs,len(parts))
    print(("%5d: "%(line.lineno+1))+repr(text))
  print("--mintabs =",mintabs,", maxtabs =",maxtabs)

  # All Done
  print('')
  print("!!!OKEY DOKEY!!!\n"      )
  