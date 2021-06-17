#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BNF_functions - General functions
#
# Purpose: Separate functions from Classes, etc.
#
# Notes:   These are only here because they don't naturally fit anywhere else.
#
#===============================================================================
# Copyright (©) 2004-2018, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from errors import *

#===============================================================================
#
# Function: is_int  - Does a string represent a valid 'int' value?
#           is_long - Does a string represent a valid 'long' value?
#           is_float - Does a string represent a valid 'float' value?
#           is_double - Does a string represent a valid 'double' value?
#
# Usage:    answer = is_int(s)
#           answer = is_long(s)
#           answer = is_float(s)
#           answer = is_double(s)
#
# Where:    s      - String: Value to test
#
# Returns:  answer - Boolean: True  = String represents an int
#                             False = Not a String or doesn't represent an int value
#

def is_int(s):
  if type(s) != str:
    return False
  try: 
    int(s)
    return True
  except ValueError:
    return False

def is_long(s):
  if type(s) != str:
    return False
  try:
    long(s)
    return True
  except ValueError:
    return False

#===============================================================================
#
# Utility Functions
#
# Function: IntToHex      - Convert integer string to hexidecimal string
#           QuoteStrToInt - Convert quotes string to int
#           IntStrToInt   - Convert integer string to int
#           HexStrToInt   - Convert hexidecimal string (0xnnn) to int
#

def IntToHex(n):

  s = ''
  if type(n) not in (int,long):
    return s
  while True:
    dig = n & 15
    if dig >= 10:
      dig = chr(ord('a')+dig-10)
    else:
      dig = chr(ord('0')+dig)
    s = dig + s
    n /= 16
    if n == -1 and len(s)%2 == 0 and s[0:2] == 'ff':
      break
    if n == 0 and len(s)%2 == 0:
      break
  return s
  
def QuoteStrToInt(str):

  if len(str) < 2:
    return None
  if str[0] in "\"'" and str[-1] == str[0]:
    str = str[1:-1]
  if len(str) == 4 and str[0:2] == "\\x":
    str = "0x"+str[2:]
    return HexStrToInt(str)
  if len(str) != 1:
    return None;
  val = ord(str[0])
  return val
  
def IntStrToInt(str):

  try:
    val = int(str)
  except:
    val = None
  return val
  
def HexStrToInt(s):

  if type(s) != str:
    return None
  s = s.lower()
  if len(s) <= 3:
    return None
  if s[1].lower() != 'x':
    return None
  val = 0
  for pos in range(2,len(s)):
    val <<= 4
    c = s[pos]
    if c in '0123456789':
     dig = ord(c)-ord('0')
    elif c in 'abcdef':
     dig = 10 + ord(c)-ord('a')    
    else:
      return None
    val += dig
  return val

#===============================================================================
#
# Function: numeric_type - Figure out what numeric type is best for this string
#
# Usage:    type = numeric_type(val)
#
# Where:    val - str: value to test
#           type - type: int, long, float, complex
#                        (nonetype if neither of those)
#

def numeric_type(val):

  types = [int,long,float,complex]
  
  for typ in types:
  
    try:
      a = typ(val)
      if type(a) == typ:
        return typ
    except ValueError:
      pass
    
  return type(None)
  
#===============================================================================
#
# Function: ProcessURL - Pull apart URL into dictionary
#
# Usage:    dict = ProcessURL(url)
#
# Where:    url  - String: '<protocol>:[<db>[:<schema>]//<host>[:<port>][/<db>[/<schema>]][?[<arg>...]]'
#
#             Where: <protocol> - Database protocol (type) ('postgresql' default)
#                    <host>     - Host name (like '127.0.0.1', 'localhost', etc.)
#                    <port>     - Port number (Default postgresql is 5432)
#                    <db>       - Database name
#                    <schema>   - Schema to use
#                    <arg>      - One of the following...
#                                   user=<username>
#                                   password=<password>
#
# Returns:  dict - Dictionary of name/value pairs...
#
#             'dbtype'   = Database type ('postgresql' for now)
#             'host'     = Host name
#             'port'     = Port number
#             'database' = Database name (or None)
#             'schema'   = Schema name (or None)
#             'user'     = User name
#             'password' = User password
#
# Notes:     If the URL has <db> or <schema> specified more than once (a silly
#            thing to do), they must match.
#

dbports = {'postgresql':5432, 'mysql':3306}
protocols = ['postgresql','pg','postgres','mysql']

