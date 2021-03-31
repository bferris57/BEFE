#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: nodewalker.py - General purpose node walking
#
# Classes: NodeWalker
#          NodeNameWalker
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

from ids    import *
from error  import *
from object import *
from node   import *

#-----------------------------------------------------------------------
#
# IsNodeWalker() - Is python object a valid NodeWalker object?
#

def IsNodeWalker(e):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return (IsId(typeid) and typeid == Id_TypeNodeWalker)
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: NodeWalker - General hierarchy member
#
# The Node class implements a hierarchy walker.  The position entries
# are presented as a tuple in the following format:
#
#   (before,id,name,after)
#
#   Where: before = Number of sibling nodes before the current one
#          id     - The Object Id of the node
#          name   - The Name of the node ('' = None)
#          after  - Number of sibling nodes after the current one
#
# The before, name, and after entries indicate the state of the
# hierarchy level at the time it was first encountered.  This
# means that the real hierarchy could have changed so the node's
# name could be different from last time and the number of
# siblings before/after could have changed so you could get
# negative numbers.  For example, if "before" is -N, that means
# someone removed N previous siblings during the walking.  If
# "after" is -N, that means someone added N siblings during
# the walking.
#
# The reason we keep this strange behaviour is so that we can
# do something reasonable graphically based on the state of the
# child level when we encountered it.
#
# Members: **None**
#
# Methods: Object.Live
#          Object.Die
#          Object.Validate            - Self check
#
#          NodeWalker.SetRoot         - Change root
#          NodeWalker.GetPosition     - Get current position as a tuple
#          NodeWalker.GetFullPosition - Get full stack to current position
#
#          NodeWalker.Up
#          NodeWalker.Down
#          NodeWalker.Next
#          NodeWalker.Previous
#
# Python:  __init__
#          __str__
#          __repr__
#          __getitem__
#          __setitem__
#          append
#
#-----------------------------------------------------------------------

class NodeWalker(Object):

  def Live(self,root=None):

    Object.Live(self,Id_TypeNodeWalker)

    self.root = root
    self.stack = []
    self.Validate()
    entry = [0,root,0]
    self.stack = [entry]

    root.Open()

  def Die(self):

    if self.root:
      self.root.Close()
    self.root   = None
    self.stack  = []
    Node.Die(self)

  def Validate(self):

    if self.root == None:
      raise Error("Invalid NodeWalkerRoot specified")
    if type(self.stack) != list:
      raise Error("Stack is not a list")

  def SetRoot(self,root):


    self.root = root
    entry = [0,root,0]
    self.stack = []
    self.stack.append(entry)
    self.Validate()

  def GetPosition(self):

    before,obj,after = self.stack[-1]

    return (before,Id(obj),obj.GetName(),after)

  def GetFullPosition(self):
    entries = []
    for entry in self.stack:
      entries.append(entry)
    return tuple(entries)

  def Up(self):

    if len(self.stack) == 1:
      return None
    entry = self.stack[-1]
    before,obj,after = entry
    obj.Close()
    del self.stack[-1]
    before,obj,after = entry
    obj.Close()
    entry = (before,obj,nc-1)
    return entry

  def Down(self):

    before,obj,after = self.stack[-1]
    fc = obj.Open()
    nc = obj.GetLength()
    if fc == None:
      return None
    obj = fc
    entry = (0,obj,nc-1)
    self.stack.append(entry)
    return entry

  def Next(self):

    before,obj,after = self.stack[-1]
    obj = obj.GetNextSibling()
    if obj == None:
      return None
    before = before+1
    after = after-1
    entry = (before,obj,after)
    self.stack[-1] = entry
    return entry

  def Previous(self):

    before,obj,after = self.stack[-1]
    obj = obj.GetPreviousSibling()
    if obj == None:
      return None
    before = before - 1
    after = after+1
    entry = (before,obj,after)
    self.stack[-1] = entry
    return entry

  def __init__(self,root=None):

    self.Live(root)

if __name__ == '__main__':

  from nodefile import *

  root = NodeFileRoot()
  walker = NodeWalker(root)
  entry = walker.Down()  # Should go to C:
  entry = walker.Next()  # Should go to D:
  entry = walker.Down()  # Should go to D:/Recycle bin
  entry = walker.Next()  # Should go to D:/backups
  entry = walker.Next()  # Should go to d:/code
  entry = walker.Down()  # Should go to d:/code/Documentation
  entry = walker.Next()  # Should go to d:/code/...
  entry = walker.Next()  # Should go to d:/code/...
  entry = walker.Next()  # Should go to d:/code/projects
  entry = walker.Down()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/...
  entry = walker.Next()  # Should go to d:/code/projects/org
  entry = walker.Down()  # Should go to d:/code/projects/org/contents
  entry = walker.Next()  # Should go to d:/code/projects/org/encode.py
  before,obj,after = entry
  src = obj['src']

  print("nodewalker: src =",src)

  full = walker.GetFullPosition()
  print("          :",full[0])
  for entry in full[1:]:
    print("          :",entry)
