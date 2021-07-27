#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File: BEFE_expression_ops - Expression operator declarations (variables)
#
# Variables: operatorDefs - dict: keyvalue pairs. Turns lexical operators into
#                                 distinctly named/classified operator names...
#
#                           Where:  key   - str:   Operator name
#                                   value - tuple: (prec,prefix,infix,postfix)
#                                           Where: prec    - int: Precedence (infix only)
#                                                  prefix  - str: None, or MinExp seed
#                                                  infix   - str: None, or MinExp seed
#                                                  postfix - str: None, or MinExp seed
#
#             prefixOps   - tuple: Sorted tuple of MinExp.Shortest() prefix operator names
#             infixOps    - tuple: Sorted tuple of MinExp.Shortest() infix operator names
#             postfixOps  - tuple: Sorted tuple of MinExp.Shortest() postfix operator names
#
# Notes: operatorDefs is used by parseExpression() turning words into nodes. The names
#        in the lexical words ('*','+','--', etc.), based on context, are turned into
#        specialised "normalised" names like ('mult','add','postdec', etc.)
#
#        Given the normalised names, simple checks like "node.name in infixOps" 
#        can be used to determine how many sub-nodes the operator consumes, etc.
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

# DEBUG...
from BEFE_debug import *
# ...DEBUG
#from BEFE_ParseTree import ParseNode
from BEFE_MinExp    import MinExp
from BEFE_errors    import *

#===============================================================================
#
# Operator declarations
#

L2R = True
R2L = False

