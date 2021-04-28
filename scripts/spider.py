#!/usr/bin/env python3
#  coding=utf-8

import                    time
import                    curses
import                    random
from   curses      import wrapper
import                    sys
from   funcs       import dtNow
from   funcs       import dtToNonce
from   funcs       import dtToReadable
from   funcs       import red,blue,green
from   funcs       import postEllipse
from   errors             import *
from   strscreen   import StrScreen

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

#keys = 'dd'+ld_darr+ld_darr+ld_uarr+'b'+ld_uarr  # +'q'
keys = 'bb'

maxx = None
maxy = None

topnum = '┌─x─┐'
top    = '┌───┐'
topvis = top #ld_club
mid    = '│   │'
midnum = '│ x │'
bot    = '└───┘'

allCards    = []
curSelected = None
msgbot      = ''
msgtop      = ''

#
# Debugging...
#

debug       = 1

if debug:
  out  = open('spider.out','w')                # For Debugging
  keys = 'dd'+ld_larr                  # +'q'  # For Debugging
else:
  keys = ''

def oprint(msg,end='\n'):
  if not debug: return
  out.write(msg)
  if end:
    out.write(end)
  out.flush()

breaker = 0

def obreak(msg=None,end='\n'):

  global breaker

  breaker += 1
  if not debug: return
  if not msg:
    msg = ' Break %d '%breaker
  oprint('='*30+msg+'='*30)


oprint('----- BEGIN... %s -----'%dtToReadable(dtNow())[:-4])

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

  def normalise(self):

    if not self.tl or not self.br:
      return
    tl = Point(min(self.tl.y,self.br.y),min(self.tl.x,self.br.x))
    br = Point(max(self.tl.y,self.br.y),max(self.tl.x,self.br.x))
    self.tl = tl
    self.br = br

class Card(Rect):

  seq = 0

  def __init__(self,cardno,suit=None):

    super().__init__(tl=None,br=None)
    if not isinstance(cardno,int):
      raise InternalError('Card() expected cardno to be an int()')
    if suit != None and not isinstance(suit,int):
      raise InternalError('Card() expected suit to be an int()')
    # TEMP...
    self.seq      = Card.seq
    Card.seq += 1
    # ...TEMP
    self.cardno   = cardno
    self.suit     = suit
    self.pos      = None
    self.selected = False

  def __str__(self):

    return 'Card(seq=%s,pos=%s,cardno=%s)'%(repr(self.seq),repr(self.pos),repr(self.cardno))

  def __repr__(self):

    return str(self)

  def dist(self,that): # Calculate distance squared to another card...

    dist = 0
    if that == None or not isinstance(that,Card):
      return dist

    posThis = self.pos
    posThat = that.pos
    if posThis == None or posThat == None:
      return dist

    deltax = posThis.tl.x - posThat.tl.x
    deltay = posThis.tl.y - posThat.tl.y
   
    dist = (deltax*deltax)+(deltay*deltay)

    return dist

  def getLeftRight(self):

    global msgtop

    that = None
    if not self.pos: 
      return [None,None]

    distLeft  = 0x7fffffff
    thatLeft  = None
    distRight = 0x7fffffff
    thatRight = None
    for i in range(0,len(allCards)):
      that = allCards[i]
      if that.cardno <= 0 or (that.pos and that.pos.tl.x == self.pos.tl.x):
        continue
      d = self.dist(that)
      if d and that.pos:
        if that.pos.tl.x < self.pos.tl.x and distLeft and d < distLeft:
          distLeft = d
          thatLeft = that
        if that.pos.tl.x > self.pos.tl.x and distRight and d < distRight:
          oprint('DEBUG: thatRight: that.pos = %s, d = %s'%(repr(that.pos),repr(d)))
          distRight = d
          thatRight = that

    ret = (thatLeft,thatRight)
    oprint("getLeftRight: ret = %s"%repr(ret))

    return ret

  def getUpDown(self):

    global msgtop

    that = None
    if not self.pos: 
      return [None,None]

    distUp   = 0x7fffffff
    thatUp   = None
    distDown = 0x7fffffff
    thatDown = None
    for i in range(0,len(allCards)):
      that = allCards[i]
      if that.cardno <= 0 or (that.pos and that.pos.tl.y == self.pos.tl.y):
        continue
      d = self.dist(that)
      if d and that.pos:
        if that.pos.tl.y < self.pos.tl.y and distUp and d < distUp:
          oprint('DEBUG: thatUp: that.pos = %s, d = %s'%(repr(that.pos),repr(d)))
          distUp = d
          thatUp = that
        if that.pos.tl.y > self.pos.tl.y and distDown and d < distDown:
          oprint('DEBUG: thatDown: that.pos = %s, d = %s'%(repr(that.pos),repr(d)))
          distDown = d
          thatDown = that

    ret = (thatUp,thatDown)
    oprint("getUpDown: ret = %s"%repr(ret))

    return ret


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

