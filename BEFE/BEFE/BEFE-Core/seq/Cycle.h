//!befe-publish inc
//----------------------------------------------------------------------
// File: Cycle.h - Declarations for the Cycle class
//----------------------------------------------------------------------
//
// This Class is a sequence foundation class.  As such, it appears
// throughout the product when manipulating sequences.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CYCLE_H  // CYCLE_H...
#define CYCLE_H

#include "Primitives.h"

namespace BEFE {

struct Cycle { // Struct Cycle...

  // Members
  UInt curPos;
  UInt curLen;
  
  // C++ Lifecycle
  Cycle();
  Cycle(UInt len);
  Cycle(Cycle const &that);
  ~Cycle();
  
  // C++ Casting
  operator UInt() const;
  operator Int() const;

  // C++ Operators
  UInt operator  = (Cycle const &that);
  UInt operator  = (UInt pos);
  UInt operator  = (Int pos);

  UInt operator += (UInt val);
  UInt operator +  (UInt val) const;
  UInt operator += (Int  val);
  UInt operator +  (Int  val) const;

  UInt operator -= (UInt val);
  UInt operator -  (UInt val) const;
  UInt operator -= (Int  val);
  UInt operator -  (Int  val) const;

  // C++ Prefix/Posfix operators
  UInt operator ++ ();      // ◄── Prefix  ++
  UInt operator ++ (int);   // ◄── Postfix ++
  UInt operator -- ();      // ◄── Prefix  --
  UInt operator -- (int);   // ◄── Postfix --
  
  // Query/Adjustment
  UInt   Length() const;
  UInt   Position() const;
  Cycle &SetLength(UInt len);
  Cycle &SetPosition(UInt pos);
  
}; // ...Struct Cycle

} // ...Namespace BEFE

#endif // ...CYCLE_H