//!befe-publish inc
//----------------------------------------------------------------------
// File: PersistHead.h - Persistence Header related declarations
//----------------------------------------------------------------------
//
// This header contains the declarations releated to Persistent 
// BEFE Headers.
//
// A BEFE Persistent header is a 16-byte header in the following basic
// format...
//
// Palette: ─ │ ┌ ┐ └ ┘ ├ ┤ ∟ ┬ ┴ ┼
//
//  ┌───┬───┼───┬───┼───┬───┼───┬───┬───┬───┼───┬───┬───┬───┼───┬───┐
//  │0xBEFE │ VerHi │ VerLo │ Revision      │ TypeName      │TypeRev│
//  ├───┴───┼───┴───┼───┴───┼───┴───┴───┴───┼───┴───┴───┴───┼───┴───┤
//   └──┬──┘ └──┬──┘ └──┬──┘ └──────┬──────┘ └──────┬──────┘ └──┬──┘
//      │       │       │           │               │         Type Revision
//      │       │       │           │               │         (Binary, MSB)
//      │       │       │           │               │
//      │       │       │           │               └── Type Name
//      │       │       │           │                   (Blank Padded)
//      │       │       │           │
//      │       │       │           └── BEFE Revision (Binary, MSB)
//      │       │       └── BEFE Sub-Version
//      │       └── BEFE Main-Version
//      └── 0xBEFE Signature
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef PERSISTHEAD_H // PERSISTHEAD_H...
#define PERSISTHEAD_H

#include "Primitives.h"
#include "Stream.h"

namespace BEFE { // Namespace BEFE...

class PersistHead { // Class PersistHead...

  // C++ Lifecycle
  public: PersistHead();
  public: PersistHead(PersistHead const &that);
  public: ~PersistHead();
  public: PersistHead &operator = (PersistHead const &that);
  
  // BEFE Lifecycle...
  public: Status       StartUp();
  public: Status       ShutDown();
  public: Status       Reset();
  public: Boolean      IsNull() const;
  public: Status       SetNull();
  public: Boolean      IsEmpty() const;
  public: Status       SetEmpty();
  public: Status       MoveFrom(PersistHead const &that);
  public: Status       CopyFrom(PersistHead const &that);
  public: PersistHead &Consumable() const;
  public: PersistHead &NotConsumable() const;
  public: Boolean      IsConsumable() const;
  public: Boolean      IsValid() const;
  public: UInt         Length() const;
  public: UInt         Size() const;
  public: UInt         PersistSize() const;
  public: Status       WriteTo(Stream &out);  
  public: Status       ReadFrom(Stream &in);

  // Operational Methods
  public: String ToString() const;
  public: UInt16 GetSignature() const;
  public: UInt16 GetMainVersion() const;
  public: UInt16 GetSubVersion() const;
  public: UInt32 GetRevision() const;
  public: String GetTypeName() const;
  public: UInt16 GetTypeRevision() const;  
  
  public: Status SetTypeName(String const &newName);
  public: Status SetTypeRevision(UInt16 newRev);
  
  //
  // Protected Members
  //
  
  protected: Byte   befeSignature[2];
  protected: Byte   befeVersionHi[2];
  protected: Byte   befeVersionLo[2];
  protected: Byte   befeRevision[4];
  protected: Byte   typeName[4];
  protected: Byte   typeRevision[2];
  
}; // ...Class PersistHead

//----------------------------------------------------------------------
//
// General PersistHead Functions
//

BEFE_INLINE Boolean IsNull(PersistHead const &that)   {return that.IsNull();}
BEFE_INLINE Status  SetNull(PersistHead &that)        {return that.SetNull();}
BEFE_INLINE Boolean IsValid(PersistHead &that)        {return that.IsValid();}
BEFE_INLINE String  ToString(PersistHead const &that) {return that.ToString().Consumable();}  
  

} // ...Namespace BEFE

#endif // ...PERSISTHEAD_H