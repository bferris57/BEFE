from ids   import *
from error import *
from object import *

#-----------------------------------------------------------------------
#
# IsList() - Is python object a valid List object?
#

def IsList(e):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return IsId(typeid) and typeid == Id_TypeList
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: List - General List object
#
# The List class implements a non-sparse mutable list of items
# identified by index.  Valid indexes are zero through the length of the
# list minus one.  It is extremely similiar in behaviour to Python lists
# except for 'slicing' (see below).
#
# The List type supports slicing.  A slice is specified by a lower and
# upper boundary.  If either is less than zero, the list length is added
# to it.  If the resulting lower or upper bounds are still outside of
# the range 0..len-1 an exception is raised.
#
# Note: Slicing differs from Python's implementation when upper is less
# than lower.  In Python's case it returns an empty list.  In our case
# we return a list of length (lower-upper) but populated in reverse
# order.
#
# Members: list
#
# Methods: Object.Live
#          Object.Die
#          Object.Validate
#
#          List.GetLength
#          List.GetItem
#          List.SetItem
#          List.DeleteItem
#          List.AppendItem
#
# Python:  object.__init__
#          object.__len__
#          object.__str__
#          object.__repr__
#          object.__getitem__
#
#-----------------------------------------------------------------------

class List(Object):

  @classmethod
  def New(cls,id=None,typeid=None):

    obj = cls.__new__(cls,id=id,typeid=typeid)
    return obj

  def Construct(self,typeid=Id_TypeList,id=None,value = None):

    Object.Construct(self,typeid,id)

    Members(self)['list'] = None

    if type(value) in (list,tuple,str):
      self.list = list(value)
    elif value == None:
      self.list = []
    elif IsObject(value) and value.GetType() == Id_TypeList:
      self.list = list(value._value)
    else:
      raise Error("Expected List object or python list, tuple, string, or None object")

  def Die(self):

    Members(self)['list'] = None
    Object.Die(self)

  def resolveindex(self, index):

    if type(index) != int:
      raise Error("Only integer indexes are allowed")
    len = self.GetLength()
    if index < 0: index = index + len
    if index < 0 or index >= len:
      raise Error("Index %d out of bounds 0..%d"%(index,len-1))
    return index

  def GetLength(self):

    return len(self.list)

  def GetItem(self,index):

    rindex = self.resolveindex(index)
    return self.list[rindex]

  def SetItem(self,index,value):

    rindex = self.resolveindex(index)
    self.list[rindex] = value

  def DeleteItem(self,index):

    rindex = self.resolveindex(index)
    del self.list[rindex]

  def AppendItem(self,value):

    self.list.append(value)

  #
  # Python wrappers
  #

  def __init__(self,id=None,typeid=None,value = None):

    self.Construct(id=typeid,typeid=typeid,value=value)

  def __len__(self):

    return self.GetLength()

  def __str__(self):

    return str(self.list)

  def __repr__(self):

    return repr(self.list)

  def __getitem__(self,index):

    if type(index) == int:
      try:
        return self.GetItem(index)
      except:
        raise(IndexError("list index out of range"))
    elif type(index) == slice:
      raise Error("Type List doesn't support python slicing")
    raise Error("Expected python int or slice index")

  def __setitem__(self,index,value):

    if type(index) == int:
      return self.SetItem(item,value)
    elif type(index) == slice:
      raise Error("Type List doesn't support python slicing")
    raise Error("Expected python int or slice index")

  def append(self,value):

    self.AppendItem(value)