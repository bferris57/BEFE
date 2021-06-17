#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:       BEFE_Tree - BEFE Tree and related classes/code
#
# Classes:    Tree       = Tree (contains TreeNodes)
#             TreeNode   - Tree Node
#             TreeWalker - Walk a tree
#
# Generators: TopDownLeftRight - Walk an entire tree generating TreeNodes
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_Struct import *
from BEFE_Stack  import *
from BEFE_errors import *

# DEBUG...
from BEFE_debug  import *
# ...DEBUG

class Tree(Struct):

  members = ["nodes","heads"]
               
  def __init__(self):
  
    Struct.__init__(self,';'.join(Tree.members))

    self.nodes    = []
    self.heads    = []

  def find(self,node):
    if type(node) == int:
      if node < 0: node = node+len(self.nodes)
      if node < 0 or node >= len(self.nodes) or not isinstance(self.nodes[node],TreeNode):
        return None
      return node
    try:
      pos = self.nodes.index(node)
    except ValueError:
      pos = None
    return pos

  #@accepts(Parse,ParseNode)
  def insert(self,node):
  
    pos = self.find(node)
    if pos is not None: return pos
    
    # Sanity checks...
    if not isinstance(node,TreeNode):
      raise ValueError("Expected instance of ParseNode")

    # Do it...
    pos = len(self.nodes)
    self.nodes.append(node)
    node.tree  = self
    if node.parent is None:
      self.heads.append(node)
    
    return pos

  def delete(self,node):
  
    pos = self.find(node)
    if pos is None:
      raise ValueError("Head TreeNode does not exist")
    # DEBUG...
    print("Tree.delete: pos = %d, heads = %d"%(pos,len(self.heads)))
    # ...DEBUG
    del self.heads[pos]
    
  def treeHeads(self):
  
    h = []
    for i in range(0,len(self.nodes)):
      if isinstance(self.nodes[i],TreeNode) and self.nodes[i].parent is None:
        h.append(self.nodes[i])
    return h
    
  def __getitem__(self,key):
  
    if type(key) != int:
      raise ValueError("Expected int index")
    if key < 0:
      key = len(self.nodes)+key
    if key < 0 or key >= len(self.nodes):
      raise valueError("Invalid index %d, should be 0..%d"%(key,len(self.nodes)-1))
    return self.nodes[key]
    
  def __getitem__(self,key):
  
    if type(key) is not int:
      return None
    if key < 0:
      key = len(self.nodes)+key
    if key < 0 or key >= len(self.nodes):
      return None
    return self.nodes[key]

  def __str__(self):

    return str(len(self.nodes))+' nodes'

  def __len__(self):

    return len(self.nodes)
    
  def newTreeNode(self):
  
    n = TreeNode(self)
    return n
    
  def numNodes(self):
  
    return len(self.nodes)
    
