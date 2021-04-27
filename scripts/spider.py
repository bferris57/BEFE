#!/usr/bin/env python3
#  coding=utf-8

import                    time
import                    curses
import                    random
from   curses      import wrapper
import                    sys
from   funcs       import dtNow
from   funcs       import dtToNonce
from   funcs       import red,blue,green
from   errors             import *

lines = '∙√─│┌┐└┘├┬┴┼═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟╠╡╢╣╤╥╦╧╨╩╪╫╬▲►▼◄◊☺☻'

ld_tl   = '┌'
ld_tr   = '┐'
ld_bl   = '└'
ld_br   = '┘'
ld_hor  = '─'
ld_vert = '│'
ld_rarr = '►'
ld_larr = '◄'
ld_uarr = '▲'
ld_darr = '▼'
ld_0    = '\u24ea'
ld_1    = '\u2460'
ld_2    = '\u2461'
ld_3    = '\u2462'
ld_4    = '\u2463'
ld_5    = '\u2464'
ld_6    = '\u2465'
ld_7    = '\u2466'
ld_8    = '\u2467'
ld_9    = '\u2468'
ld_10   = '\u2469'
ld_11   = '\u246a'
ld_12   = '\u246b'
ld_13   = '\u246c'

ld_numbers = ld_0+ld_1+ld_2+ld_3+ld_4+ld_5+ld_6+ld_7+ld_8+ld_9+ld_10
ld_club    = ld_tl+ld_hor+'\u2660'+ld_hor+ld_tr
ld_suit    = '\u2660\u2661\u2662\u2663'
#ld_deck    = ld_0+'A'+ld_2+ld_3+ld_4+ld_5+ld_6+ld_7+ld_8+ld_9+ld_10+'JQK'
ld_deck    = '?A23456789TJQK'

maxx = None
maxy = None

topnum = '┌─x─┐'
top    = '┌───┐'
topvis = top #ld_club
mid    = '│   │'
midnum = '│ x │'
bot    = '└───┘'

visCards = []

#---
#
# Various classes...
#
#---

class Point(object):

  def __init__(self,y=None,x=None):

    if not isinstance(y,int) or not isinstance(x,int):
      raise InternalError("Point expected two integers (y,x)")
    self.y = y
    self.x = x

  def __str__(self):

    return 'Point(%d,%d)'%(self.y,self.x)

class Rect(object):

  def __init__(self,tl,br):

    if (tl != None and not isinstance(tl,Point)) or \
       (br != None and not isinstance(br,Point)):
      raise Error("Rect() expected two Point() instances")
    self.tl = tl
    self.br = br
    self.normalise()

  def __str__(self):

    return 'Rect([%d,%d]->[%d,%d])'%(self.tl.y,self.tl.x,self.br.y,self.br.x)

  def area(self):

    return (self.br.y - self.tl.y) * (self.br.x - self.tl.x)

  def normalise(self):

    if not self.tl or not self.br:
      return
    tl = Point(min(self.tl.y,self.br.y),min(self.tl.x,self.br.x))
    br = Point(max(self.tl.y,self.br.y),max(self.tl.x,self.br.x))
    self.tl = tl
    self.br = br

class Card(Rect):

  def __init__(self,cardno,suit=None):

    super().__init__(tl=None,br=None)
    if not isinstance(cardno,int):
      raise InternalError('Card() expected cardno to be an int()')
    if suit != None and not isinstance(suit,int):
      raise InternalError('Card() expected suit to be an int()')
    self.cardno   = cardno
    self.suit     = suit
    self.pos      = None
    self.selected = False

  def dist(self,that): # Calculate distance squared to another card...

    dist = 0
    if that == None or not isinstance(that,Card):
      return dist

    posThis = self.pos
    posThat = that.pos
    if posThis == None or this.area() == 0:
      return dist
    if posThat == None or that.area() == 0:
      return dist

    deltax = posThis.x - posThat.x
    deltay = posThis.y - posThat.y
   
    dist = (deltax*deltax)+(deltay*deltay)

    return dist

  def getLeftRight(self):

    that = None
    if not self.pos: 
      return None

    distLeft  = 0
    thatLeft  = None
    distRight = 0
    thisRight = None
    for i in range(0,len(visCards)):
      card = visCards[i]
      if that.pos and that.pos.x == self.pos.x:
        continue
      d = self.dist(that)
      if that.pos.x < self.pos.x and d < distLeft:
        distLeft = d
        thatLeft = that
      if that.pos.x > self.pos and d < distRight:
        distRight = d
        thatrRight = that

    return (thatLeft,thatRight)

  def getUpDown(self):

    that = None
    if not self.pos:
      return None

    distUp   = 0
    thatUp   = None
    distDown = 0
    thisDown = None
    for i in range(0,len(visCards)):
      card = visCards[i]
      if that.pos and that.pos.y == self.pos.y:
        continue
      d = self.dist(that)
      if that.pos.y < self.pos.y and d < distUp:
        distUp = d
        thatUp = that
      if that.pos.y > self.pos.y and d < distDown:
        distDown = d
        thatDown = that

    return (thatUp,thatDown)        

