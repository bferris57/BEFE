#!/usr/bin/env python3
#  coding=utf-8

from collections import namedtuple

if __name__ == '__main__':

  print('We\'re here!')

  x = namedtuple(0,1,2,x=3,y=4)
  print(repr(x))
