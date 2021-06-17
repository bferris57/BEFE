#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File: BEFE_ParseMethods.py - Extended Parse class methods
#
# Methods: parseFileNode             - Parse a single node's words
#          breakMultipleStatements   - Break Word list into multiple statements
#          isMultipleStatements      - "Is this set of Words multiple statements?"
#          indentChildren            - Indent child Nodes based on Node.indent
#          findEndOfLogicalLine      - In list of Word, find logical statment in given span 
#          findEndOfExpression       - Find end of expression as a Word index
#          deepNodes                 - Return deep list of non-leaf nodes
#          parseExpression           - Parse a single expression node, passes work on a single parent...
#          parseExpressionPass1      -   Expression Pass 1 - Convert Words to Nodes
#          parseExpressionPass2      -   Expression Pass 2 - Flag prefix, infix, and postfix operators
#          parseExpressionPass3      -   Expression Pass 3 - Process ';' and ',' operators
#          parseExpressionPass4      -   Expression Pass 4 - Process grouping specialisations and operator '.' names
#          parseExpressionPass5      -   Expression Pass 5 - Structure all nodes based on operator precedence
#          processOpTOSprefixes      -   Used by Pass 5 to process prefix operator/values
#          processOpTOS              -   Used by Pass 5 to process operator/value precedence
# DEBUG...
#          printNodeStack
# ...DEBUG
#
# Notes:   1) '.' syntax applies to NAMEs only, not types. Types (including structs,
#             unions, classes, etc.) are "imported not dotted". So, we don't really
#             treat '.' operator per say, for now. We handle them by creating a name:'.'
#             node where the children are the different parts of the name.
#
#             For example "a.b.f(x).c" gets turned into a '.' name node with the following children...
#
#               name: 'a'
#               name: 'b'
#               call: 'f(x)'
#               name: 'c'
#
#             Slightly weird but it works out for the best.
#
#          2) This '.' syntax has some, possibly unexpected, side effects...
#
#             The expression: "a.d*f().e" actually means:
#
#               (a.d) * (f().e)
#
#             May seem weird but names are first class objects and '.' has high
#             precedence. So, to get around that, if you wanted "d*f()" to
#             return a 'name' type, you'ld have to express it as "a.(d*f()).e".
#
#             May not be intuitive but it's just HOW IT IS.
#
#          3) Guess what we're trying to do with '.' syntax is give you the most
#             power you can have. Problem is we have a '..' operator used for
#             ranges and spans so we have to deal with that as well. So, some
#             odd cases arise that actually have meaning but are definitely weird...
#
#             Take the following definition...
#
#               class X:
#                 struct a:
#                   a int
#                   b int
#
#             Case: Code declared within the context of class X could use the phrase
#                   "a. .a.b" (Note: ". ." instead of "..".
#
#                   This is because the default namespace for code in X is
#                   the namespace "class X". So, "a" on its own means "the struct".
#                   The phrase "a.a" means the first int but "a.b" means the
#                   second int.
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================
 
from BEFE_Word           import *
from BEFE_Stack          import Stack
from BEFE_errors         import *
from BEFE_Span           import *
from BEFE_ParseStatics   import *
from BEFE_ParseTree      import *
from BEFE_WordList       import *
from BEFE_expression_ops import *
from BEFE_debug          import *

# Word type -> ParseNode type
wordToNodeType = {"DQUOTE":'string',
                  "SQUOTE":'string',
                  "BQUOTE":'name',
                  "INT":'number',
                  "HEX":'number',
                  "NAME":'name',
                   }

# paarseExpression: For debugging purposes but not 'verbose debugging' like in BEFE_debug...
do0 = do1 = do2 = do3 = do4 = do5 = do6 = N
if Y:
  do0 = Y
  do1 = Y
  do2 = Y
  do3 = Y
  do4 = Y
  do5 = Y
  do6 = Y

#===============================================================================
#
# Method: parseFileNode  - Parse a single file node's words
#

