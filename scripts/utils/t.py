#!/usr/bin/env python3
# coding=utf-8

import            sys
import            tty
import            termios
import            select
from term  import TerminalController
from funcs import dtNow
from funcs import dtToReadable

def getchar(timeout=5.0):
   stdin = sys.stdin
   fd = stdin.fileno()
   old_settings = termios.tcgetattr(stdin)
   try:
      tty.setraw(stdin)
      r,w,x = select.select([fd],[],[],timeout)
      if timeout and not r: return ''
      ch = stdin.read(1)
   finally:
      termios.tcsetattr(stdin, termios.TCSADRAIN, old_settings)
   return ch

if __name__ == '__main__':

  if 1:
    timeout = 1
    print('Press any keyi (Ctrl-C to break), timeout %s second%s...' % \
          (repr(timeout),'' if timeout == 1 else 's'))
    while 1:
      ch = getchar(timeout)
      if not ch and timeout:
        print(dtToReadable(dtNow())[-10:])
        continue
      if not ch: continue
      print('You pressed 0x%02x'%ord(ch))
      if ord(ch) == 0x03: break
      print('type(ch) = '+repr(type(ch)))

  if 0:
    term = TerminalController()
    a = b'\x1b[H\x1b[2J'
    b = eval(term.CLEAR_SCREEN)
    print("DEBUG: a = %s"%repr(a))
    print("DEBUG: b = %s"%repr(b))
    print("DEBUG: type(b) = %s"%repr(type(b)))
    print("DEBUG: type(b'') = %s"%repr(type(b'')))
    sys.stdout.buffer.write(b)
    sys.stdout.flush()
