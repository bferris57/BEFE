#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: node.py - Objects for a general purpose hierarchy
#
# Hierarchies consists of Nodes related as such...
#
#        ┌──────�?
#   ...◄─┤ Node ├►──◄─ Sibling chain
#        │      │
#        └┬─────┘
#         ▼ Parent
#         │ chain
#         ├─◄──────────┬────────────┬──────────────�?
#         │            │            │              │
#         ▲            ▲            ▲              ▲
#        ┌┴─────�?     ┌┴─────�?     ┌┴─────�?       ┌┴─────�?
#        │ Node ├►──◄─┤ Node ├►──◄─┤ Node ├►─...◄─┤ Node ├► None
#        │      │◄─�?  │      │◄─�?  │      │◄─�?    │      │◄─�?
#        └┬─────┘     └┬─────┘     └┬─────┘       └┬─────┘
#         ▼            ▼            ▼              ▼
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

from ids   import *
from error import *
from list  import *

#
# Module globals
#

NodeTypes = []

#-----------------------------------------------------------------------
#
# IsNode() - Is python object a valid Node object?
#

def IsNode(e):

  try:
    if not IsObject(e):
      return False
    typeid = Type(e)
    return typeid in (Id_TypeNode,Id_TypeNodeFile)
  except:
    raise
    return False

#-----------------------------------------------------------------------
#
# Class: Node - General hierarchy member
#
# The Node class implements a member of a hierarchy.
#
#
# Members: parent
#          prevsibling
#          nextsibling
#          firstchild
#          name
#          title
#
# Methods: Node.GetName        - Get node name (if any)
#          Node.GetTitle       - Get display title (name if none)
#          Node.SetTitle       - Set display title
#          Node.GetAttribute   - Get a named attribute
#          Node.SetAttribute   - Set a named attribute
#          Node.GetAttributes  - Get array of attribute names
#          Node.GetContent     - Get node's content object
#          Node.SetContent     - Set node's content object
#
#          Node.IsDescendentOf - Is Node a descendent of another Node?
#          Node.IsSiblingOf    - Is Node a sibling of another Node?
#          Node.GetContent     - Get content object
#          Node.SetContent     - Set content object
#
#          Node.GetParent      - Get parent node
#          Node.SetParent      - Set parent node
#          Node.Open           - "Open" the node
#          Node.Close          - "Close" the node
#          Node.GetFirstChild  - Get first child node
#          Node.GetLastChild   - Get last child node
#          Node.GetChildren    - Get tuple of all children
#          Node.GetRoot        - Get root node (up parents until parent = Null)
#          Node.PathToRoot     - Get List of nodes up to root node
#          Node.GetNextSibling - Get next sibling node with same parent
#          Node.GetPrevSibling - Get next sibling node with same parent
#          List.GetLength      - Get current number of children
#          List.GetChild       - Get Nth child
#          List.SetChild       - Set Nth child
#          List.AppendChild    - Append child
#
# Python:  __init__
#          __str__
#          __repr__
#          __getitem__
#          __setitem__
#          append
#
#
# Notes: Node.Open() and Node.Close() are used for node hierarchys that
#        are external or dynamic (like file systems) where keeping the
#        child nodes hanging around isn't required or desired.
#
#        In these cases, using Open() instead of GetFirstChild() and
#        and using Close() instead of GetParent() is prefereable because
#        the hierarchy is given the chance to free up the child nodes.
#
#        This means that if you want to get rid of a persistent Node
#        hierarchy you'll have to call GetRoot().Die() to make all the
#        objects disappear.
#
#-----------------------------------------------------------------------

