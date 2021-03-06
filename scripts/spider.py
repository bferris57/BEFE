#!/usr/bin/env python3
#  coding=utf-8

import                    time
import                    curses
import                    random
import                    sys
from   curses      import wrapper
from   funcs       import dtNow
from   funcs       import dtToNonce
from   funcs       import dtToReadable
from   funcs       import red,blue,green
from   funcs       import postEllipse
from   funcs       import execute
from   errors             import *
from   strscreen   import StrScreen
from   strscreen   import Point
from   strscreen   import Rect
from   strscreen   import setkeys
from   strscreen   import debugon
from   strscreen   import debugoff

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
ld_deck    = '─A23456789TJQK'

maxx = None
maxy = None

top    = '┌───┐'
topvis = top 
topnum = '┌─x─┐'
top10  = '┌─10┐'
mid    = '│   │'
midnum = '│ x │'
mid10  = '│ 10│'
bot    = '└───┘'

allCards    = []
curSelected = None
msgbot      = ''
msgtop      = ''

#
# Debugging...
#

debug = 0
out   = None

keys = ''
debugoff()

def oprint(msg,end='\n'):
  if not out or out.closed:
    return
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

def beep(duration=1):

  freq = 880*2
  res,sout,serr = execute('aplay -d {} {}'.format(duration, 'beep.wav'))
  if res and debug:
    oprint('DEBUG: beep: res = %s'%repr(res))
    oprint('       sout = %s'%repr(sout))
    oprint('       serr = %s'%repr(serr))

#---
#
# Various classes...
#
#---

class Decks(list): # A set of decks...

  def __init__(self):

    super(Decks,self).__init__(self)
    self.pos = Point()

  def __str__(self):

    return str(len(self))+'@'+str(self.pos)

  def __repr__(self):

    s = '['
    for i in range(0,len(self)):
      if i: 
        s += ','
      s += 'Deck()'
    s += ']'

    return s

  def append(self,card):

    if type(card) != Deck:
      raise Exception('Decks.append expects single instance of Deck')
    super(Decks,self).append(card)

  def extend(self,stuff):

    raise Exception('Decks.expand not implemented')

class Deck(list): # A single deck of cards (may be empty)...

  def __init__(self):

    super(Deck,self).__init__(self)
    self.pos   = Point()

  def __str__(self):

    s = '['
    for i in range(0,len(self)):
      if i: s += ','
      s.append(self[i].cardno)
    s += '] @ '+repr(self.pos)

    return s

  def __repr__(self):

    s = 'Deck('
    for i in range(0,len(self)):
      if i: s += ','
      s += repr(self[i])
    s += ')'    

    return s
  
  def deal(self):

    if not self.cards:
      return None

    card = self.cardsr[-1]
    del cards[-1]

    return card

  def renderRect(self):

    tl = self.pos.clone()
    br = self.pos.clone()
    br.y += len(self)-1

    return Rect(tl,br)
    
class Card(Rect): # A single, possibly dealt card

  def __init__(self,cardno,suit=None):

    super(Card,self).__init__()
    self.br = Point(4,6)

    if not isinstance(cardno,int):
      raise InternalError('Card() expected cardno to be an int()')
    if suit != None and not isinstance(suit,int):
      raise InternalError('Card() expected suit to be an int()')
    if cardno < -13 or cardno > 13:
      raise InternalError('Card() only accepts cards in range -13..13')

    self.cardno   = cardno
    self.suit     = suit
    self.selected = False

  def __str__(self):

    s = 'Card('+str(self.cardno)
    if self.suit != None:
      s += ','+str(self.suit)
    s += ')'

    return s

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

  def renderRect(self):

    return Rect(self.tl,self.br)

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

  y = 1
  x = 0
  for s in range(0,len(stacks)):
    renderDeck(y,x+s*7,stacks[s])
    if debug and s == 0:
      oprint('DEBUG: renderAll: s = 0, scr.rows[1] = %s'%scr.rows[y].strip())

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

def dealOne(decks,stack):

  if not decks:
    return
  card = decks[-1][-1]
  stack.append(card)
  del decks[-1][-1]
  if not decks[-1]:
    del decks[-1]

#---
#
# main()
#
#---
      