class TreeNode(Struct):

  members = ["tree",
             "parent",
             "children",
            ]

  def __init__(self,tree=None):
                   
    Struct.__init__(self,';'.join(TreeNode.members))

    self.initialise(tree=tree)

  def initialise(self,tree=None): 
  
    # Sanity checks
    if tree is not None and not isinstance(tree,Tree):
      raise ValueError("Expected tree to be None or a Tree instance")
      
    self.tree   = tree
    self.parent = None
    self.children = []
    if tree is not None:
      self.tree.nodes.append(self)
      self.tree.heads.append(self)
  
  def removeFromParent(self):

    if self.parent is None:
      return

    try:
      childPos = self.parent.children.index(self)
    except:
      raise InternalError("TreeNode isn't in its parent.children")
    del self.parent.children[childPos]
    self.parent = None
    self.tree.heads.append(self)
      
  def removeFromTree(self):
  
    if self.tree is None:
      raise ValueError("TreeNode not in Tree")
    nodePos = self.tree.find(self)
    if nodePos == None:
      raise InternalError("TreeNode not in tree, can't remove")
      
    if self.parent is None:
      headPos = None
      try:
        headPos = self.tree.heads.index[node]
      except:
        pass
        #raise InternalError("Node has no parent but isn't in tree.heads")
      if headPos is not None:
        del self.tree.heads[headPos]
    else:
      self.removeFromParent()

    node = self.tree.nodes[nodePos]
    
    # Remove all children from tree as well...
    stack = Stack()
    stack.push(node)
    while stack:
      if stack.TOS().children:
        stack.push(stack.TOS().children[0])
      else:
        if len(stack) > 1:
          stack.TOS().removeFromTree()
        stack.pop()
  
    del self.tree.nodes[nodePos]  
        
  def isInTree(self,tree):
  
    # Sanity checks
    if not isinstance(tree,Tree):
      raise ValueError("Expected tree argument to be a Tree")
      
    return tree.find(self) is not None
    
  def isHead(self):
  
    return self.parent is None
    
  def setParent(self,parent):

    tree = self.tree
    if tree is None or not isinstance(tree,Tree):
      raise ValueError("Cannot set parent without tree of type Tree")
    
    # Sanity checks on node...
    if self.tree is None:
      raise InternalError("Not in tree")
    node = self
        
    # Sanity checks on parent...
    if parent is not None:
      if not isinstance(parent,TreeNode):
        raise ValueError("Expected parent argument to be a TreeNode")
      if parent.tree is not tree:
        raise ValueError("Parent not in same Tree")
    
    # Remove node from current parent...
    if node.parent is not None: # Not None, remove from parent's list of children...
      try:
        pos = self.parent.children.index(self)
      except:
        raise InternalError("TreeNode not in parent.children")
      del node.parent.children[pos]
      node.parent = None
    else: # Is None, Should be in tree.heads, remove it...
      try:
        pos = tree.heads.index(node)
      except:
        raise InternalError("Expected node to be in tree.heads, it wasn't")
      del tree.heads[pos]
    
    # Add it to current parent
    node.parent = parent
    if parent is None:
      if self not in tree.heads:
        tree.heads.append(self)
      else:
        raise InternalError("parent is None and TreeNode is already in tree.heads")
    else:
      parent.children.append(node)
    
    return
    
  def depth(self):
    
    dep = 0
    curNode = self
    # DEBUG...
    parents = []
    # ...DEBUG
    while curNode is not None and curNode.parent != None:
      curNode = curNode.parent
      # DEBUG...
      if curNode in parents:
        raise InternalError("TreeNode.depth: curNode[%d] is already in parents"%repr(self.tree.find(curNode)))
      # ...DEBUG
      dep += 1
    return dep
  
  def find(self,that):
 
    if isinstance(that,TreeNode):
      try:
        pos = self.children.index(that)
      except:
        pos = None
        
    elif type(that) == int:
    
      if that < 0:
        that = len(self.children)+that
      if that < 0 or that >= len(self.children):
        pos = None
      else:
        pos = self.children(that)
      
    return pos

  #
  # Method: findChild - Find a given child's children index
  #
  
  def findChild(self,that):

    try:
      idx = self.children.index(that)
    except:
      idx = None
      
    return idx
    
  #
  # Method: insertAfterChild - Insert a TreeNode at a after a given child index
  #
  
  def insertAfterChild(self,that,idx):
  
    if type(idx) is not int:
      raise ValueError("Expected 'int' idx parameter")
    if not isinstance(that,TreeNode):
      raise ValueError("Expected 'TreeNode' that parameter")
    if idx < 0:
      idx %= len(self.children)
    if idx < 0 or idx > len(self.children):
      raise ValueError("idx %d invalid"%idx)
    # We're now the parent, which places that at end of our children...
    that.setParent(self)
    # Put in proper place now...
    self.children.insert(idx+1,that)
    # Remove from end of children
    del self.children[-1]

  #
  # Method: insertBeforeChild - Insert a TreeNode at a before a given child index
  #
  
  def insertBeforeChild(self,that,idx):
  
    if type(idx) is not int:
      raise ValueError("Expected 'int' idx parameter")
    if not isinstance(that,TreeNode):
      raise ValueError("Expected 'TreeNode' that parameter")
    if idx < 0:
      idx %= len(self.children)
    if idx < 0 or idx > len(self.children):
      raise ValueError("idx %d invalid"%idx)
    # We're now the parent, which places that at end of our children...
    that.setParent(self)
    # Put in proper place now...
    self.children.insert(idx,that)
    # Remove from end of children
    del self.children[-1]

  #
  # Method: __str__ - Convert a TreeNode to a string
  #
  
  def __str__(self):
  
    s =  'parent='+str(self.tree.find(self.parent))
    i = 0
    children = ''
    while i < len(self.children):
      if children: children += ','
      children += str(self.tree.find(self.children[i]))
      i += 1
    s += ',children=['+children+']'
    
    return s
    
class TreeWalker:

  def __init__(self,tree,*args):
  
    self.tree = tree
    self.root = None
    if not isinstance(tree,Tree):
      raise ValueError("Expected tree to be a Tree instance")
      
    if len(args) <= 0:
      heads = tree.heads()
      if len(heads) > 0:
        self.curNode = heads[0]
      else:
        self.curNode = None
    else:
      self.curNode = args[0]
    if not isinstance(self.curNode,TreeNode):
      raise ValueError("Expected TreeNode starting position")
    
  def setPos(self,node):

    if not isinstance(node,TreeNode):
      raise ValueError("Expected TreeNode position")
      
    self.tree = node.tree
    self.curNode = node
    
  def isValidPos(self):
  
    return False if self.tree is None or not isinstance(self.curNode,TreeNode) else True
    
  def current(self):
    if not self.isValidPos():
      return None
    return self.curNode
    
  def up(self):
    curNode = self.current()
    self.curNode = curNode.parent
    return self.curNode
    
  def down(self):
    
    curNode = self.current()
    if len(curNode.children) <= 0:
      return None
    self.curNode = curNode.children[0]
    return self.curNode
    
  def left(self):
  
    curNode = self.current()
    parent = curNode.parent
    if parent is None:
      pos = self.tree.heads.index(self)
    else:
      pos = parent.children.index(self.pos)
    if pos <= 0:
      return None;
    if parent is None:
      self.curNode = self.tree.heads[pos-1]
    else:
      self.curNode = parent.children[pos-1]
    return self.curNode
    
  def right(self):
  
    curNode = self.current()
    if curNode.parent == None:
      return None
    parent = curNode.parent
    if parent is None:
      pos = self.tree.heads.index(curNode)
      if pos >= len(tree.heads) - 1:
        return None
      self.curNode = self.tree.heads[pos+1]
    else:
      pos = parent.children.index(curNode)
      if pos >= len(parent.children) - 1:
        return None
      self.curNode = parent.children[pos+1]
    return self.current()