def parseFileNode(parse,parent):

  from BEFE_Parse import Parse
  
  if not issubclass(parent.__class__,TreeNode):
    raise InternalError("Expected TreeNode parameter, got %s"%repr(type(node)))
  if parent.type != 'file':
    raise InternalError("Expected 'file' Node")

  startErrorCount = 0 if not parse.errors else len(parse.errors)
  parse.styleStack.push(Style.Freeform)
  tree = parent.tree
  
  wl = WordList(parse.inFileName,parse.data)
  words = list(wl.list)

  # TEMP hack, because input may come from BEFE...
  if words:
    parse.inFileName = words[0].filename
  else:
    parse.inFileName = None
  # ...TEMP  
  
  # Clear Parse flags first...
  parent.clearFlagsDeep()
  eos = -1
  logicalLines = []
  while eos < len(words):
    eos += 1
    span = Span(eos,len(words))
    breaker,span = parse.findEndOfLogicalLine(words,span)
    if len(span) > 0:
      logicalLines.append((span,breaker))
    if span.max == len(words):
      break
    eos = span.max-1
  if parse.errors and len(parse.errors) != startErrorCount:
    return

  # TEMP...
  if not do0 or (debug and verbose0):
    idx = -1
    for ll in logicalLines:
      span,breaker = ll
      idx += 1
      wstr = ''
      for i in range(span.min,span.max):
        if wstr: wstr += ' '
        wstr += words[i].value
      if breaker is None:
        bstr = None
      else:
        bstr = words[breaker].value
      print("parseFileNode: logLine[%d]: breaker = %s, span = %s, words = %s"%(idx,repr(bstr),str(span),repr(wstr)))
      if not do0:
        return
  # ...TEMP
  
  #
  # Subpass 1: Create unnamed/untyped nodes for logical line statements...
  #
  
  idx = -1
  while idx < len(logicalLines)-1:

    idx += 1
    span,breaker = logicalLines[idx]

    # Do some validation
    if breaker is not None and breaker >= len(words):
      breaker = None
    if breaker is not None:
      if words[breaker].type not in (Word.Type.OPERATOR, Word.Type.EOL):
        raise InternalError("Expected word %d to be Word.Type.OPERATOR or EOL, instead: Word is %s"%(breaker,str(words[breaker])))
      if words[breaker].value not in (':','...','\n'):
        raise InternalError("Expected ':' or '...' at %d, got %s instead"%(breaker,repr(words[breaker].value)))
      if breaker == span.min:
        parse.postError(SyntaxError(words[breaker],"Expected statement name, found %s instead"%repr(words[breaker].value)))
        continue

    # Create intermediate node...
    newNode = parse.tree.newNode()
    newNode.setParent(parent)
    if span.min < breaker:
      newNode.word0  = words[span.min]
      if newNode.indent is None:
        newNode.indent = newNode.word0.col
      if newNode.word0.value not in statementDefs:
        parse.postError(SyntaxError(newNode.word0,"Unknown statement %s"%repr(newNode.word0.value)))
        continue
      newNode.setType('stmt')
      newNode.name   = newNode.word0.value
      if newNode.indent is None:
        newNode.indent = newNode.word0.col
      span.min += 1
    if breaker is not None and breaker >= span.min and breaker < span.max:
      newNode.words   = words[span.min:breaker]
      newNode.breaker = words[breaker]
      newNode.moreWords = words[breaker+1:span.max]
    else:
      newNode.words = words[span.min:span.max]
    if newNode.words and not newNode.word0:
      newNode.word0  = newNode.words[0]
      if newNode.indent is None:
        newNode.indent = newNode.words[0].col

    # If is a statement, make sure there is/isn't expression afterwards...
    if newNode.name:
      expTypes, = statementDefs[newNode.name]
      if expTypes is None and newNode.words:
        parse.postError(SyntaxError(newNode.words[0],"Unexpected expression for %s statement"%repr(newNode.name)))
      if expTypes is not None and not newNode.words:
        s = ''
        for i in range(0,len(expTypes)):
          if s:
            if i == len(expTypes)-1 and len(expTypes) != 1:
              s += ', or '
            else:
              s += ', '
          s += repr(expTypes[i])
        parse.postError(SyntaxError(words[breaker],"Expected %s expression here"%s))
      
  #
  # Subpass 2: Process TreeNodes created above turning them into named/typed and
  #            place "moreWords" expressions below each node
  #

  for child in parent.children:

    # Process it...
    words = child.words
    word0 = child.word0
    breaker = child.breaker
    moreWords = child.moreWords
    
    # Set word0...
    if not word0 and child.words:
      child.word0  = child.words[0]
      if child.indent is Nont:
        child.indent = child.words[0].col
      word0 = child.word0
      child.wordN = child.words[-1]
      
    # If it's broken by ':', put words as first expression...
    if breaker and breaker.value == ':':
      if isMultipleStatements(child.words):
        raise InternalError("Uh oh!!! child.words is multiple statements, this shouldn't happen!!!")
      if child.words:
        child.word0 = child.words[0]
        if child.indent is None:
          child.indent = child.word0.col
        child.wordN = child.words[-1]
      exp = parse.tree.newNode()
      exp.setType('stmtexpr')
      exp.setType('ind')
      exp.words = child.words
      child.words = []
      exp.setParent(child)
         
    # Tweak for '...'
    if breaker and breaker.value == '...':
      if not child.words and not child.moreWords:
        parse.postError(SyntaxError(breaker,"Dangling '...'"))
        continue
      if child.words:
        child.setType('type...')
        child.setType('ind')
        child.word0  = child.words[0]
        if child.indent is None:
          child.indent = child.word0.col
        child.name   = child.word0.value
        child.words  = child.words[1:]
        if child.moreWords:
          otherStuff = parse.tree.newNode()
          otherStuff.setType('expr')
          otherstuff.setType('ind')
          otherStuff.words  = child.moreWords
          otherStuff.word0  = child.moreWords[0]
          if otherStuff.indent is None:
            otherStuff.indent = otherStuff.word0.col
          child.moreWords   = None
          otherStuff.setParent(child)
        continue
      else:
        child.name      = child.moreWords[0].value
        child.words     = child.moreWords
        child.moreWords = None
        child.word0     = child.words[0]
        if child.indent is None:
          child.indent    = child.word0.col
        child.words     = child.words[1:]
        continue
      
    # Not a known statement and has ':' breaker, syntax error
    if not child.name and breaker and breaker.value == ':':
      if child.words and len(child.words) > 1:
        parse.postError(SyntaxError(breaker,"Unknown statement %s, use of %s invalid here"%(repr(child.word0.value),repr(breaker.value))))
      else:
        child.setType('label')
        child.setType('ind')
        if child.moreWords:
          newNode = parse.tree.newNode()
          newNode.setParent(child)
          newNode.words  = child.moreWords
          newNode.word0  = child.moreWords[0]
          if newNode.indent is None:
            newNode.indent = newNode.word0.col
          newNode.wordN  = child.moreWords[-1]
          theNode.setType('expr')
          theNode.setType('ind')
          child.moreWords = None

    # Make sure word0 is set if applicable...
    if child.word0 is None and child.words:
      child.word0  = child.words[0]
      if child.indent is None:
        child.indent = child.word0.col
      
    # Break down moreWords
    if child.moreWords:
      parse.breakNodeExpressionWords(child,child.moreWords)
      child.moreWords = None
      if child.children and child.word0 is not None:
        child.setType('expr')
        child.setType('ind')
    else:
      child.setType('expr')
      child.setType('ind')

  #
  # Subpass 3: Indent top level children nodes (indentation only makes sense there,
  #            since Style.Freeform elsewhere)...
  #

  # TEMP...
  #return
  # ...TEMP
  
  parse.indentChildren(parent)

  #
  # Subpass 4: Break 'expression' nodes into single expressions...
  #
  
  nodeIdxs = []
  for node in TopDownLeftRight(parent):
    if node.testType('expr') or node.testType('stmtexpr'):
      nodeIdxs.append(parse.tree.find(node))
  for nodeIdx in nodeIdxs:
    node = parse.tree.nodes[nodeIdx]
    if node.parent is None:
      continue
    breakMultipleStatements(parse,node)

  #
  # Subpass 5: Parse all expression nodes (the real fun begins here)...
  #

  parse.parseExpression(parent)
    
def breakMultipleStatements(parse,node):

  if isMultipleStatements(node.words):
    
    childIdx = node.parent.findChild(node)
    insertIdx = childIdx+1
    words = node.words
    span = Span(0,len(words))
    while True:
      breakIdx = findEndOfExpression(parse,words,span)
      thisSpan = Span(span.min,breakIdx)
      if thisSpan.min <= thisSpan.max:
        # Insert a new node...
        newNode = parse.tree.newNode()
        newNode.setType('stmt')
        newNode.words = words[thisSpan.min:thisSpan.max]
        if newNode.words:
          newNode.word0  = newNode.words[0]
          newNode.indent = newNode.word0.col
          newNode.wordN  = newNode.words[-1]
        node.parent.insertAfterChild(newNode,insertIdx)
        insertIdx += 1
      span.min = breakIdx+1;
      span.max = len(words)
      if span.min >= span.max:
        break
    node.removeFromTree()
    
def isMultipleStatements(words):

  idx = -1
  stack = Stack()
  while words and idx < len(words)-2:
    idx += 1
    word = words[idx]
    if word.type == Word.Type.OPERATOR:
      if word.value in '[{(':
        stack.push(word)
        continue
      if word.value in ']})':
        stack.pop()
        continue
      if not stack and word.value in (';',':') and idx != len(words)-1:
        return True
  return False
  
def breakNodeExpressionWords(parse,node,words):

  count = 0
  while words:
    span = Span(0,len(words))
    breaker = parse.findEndOfExpression(words,span)
    newNode = parse.tree.newNode()
    newNode.setType('expr')
    newNode.setType('ind')
    newNode.setParent(node)
    if breaker and breaker < len(words):
      newNode.breaker = words[breaker]
    newNode.word0  = words[span.min]
    newNode.indent = newNode.word0.col
    newNode.words  = words[span.min:span.max]
    node.words = None
    if breaker:
      words = words[breaker+1:]
    else:
      words = []
    count += 1

  return count

