#!/usr/bin/env python3
#  coding=utf-8
from __future__ import print_function
#===============================================================================
#
# Module:  strscreen - Class StrScreen implementation
#
# Classes: StrScreen - A line (row) based implementation on top of curses
#          Point     - A position on a StrScreen
#          Rect      - An area on a StrScreen
#          Movable   - A movable highlighted area on a StrScreen
#
# Notes:   We've improved over 'curses' by introducing the concept of a named
#          movable and highlighted area using a dict of named Movable()
#          instances.
#
#          A bit of an explanation about the Rect class...
#
#            Rectangles, in general, can either be used to represent areas on a
#            screen or geometric areas.  The difference is subtle and we won't
#            explain more here but the ones defined here are intended to be used
#            for screen manipulations NOT geometrical ones.  As such, only
#            the concept of 'equality' is implemented and they're normalised.
#
#            This means a Rect([0,0],[0,0]) has a 'visable area' of 1 and a
#            'physical area' of 0 so can be 'filled' and represents the
#            same thing as Point(0,0).
#
#            Same goes for the Point class, it represents an area on the screen.
#            So it can be filled.  ;-)
#
#            If you want geometric rectangles and points, then go look 
#            somewhere else!
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
from   funcs  import postEllipse

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
CLEAR_SCREEN = terminal.CLEAR_SCREEN

#------------------------------------------------------------------------------
#
# Class: Point - A position on a StrScreen
#

class Point(object):

  def __init__(self,y=0,x=0):

    if not isinstance(y,int) or not isinstance(x,int):
      raise InternalError("Point expected two integers (y,x)")
    self.y = y
    self.x = x
    self.normalise()

  def __eq__(self,that):

    if not isinstance(that,Point): return False

    return True if self.x == that.x and self.y == that.y else False

  def __str__(self):

    return '[%d,%d]'%(self.y,self.x)

  def __repr__(self):

    return 'Point('+str(self)+')'

  def __bool__(self):

    if type(self.y) != int or self.y < 0: return False
    if type(self.x) != int or self.x < 0: return False
    
    return True

  __nonzero__ = __bool__

  def normalise(self):

    if type(self.y) != int or self.y < 0:
      self.y = 0
    if type(self.x) != int or self.x < 0:
      self.x = 0

  def setnull(self):

    self.y = self.x = None

  def clone(self):

    return Point(self.y,self.x)

#------------------------------------------------------------------------------
#
# Class: Rect - An area on a StrScreen
#
    
class Rect(object):

  def __init__(self,tl=None,br=None):

    if (tl != None and not isinstance(tl,Point)) or \
       (br != None and not isinstance(br,Point)):
      raise Error("Rect() expected two Point() instances")

    self.tl = tl.clone() if tl else None
    self.br = br.clone() if br else None

    self.normalise()

  def __str__(self):

    if isinstance(self.tl,Point):
      left = str(self.tl)
    else:
      left = 'None'
    if isinstance(self.br,Point):
      right = str(self.br)
    else:
      right = 'None'

      
    return left + '->' + right

  def __repr__(self):

    return 'Rect('+ str(self) + ')'

  def __bool__(self):

    return bool(self.tl) and bool(self.br)

  __nonzero__ = __bool__

  def area(self):

    if not self.tl or not self.br:
      return 0

    return (self.br.y - self.tl.y + 1) * (self.br.x - self.tl.x + 1)

  def normalise(self):

    if not self.tl or not self.br:
      return

    self.tl.normalise()
    self.br.normalise()
    tl = Point(min(self.tl.y,self.br.y),min(self.tl.x,self.br.x))
    br = Point(max(self.tl.y,self.br.y),max(self.tl.x,self.br.x))
    self.tl = tl
    self.br = br

    return

  def setnull(self):

    self.tl.setnull()
    self.br.setnull()

  def clip(self,that):

    if not isinstance(that,Rect) or \
       not bool(self)            or \
       self.tl.y > that.br.y     or \
       self.tl.x > that.br.x     or \
       self.br.y < that.tl.y     or \
       self.br.x < that.tl.x:

      return Rect()

    else:

      tl = Point(max(self.tl.y,that.tl.y), max(self.tl.x,that.tl.x))
      br = Point(min(self.br.y,that.br.y), min(self.br.x,that.br.x))

      return Rect(tl,br)

  def clone(self):

    return Rect(self.tl.clone(),self.br.clone())
   
