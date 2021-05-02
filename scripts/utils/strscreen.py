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

debug   = False
keys    = ''
isatty  = False
endline = '\n'

out = None

def oprint(msg,end='\n',output=None):

  tout = output
  if not tout:
    tout = out
  if tout and not tout.closed:
    tout.write(msg)
    if end: tout.write(end)
    tout.flush()
  
def setkeys(them):
  global keys
  keys = them
  return

def debugon(output=None):
  global debug
  global out
  debug = True
  out   = output

def debugoff(output=None):
  global debug
  debug = False
  out = output

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

  def width(self):

    if not(self): return 0
    return self.br.x - self.tl.x + 1

  # where -> Rect: Where to move (Top Left)
  def moveto(self,where):

    if bool(self):
      dy = where.y - self.tl.y
      dx = where.x - self.tl.x
      self.tl.y = self.tl.y + dy
      self.tl.x = self.tl.x + dx
      self.br.y = self.br.y + dy
      self.br.x = self.br.x + dx
      return True
    return False

  def height(self):

    if not(self): return 0
    return self.br.y - self.tl.y + 1

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
# Class: Movable - A movable highlighted area on a StrScreen
#

class Movable(object):

  # where -> Rect, color=curses.color
  def __init__(self,where=None,color=curses.A_REVERSE):

    if not isinstance(where,Rect):
      raise InternalError('Expected \'where\' to be a Rect')

    self.where = where
    self.color = color

  # where -> Point: Where to move (Top Left)
  def moveto(where=Point(0,0)):

    if not isinstance(where,Point):
      raise InternalError('Expected \'where\' to be a Point')

    if where:
      self.where.move(where)


#------------------------------------------------------------------------------
#
# Class: StrScreen - ncurses-like screen class based on strings
#

