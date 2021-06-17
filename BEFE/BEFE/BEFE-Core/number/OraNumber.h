//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: OraNumber.h - Declaration of the OraNumber Class
//----------------------------------------------------------------------
//
// The Number Class implements a general purpose Oracle Number.
// Instances of this Class are very similiar to Oracle numbers...
// In fact, the this implementation uses the EXACT internal
// representation as Oracle numbers.
//
// *** See technical documentation for a more detailed description ***
//
// Each number can be treated as a string and sorted lexically without
// examining it's contents.
//
// A Number is actually a variable length (up to MAXNUMLEN-1 bytes)
// NUL terminated string totalling, at max, MAXNUMLEN bytes.
//
// Each number consists of the following parts...
//
//   o Sign
//   o Exponent
//   o Mantissa...
//     - Integer part
//     - Fractional part
//
// The Exponent is a signed power of 100 (100**+63 to 100**-63)
//
// The first byte is the sign/exponent byte in the following format...
//
//   ┌─┬─┬─┬─┬─┬─┬─┬─┐
//   │S│ Exponent    │
//   └─┴─┴─┴─┴─┴─┴─┴─┘
//
//   Where: S        - Negated sign (0 = Negative, 1 = Positive)
//          Exponent - 64 offset signed exponent (power of 100)
//
//                      Positive Numbers     Negative Numbers (see below)
//                      ----------------     ----------------
//                      0x01 = E 100** -63   0x01 = E 100** +63
//                      0x3f = E 100** -1    0x3f = E 100** 1
//                      0x40 = E 100** 0     0x40 = E 100** 0
//                      0x41 = E 100** 1     0x41 = E 100** -1
//                      0x7f = E 100** +63   0x7f = E 100** -63
//
// The exponent is base 100.  So E+1 = 100, E-1 = .01
//
// The next N bytes are the mantissa.  Each byte in the mantissa is
// is 2 decimal digits (0..99) +1.
//
// This ensures there will be no NUL bytes in the string.
//
// The special case of a ZERO LENGTH string indicates NULL.
//
// One byte strings have the following special values:
//
//   0x80 = Zero
//   0x81 = Positive infinity
//   0x3f = Negative infinity
// 
// Negative numbers (Sign == 0) have the exponent stored
// as 64 - exponent.  For example negative number with E+00
// is 0x40, E+01 = 0x3f, E-01 = 0x41.  Bytes in the mantissa
// are stored 100 - digits + 1.  For example, digits 01 = 0x64,
// digits 99 = 0x02, etc.
//
// Negative numbers will always have a trailing 0x66.
//
// This storage scheme (inverted sign, mantissa+1, negative
// numbers with 64-exponent and trailing 0x66) allows us to
// sort numbers lexically using a standard string compares.
//
//
// NUMBER LIMITS:    -9e+123 <= N <= 9e+123
//
// PRECISION LIMITS: -1e-130..1e+130
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ORANUMBER_H // ORANUMBER_H...
#define ORANUMBER_H

#include "Primitives.h"
#include "Error.h"

namespace BEFE { // Namespace BEFE...

//
// Handy defines
//

#define MAXNUMLEN   22  // Maximum bytes per number (same as Oracle)

//------------------------------------------------------------------------------
//
// Class: OraNumber
//

class OraNumber { // Class OraNumber...

  //
  // Public member functions
  //

  // C++ Lifecycle
  public: OraNumber(void);                      // Constructor
  public: OraNumber(char const *cstr);          // String constructor
  public: OraNumber(String const &str);         // String constructor
  public: OraNumber(OraNumber const &that);     // Copy constructor
  public: ~OraNumber();                         // Destructor
  public: OraNumber &operator= (OraNumber const &num);
  
  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown();
  public: Status  Reset();
  public: Boolean IsNull() const;
  public: Status  SetNull();
  public: Status  MoveFrom(OraNumber const &that);
  public: Status  CopyFrom(OraNumber const &that);
  public: UInt    Length() const;
  public: UInt    Size() const;
  
  // Setting contents
  public: Status FromString(String const &string); // Build Number from String

  // C++ stuff
  
  // Getting contents
  public: String ToString() const;
  public: Status ToString(String &string) const;
  
  // Operations
  public: Status Negate();            // Negate the number
  
  // Query contents
  public: Boolean IsZero() const;     // "Is this number zero?"
  public: Boolean IsNegative() const; // "Is this number less that zero?"
  
  // Comparison: -1 => Less, 0 => Equal, 1 => Greater
  public: Int Compare(OraNumber const &that);
  
  // Utility functions
  public: String ToHex() const;       // Dump to hex string
  public: String ToDebug() const;     // Dump to debuggable string
  
  //
  // Protected member functions
  //

  protected: void   SetNaN();         // Set to NaN
  
  //
  // Protected members
  //

  protected: Byte value[MAXNUMLEN];   // The number value
  
};  // ...Class OraNumber

} // ...Namespace BEFE

#endif // ...ORANUMBER_H