#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:      BNF_Rules - BNF Rule and Rules classes and related stuff
#
# Classes:   Rules - Maintain a list of BNF Rule instances
#
# Functions: _FileRules          - Populate name:Rule map of Rule instances from a BNF file
#            _RuleValidate       - Validate a Rule
#            _RuleValidateExists - Validate all Rules referenced by a Rule exist
#            _RuleNormalise      - Normalise a Rule declaration from it's constituent Words
#            _RuleReferenced     - Calculate Rule.referencedby for each Rule
#            _RuleUnreferenced   - Return list of unreferenced Rule.name
#            _RuleRecursion      - Calculate recursion words for Rules
#            FormatOccur         - Format a rule into a string
#            FormatName          - Format a rule name into a string
#            PrintRule           - Print a single rule
#            PrintRules          - Print all Rules in a Rules/dict instance

import sys
import os
from BNF_errors    import *
from BNF_Struct    import *
from BNF_Enum      import *
from BNF_ElmDef    import *
from BNF_FileWords import *
from BNF_Word      import *
from BNF_WordList  import *
from BNF_Range     import *
from BNF_Rule      import *
from BNF_Stack     import *

# Python 2.7 has type long, Python 3.x doesn't...
if sys.version_info.major >= 3:
  long = int
           
#===============================================================================
#
# Class:    Rules
#
# Purpose:  Maintain a list of BNF Rule instances
#
# Usage:    rules = Rules(filename=None)
#
# Where:    filename - str:   Name of file containing BNF declarations
#
#           rules    - Rules: Set of Rules
#

class Rules():

  # Here instead of Rule itself because 'type' fowls thigs up
  typenames = ['TERM','OR','AND','TYPE','TOKWN','ACTION'] # Named rule types
  types     = Enum(','.join(typenames))    # Numbered rule types
  startops = "([{"
  endops = ")]}"
  anonID = 0       # Used to generate anonymous rules during normalisation


  def __init__(self,filename=None,rules=None):
  
    self.rules      = {}
    self.errors     = []
    self.isvalid    = None
    self.statenames = ["null","normalised"]
    self.states     = Enum(','.join(self.statenames))
    self.state      = self.states.null
    self.files      = {}
    if filename:
      self.errors = self.ProcessFile(filename)
      
  def __index__(self,idx):
  
    if type(idx) != str:
      raise AttributeError("Rules can only be indexed by name")
    if not self.rules.has_key(idx):
      return None
    return self.rules[idx]

  def GetState(self):

    return self.statenames[self.state]
    
  @staticmethod
  def NewAnonymous(rule):

    rule = Rule(filename=rule.filename,rules=rule.rules)
    Rules.anonID += 1
    rule.name = "_anon%d"%Rules.anonID
    return rule
  
  def ProcessFile(self,filename):

    # State check...
    if self.state != self.states.null:
      raise Failed("Rules.ProcessFile() can only be called in the 'null' state")

    # If already processed, remove rules already there...
    if filename in self.files:
      raise Failed("File %s already processed"%filename)
      
    errors = []
    rules = {}
      
    try:
      if not os.path.exists(filename):
        raise SyntaxError(-1,-1,"File "+repr(filename)+" does not exist")
      if os.path.isdir(filename):
        raise SyntaxError(-1,-1,"File "+repr(filename)+" is a directory")
      data = open(filename).read()
      self.files[filename] = data
      print("File "+repr(filename)+"...")
      
      # Get rule declarations...
      rules = _FileRules(filename,data)
      #for rule in rules:
      #  rule.filename = filename
      
      # Basic validation...
      names = rules.keys()
      names.sort()
      for name in names:
        rule = rules[name]
        if self.rules.has_key(rule.name):
          w = rule.words[0]
          errors.append(SyntaxError(w.line,w.col,"Rule %s already defined"%repr(rule.name)))
        else:
          errs = _RuleValidate(rules,rule)
          if errs:
            errors.extend(errs)

    except SyntaxError as s:
      self.errors.append((filename,s))

    # Merge rules into self.rules if no errors...
    if not errors:
      for name in rules:
        self.rules[name] = rules[name]
        self.rules[name].rules = self.rules

    # Merge errors into self.errors...
    for err in errors:
      self.errors.append((filename,err))
      
    # Back in null state
    self.state = self.states.null
      
    return errors

  #
  # Method: Normalise - Transit from "null" to "normalised" state
  # 
  # Notes:  Called after all files processed
  #
  
  def Normalise(self):

    if self.state != self.states.null:
      if self.errors:
        raise Failed("Rules has errors")
      else:
        raise Failed("Rules.Normalise() invalid unless in 'null' state")
      
    rules = self.rules
    names = [i for i in rules]
    names.sort
    errors = []

    # Make sure everything's declared...
    for name in rules:
      errs = _RuleValidateExists(rules,rules[name])
      errors.extend(errs)
        
    # Normalise the Rules...
    if not errors:
    
      names = list(rules)
      names.sort()
      errs = []
      while names:
        rule = rules[names[0]]
        del names[0]
        _RuleNormalise(rules,rule)
      
      # UnReferenced...
      _RuleReferenced(self.rules)

      # Recursion checks...
      _RuleRecursion(rules)

    # Handle any errors...
    if errors:
      if not self.errors:
        self.errors = errors
      else:
        self.errors.extend(errors)
      self.state = self.states.null
    else:
      self.state = self.states.normalised

    return self.errors
    
  def Unreferenced(self):
  
    return _RulesUnreferenced(self.rules)

  def Terminals(self):
  
    terms = []
    for name in self.rules:
      if not self.rules[name].References():
        terms.append(name)
    return terms
    
  def Recursives(self):
    terms = []
    for name in self.rules:
      if self.rules[name].IsRecursive():
        terms.append(name)
    return terms
      
  def HeadRecursives(self):
    terms = []
    for name in self.rules:
      if self.rules[name].IsHeadRecursive():
        terms.append(name)
    return terms
      
