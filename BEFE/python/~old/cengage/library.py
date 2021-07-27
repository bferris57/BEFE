#----------------------------------------------------------------------
# File: library.py - Module for commonly used Cengage Library Application
#----------------------------------------------------------------------
#
# This module is the main DATA MANIPULATION module for the sample
# Cengage Python programming test, as written by Bruce Ferris
#
# The "given" part of the test is the following...
#
#   Task: Write a small Library application in the python language.
#
# As such, questions like "what is meant by 'application'", "does it
# need to be single or multi-threaded?", etc. are not specified.
#
# So, with that in mind, I've focused on the Library data structures,
# classes and methods, etc. needed to backup a small and lightweight
# console command based application instead of writing a weighty TK or
# other messy and time consuming GUI front end.
#
# The main focus here has been on writing some Python classes and
# methods that, as the specification expands, the existing code will
# remain fairly backward compatible and unlikely to fail drastically.
#
# Note: Since Python is case sensitive as far as names are concerned,
#       I use "initcaps" as a naming convention for all python classes
#       and their members.  This BY NO MEANS should promote or favour
#       such an approach across the board...  I just did it here so
#       that there no naming convention clashes with Python and its
#       language keyword underpinnings.  ;-)
#
# Note: Since it's clear from the outset that any professional
#       "library application" needs to ensure the consistency of the
#       underlying data AND that this data may need exist somewhere
#       other than memory as the specifications progress and, in
#       addition, the data itself if central to the design/implementation
#       and I've tried to make it transaction/journal based.
#
# Note: Througout the implementation I've introduced the concept of
#       an Id...  Unlike a Python id, an Id is sequential number that
#       is allocated, in increasing order, ensure that it uniquely
#       identifies unique objects within whatever space the Id is in.
#
#       These are mainly used for the Id members of LibItem instances
#       but are used elsewhere as well.
#
# Note: Beware... I don't personally enjoy Python exceptions as a form
#       of stanard process flow control.  This is basically because
#       they're not easily marshalled across remote procedure calls.
#       So, what I've done is implement a rather pedestian calling
#       sequence in many places here so that most functions that can,
#       or are likely to, fail return a single integer instead of
#       raising an exception. This integer indicates an error message
#       (see liberrors.h).
#
#       This doesn't mean I don't write code that deals with Python
#       exceptions, it just means that I've chosen this convention for
#       the code I've written here, for various reasons to be discussed
#       elsewhere.
#
#----------------------------------------------------------------------

from   liberrors import LibError
import libitems

HighItemId = 0
ItemTypes = ["***Unknown***","Book","eBook","CD","DVD","Periodical","Newspaper","Location","Customer"]

#----------------------------------------------------------------------
#
# Class: Library - Main driving class for the whole lot
#
# This class is the container for all LibItem<xxx> instances.
#

class Library:

  def __init__(self):
  
    self.items = []

  #------------------
  #
  # Item Type methods
  #
  #------------------
 
  def IsValidItemTypeName(self, name): 
    ''' Answer: "Is this a known ItemType Name?" '''
    if GetItemType(name) != 0:
      return True
    return False
    
  def GetItemTypeByName(self, name):
    ''' Returns ItemType index (int) given a name '''

    global ItemTypes

    if ItemTypes.count(name) != 0:
      return ItemTypes.index(name)
    return 0
  
  #-------------
  #
  # LibItem Methods
  #
  #-------------
  
  def AllocateLibItemId(self):
    ''' Allocates a new LibItem Id and bumps the high water mark '''

    global HighItemId
  
    id = HighItemId + 1
    HighItemId = HighItemId + 1
    
    return id

  def NewLibItemByTypeName(self, name):
    ''' Returns a new LibItem<xxx> instance or None, given <xxx> name '''
  
    if name == "Book":
      item = libitems.LibItem_Book(self)
    elif name == "eBook":
      item = libitems.LibItem_eBook(self)
    elif name == "CD":
      item = libitems.LibItem_CD(self)
    elif name == "DVD":
      item = libitems.LibItem_DVD(self)
    elif name == "Periodical":
      item = libitems.LibItem_Periodical(self)
    elif name == "Newspaper":
      item = libitems.LibItem_Newspaper(self)
    elif name == "Location":
      item = libitems.LibItem_Location(self)
    elif name == "Customer":
      item = libitems.LibItem_Customer(self)
    else:
      item = None

    return item

  def NewLibItemByTypeIndex(self, index):
    ''' Returns a new LibItem<xxx> instance or None, given Type Index '''
 
    if type(index) != int or index <= 0 or index >= len(ItemTypes):
      return None
    
    return self.NewLibItemByTypeName(ItemTypes[index])
    
  def UpdateLibItem(self, item):
    ''' Updates/Inserts a given item and return 0 if it is valid.
        If the item is NOT valid, a LibError number is returned.
        
        Note: If the item's Id is None, and the item is valid,
              a new Id will be allocated, the item will be
              inserted, and the caller's item.Id will be updated
              to reflect the new Id.
    '''

    if not item.IsValid():
      return LibError("library.LibItemInvalid")

    # If no Id, allocate a new one
    if item.Id == None:
      item.Id = self.AllocateLibItemId()
      index = None
      
    # Otherwise, find it...
    else:
      index = FindLibItemById(item.Id)

    # Take a copy of it      
    newItem = item.Copy()
    
    # Update or Insert it...
    if index == None:
      self.items.append(newItem)
    else:
      self.items[index] = newItem
      
    return 0;

  #-------------
  #
  # Query methods
  #
  # Note: If the items were stored on a database somewhere,
  #       these methods could easily be turned into code that
  #       invoked the proper SQL queries.
  #
  #-------------
  
  def FindLibItemById(self, id):
    ''' Returns a given LibItem's index (or None), given the Item's Id '''

    for index in range(0, length(self.items)):
      if self.items[index].Id == id:
        return index
      
    return None
    
  def GetLibItemById(self, id):
    ''' Returns a copy of LibItem (or None), given the Item's Id '''

    index = self.FindItemById(id)
    if index == None:
      return None
    return self.items[index].Copy()
    