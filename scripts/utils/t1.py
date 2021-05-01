#!/usr/bin/env python3
# coding=utf-8

def test(*args,**vargs):

  print('test: len(args) = %d'%len(args))
  print('test: len(vargs) = %d'%len(vargs))

  print('  args %s...'%repr(type(args)))
  for i in range(0,len(args)):
    print('    %d: %s'%(i,repr(args[i])))

  print('  vargs %s...'%repr(type(vargs)))
  for key in vargs.keys():
    print('    %s: %s'%(repr(key),repr(vargs[key])))

if __name__ == '__main__':

  print('test 1...')
  test(1,2,3.5,x='x',y='y')
  print('test 2...')
  test(1,2,3.5,4,5)

