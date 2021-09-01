//!befe-publish inc
//----------------------------------------------------------------------
// File: Number.h - Declaration of the Number Class
//----------------------------------------------------------------------
//
//  The Number Class implements a general purpose Number.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef NUMBER_H // NUMBER_H...
#define NUMBER_H

#include "Primitives.h"
#include "Error.h"

namespace BEFE { // Namespace BEFE...

//
// Handy defines
//

#define MAXNUMLEN   22  // Maximum bytes per number (same as Oracle)

//------------------------------------------------------------------------------
//
// Class: Number
//

class Number { // Class Number...

  //
  // Public member functions
  //

  // C++ Lifecycle
  public: Number(void);               // Constructor
  public: Number(char const *cstr);   // String constructor
  public: Number(String const &str);  // String constructor
  public: Number(Number const &that); // Copy constructor
  public: ~Number();                  // Destructor
  
  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown();
  public: Status  Reset();
  public: Boolean IsNull() const  { return IsNaN(); };
  public: Status  SetNull()       { return Reset(); };
  public: Boolean IsEmpty() const { return Length() == 0; };
  public: Status  SetEmpty()      { return Reset(); };
  public: Status  MoveFrom(Number const &that);
  public: Status  CopyFrom(Number const &that);
  public: Number &Consumable() const;           // ◄── Called mainly before returning local Number
  public: Number &NotConsumable() const;        // ◄── Called mainly from MoveFrom
  public: Boolean IsConsumable() const;         // ◄── "Is this Number Consumable?"
  public: UInt    Length() const;
  public: UInt    Size() const;

  // C++ Operators
  public: Boolean operator == (Number const &that) const;
  public: Number &operator =  (Number const &that);
  
  // Setting contents
  public: Status  FromString(String const &string); // Build Number from String

  // Getting contents
  public: String  ToString() const;
  public: Status  ToString(String &string) const;
  public: String  ToExpString() const;
  public: Status  ToExpString(String &string) const;  
  
  // Operations
  public: Status  Negate();            // Negate the number
  
  // Query
  public: Boolean IsNaN() const;      // "Is this number NaN?"
  public: Boolean IsZero() const;     // "Is this number zero?"
  public: Boolean IsNegative() const; // "Is this number less that zero?"
  public: Int     Exponent() const;   // Get the Number's base 10 exponent
  public: Span    ExponentSpan() const;// Get the Number's base 10 exponent Span
  public: UInt    Hash() const;       // get the Number's Hash value
  
  // Comparison
  public: Int Compare(Number const &that) const;
  
  //
  // Protected Methods
  //

  protected: void   SetNaN();         // Set to NaN
  
  //
  // Protected members
  //

  protected: Boolean sign:1;          // The Number sign (true = Negative)
  protected: Boolean eSign:1;         // The exponent sign (true = Negative)
  protected: Boolean isInf:1;         // Is Infinite? (true = Infinite)
  protected: UInt    len:3;           // Number length in 100's digits (0=Buffer, >0 = Length 1..6)
  protected: Int     exp:10;          // Exponent (0..1023, power of 10)
  protected: Byte    value[2];        // The Digits (if type > 0)
  union {
    Byte  digits2to5[4];
    Byte *pointer;
  } u;
  
  // For debugging purposes...
  public: Strings ToDebugStrings();
    
};  // ...Class Number

//----------------------------------------------------------------------
//
// General String Functions...
//

Boolean IsNull(Number const &v);

} // ...Namespace BEFE

#endif // ...NUMBER_H