def indentChildren(parse,node):

  # DEBUG...
  if debug and verbose0:
    print("indentChildren: node[%d]..."%parse.tree.find(node))
  # ...DEBUG
  
  curIdx = -1
  stack = Stack()
  
  while True:
  
    curIdx = curIdx + 1
    if curIdx >= len(node.children):
      break
    child = node.children[curIdx]
      
    if not child.testType('ind'):
      child.setType('ind')
    
    # Handle type... indentation...
    if child.testType('type...'):
      stack.push(child)
      continue
    if child.testType('...type'):
      if not stack:
        parse.postError(SyntaxError(child.word0,"..."+child.name+" without matching "+child.name+"..."))
        continue
      if stack.TOS().name != child.name:
        parse.postError(SyntaxError(child.word0,"... mismatch. Expected ..."+stack.TOS().name))
        continue
      stack.pop()
      continue
    if stack:
      child.setParent(stack.TOS())
      curIdx -= 1
      continue
      
    # Don't want to process first one here...
    if curIdx == 0:
      continue

    # DEBUG...
    if debug and verbose0:
      print("indentChildren: Checking child node[%d]..."%parse.tree.find(child))
    # ...DEBUG

    # "Normal" cases...
    prev  = node.children[curIdx-1]
    if prev is None or prev.indent is None or child.indent is None or child.indent <= prev.indent:
      continue
    if prev.breaker and prev.breaker.value == ':':
      # Find lowest child down the tree...
      while prev.children and prev.indent and prev.indent < child.indent:
        thisOne = prev.children[-1]
        if thisOne.word0 and thisOne.indent < child.indent:
          prev = thisOne
        else:
          break
      # Reparent now...
      if prev.indent < child.indent:
        if 'ind:' not in child.type:
          child.type = 'ind:'+child.type
        child.setParent(prev)
        curIdx -= 1

#===============================================================================
#
# Method: findEndOfLogicalLine - Given list of Word, find logical statment
#                                in given span 
#
# Arguments: parse - Parse: Context
#            words - list:  List of Word instances
#            pSpan - Span:  words span to look in (untouched)
#
# Returns:   tuple(breaker,span)
#
#              breaker - int:  Index of logical line breaker
#              span    - Span: Span of logical line
#          
# Note:      We get rid of EOL words when appropriate so the Word list is
#            modified by this function.
#

def findEndOfLogicalLine(parse,words,pSpan):

  start   = pSpan.min
  end     = pSpan.max
  if end is None:
    end = len(words)
  breaker = None
  span    = Span(start,end)
  if start < 0 or start >= end:
    return breaker,span

  # Go for it...
  nesting = Stack()
  curIdx = start-1
  style = parse.styleStack.TOS()
  
  while curIdx <= len(words)-2:
  
    curIdx  += 1      
    word     = words[curIdx]
    
    # If EOL...
    if word.type == Word.Type.EOL:
    
      # DEBUG...
      #if debug and verbose0:
      #  print("findEndOfLogicalLine: EOL at %d, nesting = %s"%(curIdx,repr(bool(nesting))))
      # ...DEBUG

      del words[curIdx]
      span.max = curIdx
      curIdx -= 1
      
      if not nesting:
        # DEBUG...
        #if debug and verbose0:
        #  print("findEndOfLogicalLine: End of logical line, span=%s"%str(span))
        # ...DEBUG
        if (span.max - span.min) > 0:
          break
        continue
        
      # Not end of logical line, ignore it...
      continue
      
    # Ignore if not operator...
    if word.type  != Word.Type.OPERATOR:
      continue
      
    # Handle operator '\\'...
    if word.value == '\\' and              \
       curIdx <= len(words)-2 and          \
       words[curIdx+1].type  == Word.Type.EOL:
      del words[curIdx:curIdx+2]
      span.max -= 1
      curIdx -= 2
      continue
        
    # DEBUG...
    #if debug and verbose0:
    #  print("findEndOfLogicalLine: Operator %s..."%repr(word.value))
    # ...DEBUG

    # Stack if '(', '[' or '{'...
    if word.value in "([{":
      nesting.push(word)
      continue
      
    # Unstack if ')', ']', '}'...
    if word.value in ")]}":
      if not nesting or                                          \
         ( (word.value == ')' and nesting.TOS().value != '(') or \
           (word.value == ']' and nesting.TOS().value != '[') or \
           (word.value == '}' and nesting.TOS().value != '{')    \
         ):
        parse.postError(SyntaxError(word,"Mismatched '%s'"%(word.value)))
        continue
      nesting.pop()

    # Handle '\\'...
    #
    # Note: If '\\' in line without EOL following we simply ignore it...
    if word.value == '\\':
      if curIdx == len(words)-1:
        parse.postWarning(SyntaxError(word,"Trailing '\\' at end of file, ignored"))
        continue
      del words[curIdx]
      curIdx -= 1
      continue
      
    # Handle ':', and '...'...
    if word.value in (':','...') and not nesting:
      if breaker is None:
        breaker = curIdx
      if style == Style.Freeform and word.value == ';':
        break

    # DEBUG...
    #if debug and verbose0:
    #  print("findEndOfLogicalLine: Operator %s, ignoring..."%repr(word.value))
    # ...DEBUG
    
  # Handle unmatched parens...
  while nesting:
     parse.postError(SyntaxError(nesting[0],"Unmatched '%s'"%(nesting[0].value)))
     del nesting[0]
     
  # DEBUG...
  if debug and verbose0:
    print("findEndOfLogicalLine: Finished: breaker = %s, span = (%s:%s)"%(repr(breaker),repr(span.min),repr(span.max)))
    if span.min == span.max:
      if span.min < len(words):
        w = words[span.min]
        print("                      len(span) is 0: Word[%s] line = %s, col = %s"%(repr(span.min),repr(word.line+1),repr(word.col+1)))
  # ...DEBUG

  return breaker,span
  
def findEndOfExpression(parse,words,span):

  start   = span.min
  end     = span.max

  if end is None:
    end = len(words)
  if end > len(words):
    end = len(words)
  breaker = None
  if start < 0 or start >= end:
    span.min = span.max
    return breaker
  
  # Go for it...
  nesting = Stack()
  curIdx = start-1
  style = parse.styleStack.TOS()
  
  debugit = False
  while curIdx < end-1:
  
    curIdx += 1
    word = words[curIdx]
    
    # Ignore if not operator...
    if word.type != Word.Type.OPERATOR:
      continue
      
    # Stack if '(', '[' or '{'...
    if word.value in "([{":
      nesting.push(word)
      continue
      
    # Unstack if ')', ']', '}'...
    if word.value in ")]}":
      if not nesting or                                          \
         ( (word.value == ')' and nesting.TOS().value != '(') or \
           (word.value == ']' and nesting.TOS().value != '[') or \
           (word.value == '}' and nesting.TOS().value != '{')    \
         ):
        parse.postError(SyntaxError(word,"Unmatched '%s'"%(word.value)))
        continue
      nesting.pop()
      
      continue
      
    # Handle ';', ':', and '...'
    if not nesting and word.value in (';',':','...'):
      breaker = curIdx
      break

  # Adjust span
  span.max = curIdx
  
  # Handle unmatched parens...
  while nesting:
     parse.postError(SyntaxError(nesting[0],"Unmatched '%s'"%(nesting[0].value)))
     del nesting[0]

  if breaker is None:
    breaker = len(words)
    span.max = len(words)
     
  return breaker

#
# Note: deepNodex returns of deep nodes in a Tree, guaranteed that
#       each one appears only once.
#

