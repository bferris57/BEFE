#!/usr/bin/ev python3
#coding=utf-8

decks = []

class Deck:

  def __init__(self,random=True):

   self.cards = ['s 1','s 2','s 3','s4','s5','s6','s7','s8','s9','s10','s11','s12','s13']

  def __str__(self):

    s = ''
    for i in range(0,len(self.cards)):
      card = self.cards[i]
      s += card[1:]
      if i < len(self.cards)-1:
        s += ', '
    
    return s
    
if __name__ == '__main__':

  d = Deck()
  print("d = %s"%str(d))
