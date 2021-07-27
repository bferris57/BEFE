from ids import *
from object import *
from error import *

#-----------------------------------------------------------------------
#
# IsSlice() - Is object a valid Slice object?
#

def IsSlice(e):

  try:
    if not IsObject(e):
      return False
    typeid = e.GetType()
    return IsId(typeid) and typeid == Id_TypeSlice
  except:
    return False

#-----------------------------------------------------------------------
#
# Class: Slice - General slicing object
#
# Members: start
#          stop
#          step
#
# Methods: Normalise - Returned normalised slice based on length
#
#
#-----------------------------------------------------------------------

class Slice(Object):

  def __init__(self,start=None,stop=None,step=None,len=None):

    Object.__init__(self)

    if type(start) in (Slice,slice) and stop == None and step == None:
      start = start.start
      stop  = start.stop
      step  = start.step
    if type(start) not in (int,type(None)): raise Error("Slice.start must be an integer or None")
    if type(stop)  not in (int,type(None)): raise Error("Slice.stop must be an integer or None")
    if type(step)  not in (int,type(None)): raise Error("Slice.step must be an integer or None")

    self.start = start
    self.stop  = stop
    self.step  = step
    if type(len) != int and len != None: raise Error("Slice.Normalise requires an integer length")
    if len:
      self.Normalise(len)

  def Normalise(self,len=None):

    if len == None:
      return

    if type(len) != int: raise Error("Slice.Normalise requires an integer length")
    if len < 0:          raise Error("Slice.Normalise requires a positive or zero length")

    if self.start == None: self.start = 0
    if self.stop == None:  self.stop  = len
    if self.start < 0:     self.start = len+self.start
    if self.stop  < 0:     self.stop  = len+self.stop
    if self.step == None:
      if self.stop < self.start:
        self.step = -1
      else:
        self.step = 1
    if self.start > len:
      self.start = len
    if self.stop > len:
      self.top = len

  #
  # Python wrappers
  #

  def __str__(self):

    return '[%s:%s:%s]'%(repr(self.start),repr(self.stop),repr(self.step))

  def __repr__(self):

    return str(self)

#-----------------------------------------------------------------------
#
# MAIN PROGRAM if run as script
#
#-----------------------------------------------------------------------

if __name__ == '__main__':

  indent = ''
  print(indent+"Testing Slice...")
  indent = indent+'  '

  s = Slice()
  print(indent+"Slice()             =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)

  s = Slice(10,None,None)
  print(indent+"Slice(10,None,None) =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)
  s = Slice(None,10,None)
  print(indent+"Slice(None,10,None) =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)
  s = Slice(None,None,10)
  print(indent+"Slice(None,None,10) =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)

  s = Slice(-1,10)
  print(indent+"Slice(-1,10)        =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)
  s = Slice(-1,10)
  print(indent+"Slice(-1,10)        =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)
  s = Slice(-1,-3)
  print(indent+"Slice(-1,-3)        =",s)
  s.Normalise(7)
  print(indent+"      Normalised(7) =",s)