def deepNodes(parent):

  deep = []
  for node in TopDownLeftRight(parent):
    if node not in deep:
      deep.append(node)
  return deep
  
#===============================================================================
#
# Function: parseExpression - Parse an expression Node
#
# Notes: 1) This is for local/internal use only
#        2) By the time this is called, we're ensure that all "[...", "{...",
#           and "(..." nodes are guaranteed to have matching end grouping words.
#

def parseExpression(parse,parent):

  def performPass(parse,parent,func,flag):

    startErrors = parse.numErrors()
    nodeList = deepNodes(parent)    
    # DEBUG...
    if debug and verbose0:
      s = ''
      for node in nodeList:
        if s: s += ','
        s += str(parse.tree.find(node))
      print("performPass: parent = %d, flag = %s, nodeList = [%s] (func=%s)"%(parse.tree.find(parent),repr(flag),s,repr(func)))
    # ...DEBUG
    curIdx = -1
    while curIdx < len(nodeList)-1:
      curIdx += 1
      node = nodeList[curIdx]
      if node.testFlag(flag):
        del nodeList[curIdx]
        curIdx -= 1
        
    for node in nodeList:
      if parent.tree.find(node) is None:
        continue
      if not node.testFlag(flag):
        func(parse,node)
        node.setFlag(flag)
        
    return parse.numErrors() > startErrors
  
  # Sanity checks...
  if not issubclass(parent.__class__,TreeNode):
    raise InternalError("Expected TreeNode parameter, got %s"%repr(type(node)))
    
  # Passes...
  if do1 and performPass(parse,parent,parseExpressionPass1,ParseNode.Flag.expPass1): return
  if do2 and performPass(parse,parent,parseExpressionPass2,ParseNode.Flag.expPass2): return
  if do3 and performPass(parse,parent,parseExpressionPass3,ParseNode.Flag.expPass3): return
  if do4 and performPass(parse,parent,parseExpressionPass4,ParseNode.Flag.expPass4): return
  if do5 and performPass(parse,parent,parseExpressionPass5,ParseNode.Flag.expPass5): return
  if do6 and performPass(parse,parent,parseExpressionPass6,ParseNode.Flag.expPass6): return
  
#===============================================================================
#
# Function: parseExpressionPass1 - Convert ParseNode.words to Nodes and
#                                  structure groups
#

def parseExpressionPass1(parse,parent):

  # DEBUG...
  if debug and verbose1:
    print("Pass1: parent %d = %s"%(parse.tree.find(parent),str(parent)))
  # ...DEBUG
  
  words = parent.words
  parent.words = None
  if not words:
    return
    
  groupOps = startGroups + endGroups
  
  curIdx = -1
  valStack = Stack()
  
  while curIdx < len(words)-1:

    # Get next word
    curIdx += 1
    word = words[curIdx]
    
    # Handle begin/end groupings...
    if word.type == Word.Type.OPERATOR and word.value in groupOps:
    
      # Handle end groupings...
      if word.value in endGroups:
      
        which = endGroups.index(word.value)
        
        # Find matching start of group...
        node = valStack.TOS()
        if node.name != startGroups[which]:
          raise InternalError("Out of synch: Expected %s on valStack but it's %s instead"%repr(startGroups[which],repr(node.name)))
          
        # Update grouping Node...
        node = valStack.TOS()
        node.name += word.value
        node.wordN = word
        parent = valStack.TOS().parent
        temp = valStack.pop()
        
      # Handle begin groupings...
      else:
        newNode = parse.tree.newNode()
        newNode.setType('group')
        newNode.setType('operator')
        newNode.name   = word.value
        newNode.word0  = word
        newNode.indent = word.col
        # Need this???...
        newNode.setParent(parent);
        # ...Need this???
        valStack.push(newNode)

        parent = newNode
      
      continue

    # Hack names that are also types...
    if word.type == Word.Type.NAME:
      if word.value in operatorDefs:
        word.type = Word.Type.OPERATOR;
        nextWord = None if curIdx == len(words)-1 else words[curIdx+1]
        if nextWord and nextWord.type == Word.Type.NAME and nextWord.value in operatorDefs:
          if word.value == 'is' and nextWord.value == 'not':
            word.value = 'isnot'
            del words[curIdx+1]
          elif word.value == 'not' and nextWord.value == 'in':
            word.value = 'notin'
            del words[curIdx+1]
            
    # Handle operators...
    if word.type == Word.Type.OPERATOR:

      # Get last node...
      curNode = None if not parent.children else parent.children[-1]
      
      # Make sure it's handled (Word may have a bigger list than us)...
      if word.value not in operatorDefs:
        parse.postError(SyntaxError(word,"Unimplemented operator %s"%repr(word.value)))
        return
        
      # Get the operator options...
      prec,l2r,prefix,infix,postfix = operatorDefs[word.value]
      
      # Special case for multiple operator '.'...
      #
      # Note: I know it's weird, but 'dot' nodes are the only operators
      #       that have a value and it means "number of contiguous '.'" for 
      #       traversing 'up the namespace' ;-)
      if word.value == '.' and curNode and curNode.name == '.':
        curNode.value += 1
        continue
      
      # For now...
      newNode = parse.tree.newNode()
      newNode.setType('operator')
      newNode.setType('expr')
      newNode.name = word.value
      if newNode.name == '.':
        newNode.value = 1   
        newNode.wordN = word
      newNode.word0  = word
      newNode.indent = word.col
      newNode.setParent(parent)
      continue
    
    #
    # Is a terminus...
    #

    # Sanity checks...
    wordTypeName = Word.typenames[word.type]
    if wordTypeName not in wordToNodeType:
      raise InternalError("Unexpected Word.Type %s"%repr(wordTypeName))

    # Push it...
    newNode = parse.tree.newNode()
    theTypeName = wordToNodeType[wordTypeName]
    if theTypeName == 'name' and word.value and word.value[0] != '`':
      newNode.setType(theTypeName)
    else:
      newNode.type = theTypeName
    newNode.name   = word.value
    newNode.word0  = word
    newNode.indent = word.col
    newNode.setParent(parent)

    #    
    # Handle qualifiers on string and name literals...
    #
    
    if newNode.testType('string') or ((newNode.testType('name') and newNode.name and newNode.name[0] == '`')):

      # DEBUG...
      if debug and verbose1:
        print("Pass1: Checking for qualifiers...")
        if len(words) - curIdx > 1:
          print("       words[curIdx+1].type = %s"%repr(words[curIdx+1].type))
      # ...DEBUG
    
      if len(words) - curIdx > 1 \
         and words[curIdx].eoff == words[curIdx+1].soff \
         and words[curIdx+1].type == Word.Type.NAME :
        
        newQual = parse.tree.newNode()
        newQual.type = 'qual'
        newQual.name = 'insensitive'
        newQual.value = False
        newQual.setParent(newNode)
        newQual = parse.tree.newNode()
        newQual.type = 'qual'
        newQual.name = 'mutable'
        newQual.value = False
        newQual.setParent(newNode)

        qual = words[curIdx+1]
        val = qual.value.lower()
        if len(val) > 2 or val not in ('i','m','im','mi'):
          parse.postWarning(SyntaxError(words[curIdx+1],"Expected 'i', 'm', 'im', or 'mi' here, ignoring %s"%repr(qual.value)))
        else:
          if 'i' in val:
            newNode.children[0].value = True
          if 'm' in val:
            newNode.children[1].value = True
          del words[curIdx+1]

  #
  # Finally, do Pass: 1.5 - Do '.' syntax and call/index parsing...
  #                   1.6 - Do <name> <type> detection
  #
  
  parseExpressionPass1p5(parse,parent)
  parseExpressionPass1p6(parse,parent)
  
