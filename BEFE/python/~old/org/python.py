from object import *
from error  import *
from ids    import *

def IsPythonWrapper(object):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return IsId(typeid) and typeid == Id_TypePythonWrapper
  except:
    return False

def Wrap(obj):

  if IsPythonWrapper(obj):
    return obj

  if IsObject(obj):
    return obj

  return PythonWrapper(obj)

class PythonWrapper(Object):

  #
  # Class methods
  #

  @classmethod
  def New(cls,pythonobject):

    obj = cls.__new__(cls,value=pythonobject)
    obj.Construct()
    return obj

  #
  # Instance methods
  #

  def Construct(self,typeid=Id_TypePythonWrapper,value=None):

    Object.Construct(self,typeid)

    if IsObject(value):
      raise Error("Expected non Object as an initial value")

    Members(self)['value'] = value

    return Id(self)

  def Die(self):

    self.value = None
    Object.Die(self)

  def Validate(self):

    return None

  #
  # Python wrappers
  #

  def __init__(self,value=None):

    self.Construct(value=value)

  def __len__(self):

    try:
      return len(Members(self)['value'])
    except:
      raise Error("PythonWrapper object has no python len()")

  def __getattr__(self,name):

    try:
      return Members(self)['value'].__getattr__(name)
    except:
      raise Error("PythonWrapper object cannot get python attribute %s"%repr(name))

  def __setattr__(self,name,value):

    try:
      return Members(self)['value'].__setattr__(name,value)
    except:
      raise Error("PythonWrapper object cannot set python attribute %s to value %s"%(repr(name),repr(value)))

  def __getitem__(self,index):

    try:
      return Members(self)['value'].__getitem__(index)
    except:
      raise Error("PythonWrapper object cannot get item %s"%repr(index))

  def __setitem__(self,index,value):

    raise Error("PythonWrapper __setitem__ not implemented yet")

  def __delitem__(self,index):

    raise Error("PythonWrapper __delitem__ not implemented yet")

  def __str__(self):

    try:
      return Members(self)['value'].__str__()
    except:
      raise Error("PythonWrapper object cannot return string of itself")

  def __repr__(self):

    try:
      return Members(self)['value'].__repr__()
    except:
      raise Error("PythonWrapper object cannot return representation of itself")

  def append(self,value):

    try:
      return Members(self)['value'].append(value)
    except:
      raise Error("PythonWrapper object cannot append value %s"%(repr(value)))
