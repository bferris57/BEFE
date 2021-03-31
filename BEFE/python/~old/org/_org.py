#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: org.py - General organisation of things
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

from ids   import *
from error import *
from node  import *

#-----------------------------------------------------------------------
#
# IsOrganisation() - Is python object a valid Organisation object?
#

def IsOrganisation(e):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return IsId(typeid) and typeid == Id_TypeNode
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: Org
#
# An Org is the root node of an entire organised structure.
#
# Organisation is done in a hierarchy.  Each node in an organisation
# hierarchy is one of the following types or subtypes:
#
# Members: resolver   - Our resolver
#          typesystem - Our type system
#          nodes      - List of known nodes
#          types      - List of type system orgs
#
#
# Methods: Object.Live
#          Object.Die
#          Object.Validate       - Self check
#
#
# Python:  __init__
#          __str__
#          __repr__
#
#-----------------------------------------------------------------------

class Organisation(Node):

  def Live(self):

    List.Live(self)
    self.type = Id_TypeOrganisation
    del self.list
    self.parent      = None
    self.prevsibling = None
    self.nextsibling = None
    self.firstchild  = None
    self.content     = None

  def Die(self):

    Object.Die(self)
    try:
      self.SetParent(None)
      self.content == None:
    except:
      pass

  #
  # Node.Validate()
  #

  def Validate(self):

    List.Validate(self)

    if self.parent != None and not IsNode(self.parent):
      raise Error("Internal Error: Node parent isn't a Node")

    if self.nextsibling != None:
      if not IsNode(self.nextsibling):
        raise Error("Internal Error: Next sibling isn't a Node")
      if self.nextsibling.prevsibling != self:
        raise Error("Internal Error: Next sibling Node's previous sibling isn't self!")
      if self.nextsibling.parent != self.parent:
        raise Error("Internal Error: Next sibling Node's parent isn't self's parent!")

    if self.prevsibling != None:
      if not IsNode(self.prevsibling):
        raise Error("Internal Error: Previous sibling isn't a Node")
      if self.prevsibling.nextsibling != self:
        raise Error("Internal Error: Previous sibling Node's next sibling isn't self!")
      if self.prevsibling.parent != self.parent:
        raise Error("Internal Error: Previous sibling Node's parent isn't self's parent!")
    elif self.parent != None:
      if self.parent.firstchild != self:
        raise Error("Internal Error: Parent Node's first child isn't self!")

    return

