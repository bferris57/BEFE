from ids  import *
from error import *

#-----------------------------------------------------------------------
#
# Id()   - Get any object's Id
# Type() - Get any object's type Id
#

def Id(obj):

  try:
    id = obj.GetId()
  except:
    id = None
  return id

def Type(obj):

  try:
    id = obj.GetType()
  except:
    id = None
  return id

#-----------------------------------------------------------------------
#
# IsObject() - Is python object a valid object?
#

def IsObject(obj):

  try:
    objid  = obj.GetId()
    typeid = obj.GetType()
    return IsId(objid) and IsId(typeid)
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: Object - General Object
#
# Class Members:    Live  - All live objects
#                   Types - Registered root types
#
# Class Methods:    Register        - Register a live object
#                   Unregister      - Unregister a live object
#                   Exists          - Is a given object registered?
#
# Instance Members: id
#                   type
#
# Instance Methods: Object.Live     - Become born
#                   Object.Die      - Become dead (possibly reincarnate)
#                   Object.Validate - Perform self validation
#
#                   Object.GetId
#                   Object.GetType
#
# Python:  object.__init__
#
#-----------------------------------------------------------------------

class Object:

  instances = {}
  members   = {}
  methods   = {}

  #
  # Class methods
  #

  @classmethod
  def RegisterMember(cls, name,getmethod,setmethod=None):

    if name in cls.members:
      raise Error("Member %s already registered"%repr(name))
    else:
      Object.members[name] = (getmethod,setmethod)

  @classmethod
  def RegisterMethod(cls,name,method=None):

    if name in cls.methods:
      raise Error("Method %s already registered"%repr(name))
    else:
      Object.methods[name] = (method,)

  @classmethod
  def RegisterClass(cls):

    if not cls.members and not cls.methods:
      cls.RegisterMember('id'     ,Object.GetId)
      cls.RegisterMember('type'   ,Object.GetType)
      cls.RegisterMethod('Live'   ,Object.Live)
      cls.RegisterMethod('Die'    ,Object.Die)
      cls.RegisterMethod('GetId'  ,Object.GetId)
      cls.RegisterMethod('GetType',Object.GetType)

  @classmethod
  def Register(cls,obj):

    try:
      id = obj.id
      if id != None:
        if id in cls.instances:
          return Error("Object already registered")
        else:
          cls.instances[id] = obj
      return obj
    except:
      raise Error("Object cannot be registered")

  @classmethod
  def Unregister(cls,obj):

    try:
      id = obj.id
      if id != None and id in cls.instances:
        del cls.instances[id]
    except:
      pass

  #
  #  Instance methods
  #

  def Live(self,typeid=Id_TypeObject):

    Object.RegisterClass()

    if typeid != None and not IsId(typeid):
      raise Error("Expected valid typeid")

    try:
      if self.id != None:
        obj = self
      else:
        self.id = NewId()
    except:
      self.id = NewId()

    self.type = typeid
    try:
      obj = Object.Register(self)
    except:
      raise
      obj = Error("Failed to register object")

    return obj

  def Die(self):

    Object.Unregister(self)
    del self.id
    del self.type

  def Validate(self):

    if self.id and not IsId(self.id):
      raise Error("Object's Id isn't an Id!")
    if self.type:
      if not IsId(self.type):
        raise Error("Object's Type isn't an Id!")

  def GetId(self):
    if not self.id:
      self.id = NewId()
    return self.id

  def GetType(self):
    return self.type

  def Copy(self,deep=False):
    self.Check()
    copy = Object()
    copy.type = self.type
    return copy

  #
  # Python wrappers
  #

  def __init__(self):

    self.Live()

  def __str__(self):

    if IsObject(self):
      return str(self.id)
    else:
      raise Error("Dead Object")

  def __getitem__(self,index):

    if type(index) == str:
      if index in Object.members:
        get,set = Object.members[index]
        return get(self)
      elif index in Object.methods:
        return Object.methods[index]
      else:
        raise Error("Object doesn't have a member/method named %s"%repr(index))
    else:
      raise IndexError("Expected string index")

  def __setitem__(self,index):

    raise IndexError("Object.__setitem_ not implemented yet")

  def __repr__(self):

    try:
      if self.id != None:
        return str(self)
      else:
        return "Unborn <Object> @ 0x%08x"%id(self)
    except:
      return "Dead <Object>"

#-----------------------------------------------------------------------
#
# Other modules we automatically expose along with ourselves
#
#-----------------------------------------------------------------------

from pythonwrap import *