#---
#
# Various Functions...
#
#---

def shuffle(arr): # Shuffle list of ints...

  if type(arr) != type([]):
    raise Error('shuffle: Expected list')

  deck = []
  for x in arr: deck.append(x)

  ret = []

  while deck:
    thisone = random.randint(0,len(deck)-1)
    ret.append(deck[thisone])
    del deck[thisone]

  return ret

def renderAll(decks,stacks,msg):

  scr.clear()

  y = 1
  x = 0
  for s in range(0,len(stacks)):
    renderDeck(y,x+s*7,stacks[s])

  scr.refresh()

def renderDeck(y,x,deck):

  if not deck:
    return

  selected = None

  for cardno in range(0,len(deck)):
    card = deck[cardno]
    if card.cardno <= 0:
      tstr = topvis
    else:
      # DEBUG...
      if card.cardno == 0:
        scr.addstr(0,0,'DEBUG: cardno = %s, card.cardno = %d...'%(cardno,card.cardno))
      # ...DEBUG
      tstr = ld_deck[card.cardno].join(topnum.split('x'))
    if card.selected:
      scr.addstr(y+i,x,tstr,curses.A_REVERSE)
    else:
      scr.addstr(y+cardno,x,tstr)
    tl = Point(y,x)
    if cardno == len(deck)-1:
      y = y + len(deck) - 1
      scr.addstr(y  , x, top)
      scr.addstr(y+1, x, mid)
      scr.addstr(y+2, x, ld_deck[card.cardno].join(midnum.split('x')))
      scr.addstr(y+3, x, mid)
      scr.addstr(y+4, x, bot)
      br = Point(y+4, x+len(bot))
    else:
      br = Point(y, x+len(bot))
    card.pos = Rect(tl,br)

  scr.refresh()

def dealOne(decks,stack):

  if not decks:
    return
  card = decks[-1][-1]
  stack.append(card)
  del decks[-1][-1]
  if not decks[-1]:
    del decks[-1]
      
def main(screen):

  global scr
  global maxx
  global maxy

  scr = screen

  scr.clear()
  scr.refresh()
  scr.timeout(200)
  curses.curs_set(0)

  maxy, maxx = scr.getmaxyx()

  scr.addstr(maxy-1,0,'Press any \'q\' or \'x\' to exit...')
  scr.refresh()

  # Create randomly shuffled decks...

  deck = [ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13]
  decks = []
  for i in range(0,8): # Create original decks...
    thisone = shuffle(deck)
    for j in range(0,len(thisone)):
      thisone[j] = Card(thisone[j])
    decks.append(thisone)

  # Deal stacks...
  stacks = ([],[],[],[],[],[],[],[],[],[])
  cardno = 0
  for d in range(0,6):
    while cardno < 54:
      dealOne(decks,stacks[cardno%len(stacks)])
      cardno += 1

  for s in range(0,len(stacks)):
    stack = stacks[s]
    if stack and stack[-1].cardno < 0:
      stack[-1].cardno = -(stack[-1].cardno)

  renderAll(decks,stacks,"Press 'q|x'='Exit', 'd'='Deal'...")

  #
  # Do it...
  #

  key = ''
  while key not in ('q','Q','x','X'):
    dt = dtToNonce(dtNow()).split('T')[1][:-4]
    scr.addstr(0,maxx-len(dt),dt)
    key = scr.getch()
    if key < 0: continue
    # TEMP...
    scr.addstr(0,0,'Key: 0x'+'%x'%key+'          ',curses.A_REVERSE)
    # ...TEMP
    key = chr(key)
   
    if key in ('d','D'): # Deal...
      for s in range(0,len(stacks)):
        stack = stacks[s]
        dealOne(decks,stack)
        if stack and stack[-1].cardno < 0:
          stack[-1].cardno = -(stack[-1].cardno)
      renderAll(decks,stacks,'Continue on...')

  #scr.clear()
  #scr.refresh()
  #time.sleep(3)

#------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

  if 1:

    wrapper(main)

  if 0:

    deck = shuffle([1,2,3,4,5,6,7,8,9,10,11,12,13])
    print('deck = %s'%repr(deck))
      
  if 0:

    try:
      raise(InternalError('Testing InternalError()'))
    except InternalError as e:
      red(str(e))

  if 0:

    r = Rect(Point(25,80),Point(0,0))
    print('r      = %s'%str(r))
    print('r.area = %s'%str(r.area()))

    ex = Point()
