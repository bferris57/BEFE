//!befe-publish inc
//----------------------------------------------------------------------
// File: CryptKey.h - Declarations of the CryptKey Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CRYPTKEY_H // CRYPTKEY_H...
#define CRYPTKEY_H

#include "Primitives.h"
#include "Stream.h"
#include "UUID.h"

namespace BEFE { // Namespace BEFE...

struct CryptKey { // CryptKey Struct...

  // Members
  UInt keyType;
  union {
    Byte key_0[sizeof(UUID)];
  };
  
  // C++ Lifecycle
  CryptKey();
  CryptKey(CryptKey const &that);
  ~CryptKey();
  CryptKey &operator = (CryptKey const &that);
  
  // BEFE Lifecycle
  Status    StartUp();
  Status    ShutDown();
  Status    Reset();
  Boolean   IsNull() const;
  Status    SetNull();
  Boolean   IsEmpty() const;
  Status    SetEmpty();
  Status    MoveFrom(CryptKey const &that);
  Status    CopyFrom(CryptKey const &that);
  CryptKey &Consumable() const;
  CryptKey &NotConsumable() const;
  Boolean  IsConsumable() const;
  UInt     Length() const;
  UInt     Size() const;
  Status   WriteTo(Stream &out) const;
  Status   ReadFrom(Stream &in);

  // Operational Methods
  Boolean  IsValid() const;
  String   ToString() const;
  Status   NewRandom();
  
}; // ...CryptKey Struct

//----------------------------------------------------------------------
//
// General CryptKey Functions
//

BEFE_INLINE Boolean IsNull(CryptKey const &that)   {return that.IsNull();}
BEFE_INLINE Status  SetNull(CryptKey &that)        {return that.SetNull();}
BEFE_INLINE String  ToString(CryptKey const &that) {return that.ToString().Consumable();}  
  
} // ...Namespace BEFE

#endif // ...CRYPTKEY_H