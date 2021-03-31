#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: resolver.py - General Resolver object definition
#
#
# Resolvers are intended to resolve names on behalf of a system.  This
# resolution can include internal names, types, files, directories,
# etc.
#
# Specific resolver code isn't implemented here.  It is up to the
# implementer to subtype this type and implement specific name
# resolution.
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

from ids    import *
from object import *
from error  import *

#-----------------------------------------------------------------------
#
# IsResolver() - Is python object a valid Resolver object?
#

def IsResolver(e):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return IsId(typeid) and typeid == Id_TypeResolver
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: Node - General hierarchy member
#
# The Node class implements a member of a hierarchy.
#
#
# Members: name
#
# Methods: Object.Live
#          Object.Die
#          Object.Validate       - Self check
#
#          Resolver.GetName
#          Resolver.SetName
#          Resolver.Resolve()
#
# Python:  __init__
#          __str__
#          __repr__
#
#-----------------------------------------------------------------------

class Resolver(Object):

  def Live(self,name=None):

    Object.Live(self)
    self.type = Id_TypeResolver
    if name != None:
      self.SetName(name)
    else:
      self.name = None

  def Die(self):

    pass

  def Validate(self):

    pass

  def SetName(self,name):

    if type(name) != str:
      return Error("Resolver names may only be strings")
    if self.name != None:
      return Error("Resolver is already named")
    self.name = name

  def GetName(self):

    return self.name

  def Resolve(self,name):

    return None

  #
  # Python wrappers
  #

  def __init__(self,name = None):

    self.Live(name)

  def __str__(self):

    return str(self.name)

  def __repr__(self):

    return str(self)