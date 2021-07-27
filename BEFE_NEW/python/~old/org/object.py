import ids
ids.GenerateTypes(ids.__dict__)
from   ids   import *
from   error import *
import uuid

#
# Object Repository
#

PythonId = {}     # [python obj] contains obj's Object Id
IdPython = {}     # [Object Id] contains python obj
IdType   = {}     # [Object Id] contains Type Id
IdMembers = {}    # [Object Id] contains dictionary of members
IdMethods = {}    # [Object Id] contains dictionary of methods
IdInterfaces = {} # [Object Id] contains dictionary of interfaces

#-----------------------------------------------------------------------
#
# Id()      - Get any object's Id
# Type()    - Get any object's type Id
# Members() - Get an object's member dictionary
# Methods() - Get an object's method dictionary
#

def Id(obj):

  # If it's registered, got it...
  try:
    if obj in PythonId:
      return PythonId[obj]
  except:
    pass

  # Not registered, if it's a string, maybe it's a UUID form of Id...
  if type(obj) == str:
    try:
      u = uuid.UUID(obj)
      #return u.bytes
      return obj
    except:
      pass

  return None

def Type(obj):

  objid = Id(obj)
  if objid in IdType:
    return IdType[objid]
  return None

def Members(obj):

  objid = Id(obj)
  if objid in IdMembers:
    return IdMembers[objid]
  return None

#-----------------------------------------------------------------------
#
# IsObject() - Is python object a valid object?
#

def IsObject(obj):

  try:
    objid  = Id(obj)
    typeid = Type(obj)
    return objid != None and typeid != None
  except:
    raise
    return False

#-----------------------------------------------------------------------
#
# Function: Python(obj) - Get underlying python object (if any)
#

def Python(obj):

  if IsObject(obj) and Type(obj) == Id_TypePythonWrapper:
    return Members(obj)['value']
  if obj in PythonId:
    obj = PythonId[obj]
  if obj in IdPython:
    return IdPython[obj]

  return obj

#-----------------------------------------------------------------------
#
# Class: Object - General Object
#
# Class Methods:    New              - Create and construct new Object
#
# Instance Members: **None**
#
# Instance Methods: Object.Construct - Become born
#                   Object.Die       - Become dead (possibly reincarnate)
#                   Object.Validate  - Perform self validation
#
#                   Object.GetId     - Get the object's Id
#                   Object.GetType   - Get the object's Type's Id
#
# Python:  object.__init__
#          object.__str__
#          object.__repr__
#          object.__getitem__
#          object.__setitem__
#
#-----------------------------------------------------------------------

class Object:

  #
  # Class methods
  #

  @classmethod
  def New(cls):

    obj = cls.__new__(cls)
    obj.Construct()
    return obj

  #
  #  Instance methods
  #

  def Construct(self,typeid=Id_TypeObject,id=None):

    if Id(typeid) == None:
      raise Error("Expected valid typeid")

    if self in PythonId:
      id  = PythonId[self]
    else:
      if not IsId(id):
        id = NewId()
      else:
        if id in IdPython:
          raise Error("Id %s is already registered"%repr(id))
      PythonId[self] = id
      IdPython[id]   = self
      IdType[id]     = typeid
      IdMembers[id]  = {}

    return id

  def Die(self):

    id = Id(self)
    typeid = Type(self)
    if self in PythonId:
      del PythonId[self]
    if id in IdPython:
      del IdPython[id]
    if id in IdType:
      del IdType[id]
    if id in IdMembers:
      del IdMembers[id]

  def Validate(self):

    if not IsObject(self):
      return False

    id = Id(self)
    typeid = Type(id)

    if Id(self) == None:
      raise Error("Object has no Id!")
    if Type(self) == None:
      if not IsId(self.type):
        raise Error("Object has no Type")

  def GetId(self):
    return Id(self)

  def GetType(self):
    return Type(self)

  #
  # Python wrappers
  #

  def __init__(self):

    self.Construct()

  def __del__(self):

    #if IsObject(self):
    #  self.Die()
    pass

  def __str__(self):

    if IsObject(self):
      return str(Id(self))
    else:
      raise Error("Dead Object")

  def __repr__(self):

    if Id(self) != None:
      return str(self)
    return "Dead <Object>"

  def __getattr__(self,name):

    members = Members(self)
    if members == None:
      members = {}

    if type(name) == str:
      if name in members:
        return members[name]
      else:
        if name in ['id','Id']:
          return Id(self)
        if name in ['type','Type']:
          return Type(self)
        raise Error("Object has no '%s' member"%str(name))
    else:
      raise Error("Expected string name, got '%s'"%repr(name))

  def __setattr__(self,name,value):

    members = Members(self)
    if members == None:
      members = {}

    if type(name) == str:
      if name in members:
        members[name] = value
      else:
        if name in ['id','Id']:
          raise Error("Object's Id is not mutable")
        if name in ['type','Type']:
          raise Error("Object's Type is not mutable")
        raise Error("Object has no '%s' member"%str(name))
    else:
      raise Error("Expected string name, got '%s'"%repr(name))

  def __getitem__(self,index):

    return self.__getattr__(index)

  def __setitem__(self,index,value):

    return self.__setattr__(index,value)

#
# Also include PythonWrapper (including Wrap()) as part of us
#

from python import *