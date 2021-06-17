#!/usr/bin/env python3
#  coding=utf-8

import            signal
import            os
import            sys
import            time
from funcs import dtNow

maxy = maxx = 0
old_sigwinch = None

def elapsed(dt1,dt2):
  diff = dt2-dt1
  seconds = diff.seconds + float(diff.microseconds)/100000
  return seconds

def sighandler(signum,frame):

  global maxy
  global maxx
  global old_sigwinch

  if signum == signal.SIGWINCH:
    maxy,maxx = getmaxyx()
    print('maxy = %d, maxx = %d'%(maxy,maxx))
    if old_sigwinch:
      old_sigwinch(signum,frame)

def getmaxyx() -> tuple:

  rows, cols = os.popen('stty size', 'r').read().split()
  try:
    rows = int(rows)
    cols = int(cols)
  except:
    rows = cols = 0
    
  return (rows-1, cols-1)
  
if __name__ == '__main__':
  
  maxy,maxx = getmaxyx()
  print('maxy = %d, maxx = %d'%(maxy,maxx))
  old_sigwinch = signal.signal(signal.SIGWINCH,sighandler)

  then = dtNow()
  while elapsed(then,dtNow()) < 60:
    time.sleep(1)
    print('.',end='')
    sys.stdout.flush()
  print('\n')

  signal.signal(signal.SIGWINCH,original_sigwinch)
