import ids
ids.GenerateTypes(ids.__dict__)
from ids    import *

#-----------------------------------------------------------------------
#
# Function: IsError - Is an object an error instance?
#

def IsError(e):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return IsId(typeid) and typeid == TypeError_Id
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: Error - Errors as objects and python exceptions
#
# Methods: GetId
#          GetType
#

class Error(BaseException):

  def __init__(self, msg, args = ()):

    self._id      = None
    self._type    = Id_TypeError
    self._message = msg
    self._args    = args

  def GetId(self):

    if not self._id:
      self._id = NewId()
    return self._id

  def GetType(self):
    return TypeError_Id

  #
  # Python wrappers
  #

  def __str__(self):

    try:
      s = self._message % self._args
    except:
      s = self._message
    return "<Error>: "+s

  def __repr__(self):

    return str(self)