def renderAll(decks,stacks,msgbot=msgbot,msgtop=msgtop):

  scr.clear()

  y = 1
  x = 0
  for s in range(0,len(stacks)):
    renderDeck(y,x+s*7,stacks[s])

  maxy,maxx = scr.getmaxyx()
  if msgbot:
    scr.addstr(maxy-1,0,postEllipse(msgbot,maxx-1),curses.A_REVERSE)
  if msgtop:
    scr.addstr(0,0,postEllipse(msgtop,maxx-8),curses.A_REVERSE)

def renderDeck(y,x,deck):

  if not deck:
    return

  selected = None

  for i in range(0,len(deck)):
    card = deck[i]
    if card.cardno <= 0:
      tstr = topvis
    else:
      tstr = ld_deck[card.cardno].join(topnum.split('x'))
    if card.selected:
      scr.addstr(y+i,x,tstr,curses.A_REVERSE)
    else:
      scr.addstr(y+i,x,tstr)
    tl = Point(y+i,x)
    if i == len(deck)-1:
      y = y + len(deck) - 1
      color = curses.A_REVERSE if card.selected else 0
      scr.addstr(y  , x, top,color)
      scr.addstr(y+1, x, mid)
      scr.addstr(y+2, x, ld_deck[card.cardno].join(midnum.split('x')))
      scr.addstr(y+3, x, mid)
      scr.addstr(y+4, x, bot)
      br = Point(y+4, x+len(bot))
    else:
      br = Point(y, x+len(bot))
    card.pos = Rect(tl,br)
    oprint('renderDeck: y = %d, i = %d, card.pos = %s'%(y,i,repr(card.pos)))

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
  global curSelected
  global msgbot
  global msgtop
  global keys

  # ...TESTING

  scr = screen

  scr.clear()
  scr.refresh()
  scr.timeout(200)
  if type(scr) != StrScreen:
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
      allCards.append(thisone[j])
    decks.append(thisone)

  # Deal stacks...
  stacks = ([],[],[],[],[],[],[],[],[],[])
  cardno = 0
  for d in range(0,6):
    while cardno < 54:
      dealOne(decks,stacks[cardno%len(stacks)])
      cardno += 1

  selected = False
  for s in range(0,len(stacks)):
    stack = stacks[s]
    if stack and stack[-1].cardno < 0:
      stack[-1].cardno = -(stack[-1].cardno)
  
  # select first visible card in first stack...
  stack = stacks[0]
  for i in range(0,len(stack)):
    card = stack[i]
    if card.cardno >= 0 and not curSelected:
      card.selected = True
      curSelected   = card
      break

  renderAll(decks,stacks)

  #
  # Do it...
  #

  key = ''
  while key not in ('q','Q','x','X'):

    dt = dtToReadable(dtNow())
    scr.addstr(0,maxx-len(dt),dt)
    if keys:
      key = ord(keys[0])
      keys = keys[1:]
    else:
      key = scr.getch()
    if key < 0: continue
    msgtop = 'Key: 0x'+'%x'%key
    okey = key
    key  = chr(key)
   
    msg = 'Continue on...'

    if key in ('d','D'): # Deal...
      for s in range(0,len(stacks)):
        stack = stacks[s]
        dealOne(decks,stack)
        if stack and stack[-1].cardno < 0:
          stack[-1].cardno = -(stack[-1].cardno)
      renderAll(decks,stacks,msg)
      screen.refresh()
      continue

    left  = [ord('L'),ord('l'),0x104,ord(ld_larr)]
    right = [ord('R'),ord('r'),0x105,ord(ld_rarr)]
    if curSelected and (okey in left or okey in right):
      card = curSelected
      lr   = card.getLeftRight()
      next = lr[0] if okey in left else lr[1]
      if next:
        curSelected   = next
        next.selected = True
        card.selected = False
      elif type(scr) != StrScreen:
        curses.beep()

    up   = [ord('U'),ord('u'),0x103,ord(ld_uarr)]
    down = [                  0x102,ord(ld_darr)]
    if curSelected and (okey in up or okey in down):
      card = curSelected
      ud   = card.getUpDown()
      next = ud[0] if okey in up else ud[1]
      if next:
        curSelected   = next
        next.selected = True
        card.selected = False

    renderAll(decks,stacks,msgbot=msgbot,msgtop=msgtop)

  if key in ('b','B'):
    obreak()

  #scr.clear()
  #scr.refresh()
  #time.sleep(3)

#------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

  if 0:

    wrapper(main)

  else:

    screen = StrScreen()
    main(screen)

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
