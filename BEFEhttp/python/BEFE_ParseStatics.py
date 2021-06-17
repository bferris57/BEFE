#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    BEFE_ParseStatics - Static data extensions to the Parse class
#
# Notes: This is included verbatim by BEFE_Parse.Parse so anything declared
#        here ends up in the Parse class. Pretty weird, eh?
#
#===============================================================================
# Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
#===============================================================================

from BEFE_Enum       import *
from BEFE_MinExpDict import *

# Parse styles: Freeform - C/C++ like, free flow
#               Line     - Simple contents, line by line
#               Indent   - Pyhonesque
#
Style = Enum("Freeform,Line,Indent")

tempHandlers = {"n[ame]s[pace][s]":None,
                "imp[ort]":None,
                "from":None,
                "loc[al]":None,
                "glob[al][s]":None,
                "func[tion]":None,
                "proc[edure]":None,
                "memb[er][s]":None,
                "meth[od][s]":None,
                "if":None,
                "else":None,
                "for":None,
                "while":None,
                "do":None,
                "until":None,
                "loop":None,
                "switch":None,
                "case":None,
                "alias":None,
                "struct":None,
                "class":None,
                "union":None,
                "type":None,
                "default":None,
                "finally":None,
                # For Python compatability...
                "def":None,
                "elif":None,
                # ...TEMP
               }

stmtHandlers = MinExpDict(tempHandlers)
