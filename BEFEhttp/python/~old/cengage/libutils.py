#----------------------------------------------------------------------
# File: libutils.py - Module for various Library functions
#----------------------------------------------------------------------
#
# This module contains various miscellaneous functions used to validate
# values and such...
#
#   ValidateISBN     - Validate an ISBN 2007 string (13 digits)
#   ValidateISSN     - Validate an ISSN string (9 digits)
#   ValidateDate     - Validate a Date string (dd/mm/yyyy)
#   ValidateAddress  - Validate an Address string
#   ValidatePostcode - Validate a Postcode string
#
#----------------------------------------------------------------------

from liberrors import *

#----------------------------------------------------------------------
#
# Function: ValidateISDN - Validate an ISDN 2007 string (13 digits)
#
#    Usage: result = ValidateISDN(isbn)
#
#    Where: isbn   - The ISDN string to validate
#           result - LibError number (0 = Okay)
#
# Note: ISBN 2007 numbers are in the following format...
#
#         nnn-n-nn-nnnnnn-n
#
#       Where the 'n' indicates a numeric digit
#

def ValidateISDN(isdn):

  message = "libutils.MalformedISDN"
  
  if type(isdn) != str:
    return LibError(message)
    
  parts = isdn.split('-')
  if len(parts)    != 5 or \
     len(parts[0]) != 2 or \
     len(parts[1]) != 1 or \
     len(parts[2]) != 3 or \
     len(parts[3]) != 6 or \
     len(parts[4]) != 1:
    return LibError(message) 

  for part in parts:
    for char in part:
      if not char in "0123456789":
        return LibError(message)

  return 0
  
#----------------------------------------------------------------------
#
# Function: ValidateISSN - Validate an ISSN string (9 digits)
#
#    Usage: result = ValidateISSN(issn)
#
#    Where: issn - The ISSN string to validate
#           result - LibError number (0 = Okay)
#
# Note: ISSN numbers are in the following format...
#
#         nnnn-nnnn
#
#       Where the 'n' indicates a numeric digit
#

def ValidateISSN(issn):

  message = "libutils.MalformedISSN"
  
  if type(issn) != str:
    return LibError(message)

  parts = issn.split('-')
  if len(parts)    != 2 or \
     len(parts[0]) != 4 or \
     len(parts[1]) != 4:
    return LibError(message) 

  for part in parts:
    for char in part:
      if not char in "0123456789":
        return LibError(message)

  return 0
  
#----------------------------------------------------------------------
#
# Function: ValidateDate - Validate a date string (dd/mm/yyyy)
#
#    Usage: result = ValidateDate(date)
#
#    Where: date   - The Date string to validate
#           result - LibError number (0 = Okay)
#

def ValidateDate(date):

  message = "libutils.MalformedDate"
  if type(date) != str:
    return LibError(message)
    
  parts = date.split('/')
  if len(parts)    != 3 or \
     len(parts[0]) != 2 or \
     len(parts[1]) != 2 or \
     len(parts[2]) != 4:
    return LibError(message)
    
  for part in parts:
    for char in part:
      if not char in "0123456789":
        return LibError(message)

  return 0
  
#----------------------------------------------------------------------
#
# Function: ValidateAddress - Validate an Address string
#
#    Usage: result = ValidateAddress(address)
#
#    Where: address - The Address string to validate
#           result  - LibError number (0 = Okay)
#

def ValidateAddress(address):

  message = "libutils.MalformedAddress"
  if type(address) != str:
    return LibError(message)
    
  return 0
 
#----------------------------------------------------------------------
#
# Function: ValidatePostcode - Validate a Postcode string
#
#    Usage: result = ValidatePostcode(postcode)
#
#    Where: postcode - The Postcode string to validate
#           result   - LibError number (0 = Okay)
#

def ValidatePostcode(postcode):

  if type(postcode) != str:
    return LibError("libutils.MalformedPostcode")
  
  return 0

#----------------------------------------------------------------------
#
# Function: ValidateIdString - Validate an Id string
#
#    Usage: result = ValidateIdString(idString)
#
#    Where: idString - The Id string to validate
#           result   - LibError number (0 = Okay)
#

def ValidateIdString(idString):

  if type(idString) == type(None):
    return 0

  if type(idString) != str:    
    return LibError("libutils.BadId")

  idString = idString.strip()
  if len(idString) > 9:
      return LibError("libutils.BadId")
  
  for c in idString:
    if '0123456789'.count(c) == 0:
      return LibError("libutils.BadId")
      
  return 0
  