#===============================================================================
#
# Function: _FileRules
#
# Purpose:  Populate name:Rule map of Rule instances from a BNF file
#
# Usage:    rules = _FileRules(data)
#
# Where:    data - str: File's entire text content
#
#           rules - map: Map of name:Rule instances
#
# Note:     Any errors encountered will raise a SyntaxError exception
#

def _FileRules(filename,data):

  rules = {}
  
  #
  # For each input Word...
  #
  
  curname = ''
  defseq = -1

  wordlist = WordList(filename,data,True)
  wordidx = -1
  skipcount = 0
  brackcount = 0
    
  for word in wordlist:

    wordidx += 1 # Index (0..n) of current word in index
    defseq  += 1 # Index (0..n) of current word in declaration

    # Skip if we're supposed to...
    if skipcount:
      skipcount -= 1
      continue
  
    # See if we're starting a new declaration
    prvword = wordlist[wordidx-1]
    nxtword = wordlist[wordidx+1]
    newdecl = False
    if not brackcount:
      if not prvword:
        newdecl = True
      elif prvword.type == Word.Type.operator and prvword.value == ';':
        newdecl = True
      elif word.type == Word.Type.name:
        if nxtword and nxtword.type == Word.Type.operator and nxtword.value == '::=':
          newdecl = True
           
    # Count curly brackets...
    if word.type == Word.Type.operator:
      if word.value in '{([':
        brackcount += 1
      elif word.value in '})]':
        brackcount -= 1
        
    # Can pretty much ignore ';' now that we've checked above...
    if word.type == Word.Type.operator and word.value == ';':
      if not brackcount:
        defseq = -1
      continue
      
    # Handle '<name> ::='
    if newdecl:
   
      # Validate start of declaration   
      if word.type != Word.Type.name:
        raise SyntaxError(word.line,word.col,"Expected name here")
      if nxtword.type != Word.Type.operator or nxtword.value != '::=':
        raise SyntaxError(nxtword.line,nxtword.col,"Expected '::=' here")
      if rules.has_key(word.value):
        raise SyntaxError(word.line,word.col,"Rule '%s' already declared'"%word.value)

      # Start a new Rule
      rule = Rule(filename=filename,rules=rules)
      rule.name = word.value
      rules[rule.name] = rule
      curname = rule.name
      skipcount = 1
      brackcount = 0
            
      continue
      
    # Make sure not an embedded '::=' somewhere...
    if word.type == Word.Type.operator and word.value == '::=':
      raise SyntaxError(word.line, word.col, "Unexpected start of declaration here")
      
    # Everything else just gets appended to the current rule
    rules[curname].words.append(word)
      
  return rules

