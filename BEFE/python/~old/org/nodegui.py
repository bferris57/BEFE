from tkinter import Tk, Canvas
from idlelib.TreeWidget import TreeItem, TreeNode
import nodefile
from error import *

class NodeGUI(TreeItem):

  def __init__(self, node):
    self.node = node

  def GetText(self):
    return self.node['name']

  def IsExpandable(self):
    node = self.node
    return node != None and len(node) > 0

  def GetSubList(self):
    parent = self.node
    self.node.PopulateChildren()
    children = self.node.GetChildren()
    print("NodeGui has %d children"%len(children))
    node = parent.GetFirstChild()
    print("parent.GetFirstChild() returned:",str(node))
    prelist = []
    for child in children:
      prelist.append(NodeGUI(child))
    return prelist

def MakeNodes():

  return nodefile.NodeFileRoot()

if __name__ == '__main__':

    root = Tk( )
    canvas = Canvas(root)
    canvas.config(bg='white')
    canvas.pack( )
    parent = MakeNodes()
    item = NodeGUI(parent)
    node = TreeNode(canvas, None, item)
    node.update( )
    node.expand( )
    root.mainloop( )