def ProcessURL(url):

  origURL = url
  dict = {'dbtype'  : None,
          'host'    : None,
          'port'    : None,
          'database': None,
          'schema'  : None,
          'user'    : None,
          'password': None,
         }

  # Pull apart starting at '//'...
  parts = url.split('//')
  if len(parts) < 2:
    raise Failed("Missing '//' in '"+url+"'")
  if len(parts) > 2:
    raise Failed("Multiple '//' in '"+url+"'")
  protocol,url = parts
  
  # Handle <dbtype>
  parts = protocol.split(':')
  if len(parts) < 1:
    raise Failed("protocol missing ':' in '"+origURL+"'")
  if len(parts) > 2:
    raise Failed("Protocol has multiple ':' in '"+origURL+"'")
  parts[1] = parts[1].strip()
  if parts[1]:
    raise Failed("Unexpected value after ':' in '"+origURL+"'")
  protocol = parts[0]
  if protocol not in ['postgresql','pg','postgres','mysql']:
    raise Failed("Unknown protocol '"+protocol+"'")
  if protocol[0] == 'p':
    protocol = 'postgresql'
  dict['dbtype'] = protocol
  dict['port'] = dbports[protocol]
  if len(parts) > 1:
    dict['database'] = parts[1]
    
  # Pull a bit apart...
  parts = url.split('?')
  nparts = len(parts)
  if nparts > 2:
    raise Failed("Expected '?' only once in URL '"+origURL+"'")
  if nparts < 1:
    raise Failed("Expected something after '//' in URL '"+origURL+"'")
  if nparts == 1:
    url = parts[0]  
    params = ''
  else:
    url,params = parts
    url = url.strip()
    params = params.strip()
    
  # Process <host>[/dbname]
  parts = url.split('/')
  nparts = len(parts)
  host = dbname = schema = None
  if nparts < 1:
    raise Failed("Expected something after '//' in URL '"+origURL+"'")
  if nparts > 3:
    raise Failed("Expected <host>, <host>/<db>, or <host>/<db>/<schema> after '//' in URL '"+origURL+"'")
  host = parts[0].strip()
  if nparts > 1:
    dbname = parts[1].strip()
  if nparts > 2:
    schema = parts[2].strip()

  # Process <host>    
  parts = host.split(':')
  nparts = len(parts)
  if nparts > 1:
    if nparts > 2:
      raise Failed("Expected single ':' as <host>:<port> in URL '"+origURL+"'")
    host,port = parts
    if not is_int(port):
      raise Failed("Invalid port number in URL '"+origURL+"'")
    dict['port'] = int(port)
  
  dict['host'] = host
  
  # Process <db>
  if not dbname:
    raise Failed("Missing <db> in URL '"+origURL+"'")
  if dict['database'] and dict['database'] != dbname:
    raise Failed("<db> '"+dbname+"' and '"+dict['database']+"' conflicting")
  dict['database'] = dbname
  
  # Process <schema>
  if schema:
    if dict['schema'] and dict['schema'] != schema:
      raise Failed("<schema> '"+schema+"' and '"+dict['schema']+"' conflicting")
    dict['schema'] = schema
    
  # Handle parameters...
  parts = params.split(',')  
  for i in range(0,len(parts)):
    parts[i] = parts[i].strip()

  for part in parts:
    if part.find('=') < 0:
      raise Failed("Expected '=' in parameter '"+part+"' in URL"+origURL+"'")
    name,val = part.split('=')
    name = name.strip()
    val  = val.strip()
    if dict.has_key(name) and dict[name] != None:
      raise Failed("Parameter '"+name+"' already specified in URL '"+origURL+"'")
    if is_int(val):
      val = int(val)
    dict[name] = val

  return dict

#===============================================================================
#
# Function: StatementType - Determine SQL statement type
#
# Usage:    stype = StatementType(stmt)
#
# Where:    stmt - String: SQL statement
#
# Returns:  stype - String: Statement type ('select','drop', etc.) or None
#

def StatementType(stmt):
  if type(stmt) != str:
    return None
  s = ''
  for c in stmt:
    if c == ' ':
      if s: break
      continue
    s += c
  return s
  
if __name__ == '__main__':

  vals = ['123',
          '123456789',
          '123456789101112',
          '123.456',
          'xxx',
         ]
         
  for val in vals:
    print(repr(val)+' = '+repr(numeric_type(val)))
    
#===============================================================================
#
# Function: FindChars - Find char(s) in string
#
# Usage:    pos = FindChars(instr,chars,startPos=None)
#
# Where:    instr    - str: String to search
#           chars    - str: Character(s) to search for
#           startPos - int: Starting position (None = "find first")
#
# Returns:  pos - int: Position found (-1 = "Not found")
#

def FindChars(instr,chars,startPos=None):

  # Sanity checks...
  if type(instr) != str: raise ValueError("FindChars: Expected instr to be type 'str'")
  if type(chars) != str: raise valueError("FindChars: Expected chars to be type 'str'")
  if startPos == None: startPos = 0
  if type(startPos) != int: raise valueError("FindChars: Expected startPos to be type 'int'")
  
  # Do it...
  chars = set(chars) # Possibly more efficient "x in y" than a string search ;-)
  while startPos < len(instr):
    c = instr[startPos]
    if c in chars:
      return startPos
    startPos += 1
    
  return -1
    
  