#===============================================================================
#
# Function: _RuleValidate
#
# Purpose:  Validate a Rule
#
# Usage:    errors = _RuleValidate(rules,rule)
#
# Where:    rules - map:  Rule instances by name
#           rule  - Rule: Rule to Validate
#
#           errors - List: List of errors
#
# Notes:    Special case of '(' quote ')' is handled here by turning the Rule
#           into a Rules.types.OR and splitting it into N <char>|<char>...
#           words.
#
#           Additionally, empty "[]", "{}", and "()" entries are removed.
#
# 

def _RuleValidate(rules,rule):

  #
  # Function: QuoteSplit - Special handy case of '(' quote ')'
  #
  # Note: We'll turn the rule into a set of entries for each character
  #
  def QuoteSplit(rule):
    words = rule.words
    if len(words) == 3:
      if     words[0].type == Word.Type.operator and words[0].value == '(' \
         and words[2].type == Word.Type.operator and words[2].value == ')' \
         and words[1].type in [Word.Type.dquote,Word.Type.squote]:
        val = words[1].value
        rule.type = Rules.types.OR
        rule.minoccur = 1
        rule.maxoccur = 1
        rule.recursion = None
        rule.referencedby = None
        word = rule.words[1]
        rule.words = []
        word.lineseq += 1
        word.len = 1
        for i in range(1,len(val)-1):
          w = copy.deepcopy(word)
          w.col = word.col+i
          w.soff = word.soff+i
          w.eoff = word.soff+i+1
          w.len = 1
          w.value = val[i]
          rule.words.append(w)
        return True
    return False

  #
  # Validation...
  #
  
  errors = []
  
  # Check for unterminated/mismatched brackets, parens, etc...
  delims = ['(',')',
            '{','}',
            '[',']',
            ]
  others = ['|',';','..']
  stack = []
  pos = -1
  words = rule.words

  while True:
    pos += 1
    if pos >= len(words):
      break
    word = words[pos]
    if word.type != Word.Type.operator:
      continue
    if word.value not in delims:
      if word.value not in others or \
         (pos and words[pos-1].type == Word.Type.operator and words[pos-1].value not in '}])'):
        errors.append(SyntaxError(word.line,word.col,"Misplaced operator '"+word.value+"'"))
      # Validate <hex>..<hex> range
      if word.value == '..':
        if pos == 0:
          errors.append(SyntaxError(word.line,word.col,"Misplaced '..'"))
          continue
        if pos == len(words)-1:
          errors.append(SyntaxError(word.line,word.col,"Dangling '..'"))
          continue
        oktypes = [Word.Type.hex, Word.Type.int, Word.Type.dquote,Word.Type.squote]
        if words[pos-1].type not in oktypes:
          errors.append(SyntaxError(words[pos-1].line,words[pos-1].col,"Expected quote, int, or hexidecimal constant here, got %s"%repr(Word.Type.names[words[pos-1].type])))
        if words[pos+1].type not in oktypes:
          errors.append(SyntaxError(words[pos+1].line,words[pos+1].col,"Expected quote, int, or hexidecimal constant here, got %s"%repr(Word.Type.names[words[pos+1].type])))
        # Combine range words...
        tword = copy.deepcopy(words[pos-1])
        tword.eoff = words[pos+1].eoff
        tword.type = Word.Type.range
        tword.value = Range(words[pos-1].value,words[pos+1].value)
        if tword.value.min is None:
          errors.append(SyntaxError(words[pos-1].line,words[pos-1].col,"Malformed Range Minimum Value"))
        if tword.value.max is None:
          errors.append(SyntaxError(words[pos+1].line,words[pos+1].col,"Malformed Range Maximum Value"))
        tword.len   = len(str(tword.value))
        words[pos].type = Word.Type.range
        words[pos].value = tword
        del words[pos-1:pos+2]
        words.insert(pos-1,tword)
      
      # Finished with this one...
      continue
    
    dpos = delims.index(word.value)
    if dpos%2 == 0:
      stack.append(word)
    else:
      if not stack:
        errors.append(SyntaxError(word.line,word.col,"Found '"+word.value+"' without preceeding '"+delims[dpos-1]+"'"))
        continue
      prev = stack.pop()
      if prev.value != delims[dpos-1]:
        errors.append(SyntaxError(word.line,word.col,"Mismatched '"+word.value+"'"))
        stack.append(prev)

  # Save the words in case Range modified it...
  rule.words = words
    
  if stack:
    for word in stack:
      errors.append(SyntaxError(word.line,word.col,"Unterminated delimeter '%s'"%word.value))

  #QuoteSplit(rule)
    
  return errors