#===============================================================================
#
# Pass 1.5 - Do dotted name groupings and call/index handling...
#
# Note: This is called at the end of parseExpressionPass1()
#
  
def parseExpressionPass1p5(parse,parent):

  nodes = []
  for node in TopDownLeftRight(parent):
    if node.children:
      nodes.append(node)

  for node in nodes:

    curIdx = -1
    
    while curIdx < len(node.children)-1:
    
      curIdx += 1
      
      # If at '.' node, put stuff before it and after it under it...
      dotNode = node.children[curIdx]
      dotIdx = curIdx
      if (dotNode.testType('operator') and dotNode.name == '.'):
        # Fiddle with the type...
        if dotNode.testType('operator'):
          dotNode.unsetType('operator')
          dotNode.setType('dotted')
          dotNode.setType('name')
        # Find starting index...
        startIdx = curIdx
        while startIdx > 0:
          startIdx -= 1
          curNode = node.children[startIdx]
          if curNode.testType('operator') and curNode.name != '.':
            startIdx += 1
            break
        # Find ending index...
        endIdx = curIdx
        while endIdx < len(node.children)-1:
          endIdx += 1
          curNode = node.children[endIdx]
          if curNode.testType('operator') and curNode.name != '.':
            endIdx -= 1
            break
        # Now, turn the original '.' node into an unnamed 'name' node and put the starting/ending stuff in it...
        startCount = dotIdx-startIdx
        endCount   = endIdx-dotIdx
        dotNode.name   = None
        dotNode.word0  = None
        dotNode.indent = None
        dotNode.wordN  = None
        # DEBUG...
        if debug and verbose1:
          print("Pass1.5: startCount = %d, endCount = %d"%(startCount,endCount))
        # ...DEBUG
        if True:
          while startCount:
            curNode = node.children[startIdx]
            curNode.setParent(dotNode)
            if not dotNode.word0: dotNode.word0 = curNode.word0
            if curNode.wordN:     dotNode.wordN = curNode.wordN
            dotIdx -= 1
            startCount -= 1
          # Append a new 'name' '.' because it'll have gone missing...
          newNode = parse.tree.newNode()
          newNode.setType('name')
          newNode.name = '.'
          #newNode.word0 = dotNode.word0
          newNode.setParent(dotNode)
          # Now, put stuff after it into the dot node...
          while endCount:
            curNode = node.children[dotIdx+1]
            curNode.setParent(dotNode)
            # DEBUG...
            if debug and verbose1:
              print("Pass1.5: endCount = %d, curNode[%d], curNode.type = %s, curNode.name=%s"%(endCount,parse.tree.find(curNode),repr(curNode.type),repr(curNode.name)))
            # ...DEBUG
            if curNode.testType('operator'):
              curNode.unsetType('operator')
              curNode.unsetType('expr')
              curNode.setType('name')
            if not dotNode.word0:
              dotNode.word0  = curNode.word0
              dotNode.indent = cuNode.word0.col
            if curNode.wordN:     
              dotNode.wordN = curNode.wordN
            else:
              if curNode.word0:
                dotNode.wordN = curNode.word0
            endCount -= 1

        # Finally, adjust curIdx
        curIdx = dotIdx         

#===============================================================================
#
# Pass 1.6 - Do dotted name groupings and call/index handling...
#
# Note: This is called at the end of parseExpressionPass1()
#
  
def parseExpressionPass1p6(parse,parent):

  def isTypeQualifier(node):
  
    if not node.testType('name'):         return False
    if node.name and node.name[0] == '`': return False
    for minExp in typeQualifiers:
      if node.name in minExp:
        return True
    return False
    
  # TEMP...
  return
  # ...TEMP

  State = Enum("Looking, ProcessingType")
  
  nodes = []
  for node in TopDownLeftRight(parent):
    if node.children:
      nodes.append(node)

  for node in nodes:

    curIdx = -1
  
    state = State.Looking
    spans = []
    
    while curIdx < len(node.children)-1:
    
      curIdx += 1
      curNode = node.children[curIdx]
      
      # If State.Looking and two names side-by-side...
      if state == State.Looking and curNode.testType('name'):
      
        prevNode = node.children[curIdx-1]
        nextNode = None if curIdx >= len(node.children)-1 else node.children[curIdx+1]

        # Make sure not 'const' misuse...
        if isTypeQualifier(curNode):
          parse.postError(SyntaxError(curNode.word0,"Misuse of word %s, must follow a type or type qualifier"%repr(curNode.name)))
          break
          
        # If not two contiguous 'name' nodes, not there yet...
        if not curNode.testType('name') or not prevNode.testType('name'):
          continue

        # If next word is 'const', this is the type..
        span = Span(curIdx,curIdx+1)
        
        # Two names side by side, first one <type> if ???
        pass
        
#===============================================================================
#
# Pass 2 - Flag prefix, infix, and postfix operators...
#
# Note: By now we're sure operators are in the operatorDefs list
#
  
