from ids    import *
from object import *
from error  import *

#-----------------------------------------------------------------------
#
# IsInterface() - Is object a valid Interface object?
#

def IsInterface(i):

  try:
    if not IsObject(i):
      return False
    typeid = i.GetType()
    return IsId(typeid) and typeid == TypeInterface_UUID
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: Interface - General Interface Object
#
# Members: _name
#
# Methods: Len()                 - Get length
#          GetName()             -
#          SetName(name)         -
#          Append(entry)         -
#          GetEntry(index)       -
#          SetEntry(index,value) - Set a given entry
#          GetEntryName(index)   -
#          GetEntryType(index)   -
#
#-----------------------------------------------------------------------

TypeInterfaceMember_Id = ''
TypeInterfaceMethod_Id = ''

class Interface(Object):

  def __init__(self,name=None):

    Object.__init__(self,Id_TypeInterface)
    self._entries = []

    if type(name) != str:
      raise Error("Interface: '%s' is not a valid interface name"%str(name))
    self._name = name

  def GetName(self):

    return self._name

  def SetName(self,value):

    if type(value) != str:
      return Error('Interface: Expected string name')
    self._name = value
