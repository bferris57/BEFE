from ids   import *
from node  import *
from error import *

#-----------------------------------------------------------------------
#
# IsType() - Is python object a valid Type object?
#

def IsType(t):

  try:
    if not IsObject(t):
      return False
    typeid = t.GetType()
    return IsId(typeid) and typeid == Id_TypeType
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: TypeType - General Type object
#
# Members: name
#
# Methods: Node.Construct
#          Node.Die
#          Node.Validate       - Self check
#
#-----------------------------------------------------------------------

class TypeType(Node):

  @classmethod
  def New(cls,id=None,typeid=None):

    obj = cls.__new__(cls,id=id,typeid=typeid)
    obj.Construct(id=id,typeid=typeid)
    return obj

  #
  # Node methods
  #

  def Construct(self,typeid = None,id = None):

    if typeid == None:
      typeid = Id_TypeNode

    Node.Construct(self,typeid=typeid,id=id)
    Members(self)['name'] = ''

  def Die(self,deep=True):

    self.Validate()
    Node.Die(self,deep)

  def Validate(self):

    Object.Validate(self)
    return


#-----------------------------------------------------------------------
#
# Main program
#

def main():

  print("Test TypeType...")
  indent = '  '

  import ids
  dic = ids.__dict__
  vars = list(dic.keys())
  vars.sort()

  # Figure out biggest Id_... variable name
  maxlen = 0
  for var in vars:
    if var[0:3] == 'Id_':
      maxlen = max(maxlen,len(var))
  print (indent+"Largest Id_ name length = %d"%maxlen)

  # Print them and their values
  for var in vars:
    if var[0:3] == 'Id_':
      print("%s: %s"%(var.ljust(maxlen),repr(dic[var])))
      val = dic[var]
      typ = TypeType.New(id=val,typeid=val)

if __name__ == '__main__':

  main()