def parseExpressionPass2(parse,parent):

  # Walk the children...
  curIdx = -1

  while curIdx < len(parent.children)-1:
  
    # Next Child...
    curIdx += 1
    if curIdx >= len(parent.children): break
    curNode = parent.children[curIdx]
    
    # If not 'operator', skip it...
    if not curNode.testType('operator') or curNode.name not in operatorDefs: continue
    
    # Get operator info...
    prec,l2r,prefixName,infixName,postfixName = operatorDefs[curNode.name]
    bareTypes = ('number','name','string','group')
    hasPrefix  = prefixName  is not None
    hasInfix   = infixName   is not None
    hasPostfix = postfixName is not None
    prevNode   = Word() if curIdx == 0 or curIdx >= len(parent.children) else parent.children[curIdx-1]
    nextNode   = Word() if curIdx >= len(parent.children)-1 else parent.children[curIdx+1]
    
    # If next node is 'operator' get it's operator info...
    nextIsOperator = nextNode and nextNode.testType('operator')
    if nextIsOperator:
      nextPres,nextL2r,nextPrefixName,nextInfixName,nextPostfixName = operatorDefs[nextNode.name]
      
    # See if prefix/infix/postfix based on context...
    # DEBUG...
    if False and debug and verbose2 and curNode.word0.value == '[':
      print("parseExpressionPass2: op=%s"%repr(curNode.word0.value))
      print("                      hasPrefix=%s hasInfix=%s hasPostfix=%s"%(repr(hasPrefix),repr(hasInfix),repr(hasPostfix)))
      print("                        prevNode?                                         = %s"%repr(bool(prevNode)))
      print("                        nextNode?                                         = %s"%repr(bool(nextNode)))
      print("                        curIdx                                            = %d"%curIdx)
      print("                        len(parent.children)-1                            = %d"%len(parent.children))
      print("                        curIdx >= len(parent.children)-1?                 = %s"%repr(curIdx >= len(parent.children)-1))
      if nextNode:
        print("                        nextNode.type                                     = %s"%repr(nextNode.type))
      print("                        nextNode.type in baretypes?                       = %s"%repr(nextNode.type in bareTypes))
      print("                        (not prevNode or prevNode.type not in bareTypes)? = %s"%repr(not prevNode or prevNode.type not in bareTypes))
      print("                        curNode.word0.value                               = %s"%repr(curNode.word0.value))
    # ...DEBUG
    isPostfix = True if hasPostfix and prevNode and prevNode.oneOfTypes(bareTypes) \
                        and (not hasInfix or (nextNode and nextNode.oneOfTypes(bareTypes))) \
                     else \
                False
    isPrefix  = True if hasPrefix and nextNode \
                      and (nextNode.oneOfTypes(bareTypes) or (nextIsOperator and nextPrefixName)) \
                      and (not prevNode or not prevNode.oneOfTypes(bareTypes)) \
                     else \
                False
    isInfix   = hasInfix and not isPrefix and not isPostfix                                                                          \
                and prevNode and (prevNode.oneOfTypes(bareTypes) or (prevNode.testType('operator') and prevNode.name in postfixOps)) \
                and ((nextNode and nextNode.oneOfTypes(bareTypes)) or (nextIsOperator and nextPrefixName is not None))
    # DEBUG...
    if False and debug and verbose2:
      print("                      isPrefix=%s isInfix=%s isPostfix=%s"%(repr(isPrefix),repr(bool(isInfix)),repr(isPostfix)))
    # ...DEBUG
    
    # Resolve conflicts...
    eword = curNode.word0
    if isPrefix:
      if isInfix:
        parse.postError(SyntaxError(eword,"Prefix/Infix usage ambiguous"))
        break
      if isPostfix:
        parse.postError(SyntaxError(eword,"Prefix/Postfix usage ambiguous"))
        break
    if isInfix:
      if isPrefix:
        parse.postError(SyntaxError(eword,"Infix/Prefix usage ambigouus"))
        break
      if isPostfix:
        parse.postError(SyntaxError(eword,"Infix/Postfix usage ambiguous"))
        break
    if not isPostfix and curIdx == len(parent.children)-1 and eword.value not in ';,':
      if curNode.testType('group'):
        curNode.unsetType('group')
        curNode.unsetType('operator')
        newType = 'tuple' if curNode.name == '()' else 'list' if curNode.name == '[]' else 'posmap'
        curNode.setType(newType)
        # DEBUG...
        if debug and verbose2:
          print("Pass2: Turned group into value. curNode[%d] = %s"%(parse.tree.find(curNode),str(curNode)))
        # ...DEBUG
        continue
      else:
        parse.postError(SyntaxError(eword,"Dangling operator %s"%repr(curNode.name)))
        break
    
    # Change it...
    # DEBIG...
    if debug and verbose5:
      print("Pass5: isPrefix = %s, isInfix = %s, isPostfix = %s, curNode.name = %s"%(repr(isPrefix),repr(bool(isInfix)),repr(isPostfix),repr(curNode.name)))
    # ...DEBUG
    if isPrefix:
      curNode.name = prefixName
    elif isInfix:
      curNode.name = infixName
    elif isPostfix:
      # Subsume it under prevNode - Has to be hear instead of pass 3 because it's
      #   so closely bound to the immediately preceeding node
      #curNode.setParent(prevNode)
      #curIdx -= 1
      # Change its name
      curNode.name = postfixName
    else:
      if curNode.name not in (',',';'):
        parse.postError(SyntaxError(eword,"Invalid use of operator %s"%repr(curNode.name)))

  # DEBUG...
  #if debug and verbose2:
  #  print("parseExpressionPass2: AFTER... parent = %d"%parse.tree.find(parent))
  #  parse.tree.printNodes(indent='                      ')    
  # ...DEBUG

  return

#===============================================================================
#
# Pass 3 - Process ';' and ',' operators
#
# Note: Since pass 1 we're only processing nodes now BUT we also have to
#       sub-process groups to do the same...
#

def parseExpressionPass3(parse,parent):

  # DEBUG...
  if debug and verbose3:
    print("parseExpressionPass3: parent = %d"%parse.tree.find(parent))
  # ...DEBUG

  #
  # For ';' and ','...
  #

  for info in ( (';','stmt'), (',','expr') ):
  
    opName, changeTo = info
    
    # Find the associated nodes...
    curIdx = -1
    theSpans = []
    startIdx = 0

    while curIdx < len(parent.children)-1:    
      curIdx += 1
      curNode = parent.children[curIdx]
      if curNode.testType('operator') and curNode.name == opName:
        span = Span(startIdx,curIdx)
        theSpans.append((curNode,span))
        if len(span):
          curNode.word0  = parent.children[startIdx].word0
          curNode.indent = curNode.word0.col
          curNode.wordN  = parent.children[curIdx-1].word0
        else:
          curNode.wordN = None
        startIdx = curIdx+1

    # Add bogus ';' or ',' node if last one implied...
    if theSpans and startIdx < len(parent.children):
    
      span = Span(startIdx,len(parent.children))
      
      # Create the bogus ';'or ',' node...
      newNode = parse.tree.newNode()
      newNode.setType('operator')
      newNode.name = opName
      if len(span):
        newNode.word0  = parent.children[startIdx].word0
        newNode.indent = newNode.word0.col
        wordN = parent.children[-1].word0
      else:
        newNode.word0  = parent.wordN
        newNode.indent = newNode.word0.col
      newNode.wordN = wordN
      newNode.setParent(parent)
      
      # Add new entry to semiSpans
      theSpans.append((newNode,span))
      
    # Process them (if any) in reverse order so the numbers work out...
    while theSpans:
    
      theNode,span = theSpans.pop()
      
      # Turn the ';' node into 'stmt' node
      theNode.type = changeTo
      theNode.name = None
      # DEBUG...
      if debug and verbose3:
        print("parseExpressionPass3: Changed node %d to %s"%(parse.tree.find(theNode),changeTo))
      # ...DEBUG
      
      # Reparent the children...
      counter = span.max - span.min
      if counter > 1 or opName in [';',',']:
        while counter:
          counter -= 1
          parent.children[span.min].setParent(theNode)
      #else:
      #  theNode.removeFromTree()
                  
  return
  
#===============================================================================
#
# Pass 4 - Process operator '.' names
#