#===============================================================================
#
# Function: _RuleValidateExists
#
# Purpose:  Validate all Rules referenced by a Rule exist
#
# Usage:    errors = _RuleValidateExists(rules,rule)
#
# Where:    rules - map:  Rule instances by name
#           rule  - Rule: Rule to Validate
#
#           errors - List: List of errors
#

def _RuleValidateExists(rules,rule):

  errors = []
  # Check for undeclared rules
  for word in rule.words:
    if word.type == Word.Type.name and not rules.has_key(word.value):
      errors.append((rule.filename,SyntaxError(word.line,word.col,"Term '%s' not declared"%word.value)))
      
  return errors

#===============================================================================
#
# Function: _RuleNormalise
#
# Purpose:  Normalise a Rule declaration from it's constituent Words
#
# Usage:    _RuleNormalise_1(rules, rule)
#
# Where:    rules - map:  Rule instances by name
#           rule  - Rule: Rule to Finalise
#
# Notes:    What this function does is break a structured rule, recursively, 
#           into flat "AND/OR" and "[]" "()" word lists containing either name,
#           range, or quote types.
#
#           We assume the Rule has already been validated so (), [], and {} pairs
#           will all be matched okay.
#

def _RuleNormalise(rules,rule):

  # Anonymise Slice...  
  def _AnonymiseSlice(rule,spos,epos):

    rules = rule.rules
    
    # Create anonymous Rule containing the words...
    arule = Rules.NewAnonymous(rule)
    arule.words = rule.words[spos:epos]
    rules[arule.name] = arule
    
    # Replace those words with a reference to this rule and update stack...      
    del rule.words[spos:epos]
    word = Word()
    word.filename = arule.filename
    word.type  = word.types.name
    word.len   = len(arule.name)
    word.value = arule.name
    rule.words.insert(spos,word)

    _SetOccurs(arule)
    
    return arule

  # Set Rule Occurrences and do some tidy up...
  def _SetOccurs(rule):

    # if not '[...]' or '(...)', not much to do...
    if not rule.words or rule.words[0].type != Word.Type.operator  or rule.words[0].value not in '[(':
      if rule.type is None:
        rule.type = Rules.types.AND
      return
    
    # Handle '[...]'
    if rule.words[0].value == '[':

      rule.words = rule.words[1:-1]
      rule.minoccur = 0
      rule.maxoccur = 1
      
      return
      
    # Handle '(...)'
    if rule.words[0].value == '(':

      # Make a fake OR word, just in case
      orword = copy.deepcopy(rule.words[0])
      orword.value = '|'
      
      # Get rid of '(...)'
      words = rule.words[1:-1]
      
      # Handle special '("...")' case...
      if len(words) == 1 and words[0].type in [Word.Type.dquote,Word.Type.squote]:
        nwords = []
        for i in range(1,len(words[0].value)-1):
          c = words[0].value[i]
          word = copy.deepcopy(words[0])
          word.col = words[0].col+i
          word.value = repr(c)
          word.len = len(word.value)
          if nwords:
            nwords.append(orword)
          nwords.append(word)
        rule.words = nwords
        return
      
      # "Normal case" of "(...)"

      rule.type = Rules.types.OR      
      rule.minoccur = 1
      rule.maxoccur = None
      rule.words = rule.words[1:-1]
    
    return
      
      
  # Stack Entries...
  class Entry(Struct):
    def __init__(self,rule=None,spos=None,pos=None):
      Struct.__init__(self,"rule,spos,pos")
      self.rule = rule
      self.spos = spos
      self.pos  = pos

  #
  # Start of code...
  #

  # Initialise stack...
  stack = Stack()
  stack.push(Entry(rule,0,0))
  startops = "([{"
  endops   = ")]}"

  # Until stack is empty...
  while stack:

    # Update top entry
    entry = stack[-1]
    rule = entry.rule
    spos = entry.spos
    pos = entry.pos
    
    # See if finished with last word...
    if pos >= len(rule.words):

      # Get rid of top entry
      entry = stack.pop()
      rule = entry.rule
      
      # Set its type to AND if not already set
      #if rule.type is None:
      #  rule.type = Rules.types.AND
      #  rule.minoccur = 1
      #  rule.maxoccur = 1
        
      # If it's an OR type anonymise dangling words if more than one...
      if rule.type is Rules.types.OR and pos-spos > 1:
        arule = _AnonymiseSlice(rule,spos,pos)
        if arule is None:
          raise "Here it is"
        #arule.type = Rules.types.AND

      _SetOccurs(rule)
      
      continue
      
    # Handle "(", "[", and "{"...
    word = rule.words[pos]
    sopidx = -1
    if word.type == Word.Type.operator:
      sopidx = startops.find(word.value)      
    
    if word.type == Word.Type.operator and sopidx >= 0:

      # Update TOS...
      opos = pos      
      pos += 1
      stack[-1].pos  = pos
      
      # Find matching end op...
      depth = 1
      while pos < len(rule.words):
        word = rule.words[pos]
        if word.type == Word.Type.operator:
          if word.value in startops:
            depth += 1
            pos += 1
            continue
          eopidx = endops.find(word.value)
          if eopidx >= 0:
            depth -= 1
            if depth == 0:
              break
        pos += 1

      # Replace those words with a reference to this rule and update stack...      
      arule = _AnonymiseSlice(rule,opos,pos+1)
      stack[-1].pos = opos+1
      stack.push(Entry(arule,0,0))

      continue

    # Handle '|'...
    if word.type == Word.Type.operator and word.value == '|':

      # Anonymise preceeding words if more than 1...
      if pos-spos > 1:
        arule = _AnonymiseSlice(rule,spos,pos)
        arule.type = Rules.types.AND
        pos = spos+1

      # Set rule type and remove the '|'...
      rule.type = Rules.types.OR
      del rule.words[pos]
      entry.spos = pos
      entry.pos = pos
      stack[-1] = entry

      continue
            
    # Not operator, ignore it... 
    stack[-1].pos += 1    

  return

