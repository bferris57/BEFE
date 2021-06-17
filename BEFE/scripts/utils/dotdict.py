#!/usr/bin/env python3
#coding=utf-8

from collections import OrderedDict

#-------------------------------------------------------------------------------
#
# Class: DotDict - A 'dottable dict' (both subscriptable and dottable)
#

class DotDict(OrderedDict):

    def __init__(self,that={},insensitive=False):

        if insensitive:
            self['__insensitive'] = insensitive

        for key in that:
            self[key] = that[key]

    def __getattr__(self,name):

        if '__insensitive' in self and self.__insensitive:
          name = name.lower()

        if name in self:
            return self[name]
        else:
            raise KeyError("DotDict instance missing key %s"%repr(name))

    def __setattr__(self,name,value):

        if '__insensitive' in self and self.__insensitive:
          name = name.lower()

        self[name] = value

    def __iter__(self):

      return DotDictIter(self)

    def pop(self,name):

      if name in self:
         del self[name]

    def __contains__(self,key):

      keys = self.keys()
      return key in keys

    def keys(self):

      k = list(dict.keys(self))
      for i in range(len(k)-1,-1,-1):
        if k[i].startswith('__'):
          del k[i]

      return k

def DotDictIter(that):

  theIter = dict.__iter__(that)
  keys = list(dict.__iter__(that))

  for key in keys:
    if type(key) == str and not key.startswith('__'):
      yield key
        
if __name__ == '__main__':

    print("DotDict.py - Starting...")
    dic = DotDict(insensitive=True)
    dic.xxx = 'xxx!'

    print("dic.xxx   = %s"%repr(dic.xxx))
    print("dic.XXX   = %s"%repr(dic.XXX))
    print("repr(dic) = %s"%repr(dic))
