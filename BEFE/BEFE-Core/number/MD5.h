//!befe-publish inc
//----------------------------------------------------------------------
// File: MD5.h - MD5 Hash (RFC 1321) Implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Note: The low level code for MD5 was cribbed from L. Peter Deutsch's
//       (ghost@aladdin.com) 2002 implementation but tweeked to look
//       nice and fit into BEFE.
//
//       Have a look at "_Aladdin.h" for full Aladdin copyright
//       details.  We've put it in a single place and included that
//       header only in source/headers that were modified from the
//       originals.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MD5_H // MD5_H...
#define MD5_H

#include "Primitives.h"
#include "_Aladdin.h" // ◄── aladdin.com's copyright notice!!!

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct: MD5 - MD5 Checksum
//

struct MD5 {
  
  // Members
  Byte value[16];   // MD5 Checksum value
  
  // C++ Lifecycle
  BEFE_inline MD5()                           {StartUp();}
  BEFE_inline MD5(MD5 const &that)            {}
  BEFE_inline ~MD5()                          {}
  BEFE_inline MD5 &operator=(MD5 const &that) {CopyFrom(that); return (*this);}
  
  // BEFE Lifecycle
  Status  StartUp();
  Status  ShutDown();
  Status  Reset();
  Boolean IsNull() const;
  Status  SetNull();
  Boolean IsEmpty() const;
  Status  SetEmpty();
  Status  MoveFrom(MD5 const &that);
  Status  CopyFrom(MD5 const &that);
    
};

//----------------------------------------------------------------------
//
// Struct: MD5Calc - MD5 Calculator
//

struct MD5Calc {
  
  // Members
  Boolean isFinished:1;     // true = "Checksum is finished"
  UInt32 count[2];	        // Message length in bits, lsw first
  UInt32 abcd[4];		        // Digest buffer
  Byte   buf[64];		        // Accumulate block */
  MD5    checkSum;          // MD5 Hash
  
  // C++ Lifecycle
  BEFE_inline MD5Calc()                               {StartUp();}
  BEFE_inline MD5Calc(MD5Calc const &that)            {}
  BEFE_inline ~MD5Calc()                              {}
  BEFE_inline MD5Calc &operator=(MD5Calc const &that) {CopyFrom(that); return (*this);}
  
  // BEFE Lifecycle
  Status  StartUp();
  Status  ShutDown();
  Status  Reset();
  Boolean IsNull() const;
  Status  SetNull();
  Boolean IsEmpty() const;
  Status  SetEmpty();
  Status  MoveFrom(MD5Calc const &that);
  Status  CopyFrom(MD5Calc const &that);
  
  // Full hash Methods
  Status  Set(Byte *bytes, UInt len);
  Status  Set(String const &str);
  Status  Get(Byte &theBytes);
  Status  Get(MD5 &that);
  MD5     Get();
  
  // Incremental hash Methods
  Status  Append(Byte byte);
  Status  Append(Byte *bytes, UInt len);
  Status  Append(String const &str);
  Status  Finalise();
  
};

} // ...Namespace BEFE

#endif // ...MD5_H