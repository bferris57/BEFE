//----------------------------------------------------------------------
// File: CryptFileHead.h - Declarations of the CryptFileHead Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CRYPTFILEHEAD_H // CRYPTFILEHEAD_H...
#define CRYPTFILEHEAD_H

#include "Primitives.h"
#include "Stream.h"
#include "PersistHead.h"

namespace BEFE { // Namespace BEFE...

struct CryptFileHead { // CryptFileHead Struct...

  // Members
  String      inFileName;       // ◄── Not persistent
  PersistHead pHead;
  UInt        keyType;
  UInt        inCrc;
  UInt        outCrc;
  Long        startTime;
  Long        endTime;
  
  // C++ Lifecycle
  CryptFileHead();
  CryptFileHead(CryptFileHead const &that);
  ~CryptFileHead();
  CryptFileHead &operator = (CryptFileHead const &that);
  
  // BEFE Lifecycle
  Status         StartUp();
  Status         ShutDown();
  Status         Reset();
  Boolean        IsNull() const;
  Status         SetNull();
  Boolean        IsEmpty() const;
  Status         SetEmpty();
  Status         MoveFrom(CryptFileHead const &that);
  Status         CopyFrom(CryptFileHead const &that);
  CryptFileHead &Consumable() const;
  CryptFileHead &NotConsumable() const;
  Boolean        IsConsumable() const;
  UInt           Length() const;
  UInt           Size() const;
  UInt           PersistSize() const;
  Status         WriteTo(Stream &out) const;
  Status         ReadFrom(Stream &in);

  // Operational Methods
  Boolean  IsValid() const;
  Strings  ToStrings() const;
  
}; // ...CryptFileHead Struct

//----------------------------------------------------------------------
//
// General CryptFileHead Functions
//

BEFE_INLINE Boolean IsNull(CryptFileHead const &that)    {return that.IsNull();}
BEFE_INLINE Status  SetNull(CryptFileHead &that)         {return that.SetNull();}
BEFE_INLINE Boolean IsEmpty(CryptFileHead const &that)    {return that.IsEmpty();}
BEFE_INLINE Status  SetEmpty(CryptFileHead &that)         {return that.SetEmpty();}
BEFE_INLINE Strings ToStrings(CryptFileHead const &that) {return that.ToStrings().Consumable();}  
  
} // ...Namespace BEFE

#endif // ...CRYPTFILEHEAD_H