#------------------------------------------------------------------------------
#
# Class: StrScreen - ncurses-like screen class based on strings
#

class StrScreen(object):

  def __init__(self,screen=None,numrows=0,numcols=0):

    global isatty
    global endline

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

    self.screen = screen
    self.curpos = Point(0,0)

    if debug:
      print('DEBUG:  numrows,numcols = %s'%repr([self.numrows,self.numcols]))
   
    isatty = sys.stdout.isatty()
    
    endline = '' if isatty else '\n'

    self.clear()

  def __del__(self):

    if debug: 
      print('DEBUG: StrScreen.__del__()')

  # Returns number of movables
  def __len__(self):

    return len(self.movables)

  # [int] Returns rows[int]
  # [str] Returns movables[str]
  def __getitem__(self,index):
  
    if type(index) == int:
      if index < 0:
        return self.rows[index] if -index <= len(self.rows) else None
      else:
        return self.rows[index] if index < len(self.rows) else None

    if type(index) == Movable:
      return self.movables[index] if index in self.movables else None

    return None

  def initscr(self):

    pass

  def clear(self):

    self.rows = []
    for r in range(0,self.numrows+1):
      self.rows.append(' '*self.numcols)
    self.markup   = []
    self.movables = []

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
    # ...DEBUG

  def timeout(self,milliseconds):

    if type(milliseconds) != int:
      raise InternalError('timeout expected integer milliseconds')

  def getmaxyx(self):

    return [self.numrows,self.numcols]

  def width(self):

    return self.numcols

  def height(self):

    return self.numrows

  def refresh(self):
    
    if not isatty:
      print('-'*10+' Screen at %s'%dtToReadable(dtNow)[:-4]+'-'*10)
    else:
      stdout.buffer.write(CLEAR_SCREEN)
     
    for row in self.rows:
      print(row,end=endline)

    return

  def goto(self,where):

    if not isinstance(where,Point): return False
    y = max(min(y,self.numrows-1),0)
    x = max(min(x,self.numrows-1),0)
    self.curpos.y = y
    self.curpos.x = x

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

  if 0:

    r1 = Rect(Point(0,0),Point(0,0))
    print('area(r1)    = %d'%r1.area())
    print('visarea(r1) = %d'%r1.visarea())

    r2 = Rect(Point(-2,-2),Point(-3,-3))
    print('area(r2)    = %d'%r2.area())
    print('visarea(r2) = %d'%r2.visarea())

    r3 = Rect(Point(-1,-1),Point(1,1))
    print('area(r3)    = %d'%r3.area())
    print('visarea(r3) = %d'%r3.visarea())
   
  if 0:

    r1 = Rect(Point(0,0),Point(-1,-1))
    print('r1 = %s'%repr(r1))
    print('  if r1... %s'%repr(bool(r1)))
    print('  r1.area() = %d'%r1.area())

    r2 = Rect(Point(0,0),Point(-1,-1))
    print('r2 = %s'%repr(r2))
    print('  if r2... %s'%repr(bool(r2)))

    r3 = Rect(Point(0,0),Point(0,0))
    print('r3 = %s'%repr(r3))
    print('  if r3... %s'%repr(bool(r3)))
    print('  r3.area() = %d'%r3.area())

    r4 = Rect(Point(0,0),Point(0,1))
    print('r4 = %s'%repr(r4))
    print('  if r4... %s'%repr(bool(r4)))
    print('  r4.area() = %d'%r4.area())

    r5 = Rect(Point(0,0),Point(1,1))
    print('r5 = %s'%repr(r5))
    print('  if r5... %s'%repr(bool(r5)))
    print('  r5.area() = %d'%r5.area())

  if 0:

    print('scr.numrows = %d'%scr.numrows)
    print('scr.numcols = %d'%scr.numcols)

  if 1: # Test Rect clipping

    cliprect = Rect(Point(5,5),Point(20,20))
    rects = [Rect(Point(0,0),Point(10,10)), \
             Rect(Point(6,6),Point(10,10)), \
             Rect(Point(1,1),Point(7,7)),   \
             Rect(Point(30,30),Point(31,31)) ]
    print("cliprect %s..."%str(cliprect))
    for i in range(0,len(rects)):
      rect = rects[i]
      clipped = rect.clip(cliprect)
      print('  %2d: %s => %s'%(i+1,postEllipse(str(rect),30,fill=' '),str(clipped)))
