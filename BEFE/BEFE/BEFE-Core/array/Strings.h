//!befe-publish inc
//----------------------------------------------------------------------
// File: Strings.h - Declarations for the Strings class
//----------------------------------------------------------------------
//
// The Strings class implements an array of Strings.  Each instance of
// this class is a variable length array of Strings.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef STRINGS_H // STRINGS_H...
#define STRINGS_H

#include "Primitives.h"
#include "Array.h"
#include "Sort.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// StringsSortTarget Class - For sorting
//

class StringsSortTarget : public SortTarget {

  public: Strings *target;
  public: String  *buf;
  public: UInt     numElms;
  public: Boolean  sensitive;

  public: StringsSortTarget();
  public: StringsSortTarget(SortTarget &target);
  public: ~StringsSortTarget();

  // SortTarget overrides...
  public: virtual UInt    Length () const;
  public: virtual Int     Compare(UInt elm1, UInt elm2);
  public: virtual Status  Swap(UInt elm1, UInt elm2);
 	public: virtual Status  Move(UInt elm1, UInt elm2);
 	public: virtual Boolean Selected(UInt elm) const;

};

//
// Class: Strings - Array of Strings
//

class Strings : public ArrayValue<String> { // Class Strings...

  // C++ Overrides
  public: Strings &Consumable() const {
    return (Strings &)ArrayValue<String>::Consumable();
  }
  
  // Strings specific methods
  public: String   Join(char sep) const;
  public: String   Join(Char sep) const;
  public: String   Join(String const &sep) const;
  public: Status   Join(String const &sep, String &outString) const;
  
  public: Int      Find(String const &str) const;
  public: Boolean  Contains(String const &str) const;
  
  public: Boolean  IsSensitive();     // "Is list String sensitive?"
  public: Boolean  IsInsensitive();   // "Is list String insesitive?"
  public: Status   Sensitive();       // ◄── Make all Strings in Array case sensitive
  public: Status   Insensitive();     // ◄── Make all Strings in Array case insensitive
  public: Status   Sort();            // ◄── Sort (default: Insensitive)
  public: Status   SortSensitive();   // ◄── Case Sensitive Sort
  public: Status   SortInsensitive(); // ◄── Case Insensitive Sort
  public: Status   Sort(StringsSortTarget &target); // ◄── Sort with own specialised sort target

}; // ...Class Strings

//----------------------------------------------------------------------
//
// Commonly used Strings functions
//

BEFE_INLINE Boolean IsNull  (Strings const &t) {return ((Strings *)&t)->IsNull();}
BEFE_INLINE Status  SetNull (Strings &t)       {return ((Strings *)&t)->SetNull();}
BEFE_INLINE Boolean IsEmpty (Strings const &t) {return ((Strings *)&t)->IsEmpty();}
BEFE_INLINE Status  SetEmpty(Strings &t)       {return ((Strings *)&t)->SetEmpty();}

} // ...Namespace BEFE

#endif // ...STRINGS_H