operatorDefs = {
  #         ┌──────────────────────────────────────────────────────────────────── Precedence (InFix only)
  #         │   ┌──────────────────────────────────────────────────────────────── L2R or R2L (InFix if same operator)
  #         │   │    ┌─────────────────────────────────────────────────────────── Prefix Name
  #         │   │    │               ┌─────────────────────────────────────────── InFix Name
  #         │   │    │               │                                        ┌── Postfix Name
  #         │   │    │               │                                        │
  #         ▼   ▼    ▼               ▼                                        ▼
  '.':     (19, L2R ,None           ,'dot'                                   ,None                 ),
  '::':    (19, L2R ,None           ,'nam[e]res[olve]'                       ,None                 ),
  '[]':    (18, L2R ,None           ,None                                    ,'\[\]'               ),
  '()':    (18, L2R ,None           ,None                                    ,'\(\)'               ),
  '{}':    (18, L2R ,None           ,None                                    ,'\{\}'               ),
  '++':    (16, L2R ,'inc[rement]'  ,None                                    ,'postinc[rement]'    ),
  '--':    (16, L2R ,'dec[rement]'  ,None                                    ,'postdec[rement]'    ),
  '&':     (15, L2R ,'addr[ess]'    ,None                                    ,None                 ),
  '**':    (13, L2R ,None           ,'power'                                 ,None                 ),
  '*':     (12, L2R ,'ind[irect]'   ,'mul[t[iply]]'                          ,None                 ),
  '/':     (12, L2R ,None           ,'div[ide]'                              ,None                 ),
  '%':     (12, L2R ,None           ,'rem[ain[der]]|format'                  ,None                 ),
  '/%':    (12, L2R ,None           ,'div[ision|de][and]rem[ain[der]'        ,None                 ),
  '??':    (12, L2R ,None           ,'doublequery'                           ,None                 ),
  '^':     (12, L2R ,None           ,None                                    ,None                 ),
  '^^':    (12, L2R ,None           ,None                                    ,None                 ),
  '@':     (12, L2R ,None           ,'at'                                    ,None                 ),
  '@@':    (12, L2R ,None           ,'atat'                                  ,None                 ),
  '#':     (12, L2R ,None           ,'sharp'                                 ,None                 ),
  '##':    (12, L2R ,None           ,'sharpsharp'                            ,None                 ),
  '$':     (12, L2R ,None           ,'dollar'                                ,None                 ),
  '$$':    (12, L2R ,None           ,'m[ega]dollar|dollardollar'             ,None                 ),
  '!':     (12, L2R ,'log[ical]not' ,None                                    ,None                 ),
  '!!':    (12, L2R ,None           ,'bangbang'                              ,None                 ),
  '~':     (12, L2R ,'bit[wise]not' ,None                                    ,None                 ),
  '~~':    (12, L2R ,None           ,'notnot'                                ,None                 ),
  '+':     (11, L2R ,'pos[itive]'   ,'add'                                   ,None                 ),
  '-':     (11, L2R ,'neg[at[iv]e]' ,'sub[tract]'                            ,None                 ),
  '&':     (10, L2R ,'addr[ess]'    ,'bit[wise]and'                          ,None                 ),
  '|':     ( 9, L2R ,None           ,'bit[wise]or'                           ,None                 ),
  '<':     ( 8, L2R ,None           ,'less[than]'                            ,None                 ),
  '>':     ( 8, L2R ,None           ,'great[er][than]'                       ,None                 ),
  '<=':    ( 8, L2R ,None           ,'less[than]oreq[ual][to]'               ,None                 ),
  '>=':    ( 8, L2R ,None           ,'g[reat][er][than]oreq[ual][to]'        ,None                 ),
  '=>':    ( 8, L2R ,None           ,'moveto'                                ,None                 ),
  '!=':    ( 8, L2R ,None           ,'noteq[ual][to]'                        ,None                 ),
  '<>':    ( 8, L2R ,None           ,'noteq[ual][to]'                        ,None                 ),
  '==':    ( 8, L2R ,None           ,'eq[ual][to]'                           ,None                 ),
  '&&':    ( 7, L2R ,None           ,'log[ical]and'                          ,None                 ),
  'and':   ( 7, L2R ,None           ,'log[ical]and'                          ,None                 ),
  '||':    ( 6, L2R ,None           ,'log[ical]or'                           ,None                 ),
  '\\\\':  ( 6, L2R ,None           ,'doubleback'                            ,None                 ),
  'or':    ( 6, L2R ,None           ,'log[ical]or'                           ,None                 ),
  'not':   ( 5, L2R ,'not'          ,None                                    ,None                 ),
  'xor':   ( 5, L2R ,None           ,'[e]x[cl[usive]]or'                     ,None                 ),
  'in':    ( 5, L2R ,None           ,'in'                                    ,None                 ),
  'is':    ( 5, L2R ,None           ,'is'                                    ,None                 ),
  'notin': ( 5, L2R ,None           ,'notin'                                 ,None                 ),
  'isnot': ( 5, L2R ,None           ,'isnot'                                 ,None                 ),
  '<<':    ( 4, L2R ,None           ,'pipefrom|bit[wise]sh[ift]left'         ,None                 ),
  '>>':    ( 4, L2R ,None           ,'pipeto|bit[wise]sh[ift]right'          ,None                 ),
  '<<<':   ( 4, L2R ,None           ,'c[irc[ular]]sh[ift]left'               ,None                 ),
  '>>>':   ( 4, L2R ,None           ,'c[irc[ular]]sh[ift]right'              ,None                 ),
  ':=':    ( 3, R2L ,None           ,'assign[from]'                          ,None                 ),
  '=:':    ( 3, R2L ,None           ,'assignto'                              ,None                 ),
  '=':     ( 3, R2L ,None           ,'assign[from]'                          ,None                 ),
  '::=':   ( 3, R2L ,None           ,'assign[from]'                          ,None                 ),
  '=::':   ( 3, R2L ,None           ,'assignto'                              ,None                 ),
  '|=':    ( 3, R2L ,None           ,'bit[wise]orassign'                     ,None                 ),
  '||=':   ( 3, R2L ,None           ,'log[ical]orassign'                     ,None                 ),
  '&=':    ( 3, R2L ,None           ,'bit[wise]andassign'                    ,None                 ),
  '&&=':   ( 3, R2L ,None           ,'log[ical]andassign'                    ,None                 ),
  '!=':    ( 3, R2L ,None           ,'log[ical]notassign'                    ,None                 ),
  '~=':    ( 3, R2L ,None           ,'sub[tract]assign'                      ,None                 ),
  '*=':    ( 3, R2L ,None           ,'mul[tiply]assign'                      ,None                 ),
  '/=':    ( 3, R2L ,None           ,'div[ide]assign'                        ,None                 ),
  '/%=':   ( 3, R2L ,None           ,'div[ide]rem[ain[der]]assign'           ,None                 ),
  '%=':    ( 3, R2L ,None           ,'(rem[ain[der]]|format)assign'          ,None                 ),
  '+=':    ( 3, R2L ,None           ,'add[ition]assign'                      ,None                 ),
  '<<=':   ( 3, R2L ,None          ,'(pipefrom|bit[wise]sh[ift]left)assign'  ,None                 ),
  '>>=':   ( 3, R2L ,None          ,'(pipeto|bit[wise]sh[ift]right)assign'   ,None                 ),
  '-=':    ( 3, R2L ,None           ,'bit[wise]notassign'                    ,None                 ),
  '<-':    ( 3, L2R ,None           ,'mov[e]f[rom]'                          ,None                 ),
  '->':    ( 3, L2R ,None           ,'mov[e]t[o]'                            ,None                 ),
  '<->':   ( 3, L2R ,None           ,'exch[ange]'                            ,None                 ),
  '..':    ( 3, L2R ,None           ,'range'                                 ,None                 ),
  ':':     ( 2, L2R ,None           ,'keyvalue'                              ,None                 ),
  '?':     (12, L2R ,'query'        ,None                                    ,None                 ), # Debugging (was priority 12)
  ',':     ( 1, L2R ,None           ,None                                    ,None                 ),
  ';':     ( 0, L2R ,None           ,None                                    ,None                 ),
  #'\\':    ( 0, L2R ,None           ,None                                    ,None                 ),
}

