import uuid

#-----------------------------------------------------------------------
#
# File: typeid - Named type ids (as UUIDs)
#
# This contains the definition for the primitive type Ids used in the
# object system.
#
# The type Ids are defined in one place so they are unique and
# consistent across all basic object types.
#
# Functions: IsId  - Is a given object a valid Id?
#            NewId - Create a new unique Id
#
#-----------------------------------------------------------------------

# Used to generate the unique typeds (if required, otherwise new UUIDs)

TypeDefs = \
'''

#
# Basic types
#

Null           = '{00000000-ffff-ffff-ffff-ffffffffffff}'
Error          = '{ffffff00-ffff-ffff-ffff-ffffffffffff}'
Error.Warning  = '{ffffff01-ffff-ffff-ffff-ffffffffffff}'

Object         = '{80000000-ffff-ffff-ffff-ffffffffffff}'
Type           = '{80020000-ffff-ffff-ffff-ffffffffffff}'

# IMMUTABLE "scalar" types (e.g. can only use initial value and won't
# change through their lifetime)
Object.Bool        = '{80010001-ffff-ffff-ffff-ffffffffffff}'
Object.Int         = '{80010002-ffff-ffff-ffff-ffffffffffff}'
Object.Byte        = '{80010003-ffff-ffff-ffff-ffffffffffff}'
Object.Float       = '{80010004-ffff-ffff-ffff-ffffffffffff}'
Object.Bytes       = '{80010005-ffff-ffff-ffff-ffffffffffff}'
Object.String      = '{80010006-ffff-ffff-ffff-ffffffffffff}'
Object.Tuple       = '{80010007-ffff-ffff-ffff-ffffffffffff}'
Object.ElapsedTime = '{80010008-ffff-ffff-ffff-ffffffffffff}'

  # MUTABLE types (e.g., can change member values through their lifetime)
Object.Slice         = '{80020001-ffff-ffff-ffff-ffffffffffff}'
Object.Array         = '{80020002-ffff-ffff-ffff-ffffffffffff}'
Object.ByteArray     = '{80020003-ffff-ffff-ffff-ffffffffffff}'
Object.Interface     = '{80020004-ffff-ffff-ffff-ffffffffffff}'

# Some useful types...
Object.List               = '{80021000-ffff-ffff-ffff-ffffffffffff}'
Object.List.Node          = '{80021001-ffff-ffff-ffff-ffffffffffff}'
Object.List.Node.NodeFile = '{80021002-ffff-ffff-ffff-ffffffffffff}'
Object.NodeWalker         = '{80030000-ffff-ffff-ffff-ffffffffffff}'

# Python wrapper types
PythonWrapper     = '{c0000000-ffff-ffff-ffff-ffffffffffff}'
PythonObject      = '{c0000001-ffff-ffff-ffff-ffffffffffff}'
PythonNone        = '{c0000002-ffff-ffff-ffff-ffffffffffff}'
PythonBool        = '{c0000003-ffff-ffff-ffff-ffffffffffff}'
PythonInt         = '{c0000004-ffff-ffff-ffff-ffffffffffff}'
PythonFloat       = '{c0000005-ffff-ffff-ffff-ffffffffffff}'
PythonString      = '{c0000006-ffff-ffff-ffff-ffffffffffff}'
PythonPythonTuple = '{c0000007-ffff-ffff-ffff-ffffffffffff}'
PythonPythonList  = '{c0000008-ffff-ffff-ffff-ffffffffffff}'
PythonPythonDict  = '{c0000009-ffff-ffff-ffff-ffffffffffff}'
'''

#-----------------------------------------------------------------------
#
# Funcion: IsId() - Is an object an object ID?
#

def IsId(obj):

  global fakeuuids

  if type(obj) == str:
    try:
      id = uuid.UUID(obj)
    except:
      if fakeuuids and obj[0] == '{' and obj[-1] == '}':
        return True
      return False
    return True
  return False

#-----------------------------------------------------------------------
#
# Funcion: NewId() - Create a new universally unique Object Id
#

fakeuuids = 1000
fakeuuid  = 0

def NewId():

  global fakeuuids
  global fakeuuid

  if fakeuuid < fakeuuids:
    #id = '{%08x-%04x-%04x-%04x-%012x}'%(fakeuuid,0,0,0,0)
    id = '{%08x-0-0-0-0}'%(fakeuuid)
    fakeuuid = fakeuuid + 1
  else:
    id = '{'+str(uuid.uuid4())+'}'

  return id

#-----------------------------------------------------------------------
#
# Function: Generate Types
#
# This function reads the module global TypeDefs, interprets it and
# 'enerates' it's output.  For now, it generates module variables
# named 'ids.Id_Typexxxx' where 'xxxx' is the type name and it's
# value is the specified UUID.
#

def GenerateTypes(typedict={}):

  text =  TypeDefs
  lines = text.split('\n')

  #
  # Process the lines
  #

  lineno = 0
  prvindent = 0

  for line in lines:

    lineno = lineno + 1

    #
    # Doctor the line
    #

    # Strip trailing crap
    line = line.rstrip(' \n\r')
    # If '#' in it, ignore from there on...
    line = line.split('#')[0]
    if line == '': continue
    # Got a line, count leading spaces...
    parts = line.split(' ')
    line = ''
    indent = 0
    for part in parts:
      if part == '':
        indent = indent + 1
        continue
      line = ''.join(parts[indent:])
      break
    if line == '': continue

    # Get the name and id.  If no id, generate a new one
    parts = line.split('=')
    name = parts[0]
    if len(parts) > 1:
      id = parts[1]
    else:
      id = "'{" + str(uuid.uuid4()) + "}'"
    id = eval(id)
    # Pull name apart and make sure there's only one with the same name
    parts = name.split('.')
    typename = 'Id_Type'+parts[-1]
    if typename not in typedict:
      typedict[typename] = id

#-----------------------------------------------------------------------
#
# Main program if run as a script
#

if __name__ == '__main__':

  GenerateTypes()
