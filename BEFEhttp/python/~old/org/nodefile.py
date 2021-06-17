from node import *
from error import *
import os

NodeFileTypes = []

class NodeFile(Node):

  def Construct(self,typeid=Id_TypeNodeFile):

    global NodeFileTypes
    if typeid == None:              typeid = Id_TypeNodeFile
    if typeid not in NodeFileTypes: NodeTypes.append(typeid)
    Node.Construct(self,typeid)
    Members(self)['name']     = ''
    Members(self)['type']     = ''
    Members(self)['src']      = ''
    Members(self)['fullpath'] = ''

    self.Open()

  def Die(self,deep=True):

    Node.Die(self,deep)

  def Open(self):

    self.PopulateChildren()
    return self.firstchild

  def GetFirstChild(self):

    return self.Open()

  def Close(self):

    self.Die()

  def PopulateChildren(self):

    if self.firstchild != None:
      return

    if self.parent == None:
      return

    if self["type"] not in ("<Disk>","<Directory>"):
      return

    path = list(os.path.split(self.fullpath))
    fullpath = os.sep.join(path)
    try:
      subdirs = os.listdir(os.sep.join(path))
    except:
      subdirs = []
    for file in subdirs:
      if path[-1] == '':
        path[-1] = file
      else:
        path.append(file)
      fullpath = os.sep.join(path)
      if os.path.exists(os.sep.join(path)):
        path = path[0:-1]
        node = NodeFile()
        node.name = file
        node['name'] = node.name
        self.AppendItem(node)
        if os.path.isdir(fullpath):
          node["type"] = "<Directory>"
        else:
          node["type"] = "<File>"
        node.src = node.src + "/" + file
        node.fullpath = fullpath
        node['src'] = node.src
        node['fullpath'] = node.fullpath
    return

  def GetFirstChild(self):
    self.PopulateChildren()
    return self.firstchild

  def __len__(self):
    self.PopulateChildren()
    return Node.__len__(self)

  def __getindex__(self,index):

    if type(index) == str:
      value = self.GetAttribute(name)
    else:
      value = Node.__getindex__(self,index)
    return value

  def __init__(self):

    NodeFile.Construct(self)

def NodeFileRoot():

  drives = 'ABCDEFGHIJKLMNOPQRSTUVWXYZ'

  root = NodeFile()
  root["type"] = "<LocalHost>"
  root["src"] = "file://"
  try:
    name = os.environ['userdomain']
    root.name = name
  except:
    pass
  if not root.GetName():
    root.SetTitle("Local File System")

  for drive in drives:
    fulldrive = drive+':'
    if os.path.exists(drive+':'):
      node = NodeFile()
      root.AppendItem(node)
      node.name = fulldrive
      node.type = '<Disk>'
      node.src  = root.src+'/'+fulldrive
      node.fullpath = fulldrive
      node["type"]     = node.type
      node["src"]      = node.src
      node['name']     = node.name
      node['fullpath'] = node.fullpath
      node.PopulateChildren()

  return root


def main():

  root = NodeFileRoot()
  root.PopulateChildren()
  rootchildren = root.GetChildren()
  print("%d root children..."%len(rootchildren))
  for child in rootchildren:
    child.PopulateChildren()
    print("  '%s' has %d children"%(child.name,len(child)))
  root.Close()

if __name__ == '__main__':

  main()
