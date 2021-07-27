#file:///D:/code/projects/Cwitter/content/bbcnewslive_Clean.htm
#{45D358A4-F3B5-4c0b-9AEE-542C1E5F080A}

import uuid
from   error import *

#
#  Working context context
#

DelimsName = '<>'
DelimsUUID = '{}'
Live = {}

#-----------------------------------------------------------------------
#
# NewId() - Return a new Id string
#

def NewId():

  return DelimsUUID[0]+str(uuid.uuid4())+DelimsUUID[1]

#-----------------------------------------------------------------------
#
# IsKnownObject    - Is this a valid and registered object?
# IsValidUUID      - Is string a UUID ref?
# IsReservedName   - Is string a reserved name?
# ResolveReference - Given a reference, resolve it if we can
#

def IsKnownObject(obj):

  if obj not in Live:
    return False
  if type(obj) == str:
    resobj = ResolveReference(obj)
    if type(resobj) != str:
      return True
  else:
    return True
  return False

def IsId(uuidstr):

  if type(uuidstr) != str:
    return False
  if len(uuidstr) > 2 and uuidstr[0] == DelimsUUID[0] and uuidstr[-1] == DelimsUUID[1]:
    try:
      uuid.UUID(uuidstr[1:-1])
    except:
      return False
    return True
  return False

def IsReservedName(name):

  if type(name) == str and len(name) >= 2 and name[0] == DelimsName[0] and name[-1] == DelimsName[1]:
    return True
  return False

def ResolveReference(name):

  origname = name
  names = []
  while type(name) == str:
    if IsUUID(name) or IsReservedName(name):
      if name in names:
        s = ''
        for ref in names:
          if s: s = s + ' => '
          s = s + repr(ref)
        s = s + ' => ' + repr(name)
        return Error("Circular Reference: %s",s)
      if name not in Live:
        if IsUUID(name):
          return Error("Unresolved UUID: %s",repr(name))
        elif IsReservedName(name):
          return Error("Unresolved reserved name: %s" ,repr(name))
        else:
          return Error("Unresolved string: %s",repr(name))
      names.append(name)
      name = Live[name]
      continue
    else:
      return Error("Unresolved string %s",repr(name))

  if name == None:
    return name

  if id(name) in Live:
    return Live[name]

  if type(name) == str:
    return Error("Unresolved reference : %s" % repr(name))

  return name

def IsValidUUID(uuidstr):
  if type(uuidstr) != str or uuidstr[0] != '{' or uuidstr[-1] != '}':
    return False
  try:
    uuid.UUID(uuidstr[1:-1])
  except:
    return False
  return True

#-----------------------------------------------------------------------
#
# Function: RegisterMetaType - Register a single new metatype
#
#-----------------------------------------------------------------------

def RegisterMetaType(typename,typeuuid,obj):

  # Validate parameters
  if not IsReservedName(typename):
    raise Error("RegisterMetaType: '%s' is not a valid reserved name"%typename)
  if not IsUUID(typeuuid):
    raise Error("RegisterMetaType: %s is not a valid UUID"%typeuuid)
  if typename in Live:
    raise Error("RegisterMetaType: Name %s is already registered",typename)
  if typeuuid in Live:
    raise Error("RegisterMetaType: UUID %s is already registered",typeuuid)
  if obj in Live:
    raise Error("RegisterMetaType: Object 0x%08x is already registered",id(obj))

  # Register
  Live[typename] = typeuuid
  Live[typeuuid] = obj
  Live[obj]      = typeuuid

#-----------------------------------------------------------------------
#
# Function: Reset - Clear everything and reinstall meta types
#
#-----------------------------------------------------------------------

def Reset(instances=False):

  global Live

  newlive = {}
  for entry in Live:
    if IsUUID(entry):
      if instances:
        newlive[entry] = None
      else:
        newlive[entry] = Live[entry]
    elif IsReservedName(entry):
      newlive[entry] = Live[entry]
    else:
      pass

  Live = newlive