def parseExpressionPass4(parse,parent):

  if not parent.children: #or parent.testFlag(ParseNode.Flag.expPass4):
    return
  # DEBUG...
  displayNodes = debug and verbose4 and len(parent.children) >= 3
  if displayNodes:
    print("Pass4: BEFORE: parent = %d, parent.testFlag = %s"%(parse.tree.find(parent),repr(parent.testFlag(ParseNode.Flag.expPass4))))
    parse.tree.printNodes(indent='  ')
  # ...DEBUG

  # For each child...
  
  curIdx = -1
  while curIdx < len(parent.children)-1:
  
    curIdx += 1
    
    # Ignore if not operator '.'...
    dotNode = parent.children[curIdx]
    if dotNode.testType('operator') or dotNode.name != 'dot':
      continue
      
    # Make sure it's in a valid content...
    if curIdx == 0 or curIdx == len(parent.children)-1:
      msg = "Isolated" if len(parent.children) == 1 else \
            "Leading"  if curIdx == 0               else \
            "Trailing"
      parse.postWarning(SyntaxError(dotNode.word0,msg+" '.' invalid, ignored"))
      dotNode.removeFromTree()
      curIdx -= 1
      continue
      
      # Get previous/next nodes
      prevIdx = curIdx-1
      prevNode = parent.children[curIdx-1]
      nextNode = parent.children[curIdx+1]

    # DEBUG...
    if debug and verbose4:
      print("Pass4: prevNode.type = %s, prevNode.name = %s"%(repr(prevNode.type),repr(prevNode.name)))
      print("       nextNode.type = %s, nextNode.name = %s"%(repr(nextNode.type),repr(nextNode.name)))
    # ...DEBUG
    
    # Have '.', if previous name/group(), and next name/group(), combine it...
    beforeOk = prevNode.testType('name') \
               or (prevNode.testType('group')    and prevNode.name == '()') \
               or (prevNode.testType('operator') and prevNode.name == '.')
    afterOk  = nextNode.testType('name') \
                or (nextNode.testType('group')    and nextNode.name == '()') \
                or (nextNode.testType('operator') and nextNode.name == '.')
    # DEBUG...
    if debug and verbose4:
      print("       beforeOk = %s, afterOk = %s"%(repr(beforeOk),repr(afterOk)))
    # ...DEBUG
    if beforeOk and afterOk:
      wasDot = prevNode.type == 'name' and not prevNode.name
      # DEBUG...
      if debug and verbose4:
        print("Pass4: wasDot = %s"%repr(wasDot))
      # ...DEBUG
      if wasDot:
        dotNode.setParent(prevNode)
        nextNode.setParent(prevNode)
      else:
        prevNode.setParent(dotNode)
        newNode = parse.tree.newNode(dotNode)
        newNode.setParent(dotNode)
        newNode.setFlag(ParseNode.Flag.expPass4)
        nextNode.setParent(dotNode)    
        dotNode.type   = 'name'
        dotNode.name   = None
        dotNode.word0  = prevNode.word0
        dotNode.indent = prevNode.word0.col
        dotNode.wordN  = nextNode.wordN
        dotNode.setFlag(ParseNode.Flag.expPass4)
        curIdx -= 1
      continue
       
    # Invalid use of '.'...
    # DEBUG...
    if debug and verbose4: print("Pass4: Exception 2")
    # ...DEBUG
    parse.postError(SyntaxError(dotNode.word0,"2: Invalid use of '.' operator"))
    
  # DEBUG...
  if displayNodes:
    print("Pass4: AFTER: parent = %d, parent.testFlag = %s"%(parse.tree.find(parent),repr(parent.testFlag(ParseNode.Flag.expPass4))))
    parse.tree.printNodes(indent='  ')
  # ...DEBUG
  return
  
#===============================================================================
#
# Pass 5 - Structure all nodes based on operator precedence
#
# Note: We don't worry about postfix operators here because they've already been parented
#       in pass 2.
#

def parseExpressionPass5(parse,parent,testDefer5=False):

  if not parent.children or parent.testType('file') or parent.testFlag(parent.Flag.expPass5):
    return
  
  nodeStack = Stack()
  nodeStack.push(parent)

  # DEBUG...
  if debug and verbose5:
    print("Pass5: Nodes BEFORE (parent[%s])..."%repr(parse.tree.find(parent)))
    parse.tree.printNodes("       ")
  # ...DEBUG
  
  while nodeStack:

    parent = nodeStack[0]
    del nodeStack[0]
    if parent.testFlag(ParseNode.Flag.expPass5):
      continue
      
    # Push children nodes on nodeStack...
    for node in parent.children:
      if node not in nodeStack and not node.testFlag(ParseNode.Flag.expPass5):
        nodeStack.push(node)
    parent.setFlag(ParseNode.Flag.expPass5)
        
    #
    # Do Pass5 on this node...
    #
    
    curIdx = -1
    valStack = Stack()
    opStack  = Stack()
    
    while curIdx < len(parent.children)-1:

      curIdx += 1
      
      # Initialise
      curNode = parent.children[curIdx]
      if curNode.testFlag(ParseNode.Flag.expPass5):
        continue
      
      # DEBUG...
      if debug and verbose5:
        print("Pass5: BEFORE: curIdx = %d, curNode[%s]: %s"%(curIdx,repr(parse.tree.find(curNode)),str(curNode)))
        parse.tree.printNodes(indent='       ')
      # ...DEBUG
      
      # Ignore operator '.'...
      if curNode.testType('operator') and curNode.name == '.':
        continue
        
      # If it's not an operator, it's a value...
      if not curNode.testType('operator'):

        # Treat it as a value
        curNode.removeFromParent()
        curIdx -= 1
        valStack.push(curNode)
        # DEBUG...
        if debug and processOpTOSdebug:
          print("Pass5: Pushed nodes[%s] on valStack: %s"%(parse.tree.find(curNode),str(curNode)))
        # ...DEBUG
    
        # If next node isn't an operator, flush opStack...
        # Note: This fixes the problem of 'a * b c' producing 'a b*c'
        while valStack and opStack and (curIdx >= len(parent.children) - 1 or not parent.children[curIdx+1].testType('operator')):
           processOpTOS(parse,valStack,opStack)
        
        continue
        
      # If postfix, do it NOW...
      # DEBUG...
      if debug and processOpTOSdebug:
        print("Pass5: curNode.name = %s, bool(curNode.name in postfixOps) = %s, postFixOps = %s"% \
             (repr(curNode.name),repr(curNode.name in postfixOps),repr(postfixOps)))
        print("       curNode[%d] = %s"%(parse.tree.find(curNode),str(curNode)))
      # ...DEBUG
      if curNode.name in postfixOps:
        # DEBUG...
        if debug and processOpTOSdebug:
          print("Pass5: Pushing %s on opStack, node[%s], len(valStack) = %d, len(opStack) = %d"% \
               (repr(curNode.name),repr(parse.tree.find(curNode)),len(valStack),len(opStack)))
        # ...DEBUG
        if not valStack:
          raise InternalError("parseExpressionPass5: Postfix operator %s with no entry on valStack"%repr(curNode.name))
        opStack.push(curNode)
        curNode.removeFromParent()
        processOpTOSprefixes(parse,valStack,opStack)
        # DEBUG...
        if debug and processOpTOSdebug:
          print("       Afterwards: len(valStack) = %d, len(opStack) = %d"%(len(valStack),len(opStack)))
        # ...DEBUG
        curIdx -= 1
        continue

      # Get precedence etc...
      precThis,l2r,prefix,infix,postfix = operatorDefs[curNode.word0.value]

      # Until we're greater precedence than opStack.TOS()...
      while valStack and opStack:
        precThat,l2r,prefix,infix,postfix = operatorDefs[opStack.TOS().word0.value]
        if (l2r and precThis > precThat) or (not l2r and precThis >= precThat):
          break
        processOpTOS(parse,valStack,opStack)
              
      # Time to push this operator
      curIdx = curNode.parent.findChild(curNode) - 1    
      curNode.removeFromParent()
      opStack.push(curNode)
      # DEBUG...
      if debug and processOpTOSdebug:
        print("Pass5: Pushed nodes[%s] on opStack: node = %s"%(parse.tree.find(curNode),str(curNode)))
      # ...DEBUG

    # Flush valStack and opStack...
    # DEBUG...
    if debug and verbose5:
      print("Pass5: FINISHED 1... len(valStack) = %d, len(opStack) = %d, len(parent.children) = %d"%(len(valStack),len(opStack),len(parent.children)))
    # ...DEBUG
    while valStack and opStack:
      processOpTOS(parse,valStack,opStack)
    # DEBUG...
    if debug and verbose5:
      print("Pass5: FINISHED 2... len(valStack) = %d, len(opStack) = %d, len(parent.children) = %d"%(len(valStack),len(opStack),len(parent.children)))
    # ...DEBUG
    while valStack:
      valStack[0].removeFromParent()
      valStack[0].setParent(parent)
      # DEBUG...
      if debug and processOpTOSdebug:
        print("Pass5: Popped [%s] from valStack, reparented it to [%s]"%(repr(parse.tree.find(valStack[0])),repr(parse.tree.find(parent))))
      # ...DEBUG
      del valStack[0]
    # DEBUG...
    if debug and verbose5:
      print("Pass5: FINISHED 3... len(valStack) = %d, len(opStack) = %d, len(parent.children) = %d"%(len(valStack),len(opStack),len(parent.children)))
    # ...DEBUG
    # DEBUG...
    if debug and verbose5:
      print("Pass5: FINISHED... len(valStack) = %d, len(opStack) = %d"%(len(valStack),len(opStack)))
    # ...DEBUG
    
      
  return