class Node(List):

  @classmethod
  def New(cls,id=None,typeid=None):

    obj = cls.__new__(cls,id=id,typeid=typeid)
    return obj

  def Construct(self,typeid = None,id=None):

    global NodeTypes

    if typeid == None:
      typeid = Id_TypeNode
    if typeid not in NodeTypes:
      NodeTypes.append(typeid)

    List.Construct(self,typeid=typeid,id=id)
    Members(self)['list'] = None

    Members(self)['parent']      = None
    Members(self)['parent']      = None
    Members(self)['prevsibling'] = None
    Members(self)['nextsibling'] = None
    Members(self)['firstchild']  = None
    Members(self)['content']     = None

  def Die(self,deep=True):

    self.Validate()

    children = self.GetChildren()
    for child in children:
      child.SetParent(None)
    if deep:
      for child in children:
        child.Die(deep)

    try:
      self.SetParent(None)
      if self.content == None:
        pass
    except:
      pass

    List.Die(self)

    # Recursively remove from parent (should keep the stack locally
    # but what the hey!)

  #
  # Node.Validate()
  #

  def Validate(self):

    List.Validate(self)

    if self.parent != None and not IsNode(self.parent):
      raise Error("Internal Error: Node parent '%s' isn't a Node"%repr(self.parent))

    if self.nextsibling != None:
      if self.nextsibling == self:
        raise Error("Internal Error: Next sibling is self!")
      if not IsNode(self.nextsibling):
        raise Error("Internal Error: Next sibling isn't a Node")
      if self.nextsibling.prevsibling != self:
        raise Error("Internal Error: Next sibling Node's previous sibling isn't self!")
      if self.nextsibling.parent != self.parent:
        raise Error("Internal Error: Next sibling Node's parent isn't self's parent!")
    if self.prevsibling != None:
      if self.prevsibling == self:
        raise Error("Internal Error: Previous sibling is self!")
      if not IsNode(self.prevsibling):
        raise Error("Internal Error: Previous sibling isn't a Node")
      if self.prevsibling.nextsibling != self:
        raise Error("Internal Error: Previous sibling Node's next sibling isn't self!")
      if self.prevsibling.parent != self.parent:
        raise Error("Internal Error: Previous sibling Node's parent isn't self's parent!")
    elif IsObject(self.parent):
      if self.parent.firstchild != self:
        raise Error("Internal Error: Parent Node's first child isn't self!")

    return

  #
  # Node.GetName       - Get node name (if any)
  # Node.GetTitle      - Get display title
  # Node.SetTitle      - Set display title
  # Node.GetAttribute  - Get a named attribute
  # Node.SetAttribute  - Set a named attribute
  # Node.GetAttributes - Get a list of named attributes
  #
  # Note: GetName is implemented for a "read only" name, i.e. it can only be
  #       set by sub-types of Node.  We did this because some types of node
  #       don't have a mutable name (like a file's name) without further
  #       checking for uniqueness within parent, etc.  But we don't want to
  #       impose that restriction on the sub-type implementation.  Instead,
  #       we'll give them an 'in' to do what's right.
  #
  #       GetTitle is implemented to allow the user to change the display name
  #       if they want.

  def GetName(self):

    try:
      name = self.name
    except:
      name = Id(self)
    return name

  def GetTitle(self):

    try:
      title = self.title
    except:
      title = ''
    if len(title) == 0:
      title = self.GetName()
    return title

  def SetTitle(self,title):

    if title == None:
      try:
        del self.title
      except:
        pass
    else:
      if type(title) != str:
        raise Error("Expected a string title")
      self.title = title

    return

  def GetAttribute(self,name):

    try:
      dict = self.attributes
      value = dict[name]
    except:
      value = None
    return value

  def SetAttribute(self,name,value):

    try:
      dict = self.attributes
    except:
      dict = {}
    dict[name] = value
    Members(self)['attributes'] = dict

  def GetAttributes(self):

    try:
      dict = self.attributes
      keys = list(dict.keys())
    except:
      keys = []
    return keys

  #
  # Node.IsDescendentOf()
  #

  def IsDescendentOf(self,parent):

    self.Validate()

    parent = Id(parent)
    if parent == None:
      pass
    elif not IsNode(parent):
      return Error("Expected Node in parent search")

    this = Id(self)
    searched = []
    while this != None:
      if this == parent:
        break
      elif not IsNode(this):
        return Error("Encountered non-Node in parent search")
      searched.append(this)
      if this in searched:
        return Error("Circular parents")
      this = this.GetParent()
    return this == parent

  #
  # Node.GetParent()
  #

  def GetParent(self):

    # Sanity checks
    self.Validate()

    return Id(self.parent)

  #
  # Node.SetParent(parent)
  #

  def SetParent(self,parent=None):

    if self == None:
      return

    if parent and not IsNode(parent):
      return Error("Attempt to set non-Node as a parent")
    if parent == self.parent:
      return None

    # Sanity checks
    self.Validate()

    # Remove from current parent and sibling chains
    if self.parent != None:
      if self.parent.firstchild == self:
        self.parent.firstchild = self.nextsibling
      self.parent = None
    if self.prevsibling != None:
      self.prevsibling.nextsibling = self.nextsibling
      self.prevsibling = None
    if self.nextsibling != None:
      self.nextsibling.prevsibling = self.prevsibling
      self.nextsibling = None


    return


  #
  # Node.Open          - Open first child
  # Node.Close         - Close open Node
  # Node.GetFirstChild - Get first child node
  # Node.GetLastChild  - Get last child node
  # Node.GetChildren   - Get tuple of children nodes
  #

  def Open(self):

    return None

  def Close(self):

    return None

  def GetFirstChild(self):

    self.Validate()
    return self.firstchild

  def GetLastChild(self):

    self.Validate()
    cur = self.GetFirstChild()
    while cur:
      next = cur.GetNextSibling()
      if next == None:
        break
      cur = next
    return cur

  def GetChildren(self):

    self.Validate()
    children = []
    child = self.firstchild
    while IsObject(child):
      children.append(child)
      child = child.nextsibling
    return tuple(children)

  #
  # Node.GetRoot()
  #

  def GetRoot(self,pathlist=False):

    self.Validate()

    visited = [self]
    while True:
      parent = Python(visited[-1]).parent
      if parent == None:
        break
      if parent in visited:
        for i in range(0,len(visited)):
          visited[i] = str(visited[i])
        visited = '->'.join(visited)
        raise Error("Circular parentage:",visited)
      visited.append(parent)

    if pathlist != False:
      return list(visited)
    else:
      return visited[-1]

  #
  # Node.GetRoot()
  #

  def PathToRoot(self,pathlist=False):

    path = self.GetRoot(pathlist=True)
    if pathlist:
      return path
    else:
      return tuple(path)

  #
  # Node.GetChildIndex()
  #

  def GetChildIndex(self):

    self.Validate()

    if self.parent == None:
      return None
    index = -1
    node = self
    while node != None:
      node = node.prevsibling
      index = index + 1
    return index

  #
  # Node.GetNextSibling - Get next sibling Node
  #

  def GetNextSibling(self):

    self.Validate()

    return self.nextsibling

  #
  # Node.GetPrevSibling - Get next sibling node with same parent
  #

  def GetPrevSibling(self):

    self.Validate()

    return self.prevsibling

  # List.GetLength - Calculate number of children
  # List.GetItem   - Get Nth child
  # List.SetItem   - Set Nth child (not implemented)

  def GetLength(self):

    self.Validate()

    len = 0
    node = self.firstchild
    while node != None:
      node = node.nextsibling
      len = len+1
    return len

  def GetItem(self,index):

    self.Validate()
    index = self.resolveindex(index)
    len = 0
    node = self.firstchild
    while node != None:
      if len == index:
        break
      node = node.nextsibling
      len = len+1
    return node

  def SetItem(self,index,value):

    if type(index) == str:
      self.SetAttribute(index,value)
    else:
      raise Error("Node.SetItem not implemented, can only append or prepend")

  def AppendItem(self,node):

    self.Validate()
    node.Validate()
    node.SetParent(None)

    # Find last node
    lastnode = self.firstchild
    while lastnode != None and lastnode.nextsibling != None:
      lastnode = lastnode.nextsibling

    # If no children, it's the first child...
    if lastnode == None:
      self.firstchild = node
    # Else, append to last child
    else:
      lastnode.nextsibling = node
      node.prevsibling = lastnode
    # Set the node's new parent
    node.parent = self

  #
  # Node.GetContents - Return children as an object (list, obj, etc.)
  #

  def GetContents(self):

    self.Validate()
    return None

  #
  # Python wrappers
  #

  def __init__(self,id=None,typeid=None):

    self.Construct(id=id,typeid=typeid)

  def __str__(self):
    try:
      return "Node "+str(self.id)
    except:
      return "Node @ 0x%08x"%id(self)

  def __repr__(self):
    try:
      if self.id != None:
        return str(self)
      else:
        return "Unborn <Node> @ 0x%08x"%id(self)
    except:
      return "Dead <Node>"

  def append(self,value):

    self.AppendItem(value)

  def __getitem__(self,index):

    if type(index) == str:
      value = self.GetAttribute(index)
    else:
      value = self.GetItem(index)
    return value

  def __setitem__(self,index,value):

    self.SetItem(index,value)
    return

def main(argv=[]):

  print("Testing Node...")
  indent = '  '
  parent = Node()
  child1 = Node()
  child2 = Node()
  parent.append(child1)
  parent.append(child2)
  child2p1 = Node()
  child2p2 = Node()
  print(indent+"parent   = "+repr(parent)   + " (" + str(Type(parent))   + ")")
  print(indent+"child1   = "+repr(child1)   + " (" + str(Type(child1))   + ")")
  print(indent+"child2   = "+repr(child2)   + " (" + str(Type(child2))   + ")")
  print(indent+"child2p1 = "+repr(child2p1) + " (" + str(Type(child2p1)) + ")")
  print(indent+"child2p2 = "+repr(child2p2) + " (" + str(Type(child2p2)) + ")")
  child2.append(child2p1)
  child2.append(child2p2)
  p2r = child2p2.PathToRoot()
  print(indent+"p2r = "+repr(p2r))

  # Delete each one now
  for obj in [parent]:
    obj.Die()

if __name__ == '__main__':

  main()
