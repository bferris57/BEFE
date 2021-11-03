#!/usr/bin/env python3
#  coding=utf-8

import os

class ctx:
  def __enter__(self):
    print("ctx.__enter__()")
  def __exit__(self,etype,eval,etb):
    print("ctx.__exit__(%s)"%repr(etype))


if __name__ == "__main__":

  with ctx() as c:
    print("Running...")

  #f = '~/.bashrc'
  #f = os.path.expanduser(f)
  #with open(f) as a:
  #  contents = a.readlines()
  #  print('%s has %d lines'%(f,len(contents)))

