#!/usr/bin/env python3
#  coding=utf-8
#===============================================================================
#
# Module: strscreen - Class StrScreen implementation
#
# Notes:  We've improved over 'curses' by introducing the introducing the
#         concept of movable named highlights using the sub-class Movable.
# 
#===============================================================================
# Copyright (C) 2021 - Bruce Ferris (befe@bferris.co.uk)
#===============================================================================

import               curses
import               sys
import               os
import               termios
import               tty
import               select
from   term   import TerminalController
from   funcs  import dtNow
from   funcs  import deltaSeconds
from   funcs  import dtToReadable
from   errors import InternalError

#
# Module globals (Or 'singletons' if you're younger than me)
#

stdin  = sys.stdin
stdout = sys.stdout
stderr = sys.stderr
fdin   = stdin.fileno()
fdout  = stdout.fileno()
fderr  = stderr.fileno()

debug   = 0
isatty  = True
endline = '\n'

terminal     = TerminalController()
CLEAR_SCREEN = eval(terminal.CLEAR_SCREEN)

#------------------------------------------------------------------------------
#

class Point(object):

  def __init__(self,y=None,x=None):

    if not isinstance(y,int) or not isinstance(x,int):
      raise InternalError("Point expected two integers (y,x)")
    self.y = y
    self.x = x

  def __str__(self):

    return 'Point(%d,%d)'%(self.y,self.x)

  def __repr__(self):

    return str(self)

class Rect(object):

  def __init__(self,tl,br):

    if (tl != None and not isinstance(tl,Point)) or \
       (br != None and not isinstance(br,Point)):
      raise Error("Rect() expected two Point() instances")
    self.tl = tl
    self.br = br
    self.normalise()

  def __str__(self):

    if self.tl and self.br:
      return 'Rect([%d,%d]->[%d,%d])'%(self.tl.y,self.tl.x,self.br.y,self.br.x)
    else:
      return 'Rect(None->None)'

  def __repr__(self):

    return str(self)

  def area(self):

    if not self.tl or not self.br:
      return 0

    return (self.br.y - self.tl.y) * (self.br.x - self.tl.x)

  def visarea(self):

    if not self.tl or not self.br:
      return 0

    return (self.br.y - self.tl.y + 1) * (self.br.x - self.tl.x + 1)

  def normalise(self):

    if not self.tl or not self.br:
      return
    tl = Point(min(self.tl.y,self.br.y),min(self.tl.x,self.br.x))
    br = Point(max(self.tl.y,self.br.y),max(self.tl.x,self.br.x))
    self.tl = tl
    self.br = br

#------------------------------------------------------------------------------
#
# Class: StrScreen - ncurses-like screen class based on strings
#

class StrScreen(object):

  def __init__(self,numrows=0,numcols=0):

    global isatty
    global endline

    self.israw = False
    if debug:
      print('DEBUG: StrScreen.__init__()')
    if not numrows and not numcols:
      self.numrows, self.numcols = os.popen('stty size', 'r').read().split()
      self.numrows = int(self.numrows)
      self.numcols = int(self.numcols)
    elif not numrows:
      self.numrows = numrows
    elif not numcols:
      self.numcols = numcols

    if debug:
      print('DEBUG:  numrows,numcols = %s'%repr([self.numrows,self.numcols]))
   
    isatty = sys.stdout.isatty()
    
    endline = '' if isatty else '\n'

    self.clear()

  def __del__(self):

    if self.israw: pass
    if debug: 
      print('DEBUG: StrScreen.__del__()')

  def initscr(self):

    pass

  def clear(self):

    self.rows = []
    for r in range(0,self.numrows+1):
      self.rows.append(' '*self.numcols)
    self.markup = []

  def addstr(self,y,x,text,color=0):

    maxx = self.numcols-1
    maxy = self.numrows-1

    if y < 0 or x < 0   or  \
      type(text) != str or  \
      not text          or  \
      y >= self.numrows:

      return

    tlen = len(text)
    if x + tlen > maxx:
      tlen = self.numcols - x
      text = text[:tlen]

    rows = self.rows
    row = rows[y]
    row = row[:x] + text + row[x+tlen:]
    rows[y] = row

    # DEBUG...
    if debug:
      print('DEBUG: addstr: y,x = [%d,%d], text = %s'%(y,x,repr(text)))
    # ...DEBUG`

  def timeout(self,milliseconds):

    if type(milliseconds) != int:
      raise InternalError('timeout expected integer milliseconds')

  def getmaxyx(self):

    return [self.numrows,self.numcols]

  def refresh(self):
    
    if not isatty:
      print('-'*10+' Screen '+'-'*10)
    else:
      stdout.buffer.write(CLEAR_SCREEN)
     
    for row in self.rows:
      print(row,end=endline)

    return

  def getch(self,timeout=1):

    stdin = sys.stdin
    fd = stdin.fileno()
    old_settings = termios.tcgetattr(stdin)
    try:
      tty.setraw(stdin)
      r,w,x = select.select([fd],[],[],timeout)
      if timeout and not r: return -1
      ch = stdin.read(1)
    finally:
      termios.tcsetattr(stdin, termios.TCSADRAIN, old_settings)

    return ord(ch) if ch else -1

#---
#
# __main__
#
#---

if __name__ == '__main__':

  scr = StrScreen()

  if 0:
    print('StrScreen.numrows = %d'%scr.numrows)
    print('StrScreen.numcols = %d'%scr.numcols)

  if 0:

    now1 = dtNow()
    numrows, numcols = os.popen('stty size', 'r').read().split()
    now2 = dtNow()
    now3 = dtNow()
    print('Get tty size took      %f seconds'%deltaSeconds(now2-now1))
    print('Getting time diff took %f seconds'%deltaSeconds(now3-now2))

  if 0:

    maxy,maxx = scr.getmaxyx()
    print('maxy,maxx = %s'%repr(scr.getmaxyx()))
    msg = 'Hi there Dude!!!'
    scr.addstr(0,maxx-5,msg)
    scr.addstr(0,0,msg)

  if 1:

    r1 = Rect(Point(0,0),Point(0,0))
    print('area(r1)    = %d'%r1.area())
    print('visarea(r1) = %d'%r1.visarea())

    r2 = Rect(Point(-2,-2),Point(-3,-3))
    print('area(r2)    = %d'%r2.area())
    print('visarea(r2) = %d'%r2.visarea())

    r3 = Rect(Point(-1,-1),Point(1,1))
    print('area(r3)    = %d'%r3.area())
    print('visarea(r3) = %d'%r3.visarea())
   
