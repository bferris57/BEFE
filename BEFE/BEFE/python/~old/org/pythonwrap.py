from object import *
from error  import *

#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: Wrap - Python object wrapper
#
# Functions: Python        - Wrap a python object if need be
#
# Classes:   PythonObject  - A wrapped python object
#
# The PythonObject class is used to wrap python objects so they will
# be compatible with our object model.  There's only need for one class
# because we switch our Type identifier based on the type of python
# object.
#
# It might seem odd to implement this weird class in python but we're
# operating under different assertions...  In our system, all objects
# have an identity and it's not mutable and, further, it's universally
# distinct.  In python they implement an id() function that tells you
# the object's identity but it's mutable.
#
# Take the following python scenario...
#
#   >>> a = 1
#   >>> id(a)
#   505408872
#   >>> a = 2
#   >>> id(a)
#   505408888
#   >>> b = 1
#   >>> id(b)
#   505408872
#   >>> b = b + 1
#   >>> id(b)
#   505408888
#   >>>
#
# This shows that the "object id" in python, is the id of the value
# the object is bound to.  So, given variables 'a' and 'b', their
# identity is mutable.
#
# Further, the id of a type isn't guaranteed across all invokations
# of python or different versions.  For example, on my machine on
# python 3.1.2 I get the following consistently...
#
#    >>> id(int)
#    505211224
#
# With python 2.7 I get the following...
#
#    >>> id(int)
#    505331432
#
# Python never claims that object identity is unique and distinct,
# let alone across invocations, time or universally.
#
# That's why we chose use UUIDs to identity objects.  This means
# that an object's ID is unique across time, invocation, the world,
# and the universe.  This way we can tell that if we hand out a
# reference to an object, when that reference trys to reclaim the
# object and it succeeds, it knows it's the same object.  Of course,
# the side effect is that references may never be reclaimable.
# Tough luck, deal with it!
#
# This also helps us tell the difference between an object and a
# copy of the object, even though they appear to be the same.
#
# In strong defense, we find the notion of treating objects as
# 'physical things' like library books much more useful than treating
# them more abstractly like python and C++ do.  In python, you can't
# tell the difference between 'x' and a copy of 'x'.  In C++, you can
# use address to tell the difference between two objects and but you
# can't tell whether two references refer to the same thing, unless
# you code it (just like you have to code the copying/duplicating).
#
# The distinction of a physical thing vs. a copy/facsimile of the
# original thing is incredibly useful when you're trying to implement
# software working over the internet.
#
# To keep from disappearing up our own metaphysical backsides we
# introduce blatant use of 'identity theft'...  We've assigned UUIDs to
# each metadata object (like type and other basic self descriptions) on
# every system.  This allows us to identify old type system definitions
# from new ones and, hopefully, come up with a way of migrating the old
# definitions into the new form (just like you're not the same 'you'
# as your 'six year old you', but you're close enough so the law
# can figure out what to do with that distinction but, to you, you're
# still 'you', just older.
#
# SO THERE!
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------


_pythonmapping = {bool  : Id_TypePythonBool
                 ,int   : Id_TypePythonInt
                 ,float : Id_TypePythonFloat
                 ,str   : Id_TypePythonString
                 ,tuple : Id_TypePythonTuple
                 ,list  : Id_TypePythonList
                 ,dict  : Id_TypePythonDict
                 }

#-----------------------------------------------------------------------
#
# Function: Wrap   - Wrap a python object if need be
#           Python - "    " "      "      "  "   "
#
# This function is passed a python object and returns an object in our
# object model.  If it's already one of our model implementations, it is
# returned.  If it's not, a python wrapper of type PythonObject is
# wrapped with it's type indicating the type of python object it's
# wrapping.
#
#-----------------------------------------------------------------------

def Wrap(obj):

  if IsObject(obj) or obj == None:
    return obj
  if type(obj) in _pythonmapping:
    return PythonObject(obj,_pythonmapping[type(obj)])
  return PythonObject(obj,Id_TypePythonObject)

def Python(obj):

  return Wrap(obj)

#-----------------------------------------------------------------------
#
# Class: PythonObject - Python object
#
# Members: **None**
#
# Methods: Any python function we need to implement (e.g. __str__, etc.)
#
#-----------------------------------------------------------------------

class PythonObject(Object):

  def __init__(self,pyobj,typeid):

    if IsObject(pyobj):
      raise Error("Type Object doesn't need wrapping")
    if not IsId(typeid):
      raise Error("Invalid type specified for PythonObject")
    Object.__init__(self)
    self._pyobj = pyobj
    self._type  = typeid

  def __str__(self):  return str(self._pyobj)
  def __repr__(self): return repr(self._pyobj)