#-----------------------------------------------------------------------
#
# Class: Object - General Object
#
# Members: _uuid
#          _type
#
# Methods: __init__
#          GetUUID
#          GetType
#          GetAttributeNames
#          Register
#          Resolve
#          IsResolved
#
#-----------------------------------------------------------------------

TypeObject_UUID = '{26406D0E-5DA7-47e6-82DB-91883C20F0AB}'
TypeError_UUID  = '{A9165C8D-FE12-403d-A347-D012DA3F8F35}'
TypeType_UUID   = '{D2AFE736-7F5B-46d5-BEF9-563D83EF0C1B}'
TypeValue_UUID  = '{5316D64F-D131-4951-8771-0A5D937F1F50}'

class Object:

  def __init__(self):

    self._uuid = DelimsUUID[0]+str(uuid.uuid4())+DelimsUUID[1]
    self._type = TypeObject_UUID

  def Id(self):
    if not self._uuid:
      self._uuid = DelimsUUID[0]+str(uuid.uuid4())+DelimsUUID[1]
    return self._uuid

  def SetUUID(self,uuid):
    if IsUUID(uuid):
      self.Unregister()
      self._uuid = uuid
      self.Register()
      return uuid
    else:
      return Error("Invalid UUID")

  def GetType(self):
    return ResolveReference(self._type)

  def SetType(self,typename):
    self._type = typename

  def Register(self):

    # Make sure we're an Object
    if not issubclass(ResolveReference(self._type),Object):
      return Error("Can only register objects of %s",TypeObject_Name)

    # Validate stuff before we register
    if IsUUID(self._uuid):
      if self._uuid in Live:
        raise RuntimeError("UUID %s already registered"%self.__uuid)

    # Register the UUID
    if IsUUID(self._uuid):
      Live[self._uuid] = self

    # Register the object
    Live[self] = self._uuid

  def UnRegister(self):

    if self in Live:
      del Live[self]
    if self._uuid in Live:
      del Live[self._uuid]

  def Resolve(self):

    self._uuid = ResolveReference(self._uuid)
    self._type = ResolveReference(self._type)

  def IsResolved(self):

    if self._uuid == ResolveReference(self._uuid) and \
      self._type == ResolveReference(self._type) :   \
      return True
    return False

# Register this
RegisterMetaType(TypeObject_Name,TypeObject_UUID,Object)
RegisterMetaType(TypeError_Name,TypeError_UUID,Error)

#-----------------------------------------------------------------------
#
# *** Run on startup as a script or module ***
#
#-----------------------------------------------------------------------

def PrintLive():

  print("***Live has",len(Live),"entries...")
  for entry in Live:
    #print("  ",entry)
    if IsReservedName(entry):
      print("  %s %s"%(entry,repr(Live[entry])))
    elif IsUUID(entry):
      print("  %s 0x%08x"%(entry,id(Live[entry])))
    else:
      print("  Object 0x%08x %s"%(id(entry),Live[entry]))

def Test_References(indent = ''):

  refs = ['xxx'
          ,'<>'
          ,1
          ,'{...}'
          ,'{26306D0E-5DA7-47e6-82DB-91883C20F0AB}'
          ,TypeObject_Name
          ,TypeObject_UUID
          ]

  Reset()

  print(indent+"Testing References...")
  for ref in refs:
    result = ResolveReference(ref)
    print(indent+"  Reference '%s' resolves to %s"%(ref,repr(result)))

  print(indent+"Testing cicular References...")
  refs = ['<a>','<b>','<c>']
  for i in range(0,len(refs)-1):
    Live[refs[i]] = refs[i+1]
  Live[refs[-1]] = refs[1]
  for ref in refs:
    result = ResolveReference(ref)
    print(indent+"  Reference '%s' resolves to %s"%(ref,repr(result)))

  print('')

  Reset()

if __name__ == '__main__':

  #Reset()
  #Test_References()
  PrintLive()

  print("")

else:

  #Reset()
  pass
