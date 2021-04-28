#!/usr/bin/env python3
#  coding=utf-8
#===============================================================================
#
# File: strscreen - Class StrScreen implementation
#
#===============================================================================
# Copyright (C) 2021 - Bruce Ferris (befe@bferris.co.uk)
#===============================================================================

import               curses
import               sys
import               os
from   funcs  import dtNow
from   funcs  import deltaSeconds
from   errors import InternalError

#
# Module globals (Or 'singletons' if you're younger than me)
#

sin  = sys.stdin.fileno()
sout = sys.stdout.fileno()
serr = sys.stderr.fileno()

debug = 1

#------------------------------------------------------------------------------
#
# Class: StrScreen - ncurses-like screen class based on strings
#

class StrScreen(object):

  def __init__(self,numrows=0,numcols=0):

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
   
    self.isatty = sys.stdout.isatty()
    self.endline = '' if self.isatty else '\n'

    self.clear()

  def __del__(self):

    if self.israw: pass
    if debug: 
      print('DEBUG: StrScreen.__del__()')

  def clear(self):

    self.rowtext = []
    for r in range(0,self.numrows):
      self.rowtext.append(' '*self.numcols)
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

    # DEBUG...
    if debug:
      print('DEBUG: addstr: y,x = [%d,%d], text = %s'%(y,x,repr(text)))
    # ...DEBUG`

  def getmaxyx(self):

    return [self.numrows,self.numcols]
    
#---
#
# __main__
#
#---

if __name__ == '__main__':

  s = StrScreen()

  if 0:
    print('StrScreen.numrows = %d'%s.numrows)
    print('StrScreen.numcols = %d'%s.numcols)

  if 0:

    now1 = dtNow()
    numrows, numcols = os.popen('stty size', 'r').read().split()
    now2 = dtNow()
    now3 = dtNow()
    print('Get tty size took      %f seconds'%deltaSeconds(now2-now1))
    print('Getting time diff took %f seconds'%deltaSeconds(now3-now2))

  if 1:

    maxy,maxx = s.getmaxyx()
    print('maxy,maxx = %s'%repr(s.getmaxyx()))
    msg = 'Hi there Dude!!!'
    s.addstr(0,maxx-5,msg)
    s.addstr(0,0,msg)