startGroups = '[{('
endGroups   = ']})'

typeQualifiers   = ['const[ant]']
nameQualifiers   = ['final']
stringQualifiers = ['i[ns[ensitive]]','m[utable]']
allQualifiers    = [] # ◄── Populated below after validation

statementDefs = {
  'if':            (('boolean',),),
  'else':          (None,),
  'el[s[e]]if':    (('boolean',),),
  'do':            (None,),
  'while':         (('boolean',),),
  'for':           (('expr','stmt3'),),
  'method[s]':     (('method',),),
  'memb[er][s]':   (('member',),),
  'func[tion][s]': (('function',),),
  'def[ine]':      (('function',),),
  'from':          (('from',),),
  'import':        (('import',),),
  'unimport':      (('import',),),
  'file[s]':       (('string',),),
  'go[to]':        (('goto',),),
  }
  
#-------------------------------------------------------------------------------
#
# General fiddling with the data...
#

#
# Fiddle with operatorDefs...
#

usedKeys = {}
for key in operatorDefs:
  prec,l2r,prefix,infix,postfix = operatorDefs[key]
  
  tPrefix  = MinExp(prefix).Shortest()  if prefix  else None
  tInfix   = MinExp(infix).Shortest()   if infix   else None
  tPostfix = MinExp(postfix).Shortest() if postfix else None
  '''
  if tPrefix in usedKeys:
    raise Exception("BEFE_expression_ops: key %s (from %s) conflicts with %s (from %s)"%(repr(tPrefix),repr(key),repr(tPrefix),repr(usedKeys[tPrefix])))
  if tInfix in usedKeys:
    raise Exception("BEFE_expression_ops: key %s (from %s) conflicts with %s (from %s)"%(repr(tInfix),repr(key),repr(tInfix),repr(usedKeys[tInfix])))
  if tPostfix in usedKeys:
    raise Exception("BEFE_expression_ops: key %s (from %s) conflicts with %s (from %s)"%(repr(tPostfix),repr(key),repr(tPostfix),repr(usedKeys[tPostfix])))
  '''
  if tPrefix:  usedKeys[tPrefix]  = key
  if tInfix:   usedKeys[tInfix]   = key
  if tPostfix: usedKeys[tPostfix] = key
  operatorDefs[key] = (prec,l2r,tPrefix,tInfix,tPostfix)
  