#===============================================================================
#
# Function: _RuleReferenced
#
# Purpose:  Calculate Rule.referencedby for each Rule
#
# Usage:    RuleReferenced(rules)
#
# Returns:  None
#
# Where:    rules - map:  Rule instances by name
#
# Notes:    This function should only be called AFTER a Rule has been properly
#           normalised and missing Rule declarations have been determined.
#

def _RuleReferenced(rules):

  # Initialise everything
  for rulename in rules:
    rules[rulename].referencedby = []
  
  # For each one...
  for rulename in rules:
    rule = rules[rulename]
    for word in rule.words:
      if rules.has_key(word.value):
        thatrule = rules[word.value]
        if rule.name not in thatrule.referencedby:
          thatrule.referencedby.append(rule.name)

#===============================================================================
#
# Function: _RulesUnreferenced
#
# Purpose:  Return list of unreferenced Rule.name
#
# Usage:    unref = _RuleUnreferenced(rules)
#
# Returns:  None
#
# Where:    rules - map:  Rule instances by name
#
#           unref - list: List of unreferenced Rules, by name
#
# Notes:    This function should only be called AFTER a Rule has been properly
#           normalised and missing Rule declarations have been determined.
#
#           This function should only called AFTER RuleReferenced() has
#           been invoked.
#

def _RulesUnreferenced(rules):

  uref = []
  for rulename in rules:
    rule = rules[rulename]
    if not rule.referencedby:
      uref.append(rule.name)
  return uref

#===============================================================================
#
# Function: _RuleRecursion
#
# Purpose:  Calculate recursion words for Rules
#
# Usage:    _RuleRecursion(rules)
#
# Returns:  None
#
# Where:    rules - map: Rule instances by name
#
# Notes:    This function should only be called AFTER a Rule has been properly
#           normalised and reference counted (see above).
#
#           This function will BREAK horribly if we haven't already checked
#           for missing Rule declarations.
#

