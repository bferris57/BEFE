#----------------------------------------------------------------------
# File: libitem.py - Module for LibItem class and specialisations
#----------------------------------------------------------------------
#
# This module is the main DATA MANIPULATION module for the sample
# Cengage Python programming test, as written by Bruce Ferris
#
# The BASE class here is LibItem. Each LibItem instance has, at least,
# an Id and a Type member.
#
# Most LibItems are kept in stock and are subject to being "placed"
# except for subtypes of LibItem_Location, which indicates the location
# where an item is.  The only oddity is with LibItem_Customer, which
# doesn't imply a physical location.  ;-)
#
#----------------------------------------------------------------------

import library
import libutils
from   liberrors import LibError

#----------------------------------------------------------------------
#
# Class LibItem: BASE class for all LibItems
#

class LibItem:

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id", "Type"]
    self.__dict__["members"]     = {"Id":None, "Type":0}
 
  def __getattr__(self, name):
  
    if self.__dict__.has_key(name):
      return self.__dict__[name]
    if self.__dict__["members"].has_key(name):
      return self.members[name]
    return None

  def __setattr__(self, name, value):

    if self.__dict__["members"].has_key(name):
      self.__dict__["members"][name] = value
    else:
      self.__dict__[name] = value

  def CopyFrom(self, that):
    newItem.lib         = that.lib
    newItem.memberNames = that.memberNames[:]   
    return self
 
  def Copy(self):
    newItem = LibItem()
    newItem.CopyFrom(self)
    return newItem     
     
  def SetMember(self, name, value):
  
    if self.memberNames.count(name) == 0:
      return LibError("libitems.UnknownMember")

    oldValue = self.__getattr__(name)     
    self.__setattr__(name, value)
    error = self.ValidateMember(name)
    if error:
      self.__setattr__(name, oldValue)
      return error
      
    return 0
    
  def IsValid(self):
  
    for name in self.memberNames:
      result = self.ValidateMember(name)
      if result != 0:
        return False

    return True
    
  def Validate(self):
  
    results = []
    for name in self.memberNames:
      result = self.ValidateMember(name)
      if result != 0:
        results.append(result)
        
    return results
    
  def ValidateMember(self, name):
 
   if name == "Id":
     if self.Id != None and type(self.Id) != int:
       return LibError("libitems.BadId")
     return 0
    
   if name == "Type":
     if type(self.Type) != int:
       return LibError("libitems.BadType")

   if name == "Location":
     if self.memberNames.count("Location") != 0:
       if type(self.Location) == str:
         error = libutils.ValidateIdString(self.Location)
         if error != 0:
           return LibError("libitems.BadLocation")
       elif type(self.Location) != int:
         return LibError("libitems.BadLocation")
       
   return 0
  
  def GetMemberNames(self):

    return self.memberNames[:]

  def GetMemberByName(self, name):
  
    if self.__dict__["members"].has_key(name):
      return self.Members[name]
      
    return None
    
#----------------------------------------------------------------------
#
# Class LibItem_Book: A Physical Book
#

class LibItem_Book(LibItem):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Location","ISDN","Author","Title"]
    self.__dict__["members"]     = {"Id"       : None,
                                    "Type"     : self.lib.GetItemTypeByName("Book"),
                                    "Location" : None,
                                    "ISBN"     : None,
                                    "Author"   : None,
                                    "Title"    : None
                                   }
  
  def Copy(self):
    newItem = LibItem_Book()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):

    if name == "Type":
      if type(self.Type) != int:
        return LibError("libitems.BadType")
      return 0
      
    if name == "ISBN":
      if type(self.ISDN) != str:
        return LibError("libitems.BadISBN")
      return libutils.ValidateISDN(self.ISBN)
      
    if name == "Author":
      if type(self.Author) != str:
        return LibError("libitems.BadAuthor")
      return 0

    if name == "Title":
      if type(self.Title) != str:
        return LibError("libitems.BadTitle")
      return 0
      
    return 0
    
#----------------------------------------------------------------------
#
# Class LibItem_eBook: An eBook
#

class LibItem_eBook(LibItem_Book):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Location","ISBN","Author","Title","Format"]
    self.__dict__["members"]     = {"Id"       : None,
                                    "Type"     : self.lib.GetItemTypeByName("eBook"),
                                    "Location" : None,
                                    "ISBN"     : None,
                                    "Author"   : None,
                                    "Title"    : None,
                                    "Format"   : None}
  
  def Copy(self):
    newItem = LibItem_eBook()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):
  
    result = LibItem_Book.ValidateMember(self, name)
    if result != 0:
      return result
    
    if name == "Format":
      if type(self.Format) != str:
        return LibError()
  
    return 0
    
#----------------------------------------------------------------------
#
# Class LibItem_CD: A Physical CD
#

