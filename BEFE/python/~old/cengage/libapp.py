#----------------------------------------------------------------------
# File: libapp.py - Main module for Library Command Processor
#----------------------------------------------------------------------
#
# This module pulls together the various Lib classes and acts like a
# prompting command processor that lets you populate/query a Lib
# instance.
#
# Note: Although the Lib stuff was written such that it "could be"
#       persistent, for now we just keep the stuff hanging around
#       in memory because I didn't have time to write a fully fledged
#       "proper application".
#
# Note: The main idea here is we prompt for commands, pull the
#       command line apart, and dispatch the command to whatever
#       handlers we've had time to plug in so far.
#
#----------------------------------------------------------------------

import sys

from library   import *
from libitems  import *
from liberrors import *
from libutils  import *

# Globals
TheLib = Library()

#----------------------------------------------------------------------
#
# Function: PromptLine - Prompt, input a single line, and return it
#

def PromptLine(prompt):

  # Prompt if we're supposed to
  if type(prompt) == str:
    sys.stdout.write(prompt)
    sys.stdout.flush()
    
  # Read the line
  line = sys.stdin.readline()
  if len(line) != 0 and line[-1] == '\n': line = line[0:-1]
  if len(line) != 0 and line[-1] == '\r': line = line[0:-1]
  
  return line
  
#----------------------------------------------------------------------
#
# Function: GetArgs - Pull an input line (str) apart into a list of arguments
#
# Note: We strip '"', "'" from both ends of each argument if the same
#       quote character is at both ends.
#

def GetArgs(input):

  args     = []
  curArg    = ''
  quoteChar = ''
    
  # For each character...
  for index in range(0,len(input)):

    char = input[index]
    
    # Append and continue if we're in a quote
    if len(quoteChar) != 0 and char != quoteChar:
      curArg = curArg + char
      continue
      
    # Finish quotes and continue if we found matching
    if len(quoteChar) != 0 and char == quoteChar:
      curArg = curArg + char
      quoteChar = ''
      continue
      
    # Start quotes if need be...
    if char == "'" or char == '"':
      curArg = curArg + char
      quoteChar = char
      continue
      
    # Stop if we're at command char(s)
    if char == '#' or input[index:index+2] == '//': break
    
    # Append if it's not whitespace...
    if ord(char) > 0x20 and ord(char) < 0x7f:
      curArg = curArg + char
      continue
      
    # At whitespace, strip and flush...    
    if len(curArg) > 2 and curArg[0] == curArg[-1] and \
       (curArg[0] == "'" or curArg[0] == '"'):
      curArg = curArg[1:-1]
    if len(curArg) != 0:
      args.append(curArg)
    curArg = ''

  # Append curArg if any
  if len(curArg) > 2 and curArg[0] == curArg[-1] and \
     (curArg[0] == "'" or curArg[0] == '"'):
    curArg = curArg[1:-1]
  if len(curArg):
    args.append(curArg)
  
  return args
  
#----------------------------------------------------------------------
#
# Function: Command_Help - Perform Help command
#

def Command_Help(args):

  if len(args):
    print "***Warning: 'help' doesn't currently have any arguments, ignored..."
    
  print '''
The following commands are currently implemented...

  h          - Display this help
  help       -    "     "    "
  x          - Exit the application
  e          -  "    "       "
  exit       -  "    "       "
  
  new <item> - Create a new Library Item (see below)


Where: <item> is one of the following (case insensitive)...

         o Customer
         o Location
         o Book
         o eBook
         o CD
         o DVD
         o Periodical
         o Newspaper
         o Location
         o Customer       
'''
  
#----------------------------------------------------------------------
#
# Function: Command_New - Perform New command
#

def Command_New(args):

  if len(args) > 2:
    print "***Error: 'new' only expects one argument (eg 'new <item>')"
    return
    
  type = args[1].lower()
  index = 0
  while index < len(library.ItemTypes):
    if library.ItemTypes[index].lower() == type: break
    index = index+1
    
  # Handle unknown item...
  if index >= len(library.ItemTypes):
    print "***Error: Unknown LibItem type '"+type+"'"
    return
  
  typeName = library.ItemTypes[index]

  # Create a new local LibItem of the correct type
  item = TheLib.NewLibItemByTypeName(typeName)

  # Calculate largest name
  maxNamLen = 0
  memberNames = item.GetMemberNames()
  for name in memberNames:
    maxNamLen = max(maxNamLen, len(name))
    
  # Prompt for each one...
  index = 2
  print "\n  Enter details for new '"+typeName+"' (type '..' to abort)...\n"

  while index < len(memberNames):

    # Prompt for the value     
    prompt = "    "+memberNames[index]+":"
    for i in range(0,maxNamLen-len(memberNames[index])):
      prompt = prompt+' '
    prompt = prompt+' '
    line = PromptLine(prompt).strip()
   
    # Finished if '..'
    if line == "..": break
   
    # Set it
    error = item.SetMember(memberNames[index], line)
    if error != 0:
      print "***Error: "+LibError(error)
      continue
    error = item.ValidateMember(memberNames[index])
    if error != 0:
      print "***Error: "+LibError(error)
      continue
    
    # Bump up to the next one
    index = index + 1
    
  # Output a blank line 
  print ""
  
#----------------------------------------------------------------------
#
# MAIN PROGRAM
#

commands = [
  ['h',    Command_Help],
  ['help', Command_Help],
  ['new',  Command_New]
  
]

if __name__ == "__main__":

  print '''
-----------------------------------------------
Welcome to Bruce's sample "Library Application"
-----------------------------------------------

Type 'help' for instructions...
'''
  while True:
  
    line = PromptLine("Library> ")
    if len(line) == 0: continue
    args = GetArgs(line)
    if len(args) == 0: continue
    
    # Handle special commands
    cmd = args[0].lower()
    if cmd == 'x' or cmd == 'e' or cmd == 'exit': break
    
    # Find and execute the command...
    didIt = False
    for command in commands:    
      if command[0] == cmd.lower():
        command[1](args)
        didIt = True
        break
        
    # Unknown command
    if not didIt:
      print "***Unknown command '"+cmd+"'"