# Build list of 'monadic' (single operand) operators...
prefixOps = []
infixOps = []
postfixOps = []

for key in operatorDefs:

  prec,l2r,prefix,infix,postfix = operatorDefs[key]
  # DEBUG...
  #if prec == 18:
  #  print("BEFE_expression_ops: key = %s: prec = %d, l2r = %s, prefix = %s, infix = %s, postfix = %s"%(repr(key),prec,repr(l2r),repr(prefix),repr(infix),repr(postfix)))
  # ...DEBUG

  if prefix  and prefix  not in prefixOps:  prefixOps.append(prefix)
  if infix   and infix   not in infixOps:   infixOps.append(infix)
  if postfix and postfix not in postfixOps: postfixOps.append(postfix)

prefixOps.sort()
infixOps.sort()
postfixOps.sort()
prefixOps  = tuple(prefixOps)
infixOps   = tuple(infixOps)
postfixOps = tuple(postfixOps)

# Additional validation, just in case...
def validate():

  errs = 0
  ignore = ('list','tuple','map','code','block','index')
  for key in infixOps:
    if key in ignore: continue
    if key in prefixOps:
      print("infixOps %s is duplicated in prefixOps"%repr(key))
      errs += 1
  for key in postfixOps:
    if key in ignore: continue
    if key in infixOps and key != 'dot':
      print("postfixOps %s is duplicated in infixOps"%repr(key))
      errs += 1
    if key in prefixOps:
      print("postfixOps %s is duplicated in prefixOps"%repr(key))
      errs += 1    
  if errs:
    raise InternalError("Validation check failure")

  # DEBUG...
  if debug and verboseAll:
    print("prefixOps  = %s"%repr(prefixOps))
    print("infixOps   = %s"%repr(infixOps))
    print("postfixOps = %s"%repr(postfixOps))
  # ...DEBUG
  
validate()

#
# Fiddle with statementDefs...
#

statements = {}

for key in statementDefs:
  minexp = MinExp(key)
  badCount = 0
  for i in range(0,len(minexp)):
    alternate = minexp[i]
    if alternate.lower() in statements:
      badCount += 1
      if badCount == 1:
        print("BEFE_expression_ops.py: Errors...")
      print("  Ambiguous statement %s"%repr(alternate))
      continue
    statements[alternate.lower()] = statementDefs[key]      

# Now, replace statementDefs...
statementDefs = statements
del statements

#
# Process qualifiers...
#

temp = []
for i in range(0,len(typeQualifiers)):
  m = MinExp(typeQualifiers[i])
  for possibility in m:
    if possibility in temp:
      raise InternalError("typeModifier conflict: %s already used by previous modifier"%repr(possibility))
    temp += [possibility]
  typeQualifiers[i] = m

temp = []
for i in range(0,len(nameQualifiers)):
  m = MinExp(nameQualifiers[i])
  for possibility in m:
    if m in temp:
      raise InternalError("nameModifier conflict: %s already used by previous modifier"%repr(possibility))
    temp += [possibility]
  nameQualifiers[i] = m

temp = []
for i in range(0,len(stringQualifiers)):
  m = MinExp(stringQualifiers[i])
  for possibility in m:
    if possibility in temp:
      raise InternalError("stringModifier conflict: %s already used by previous modifier"%repr(possibility))
    temp += [possibility]
  stringQualifiers[i] = m

# Build  allModifiers as list of distinct strings...
for m in typeQualifiers:
  for p in m:
    if p not in allQualifiers:
      allQualifiers.append(p)

for m in nameQualifiers:
  for p in m:
    if p not in allQualifiers:
      allQualifiers.append(p)

for m in stringQualifiers:
  for p in m:
    if p not in allQualifiers:
      allQualifiers.append(p)
  
#-------------------------------------------------------------------------------
#
# __main__...
#

if __name__ == "__main__":

 
  print("prefixOps  = %s"%repr(prefixOps))
  print("infixOps   = %s"%repr(infixOps))
  print("postfixOps = %s"%repr(postfixOps))
  