class LibItem_CD(LibItem):

  def __init__(self, lib):

    self.__dict__["lib"]         = lib  
    self.__dict__["memberNames"] = ["Id","Type","Location","Barcode","Title","Artist"]
    self.__dict__["members"]     = {"Id"       : None,
                                    "Type"     : lib.GetItemTypeByName("CD"),
                                    "Location" : None,
                                    "ISBN"     : None,
                                    "Author"   : None,
                                    "Title"    : None}
  
  def Copy(self):
    newItem = LibItem_CD()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):
  
    result = LibItem.ValidateMember(self, name)
    if result != 0:
      return result

    if name == "Barcode":
      if type(self.Barcode) != str:
        return LibError("libitems.BadBarcode")
      return 0

    if name == "Title":
      if type(self.Title) != str:
        return LibError("libitems.BadTitle")
      return 0

    if name == "Artist":
      if self.Artist != None and type(self.Artist) != str:
        return LibError("libitems.BadArtist")
  
    return 0
      
#----------------------------------------------------------------------
#
# Class LibItem_DVD: A physical DVD
#

class LibItem_DVD(LibItem_CD):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Location","Barcode","Title","Cast","Director"]
    self.__dict__["members"]     = {"Id"       : None,
                                    "Type"     : self.lib.GetItemTypeByName("DVD"),
                                    "Location" : None,
                                    "Barcode"  : None,
                                    "Title"    : None,
                                    "Cast"     : None,
                                    "Director" : None}
  
  def Copy(self):
    newItem = LibItem_DVD()
    newItem.CopyFrom(self)
    return newItem     
     
#----------------------------------------------------------------------
#
# Class LibItem_Newspaper: A printed Newspaper
#

class LibItem_Newspaper(LibItem):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Location","Name","Date","Period"]
    self.__dict__["members"]     = {"Id"     : None,
                                    "Type"   : self.lib.GetItemTypeByName("Newspaper"),
                                    "Name"   : None,
                                    "Date"   : None,
                                    "Period" : None}
  
  def Copy(self):
    newItem = LibItem_Newspaper()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):

    result = LibItem.ValidateMember(self, name)
    if result != 0:
      return result
      
    if name == "Date":
      result = libutils.ValidateDate(self.Date)
      if result != 0:
        return result
        
    return 0
    
#----------------------------------------------------------------------
#
# Class LibItem_Periodical: A printed Periodical
#

class LibItem_Periodical(LibItem_Newspaper):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Location","ISSN", "Name", "Date", "Period"]
    self.__dict__["members"]     = {"Id"       : None,
                                    "Type"     : self.lib.GetItemTypeByName("Periodical"),
                                    "Location" : None,
                                    "ISSN"     : None,
                                    "Name"     : None,
                                    "Date"     : None,
                                    "Period"   : None}
  
  def Copy(self):
    newItem = LibItem_Periodical()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):
  
    result = LibItem_Newspaper.ValidateMember(self, name)
    
    return result

#----------------------------------------------------------------------
#
# Class LibItem_Location: A storage location
#

class LibItem_Location(LibItem):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Address","Postcode"]
    self.__dict__["members"]     = {"Id"       : None,
                                    "Type"     : self.lib.GetItemTypeByName("Location"),
                                    "Address"  : None,
                                    "PostCode" : None}
  
  def Copy(self):
    newItem = LibItem_Location()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):
  
    result = LibItem.ValidateMember(self, name)
    if result != 0:
      return result

    # Note: Addresses can be None or blank if not type "Customer"
    if name == "Address" and self.Type == self.lib.GetItemTypeByName("Customer"):
      result = libutils.ValidateAddress(self.Address)
      return result
    
    if name == "Postcode":
      result = libutils.ValidatePostcode(self.Postcode)
      return result
      
    return 0
    
#----------------------------------------------------------------------
#
# Class LibItem_Customer: A customer who checks out other items
#
# Note: This may seem weird, but we nomally modelled Customer as a
#       location because it all fits together more cleanly this way.
#

class LibItem_Customer(LibItem_Location):

  def __init__(self, lib):
  
    self.__dict__["lib"]         = lib
    self.__dict__["memberNames"] = ["Id","Type","Title","FirstName","Surname","Address","Postcode"]
    self.__dict__["members"]     = {"Id"        : None,
                                    "Type"      : self.lib.GetItemTypeByName("Customer"),
                                    "FirstName" : None,
                                    "Surname"   : None,
                                    "Address"   : None,
                                    "PostCode"  : None}
  
  def Copy(self):
    newItem = LibItem_Customer()
    newItem.CopyFrom(self)
    return newItem     
     
  def ValidateMember(self, name):
  
    result = LibItem_Location.ValidateMember(self, name)
    return result