class StrScreen(object):

  def __init__(self,screen=None,numrows=0,numcols=0,output=None):

    global isatty
    global endline
    global out
    global keys

    if output:
      out = output
    if debug:
      oprint('DEBUG: StrScreen.__init__()')
    if not numrows and not numcols:
      self.numrows, self.numcols = os.popen('stty size', 'r').read().split()
      self.numrows = int(self.numrows)
      self.numcols = int(self.numcols)
    elif not numrows:
      self.numrows = numrows
    elif not numcols:
      self.numcols = numcols

    if not debug and screen:
      curses.cbreak()

    self.screen   = screen
    self.curpos   = Point(0,0)
    self.movables = {}

    if debug:
      oprint('DEBUG:  numrows,numcols = %s'%repr([self.numrows,self.numcols]))
      oprint('DEBUG:  keys = '+repr(keys))
   
    isatty = sys.stdout.isatty()
    
    endline = '' if isatty else '\n'

    self.clear()

  def __del__(self):

    if debug: 
      oprint('DEBUG: StrScreen.__del__()')

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

  # DEBUG...
  def _dump(self,why='',output=None):

    tout = out

    if output:
      tout = output
    if debug or output or out:
      lead = '-'*20
      if why:
        why = '('+why+') '
      oprint(lead+' Screen at %s %s'%(dtToReadable(dtNow()),why)+lead)
      for i in range(0,len(self.rows)):
        if self.rows[i].strip():
          oprint('  row[%2d] = %s'%(i,repr(postEllipse(self.rows[i],80))),output=tout)
  # ...DEBUG

  def initscr(self):

    pass

  def clear(self,fill=' '):

    if debug:
      print('DEBUG: clear() called')

    self.rows = []
    for r in range(0,self.numrows+1):
      self.rows.append(fill[0]*self.numcols)
    self.markup   = []

    if not debug and self.screen:
      self.screen.clear()

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

    if not debug and self.screen:
      self.screen.addstr(y,x,text,color)

  def timeout(self,milliseconds):

    scr = self.screen

    if type(milliseconds) != int:
      raise InternalError('timeout expected integer milliseconds')

    self.timeout = milliseconds

    if not debug and scr:
      scr.timeout(milliseconds)

  def getmaxyx(self):

    return [self.numrows,self.numcols]

  def width(self):

    return self.numcols

  def height(self):

    return self.numrows

  def refresh(self):
    
    scr = self.screen

    if not debug and scr:
      scr.clear()
      maxy,maxx = scr.getmaxyx()
      for y in range(0,min(len(self.rows)-1,maxy)):
        scr.addstr(y,0,self.rows[y][0:81])
      scr.refresh()

    #if debug or out:
    #  self._dump('StrScreen.refresh()')

    return

  def goto(self,where):

    if not isinstance(where,Point): return False
    y = max(min(y,self.numrows-1),0)
    x = max(min(x,self.numrows-1),0)
    self.curpos.y = y
    self.curpos.x = x

  def getch(self,timeout=1):

    if not debug and self.screen:
      self.screen.timeout(timeout)
      return self.screen.getch()

    if debug:
      global keys
      if not keys:
        return -1
      key = ord(keys[0])
      keys = keys[1:]
      return key

    # Do our own raw getch()...
    stdin = sys.stdin
    fd = stdin.fileno()
    old_settings = termios.tcgetattr(stdin)
    ch = '\x00'
    try:
      tty.setraw(stdin)
      r,w,x = select.select([fd],[],[],timeout)
      if timeout and not r: return -1
      ch = stdin.read(1)
    finally:
      termios.tcsetattr(stdin, termios.TCSADRAIN, old_settings)

    return ord(ch) if ord(ch) > 0 else -1

  def getrect(self,rect=None):

    if not isinstance(rect,Rect) or not rect:
      return ''

    lines = self.rows

    s = ''
    for y in range(rect.tl.y,min(rect.br.y,self.numrows-1)+1):
      if s: s += '\n'
      s += lines[y][rect.tl.x:rect.br.x+1]
  
    return s

  # _strPointRect: Given a set of string lines and Point/Rect, make it
  #                all proper.
  #
  #                if rect is a Point, we calculate the rect based on 
  #                the length of lines and their width.
  #
  #                if the rect's tl not given, we use self.curpos.
  #
  #                Finally, we extend/truncate each line to the Rect and
  #                get rid of any extra lines.
  #
  #                returns (lines,rect) all fixed and ready to be used.
  #

  def _strPointRect(self,lines=[],rect=None): # Internal use only

    # Normalise rect to be a Rect and, if need be place at self.curpos
    # and, also if need be, make it as wide as largest line...
      
    point = self.curpos.clone()
    width = None 
    if not rect:
      if isinstance(rect,Point):
        if not point:
          point = self.curpos.clone()
      elif rect == None:
          point = self.curpos.clone()
      elif not rect:
        if not rect.tl:
          rect = Rect(point,rect.br)
      width = 1
      for i in range(0,len(lines)):
        width = max(width,len(line))
      if isinstance(rect,Point):
        rect = Rect(point,Point(point.y+len(lines)-+1,point.x+width-1))
      if not rect:
        point = rect.tl
        rect.br = Point(point,Point(point.y+len(lines)-1,point.x+width-1))

    # Make all lines the proper length and ensure right number of lines...
    if not width:
      width = rect.width()
      height = rect.height()
    for i in range(0,len(lines)):
      line = lines[i]
      if len(line) != width:
        if len(line) < width:
          line += ' '*(width-len(line))
        else:
          line = line[0:width]
        lines[i] = line

    # Make sure there's the right number of lines...
    height = rect.height()
    if len(lines) != height:
      if len(lines) < height:
        for i in range(0,height-len(lines)):
          lines.append(' '*width)
        lines = lines[:height]

    # Make all lines the proper length and ensure right number of lines...
    if not width:
      width = rect.width()
      height = rect.height()
    for i in range(0,len(lines)):
      line = lines[i]
      if len(line) != width:
        if len(line) < width:
          line += ' '*(width-len(line))
        else:
          line = line[0:width]
        lines[i] = line

    return (lines,rect)

  def putstr(self,string,rect=None,color=0): # bool,Movable:

    if (not isinstance(rect,Rect) and not isinstance(rect,Point)) or  \
       not rect                                                   or  \
       not isinstance(string,str)                                 or  \
       not isinstance(color,int):
      return False

    lines = string.split('\n')

    lines,rect = self._strPointRect(lines,rect)

    # Render it to our screen lines...
    scr = self.screen
    y = rect.tl.y
    x = rect.tl.x
    for i in range(0,len(lines)):
      line = self.rows[y]
      line = line[:x]+lines[i]+line[x+width:]
      self.rows[y] = line
      if scr:
        scr.addstr(y,x,lines[i],color=color)
      y += 1
      if y > self.numrows-1:
        break

    return True

