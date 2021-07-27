#!/usr/bin/python
#coding=utf-8☺☺

#
# Imports
#

from random import randint
from enum   import Enum
#
# Globals
#

decs = [[],[],[],[],[],[],[],[],[]]


#suites = '♠♣♥♦'
suites = 'schd'
Suites = Enum("spades=s,clubs=c,hearts=h,diamonds=d")
icons  = 'A234567890JQK'

#-------------------------------------------------------------------------------
#
# Class: Deck - A deck of cards with optional suite
#

class Deck:

  def __init__(self,suite=None):
  
    self.cards = [Card(x,suite) for x in icons]
    self.suite = suite
    if suite:
      for i in range(0,len(self.cards)):
        self.cards[i].suite = suite

  def shuffle(self):
   
    newCards = []
    while self.cards:
      idx = randint(0,len(self.cards)-1)
      newCards.append(self.cards[idx])
      del self.cards[idx]
    self.cards = newCards

  def __str__(self):
  
    s = ''
    for card in self.cards:
      if s: s += ' '
      s += str(card)
      
    return s
    
#-------------------------------------------------------------------------------
#
# Class: Card - A single card
#

class Card:

  def __init__(self,icon,suite=None):
  
    if icon not in icons:
      raise valueError("Icon %s invalid"%repr(icon))
    if suite and suite not in suites:
      raise valueError("Suite %s invalid"%repr(suite))
      
    self.icon  = icon
    self.suite = suite
    
  def compare(self,that):
    
    if type(that) != type(this):
      raise TypeError("Expected 'that' to be type 'Card'")
    thisIndex = icons.index(self.icon)
    thatIndex = icons.index(that.icon)
    if thisIndex < thatIndex: return -1
    if thisIndex > thatIndex: return 1
    return 0
    
  def __str__(self):
  
    icon = self.icon if self.icon else '?'
    icon = icon if icon != '0' else '10'
    return icon + ('' if not self.suite else self.suite)
    
  def __lt__(self,that): return this.compare(that) <  0
  def __le__(self,that): return this.compare(that) <= 0
  def __ge__(self,that): return this.compare(that) >= 0
  def __gt__(self,that): return this.compare(that) >  0
  def __eq__(self,that): return this.compare(that) == 0
  def __ne__(self,that): return this.compare(that) != 0
  
  def __nonzero__(self): return type(this.icon) == str and this.icon
    
#-------------------------------------------------------------------------------
#
# Class: Column - A displayable, variable length column
#

class Column:

  def __init__():
  
    self.cards = []
    
  def canAdd(self,cards):
  
    if not cards or not self.cards:
      return True
    
    lastInCol    = self.cards[-1]
    #firstInCards = ???
    
#-------------------------------------------------------------------------------
#
# Class: Spider - Main Spider Solitair Game State
#

class Spider:

  def __init__():
  
    self.numDecks = 8  # Number of Decks to
    self.numCols  = 10 # Number of columns to deal into
    self.decks    = []
    self.columns  = []
    self.solved   = []
    
    # Deal the decks...
    for i in range(0,self.numDecks):
      deck = Deck()
      deck.shuffle()
      self.decs.append(deck)
      
    # Populate initial column values...
    for i in range(0,4):
      d = self.decs.pop()
      #for 

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

  import sys
  
  deck = Deck(Suites.clubs)
  s = "Cards BEFORE shuffle: %s\n"%str(deck)
  deck.shuffle()
  print(s+"Cards AFTER  shuffle: %s\n"%str(deck))
  sys.stdout.flush()