#===============================================================================
#
# Pass 6 - Process 'group' types and single child 'expr' types
#
# Note: We don't worry about postfix operators here because they've already been parented
#       in pass 2.
#

def parseExpressionPass6(parse, parent):
  
  # If it's a 'group' node, do some naming...
  if parent.testType('group'):

    parent.unsetType('group')
  
    # Validate contents of '[] and '{}'...
    if parent.name in ('{}','[]'):
      numChildren = len(parent.children)
      tstr = 'index' if parent.name == '[]' else 'key'
      if numChildren < 2:
        # TEMP...
        print("Pass5: numChildren = %d"%numChildren)
        # ...TEMP
        parse.postError(SyntaxError(parent.word0,"Expected single "+repr(tstr)+" here"))
      if numChildren > 2:
        parse.postError(SyntaxError(parent.children[2].word0,"Expected only one "+repr(tstr)+" here"))
        
    # Rename it...
    if parent.name in ('{}','[]','()'):
      parent.name = {'[]':'posindex','{}':'keyindex','()':'call'}[parent.name]
      
  # If 'expr' with single child, replace it with child and get rid of it...
  elif parent.testType('expr') and not parent.testType('operator') and parent.children and len(parent.children) == 1:
    # DEBUG...
    if debug and verbose5:
      print("Pass5: Removing single child")
    # ...DEBUG
    nodeParent = parent.parent
    idx = nodeParent.findChild(parent)
    parent.setParent(None)
    child = parent.children[0]
    nodeParent.insertAfterChild(child,idx)
    parent.removeFromTree()

  return
  
def processOpTOSprefixes(parse,valStack,opStack):

  while opStack and valStack and opStack.TOS().name in postfixOps:

      opNode = opStack.pop()
      # DEBUG...
      if debug and processOpTOSdebug:
        print("processOpTOSprefixes: Popped prefix %s from opStack"%repr(opNode.name))
      # ...DEBUG
      
      arg0 = valStack.pop()
      if not arg0:
        raise InternalError("Value stack Empty")
        
      arg0.setParent(opNode)
      #opNode.removeFromParent()
      valStack.push(opNode)
      # DEBUG...
      if debug and processOpTOSdebug:
        print("processOpTOSprefixes: Pushed prefix %s on valStack"%repr(opNode.name))
      # ...DEBUG

def processOpTOS(parse,valStack,opStack):

  # DEBUG...
  if debug and processOpTOSdebug:
    print("processOpTOS BEFORE: valStack...")
    printNodeStack(parse,valStack,indent='                       ')
    print("                     opStack...")
    printNodeStack(parse,opStack,indent='                       ')
    print("                     Nodes...")
    parse.tree.printNodes(indent='                     ')
  # ...DEBUG
  
  # Process prefix operators first...
  processOpTOSprefixes(parse,valStack,opStack)
  
  # DEBUG...
  if debug and processOpTOSdebug:
    print("processOpTOS MIDDLE: valStack...")
    printNodeStack(parse,valStack,indent='                       ')
    print("                     opStack...")
    printNodeStack(parse,opStack,indent='                       ')
    print("                     Nodes...")
    parse.tree.printNodes(indent='                     ')
  # ...DEBUG
  
  # Now should have infix operators on stack...
  if valStack and opStack: # Process the top operator (should be inFix by now...
    
    opNode = opStack.pop()
    
    if not opNode:
      raise InternalError("Operator Stack Empty, sohouldn't be!!!")
    if not opNode.testType('operator'):
      raise InternalError("Operator Stack has non 'operator' on it!!!")

    opNode.setFlag(ParseNode.Flag.expPass5)
    opNode.removeFromParent()
      
    if opNode.name in infixOps:
    
      arg1 = valStack.pop()
      arg0 = valStack.pop()
      if not arg1 or not arg0:
        # DEBUG...
        if True:
          print("opNode       = %d"%parse.tree.find(opNode))
          print("opNode.type  = %s"%repr(opNode.type))
          print("opNode.name  = %s"%repr(opNode.name))
          print("opNode.value = %s"%repr(opNode.value))
          print("opStack has %d entries remaining"%len(opStack))
          print("opNode       = %s"%str(opNode))
          print("arg0         = %s"%str(arg0))
          print("arg1         = %s"%str(arg1))
        # ...DEBUG
        raise InternalError("Value Stack Empty")
      arg0.setParent(opNode)
      arg1.setParent(opNode)
      valStack.push(opNode)
      # DEBUG...
      if debug and processOpTOSdebug:
        print("processOpTOS: Pushed infix %s on valStack"%repr(opNode.name))
      # ...DEBUG
      
    elif opNode.name in prefixOps:
    
      if not valStack:
        raise InternalError("valStack is empty, shouldn't be")
      arg0 = valStack.pop()
      arg0.setParent(opNode)
      valStack.push(opNode)

    else:

      if valStack and len(valStack) > 1:
        raise InternalError("Operator Stack empty but Value Stack has %d entries"%len(valStack))
      
  # DEBUG...
  if debug and processOpTOSdebug:
    print("processOpTOS AFTER:  valStack...")
    printNodeStack(parse,valStack,indent='                       ')
    print("                     opStack...")
    printNodeStack(parse,opStack,indent='                       ')
    print("                     Nodes...")
    parse.tree.printNodes(indent='                     ')
  # ...DEBUG
  
  return
    
# DEBUG...
#===============================================================================
#
# printNodeStack - Debug: Print a Node Stack
#

if debug:
  def printNodeStack(parse,stack,indent=''):

    if not stack:
      print(indent+"***stack empty***")
      return
    for i in range(0,len(stack)):
      node = stack[i]
      print(indent+"[%d] nodes[%d]: %s"%(i,parse.tree.find(node),str(node)))
    return
# ...DEBUG