#---
#
# __main__
#
#---

if __name__ == '__main__':

  def showtest(testno):
    lead = ' '*40
    print(lead+' Test %d '%testno+lead)

  scr = StrScreen()
  if debug:
    scr._debugon()

  test = 0 

  print('*********** TESTS ***********\n')

  test += 1
  if 0:
    print('StrScreen.numrows = %d'%scr.numrows)
    print('StrScreen.numcols = %d'%scr.numcols)

  test += 1
  if 0:

    scr.addstr(0,0,'Hey dude!!!')
    scr.addstr(1,2,'Hi there')
    scr.addstr(2,4,'Last line')
    scr.addstr(10,0,'Real last line')

    s = scr.getrect(Rect(Point(0,0),Point(15,15)))
    print("DEBUG: s = %s"%repr(s))
    lines = s.split('\n')
    print('Lines at 0,0...')
    for i in range(0,len(lines)):
      print('  %2d: %s'%(i,repr(lines[i])))

  test += 1
  if 1:

    showtest(test)

    print('scr.numrows = %d'%scr.numrows)
    print('scr.numcols = %d'%scr.numcols)
    scr.clear('.')

    print('')
    rect = Rect(Point(5,10),Point(16,25))
    string = '''putstr line 1
2
3
4
5
putstr line 6
7
8
9
Last line'''
    result = scr.putstr(string,rect)
    print('putstr test...')
    print('  result = %s'%repr(type(result)))

    rect = Rect(Point(0,0),Point(len(scr.rows)-1,50))
    s = scr.getrect(rect)
    lines = s.split('\n')
    print('Lines at 0,0...')
    for i in range(0,len(lines)):
      print('  %2d: %s'%(i,repr(lines[i])))
  
  test += 1
  if 0:

    now1 = dtNow()
    numrows, numcols = os.popen('stty size', 'r').read().split()
    now2 = dtNow()
    now3 = dtNow()
    print('Get tty size took      %f seconds'%deltaSeconds(now2-now1))
    print('Getting time diff took %f seconds'%deltaSeconds(now3-now2))

  test += 1
  if 0:

    maxy,maxx = scr.getmaxyx()
    print('maxy,maxx = %s'%repr(scr.getmaxyx()))
    msg = 'Hi there Dude!!!'
    scr.addstr(0,maxx-5,msg)
    scr.addstr(0,0,msg)

  test += 1
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
  
  test += 1
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

  test += 1
  if 0:

    print('scr.numrows = %d'%scr.numrows)
    print('scr.numcols = %d'%scr.numcols)
    scrrect = Rect(Point(0,0),Point(scr.numrows-1,scr.numcols-1))
    print('scrrect     = '+str(scrrect))
    print('scr.height  = %d'%scrrect.height())
    print('scr.width   = %d'%scrrect.width())

  test += 1
  if 0: # Test Rect clipping

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

    rect = Rect(Point(10,10),Point(20,20))
    print('Before move rect = %s'%str(rect))
    rect.moveto(Point(5,5))
    print('After  move rect = %s'%str(rect))