#
# Generator: TopDownLeftRight - Walk a Tree/TreeNode top, down, left, and right
#

def TopDownLeftRight(treeOrNode):

  # Start off depending on if Tree or TreeNode was passed...
  if isinstance(treeOrNode,TreeNode):
    heads = [treeOrNode]
    tree = treeOrNode.tree
  elif isinstance(treeOrNode,Tree):
    tree = treeOrNode
    heads = tree.treeHeads()
  else:
    raise ValueError("Expected TreeNode or Tree as argument")
    
  # Do it...
  # DEBUG...
  if debug:
    nodes = []
  #if debug:
  #  s = ''
  #  for head in heads:
  #    if s: s += ' '
  #    s += '[' + repr(tree.find(head)) + ']'
  #  print("TopDownLeftRight: heads = "+s)
  # ...DEBUG
  for head in heads:
    walker = TreeWalker(tree,head)
    cur = walker.current()
    while cur != None:
      yield cur
      cur = walker.down()
      # DEBUG...
      if debug and cur is not None:
        if cur in nodes:
          raise InternalError("TopDownLeftRight: down node[%s] already in nodes"%repr(tree.find(cur)))
        nodes.append(cur)
      # ...DEBUG
      if cur is not None: continue
      # This was missing...
      if cur is head: break
      # ...This was missing
      while cur is None:
        cur = walker.right()
        # DEBUG...
        if debug and cur is not None:
          if cur in nodes:
            raise InternalError("TopDownLeftRight: right 1 node[%s] already in nodes"%repr(tree.find(cur)))
          nodes.append(cur)
        # ...DEBUG
        if cur is not None: break
        cur = walker.up()
        if cur is None:              break
        if cur is head:              break
        cur = walker.right()
        # DEBUG...
        if debug and cur is not None:
          if cur in nodes:
            raise InternalError("TopDownLeftRight: right 2 node[%s] already in nodes"%repr(tree.find(cur)))
          nodes.append(cur)
        # ...DEBUG
        if cur is not None: break
        continue
      if cur == None or cur is head: break
          
if __name__ == '__main__':

  tree = Tree()
  node0 = TreeNode()
  tree.insert(node0)
  node1 = TreeNode()
  tree.insert(node1)
  node1.setParent(node0)
  node2 = TreeNode()
  tree.insert(node2)
  node2.setParent(node0)
  node3 = TreeNode()
  tree.insert(node3)
  node4 = TreeNode()
  tree.insert(node4)
  node4.setParent(node3)
  node5 = TreeNode()
  tree.insert(node5)
  node5.setParent(node3)
  node6 = TreeNode()
  tree.insert(node6)
  node6.setParent(node5)
  
  print("Tree...")
  for i in range(0,len(tree)):
    node = tree[i]
    print("  [%d]: %s (depth=%d)"%(tree.find(node),str(node),node.depth()))
  print("Tree = "+str(tree))
  h = ''
  for node in tree.treeHeads():
    if h: h += ','
    h += str(tree.find(node))
  print("Heads = ["+h+"]")
  
  print("TreeWalker 1 (entire tree)...")
  for node in TopDownLeftRight(tree):
    indent = '  '*(node.depth()+1)
    print(indent+"[%d]: %s"%(tree.find(node),str(node)))

  print("TreeWalker 2 (node 0)...")
  for node in TopDownLeftRight(node0):
    indent = '  '*(node.depth()+1)
    print(indent+"[%d]: %s"%(tree.find(node),str(node)))

  print("TreeWalker 3 (node 5)...")
  for node in TopDownLeftRight(node5):
    indent = '  '*(node.depth()+1)
    print(indent+"[%d]: %s"%(tree.find(node),str(node)))

  print("TreeWalker 4 (node 3)...")
  for node in TopDownLeftRight(node3):
    indent = '  '*(node.depth()+1)
    print(indent+"[%d]: %s"%(tree.find(node),str(node)))