def main(screen):

  global scr
  global maxx
  global maxy
  global curSelected
  global msgbot
  global msgtop
  global keys
  global out

  scr = screen
  if type(scr) != StrScreen:
    oprint('DEBUG: type(scr) = %s'%repr(type(scr)))
    scr = StrScreen(None if debug else screen,output=out if not debug else sys.stdout)

  scr.timeout(2000)
  if screen and type(screen) != StrScreen:
    curses.curs_set(0)

  maxy, maxx = scr.getmaxyx()

  oprint('DEBUG: main(): maxy = %d, maxx = %d'%(maxy,maxx))

  # Create randomly shuffled decks...
  onedeck = [ -1, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11, -12, -13]
  numdecks = 8
  allcards = []
  for i in range(0,numdecks):
    allcards.extend(onedeck)
  allcards = shuffle(allcards)
  decks = []
  while allcards:
    thesecards = allcards[0:13]
    allcards   = allcards[13:]
    thisdec = Deck()
    for cardno in thesecards:
      thisdec.append(Card(cardno))
    decks.append(thisdec)

  # Deal stacks...
  numstacks = 10
  stacks = []
  for i in range(0,numstacks):
    stacks.append(Deck())

  cardno = 0
  for d in range(0,6):
    while cardno < 54:
      dealOne(decks,stacks[cardno%len(stacks)])
      cardno += 1

  selected = False
  for s in range(0,len(stacks)):
    stack = stacks[s]
    print('DEBUG: len(stacks[%d]) = %d'%(s,len(stack)))
    print('DEBUG: type(stack) = %s'%repr(type(stack)))
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
  scr.addstr(maxy-1,0,'Press \'q\' or \'x\' to exit...')

  #
  # Do it...
  #

  key = ''

  scr.refresh()

  while key not in ('q','Q','x','X',chr(0x03)):

    dt = dtToReadable(dtNow())
    scr.addstr(0,maxx-len(dt),dt)
    key = scr.getch()
    if key < 0:
      if debug:
        break
      continue

    okey = key
    key  = chr(key)
   
    if debug:
      oprint('DEBUG: Key = 0x%x = %s'%(okey,repr(key)))

    msg = 'Continue on...'

    if key in ('d','D'): # Deal...
      for s in range(0,len(stacks)):
        stack = stacks[s]
        dealOne(decks,stack)
        if stack and stack[-1].cardno < 0:
          stack[-1].cardno = -(stack[-1].cardno)
      renderAll(decks,stacks,msg)
      scr.refresh()
      continue

    left  = [0x104,ord(ld_larr)]
    right = [0x105,ord(ld_rarr)]
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

    up   = [0x103,ord(ld_uarr)]
    down = [0x102,ord(ld_darr)]
    if curSelected and (okey in up or okey in down):
      card = curSelected
      ud   = card.getUpDown()
      next = ud[0] if okey in up else ud[1]
      if next:
        curSelected   = next
        next.selected = True
        card.selected = False

    if key in ('b','B'):
      obreak()

    if key in ('c','C'):
      scr.refresh()

    if key in ('p','P'):
      if screen:
        curses.beep()
      if debug:
        oprint('DEBUG: Calling _dump()...')
      scr._dump('Key press...')

#------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

  doMain  = True
  doTests = False

  if 1:
    args = sys.argv[1:]
    while args:
      arg = args[0]
      if arg == '+d':
        debug = True
      elif arg == '-d':
        debug = False
      elif arg.endswith('.out'):
        out = open(arg,'w')
      elif arg == '-m':
        doMain = False
      elif arg == '+m':
        doMain = True
      elif arg == '-t':
        doTests = False
      elif arg == '+t':
        doTests = True
      else:
        oprint('Argument %s unknown'%repr(arg))
        sys.exit(1)
      args = args[1:]

    if doMain:
      if debug:
        #keys = 'dd'+ld_darr+ld_darr+ld_uarr+'b'+ld_uarr+'q'  # +'q'
        #keys = 'dddpq'
        debugon(out)
        setkeys(keys)
        main(None)
      else:
        debugoff()
        setkeys('')
        wrapper(main)

  if not doMain and not doTests:
    pout('Nothing to do, try \'+m\' or \'+t\'')

  if not doTests:

    sys.exit(0)

  if 0:

    deck = shuffle([1,2,3,4,5,6,7,8,9,10,11,12,13])
    oprint('deck = %s'%repr(deck))
      
  if 0:

    try:
      raise(InternalError('Testing InternalError()'))
    except InternalError as e:
      red(str(e))

  if 0:

    r = Rect(Point(25,80),Point(0,0))
    oprint('r      = %s'%str(r))
    oprint('r.area = %s'%str(r.area()))

    ex = Point()

  if 1:

    deck = Deck()

    for cardno in range(1,14):
      card = Card(cardno)
      deck.append(card)

    print('Deck: %s'%repr(deck)) 

    decks = Decks()
    decks.append(deck)
    print('Decks: %s'%repr(decks))
    print('')
    print('card.renderRect():  %s'%repr(card.renderRect()))
    card.moveto(Point(10,10))
    print('card.moveto(10,10): %s'%repr(card.renderRect()))
    print('card.width():       %s'%repr(card.width()))
    print('card.height():      %s'%repr(card.height()))
    print('deck.renderRect():  %s'%repr(deck.renderRect()))
