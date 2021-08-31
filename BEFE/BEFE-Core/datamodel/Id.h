//!befe-publish inc
//----------------------------------------------------------------------
// File: Id.h - Declarations for the Id, Id32, and Id64 Classes
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for <Ids>. As such, it appears
// throughout the product when manipulating <Ids> and <Id Spaces>.
//
// The purpose of an Id class is to maintain a distinct (eg, never
// reused) identifier for instances of things throughout the lifetime
// of an <Id Space>.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ID_H // ID_H...
#define ID_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

// Generic typedefs
#ifdef IS64BIT
  typedef struct Id64 Id;
#endif
#ifdef IS32BITA
# error This should not be happening!!!
  typedef struct Id32 Id;
#endif

//----------------------------------------------------------------------
//
// Id32 Struct
//

struct Id32 { // Struct Id32...

  // Friend classes
  friend class IdSpace;

  // Members
  UInt32 value;
  
  //
  // Public Instance methods
  //

  // C++ Lifecycle
  BEFE_inline  Id32()                           {value = UInt32NaN;};
  BEFE_inline  Id32(Id32 const &thatId)         {value = thatId.value;};
  BEFE_inline  Id32(UInt32 thatId)              {value = thatId;};
  BEFE_inline ~Id32()                           {};

  BEFE_inline Id32 &operator = (Id32 const &that) {value = that.value; return *this;};
  BEFE_inline Id32 &operator = (UInt32 someId)    {value = someId; return *this;};
  BEFE_inline       operator UInt32 () const      {return value;};

  // BEFE Lifecycle
  Boolean IsNull() const         {return (value == UInt32NaN);}
  Status  SetNull()              {value = UInt32NaN; return Error::None;}
  
  // Comparison operators
  BEFE_inline bool operator==    (const Id32 &that) {return value == that.value;};
  BEFE_inline bool operator==    (UInt32 that)      {return value == that;};
  BEFE_inline bool operator!=    (const Id32 &that) {return value != that.value;};
  BEFE_inline bool operator!=    (UInt32 that)      {return value != that;};

  // Various other operators
  BEFE_inline UInt32 operator++  ()               {return ++value;};
  BEFE_inline UInt32 operator++  (int)            {return value++;};
  BEFE_inline operator UInt32 const  ()          {return value;}
  
}; // ...Struct Id32

//----------------------------------------------------------------------
//
// Id64 Struct
//

struct Id64 { // Struct Id64...

  // Friend classes
  friend class IdSpace;

  // Members
  UInt64 value;
  
  //
  // Public Instance methods
  //

  // C++ Lifecycle
  BEFE_inline  Id64()                           {value = UInt64NaN;};
  BEFE_inline  Id64(Id64 const &thatId)         {value = thatId.value;};
  BEFE_inline  Id64(UInt64 thatId)              {value = (UInt64)thatId;};
  BEFE_inline ~Id64()                           {};

  BEFE_inline Id64 &operator = (Id64 const &that) {value = that.value; return *this;};
  BEFE_inline Id64 &operator = (UInt64 someId)    {value = someId; return *this;};
  BEFE_inline       operator UInt64 () const      {return value;};

  // BEFE Lifecycle
  Boolean IsNull() const         {return (value == UInt64NaN);}
  Status  SetNull()              {value = UInt64NaN; return Error::None;}
  
  // Comparison operators
  BEFE_inline bool operator==    (const Id64 &that) {return value == that.value;};
  BEFE_inline bool operator==    (UInt64 that)      {return value == that;};
  BEFE_inline bool operator!=    (const Id64 &that) {return value != that.value;};
  BEFE_inline bool operator!=    (UInt64 that)      {return value != that;};

  // Various other operators
  BEFE_inline UInt64 operator++  ()               {return ++value;};
  BEFE_inline UInt64 operator++  (int)            {return value++;};
  BEFE_inline operator UInt64 const  ()           {return value;}
  
}; // ...Struct Id64

//
// Struct Id128
//

class Id128 { // Class Id128...

  // Friend classes
  friend class IdSpace;

  //
  // Public Instance methods
  //

   Id128()            {id0 = id1 = UIntNaN;};
  ~Id128()            {};

  // BEFE Lifecycle
  Status SetNull();

  // C++ Methods
  Id128 &operator =  (const Id128 &that) {id0 = that.id0; id1 = that.id1; return *this;};

  //
  // Protected members
  //

  protected: UInt64 id0;
  protected: UInt64 id1;

}; // ...Class Id123

//----------------------------------------------------------------------
//
// Id BEFE Lifecycle...
//

// BEFE Lifecycle
BEFE_INLINE Boolean  IsNull   (Id32 const &rThis)  {return rThis.IsNull();}
BEFE_INLINE Status   SetNull  (Id32 &rThis)        {BEFE::SetNull(rThis.value); return Error::None;}

} // ...Namespace BEFE

#endif // ...ID_H