def _RuleRecursion(rules):

  # Clear all Recursion lists
  for name in rules:
    rules[name].recursion = None
    
  # For each rule...  
  for head in rules:

    stack = []
    offsets = {}
    stack = [head]
    offsets[head] = -1
    rules[head].recursion = []
  
    while stack:

      rulename = stack[-1]
      rule = rules[rulename]
      offset = offsets[rulename]
      offset += 1
      offsets[rulename] = offset      
      
      # Finished with this one if we're past end of its words...
      if offset >= len(rule.words):
        stack.pop()
        continue

      # If the word isn't a name (ref), ignore it        
      word = rule.words[offset]
      if word.type != Word.Type.name:
        continue
      
      referenced = rules[word.value]
      isrecursive = None

      # If referenced rule is in offsets but finished processing, that'll
      # tell us if it's recursive or not...
      if referenced.name in offsets:
        if offsets[word.value] >= len(referenced.words):
          if referenced.recursion:
            isrecursive = True
          elif referenced.recursion != None:
            isrecursive = True if referenced.name in stack else False
      
      # If not already on stack and we don't know if it's recursive, process it...
      if referenced.recursion is None:
        referenced.recursion = []
      if referenced.name in stack:
        isrecursive = True
      if referenced.name not in offsets:
        offsets[referenced.name] = -1
        stack.append(referenced.name)
        
      # If recursive, make current position in all words on stack "recursive"
      if isrecursive:
        for i in range(0,len(stack)):
          stkrule = rules[stack[i]]
          idx = offsets[stkrule.name]
          if idx >= 0 and idx not in stkrule.recursion:
            stkrule.recursion.append(idx)

      
  return
  
#===============================================================================
#
# Functions: FormatOccur - Format a rule into a string
#            FormatName  - Format a rule name into a string
#            PrintRule   - Print a single rule
#            PrintRules  - Print all Rules in a Rules/dict instance
#
  
def FormatOccur(rule):
  s = '..'
  if rule.minoccur != None:
    s = str(rule.minoccur)+s
  if rule.maxoccur != None:
    s += str(rule.maxoccur)
  if s == '..':
    s = ''
  return s
  
def FormatName(rule):
  s = rule.name + " ("
  s += "None" if rule.type is None else Rules.typenames[rule.type]
  o = FormatOccur(rule)
  if o:
    s += ' '+o
  s += ')'
  
  return s
   
def FormatWords(rule):
  words = rule.words
  if words is None: return '***None***'
  typeabbr = ["ws","cm","op","tk","nm","ac"]
  s = ''
  for i in range(0,len(words)):
    if s: s += ','
    word = rule.words[i]
    s += str(i)+':'+typeabbr[word.type]+':'+repr(word.value)
  return '['+s+']'

def PrintRule(rule,indent='',maxlen=0):

  if type(indent) in [int,long]:
    indent = ' '*indent

  name1 = FormatName(rule)
  if maxlen < len(name1):
    maxlen = len(name1)

  words = rule.words
  if not words:
    print(indent+name1+':'+' '*(maxlen-len(name1))+" ***EMPTY***")
  else:
    print(indent+name1+':'+' '*(maxlen-len(name1))+' '+str(words[0]))
    if len(words) > 1:
      for word in words[1:]:
        print(indent+' '*maxlen+'  '+str(word))
        if word.type != Word.Type.range and word.len != len(word.value):
          raise InternalError("word.len = %d, word.value = %s"%(word.len,repr(word.value)))

def PrintRules(rules,indent=''):

  if type(indent) in [int,long]:
    indent = ' '*indent

  if type(rules) != dict:
    rules = rules.rules
  
  names = rules.keys()
  names.sort()

  maxlen = 0
  for name in rules:
    name1 = FormatName(rules[name])
    maxlen = max(maxlen,len(name1))
      
  for name in names:
    rule = rules[name]
    print('')
    PrintRule(rule,indent,maxlen)
    if rule.rules is None:
      print(indent+"(rule.rules==None!!!!!)")
    print(indent+' '*maxlen+"  recursion:     "+repr(rule.recursion))
    print(indent+' '*maxlen+"  referenced by: "+repr(rule.referencedby))
    print(indent+' '*maxlen+"  References:    "+repr(rule.References()))

#===============================================================================
#
# __mai