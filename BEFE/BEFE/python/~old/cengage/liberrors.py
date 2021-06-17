#----------------------------------------------------------------------
# File: liberrors.py - Module for Library Error messages
#----------------------------------------------------------------------
#
# This module contains functions used to manage the Cengage Libary
# application error messages.
#
# From a pragmatic point of view, error messages are best realised as
# 32-bit integers.  However, to make more readible code from Python's
# point of view, we give each of them a name as well.
#
# This allows us to translate the messages (in the future) depending on
# the native language of the end user.
#
#----------------------------------------------------------------------

#----------------------------------------------------------------------
#
# Global Message dictionaries
#

LibMessageUnknown  = 9999

LibMessagesById    = {

     1:["libitems.BadId",             "Expected Id to be None or an integer"],
     2:["libitems.BadType",           "Expected Type to be an integer"],
     3:["libitems.BadLocation",       "Expected Location to be an integer"],
     4:["libitems.BadISDN",           "Expected ISDN to be a string"],
     5:["libitems.BadISSN",           "Expected ISSN to be a string"],
     6:["libitems.BadAuthor",         "Expected Author to be a string value"],
     7:["libitems.BadTitle",          "Expected Title to be a string value"],
     8:["libitems.BadFormat",         "Expected Format to be a string value"],
     9:["libitems.BadBarcode",        "Expected Barcode to be a string"],
    10:["libitems.BadTitle",          "Expected Title to be a string"],
    11:["libitems.BadArtist",         "Expected Artist to be a string"],
    12:["libitems.UnknownMember",     "Unknown member name"],
    
   100:["libutils.MalformedISDN",     "Expected ISDN to be a string in the form nn-n-nn-nnnnnn-n"],
   101:["libutils.MalformedISSN",     "Expected ISSN to be a string in the form nnnn-nnnn"],
   102:["libutils.MalformedDate",     "Expected Date to be a string in the form dd/mm/yyyy"],
   103:["libutils.MalformedAddress",  "Expected Address to be a multi-line string"],
   104:["libutils.MalformedPostcode", "Expected Postcode to a string in the form 'xxx yyy'"],

   201:["library.LibItemInvalid",     "The specified LibItem is invalid (use Validate for list of errors)"],
   
     0:["","***None***"],
   999:["NotImplemented",             "Requested action is not implemented yet"],
   
  LibMessageUnknown:["???","***Unknown***"]
  
}

LibMessagesByName = {}

#----------------------------------------------------------------------
#
# Function: LibError - Return Error message/Id
#
#    Usage: answer = LibError(msg)
#
#    Where: msg    - Message Id (if int), or Message Name (if string)
#           answer - Message Id (if msg is string), or Name (if msg is int)
#

def LibError(msg):

  global LibMessagesById
  global LibMessagesByValue
  global LibMessageUnknown
  
  # Populate LibMessagesByName if we need to
  _PopulateLibMessagesByName()
  
  # If it's an int...
  if type(msg) == int:
  
    # Add a "made up on the fly" on if it's not there yet...
    if not LibMessagesById.has_key(msg):
      return "Error " + repr(msg)
      
    # Return the passed error Id...
    return LibMessagesById[msg][1]

  # If not a string, return bogus string...
  if type(msg) != str:
    return "***???***"
    
  # Not an int, assume it's a string and see if it's already there...
  if LibMessagesByName.has_key(msg):
    id = LibMessagesByName[msg]
    return id

  # It's a string and it's there, return the Id...
  return LibMessageUnknown

#----------------------------------------------------------------------
#
# Function: FindLibError - Given a LibError name, find the int LibError
#
#    Usage: liberror = FindLibError(name)
#
#    Where: name     - The LibError's name
#           liberror - The LibError's number (or LibMessageUnknown)
#

def FindLibError(name):

  global LibMessageUnknown
  global LibMessagesByName
  
  _PopulateLibMessagesByName()
  
  if type(name) != str or not LibMessagesByName.has_key(name):
    return LibMessageUnknown
    
  return LibMessagesByName[name]
  
#----------------------------------------------------------------------
#
# Function: _PopulateLibMessagesByName - Internal: Populate the LibMessageByName global
#                                                  if it's not already populated
#

def _PopulateLibMessagesByName():

  global LibMessagesById;
  global LibMessagesByName;

  if len(LibMessagesByName) == 0:
  
    for key in LibMessagesById.keys():
      name, value = LibMessagesById[key]
      LibMessagesByName[name] = key
      
  return

  