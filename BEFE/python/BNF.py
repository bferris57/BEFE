#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BNF - Managing/manipulating Backus-Naur Form rules
#

import sys
import os
from BNF_errors import *
from BNF_Rules  import *

#===============================================================================
#
# Code: __main__
#

def PrintErrors(rules):

  errors = rules.errors
  if not errors:
    return
    
  for error in errors:
    filename,ex = error
    line = ex.line
    col  = ex.col
    if filename:
      print(filename+': '+str(ex))
    
if __name__ == "__main__":
  
  args = sys.argv[1:]

  rules = Rules()
  for arg in args:
    rules.ProcessFile(arg)

  if rules.errors:
    PrintErrors(rules)
    sys.exit(1)

  rules.Normalise()
  if rules.errors:
    PrintErrors(rules)
    sys.exit(1)
    
  if False:
    print("Combined Rules...")
    PrintRules(rules,2)
    print('')
    print("Rules state: "+rules.GetState())
  if True:
    unref = rules.Unreferenced()
    unref.sort()
    print('')
    print("Unreferenced Rules...")
    print('')
    if not unref:
      print('  **None**')
    else:
      for name in unref:
        print('  '+name)
  if True:
    print('')
    print("Terminal Rules...")
    print('')
    terms = rules.Terminals()
    terms.sort()
    if not terms:
      print('  **None**')
    else:
      for term in terms:
        print('  '+term)
    print('')
    print("Recursive Rules...")
    print('')
    terms = rules.Recursives()
    terms.sort()
    if not terms:
      print('  **None**')
    else:
      for term in terms:
        print('  '+term)
    print('')
    print("Head Recursive Rules...")
    print('')
    terms = rules.HeadRecursives()
    terms.sort()
    if not terms:
      print('  **None**')
    else:
      for term in terms:
        print('  '+term)

  #
  # Execute the grammer a sample input...
  #
  
  if args and args[0] == "../befe.bnf/BEFE.bnf":
  
    class Entry(Struct):
      def __init__(self,rule=None,pos=None):
        Struct.__init__(self,"name,rule,pos")
        self.name = rule.name
        self.rule = rule
        self.pos  = pos

    unref = rules.Unreferenced()
    if len(unref) == 0:
      raise Failed("No unreferenced Rules, don't know where to start")
    if len(unref) > 1:
      raise Failed("Too many unreferenced Rules, don't know where to start")
    
    # Get the wordlist
    #fname = 'BEFE-bnf.bnf'
    fname = '../java/HelloWorld.java'
    data = open(fname).read()
    #wordlist = WordList(fname,data)
    #wlpos = 0
    
    rules = rules.rules
    stack = Stack()
    rulenames = Stack()
    stack.push(Entry(rules[unref[0]],0))
    rulenames.push(unref[0])
    
    # Find first terminal. If none, stop at head...
    while True:
    
      entry = stack[-1]
      rule = entry.rule
      pos  = entry.pos
      # DEBUG...
      #print ("DEBUG: rule = %s, pos = %d"%(repr(rule.name),pos))
      # ...DEBUG
      if pos >= len(rule.words):
        stack.pop()
        rulenames.pop()
        if not stack: break
        entry = stack[-1]
        entry.pos += 1
        continue
      word = rule.words[pos]
      if word.type == word.types.name:
        if word.value in rulenames:
          break
        entry = Entry(rules[word.value],0)
        stack.push(entry)
        rulenames.push(entry.name)
        continue
      if word.IsTerminal():
        break
      entry.pos += 1
      
    # Print them...
    for i in range(0,len(stack)):
      entry = stack[i]
      rule = entry.rule
      pos = entry.pos
      if pos >= len(rule.words):
        s = '***END***'
      else:
        s = str(rule.words[pos])
        if rule.words[pos].IsTerminal():
          s += ' (Terminal)'
      print('%s[%d]: %s'%('  '*i+FormatName(stack[i].rule), pos, s))
