#!/usr/bin/python
#coding=utf-8

from enum   import *
from spider import *

if __name__ == "__main__":

  #suites = Enum("spades=♠,clubs=♣,hearts=♥,diamonds=♦")
  suites = Enum("spades=s,clubs=c,hearts=h,diamonds=d")
  print("spades   = %s\n" % repr(suites.spades  ),end='')
  print("clubs    = %s\n" % repr(suites.clubs   ),end='')
  print("diamonds = %s\n" % repr(suites.diamonds),end='')
  print("hearts   = %s\n" % repr(suites.hearts  ),end='')
  print("***Thats all folks!!!***")