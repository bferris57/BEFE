//!befe-publish inc
//----------------------------------------------------------------------
// File: LifeCycle.h - Declarations for the various LifeCycle Classes
//----------------------------------------------------------------------
//
// BEFE Declare various Lifecycles including C++ and BEFE.
//
// *** See technical documentation for a more detailed description ***
//
// Note: The C++ and BEFE Lifecycle Classes are essentially the 
//       equivalent of a C++ type of vtable, except the parameter types
//       are generic instead of "proper" C++ style syntax.
//
//       These wikll be used in various places throughout BEFE to
//       properly invoke the correct functions based on runtime types.
//
// Note: We got bothered by GNU's C++ a bit so we haven't properly
//       implemented/tested the C++ and BEFE LifeCycle Classes so,
//       as of time of writing, you can safely ignore that they exist.
//
// Note: The Lifecycle declarations may seem a bit massive and a bit
//       "overkill" but we've provided them to illustrate that the
//       BEFE Lifecycle is applicable to Primitives as well as Values.
//
//       Note, as well, that the only Lifecycle declarations that are
//       currently implemented are the ones preceeded by a BEFE_INLINE
//       macro use.  This is to make them efficient by placing their
//       "dispatching" behaviour inline.
//
//       This also clearly also indicates which ones we haven't needed
//       needed yet because they'll produce undefined references at
//       link time.  So, at that time, it'll should be fairly easy to
//       shove them in here if they REALLY ARE needed.
//
//       At least that's the theory...
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LIFECYCLE_H // LIFECYCLE_H...
#define LIFECYCLE_H

#include "Primitives.h"
#include "Error.h"
#include "Null.h"
#include "Time.h"
#include "MemoryUtils.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Primitive Lifecycle Functions
//

// Boolean Lifecycle...
//
// Note: This is the "odd one out" because it doesn't yet have a real
//       concept of "Sign" or "Null".  On the other hand it hasn't been
//       used yet either, as you can see by the missing BEFE_INLINEs
//       below.
            Status   StartUp       (Boolean &rThis)                       ;
            Status   ShutDown      (Boolean &rThis)                       ;
            Status   Reset         (Boolean &rThis)                       ;
            //Boolean  IsNull        (Boolean const &rThis)                 ;
            //Status   SetNull       (Boolean &rThis)                       ;
            Boolean  IsEmpty       (Boolean const &rThis)                 ;
            Status   SetEmpty      (Boolean &rThis)                       ;
            Status   MoveFrom      (Boolean &rThis, Boolean const &rThat) ;
            Status   CopyFrom      (Boolean &rThis, Boolean const &rThat) ;
            Boolean  IsConsumable  (Boolean const &rThis)                 ;
            Boolean &Consumable    (Boolean &rThis)                       ;
            Boolean &NotConsumable (Boolean &rThis)                       ;
            Boolean  IsMutable     (Boolean const &rThis)                 ;
            Boolean &Mutable       (Boolean &rThis)                       ;
            Boolean &Immutable     (Boolean &rThis)                       ;
            UInt32   Hash          (Boolean &rThis)                       ;
            Boolean  IsEqual       (Boolean &rThis, Boolean &rThat)       ;
            Int      Compare       (Boolean &rThis, Boolean &rThat)       ;
            UInt     Length        (Boolean const &rThis)                 ;
            UInt     Size          (Boolean const &rThis)                 ;

// UInt8 Lifecycle...
            Status   StartUp       (UInt8 &rThis)                         ;
            Status   ShutDown      (UInt8 &rThis)                         ;
            Status   Reset         (UInt8 &rThis)                         ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (UInt8 const &rThis)                   {return rThis == UInt8NaN;}
//BEFE_INLINE Status   SetNull       (UInt8 &rThis)                         {rThis = UInt8NaN; return Error::None;}
            Boolean  IsEmpty       (UInt8 const &rThis)                   ;
            Status   SetEmpty      (UInt8 &rThis)                         ;
            Status   MoveFrom      (UInt8 &rThis, UInt8 const &rThat)     ;
            Status   CopyFrom      (UInt8 &rThis, UInt8 const &rThat)     ;
            Boolean  IsConsumable  (UInt8 const &rThis)                   ;
            UInt8   &Consumable    (UInt8 &rThis)                         ;
            UInt8   &NotConsumable (UInt8 &rThis)                         ;
            Boolean  IsMutable     (UInt8 const &rThis)                   ;
            UInt8   &Mutable       (UInt8 &rThis)                         ;
            UInt8   &Immutable     (UInt8 &rThis)                         ;
            UInt32   Hash          (UInt8 &rThis)                         ;
            Boolean  IsEqual       (UInt8 &rThis, UInt8 &rThat)           ;
            Int      Compare       (UInt8 &rThis, UInt8 &rThat)           ;
            UInt     Length        (UInt8 const &rThis)                   ;
            UInt     Size          (UInt8 const &rThis)                   ;
// SInt8 Lifecycle
            Status   StartUp       (SInt8 &rThis)                         ;
            Status   ShutDown      (SInt8 &rThis)                         ;
            Status   Reset         (SInt8 &rThis)                         ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (SInt8 const &rThis)                   {return rThis == SInt8NaN;}
//BEFE_INLINE Status   SetNull       (SInt8 &rThis)                         {rThis = SInt8NaN; return Error::None;}
            Boolean  IsEmpty       (SInt8 const &rThis)                   ;
            Status   SetEmpty      (SInt8 &rThis)                         ;
            Status   MoveFrom      (SInt8 &rThis, SInt8 const &rThat)     ;
            Status   CopyFrom      (SInt8 &rThis, SInt8 const &rThat)     ;
            Boolean  IsConsumable  (SInt8 const &rThis)                   ;
            Byte*    Consumable    (SInt8 &rThis)                         ;
            Byte*    NotConsumable (SInt8 &rThis)                         ;
            Boolean  IsMutable     (SInt8 const &rThis)                   ;
            SInt8   &Mutable       (SInt8 &rThis)                         ;
            SInt8   &Immutable     (SInt8 &rThis)                         ;
            UInt32   Hash          (SInt8 &rThis)                         ;
            Boolean  IsEqual       (SInt8 &rThis, SInt8 &rThat)           ;
            Int      Compare       (SInt8 &rThis, SInt8 &rThat)           ;
            UInt     Length        (SInt8 const &rThis)                   ;
            UInt     Size          (SInt8 const &rThis)                   ;

// UInt16 Lifecycle...
            Status   StartUp       (UInt16 &rThis)                        ;
            Status   ShutDown      (UInt16 &rThis)                        ;
            Status   Reset         (UInt16 &rThis)                        ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (UInt16 const &rThis)                  {return rThis == UInt16NaN;}
//BEFE_INLINE Status   SetNull       (UInt16 &rThis)                        {rThis = UInt16NaN; return Error::None;}
            Boolean  IsEmpty       (UInt16 const &rThis)                  ;
            Status   SetEmpty      (UInt16 &rThis)                        ;
            Status   MoveFrom      (UInt16 &rThis, UInt16 const &rThat)   ;
            Status   CopyFrom      (UInt16 &rThis, UInt16 const &rThat)   ;
            Boolean  IsConsumable  (UInt16 const &rThis)                  ;
            UInt16  &Consumable    (UInt16 &rThis)                        ;
            UInt16  &NotConsumable (UInt16 &rThis)                        ;
            Boolean  IsMutable     (UInt16 const &rThis)                  ;
            Byte*    Mutable       (UInt16 &rThis)                        ;
            Byte*    Immutable     (UInt16 &rThis)                        ;
            UInt32   Hash          (UInt16 &rThis)                        ;
            Boolean  IsEqual       (UInt16 &rThis, UInt16 &rThat)         ;
            Int      Compare       (UInt16 &rThis, UInt16 &rThat)         ;
            UInt     Length        (UInt16 const &rThis)                  ;
            UInt     Size          (UInt16 const &rThis)                  ;

// SInt16 Lifecycle
            Status   StartUp       (SInt16 &rThis)                        ;
            Status   ShutDown      (SInt16 &rThis)                        ;
            Status   Reset         (SInt16 &rThis)                        ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (SInt32 const &rThis)                  {return rThis == SInt32NaN;}
//BEFE_INLINE Status   SetNull       (SInt32 &rThis)                        {rThis = SInt32NaN; return Error::None;}
            Boolean  IsEmpty       (SInt16 const &rThis)                  ;
            Status   SetEmpty      (SInt16 &rThis)                        ;
            Status   MoveFrom      (SInt16 &rThis, SInt16 const &rThat)   ;
            Status   CopyFrom      (SInt16 &rThis, SInt16 const &rThat)   ;
            Boolean  IsConsumable  (SInt16 const &rThis)                  ;
            Byte*    Consumable    (SInt16 &rThis)                        ;
            Byte*    NotConsumable (SInt16 &rThis)                        ;
            Boolean  IsMutable     (SInt16 const &rThis)                  ;
            SInt16  &Mutable       (SInt16 &rThis)                        ;
            SInt16  &Immutable     (SInt16 &rThis)                        ;
            UInt32   Hash          (SInt16 &rThis)                        ;
            Boolean  IsEqual       (SInt16 &rThis, SInt16 &rThat)         ;
            Int      Compare       (SInt16 &rThis, SInt16 &rThat)         ;
            UInt     Length        (SInt16 const &rThis)                  ;
            UInt     Size          (SInt16 const &rThis)                  ;

// UInt32 Lifecycle...
            Status   StartUp       (UInt32 &rThis)                        ;
            Status   ShutDown      (UInt32 &rThis)                        ;
            Status   Reset         (UInt32 &rThis)                        ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (UInt32 const &rThis)                  {return rThis == UInt32NaN;}
//BEFE_INLINE Status   SetNull       (UInt32 &rThis)                        {rThis = UInt32NaN; return Error::None;}
            Boolean  IsEmpty       (UInt32 const &rThis)                  ;
            Status   SetEmpty      (UInt32 &rThis)                        ;
            Status   MoveFrom      (UInt32 &rThis, UInt32 const &rThat)   ;
            Status   CopyFrom      (UInt32 &rThis, UInt32 const &rThat)   ;
            Boolean  IsConsumable  (UInt32 const &rThis)                  ;
            UInt32  &Consumable    (UInt32 &rThis)                        ;
            UInt32  &NotConsumable (UInt32 &rThis)                        ;
            Boolean  IsMutable     (UInt32 const &rThis)                  ;
            Byte*    Mutable       (UInt32 &rThis)                        ;
            Byte*    Immutable     (UInt32 &rThis)                        ;
            UInt32   Hash          (UInt32 &rThis)                        ;
            Boolean  IsEqual       (UInt32 &rThis, UInt32 &rThat)         ;
            Int      Compare       (UInt32 &rThis, UInt32 &rThat)         ;
            UInt     Length        (UInt32 const &rThis)                  ;
            UInt     Size          (UInt32 const &rThis)                  ;

// SInt32 Lifecycle
            Status   StartUp       (SInt32 &rThis)                        ;
            Status   ShutDown      (SInt32 &rThis)                        ;
            Status   Reset         (SInt32 &rThis)                        ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (SInt32 const &rThis)                  {return rThis == SInt32NaN;}
//BEFE_INLINE Status   SetNull       (SInt32 &rThis)                        {rThis = SInt32NaN; return Error::None;}
            Boolean  IsEmpty       (SInt32 const &rThis)                  ;
            Status   SetEmpty      (SInt32 &rThis)                        ;
            Status   MoveFrom      (SInt32 &rThis, SInt32 const &rThat)   ;
            Status   CopyFrom      (SInt32 &rThis, SInt32 const &rThat)   ;
            Boolean  IsConsumable  (SInt32 const &rThis)                  ;
            SInt32  &Consumable    (SInt32 &rThis)                        ;
            UInt32  &NotConsumable (SInt32 &rThis)                        ;
            Boolean  IsMutable     (SInt32 const &rThis)                  ;
            Byte*    Mutable       (SInt32 &rThis)                        ;
            Byte*    Immutable     (SInt32 &rThis)                        ;
            UInt32   Hash          (SInt32 &rThis)                        ;
            Boolean  IsEqual       (SInt32 &rThis, SInt32 &rThat)         ;
            Int      Compare       (SInt32 &rThis, SInt32 &rThat)         ;
            UInt     Length        (SInt32 const &rThis)                  ;
            UInt     Size          (SInt32 const &rThis)                  ;

// UInt64 Lifecycle...
            Status   StartUp       (UInt64 &rThis)                        ;
            Status   ShutDown      (UInt64 &rThis)                        ;
            Status   Reset         (UInt64 &rThis)                        ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (UInt64 const &rThis)                  {return rThis == UInt64NaN;}
//BEFE_INLINE Status   SetNull       (UInt64 &rThis)                        {rThis = UInt64NaN; return Error::None;}
            Boolean  IsEmpty       (UInt64 const &rThis)                  ;
            Status   SetEmpty      (UInt64 &rThis)                        ;
            Status   MoveFrom      (UInt64 &rThis, UInt64 const &rThat)   ;
            Status   CopyFrom      (UInt64 &rThis, UInt64 const &rThat)   ;
            Boolean  IsConsumable  (UInt64 const &rThis)                  ;
            Byte*    Consumable    (UInt64 &rThis)                        ;
            Byte*    NotConsumable (UInt64 &rThis)                        ;
            Boolean  IsMutable     (UInt64 const &rThis)                  ;
            UInt64  &Mutable       (UInt64 &rThis)                        ;
            UInt64  &Immutable     (UInt64 &rThis)                        ;
            UInt32   Hash          (UInt64 &rThis)                        ;
            Boolean  IsEqual       (UInt64 &rThis, UInt64 &rThat)         ;
            Int      Compare       (UInt64 &rThis, UInt64 &rThat)         ;
            UInt     Length        (UInt64 const &rThis)                  ;
            UInt     Size          (UInt64 const &rThis)                  ;

// SInt64 Lifecycle
            Status   StartUp       (SInt64 &rThis)                        ;
            Status   ShutDown      (SInt64 &rThis)                        ;
            Status   Reset         (SInt64 &rThis)                        ;
// Note: These next two are already declared in Null.h...
//BEFE_INLINE Boolean  IsNull        (SInt64 const &rThis)                  {return rThis == (Long)SInt64NaN;}
//BEFE_INLINE Status   SetNull       (SInt64 &rThis)                        {rThis = SInt64NaN; return Error::None;}
            Boolean  IsEmpty       (SInt64 const &rThis)                  ;
            Status   SetEmpty      (SInt64 &rThis)                        ;
            Status   MoveFrom      (SInt64 &rThis, SInt64 const &rThat)   ;
            Status   CopyFrom      (SInt64 &rThis, SInt64 const &rThat)   ;
            Boolean  IsConsumable  (SInt64 const &rThis)                  ;
            SInt64  &Consumable    (SInt64 &rThis)                        ;
            SInt64  &NotConsumable (SInt64 &rThis)                        ;
            Boolean  IsMutable     (SInt64 const &rThis)                  ;
            Byte*    Mutable       (SInt64 &rThis)                        ;
            Byte*    Immutable     (SInt64 &rThis)                        ;
            UInt32   Hash          (SInt64 &rThis)                        ;
            Boolean  IsEqual       (SInt64 &rThis, SInt64 &rThat)         ;
            Int      Compare       (SInt64 &rThis, SInt64 &rThat)         ;
            UInt     Length        (SInt64 const &rThis)                  ;
            UInt     Size          (SInt64 const &rThis)                  ;

//----------------------------------------------------------------------
//
// C++ Lifecycle Class
//

struct LifeCycleCpp { // Struct LifeCycleCpp...
  
  UInt size;
  
  void   (*Constructor)        (Byte *pThis);
  void * (*CopyConstructor)    (Byte *pThis, Byte *that);
  void   (*Destructor)         (Byte *pThis);
  void * (*OperatorEquals)     (Byte *pThis, Byte *pThat);
  void * (*OperatorPlusEquals) (Byte *pThis, Byte *pThat);
  void * (*OperatorMultEquals) (Byte *pThis, Byte *pThat);
  void * (*OperatorDivEquals)  (Byte *pThis, Byte *pThat);
  
}; // ...Struct LifeCycleCpp

//----------------------------------------------------------------------
//
// BEFE Lifecycle Class
//

struct LifeCycleBEFE { // Struct LifeCycleBEFE...

  typedef Status  (*StartUp)       (void *pThis);
  typedef Status  (*ShutDown)      (void *pThis);
  typedef Status  (*Reset)         (void *pThis);
  typedef Boolean (*IsNull)        (void *pThis);
  typedef Status  (*SetNull)       (void *pThis);
  typedef Boolean (*IsEmpty)       (void *pThis);
  typedef Status  (*SetEmpty)      (void *pThis);
  typedef Status  (*MoveFrom)      (void *pThis, Byte *pThat);
  typedef Status  (*CopyFrom)      (void *pThis, Byte *pThat);
  typedef Boolean (*IsConsumable)  (void *pThis);
  typedef Byte*   (*Consumable)    (void *pThis);
  typedef Byte*   (*NotConsumable) (void *pThis);
  typedef Boolean (*IsMutable)     (void *pThis);
  typedef Byte*   (*Mutable)       (void *pThis);
  typedef Byte*   (*Immutable)     (void *pThis);
  // Validation/Hashing/Comparison
  typedef Boolean (*IsValid)       (void *pThis);
  typedef UInt    (*Hash)          (void *pThis);
  typedef Boolean (*IsEqual)       (void *pThis, void *pThat);
  typedef Int     (*Compare)       (void *pThis, void *pThat);
  // Logical Length/Size
  typedef UInt    (*Length)        (void *pThis); // ◄── Abstract/Logical Current Length
  typedef UInt    (*Size)          (void *pThis); // ◄── Physical size (in Bytes)
  // Physical Length/Size
  typedef ULong   (*PhyEmptySize)  (void *pThis); // ◄── In Bits (sizeof(T)*8)
  typedef ULong   (*PhyUsedSize)   (void *pThis); // ◄── Current Physical Bits used
  // Persistence
  typedef UInt    (*PersistSize)   (void *pThis); // ◄── Persistent Size (in Bytes)
  typedef Status  (*WriteTo)       (void *pThis);
  typedef Status  (*ReadFrom)      (void *pThis);
  
  UInt          sizeOf;
  char const   *className;
  
  StartUp       fpStartUp;
  ShutDown      fpShutDown;
  Reset         fpReset;
  IsNull        fpIsNull;
  SetNull       fpSetNull;
  IsEmpty       fpIsEmpty;
  SetEmpty      fpSetEmpty;
  MoveFrom      fpMoveFrom;
  CopyFrom      fpCopyFrom;
  Consumable    fpConsumable;
  IsConsumable  fpIsConsumable;
  NotConsumable fpNotConsumable;
  IsMutable     fpIsMutable;
  Mutable       fpMutable;
  Immutable     fpImmutable;
  // Validation/Hashing/Comparison
  IsValid       fpIsValid;
  Hash          fpHash;
  IsEqual       fpIsEqual;
  Compare       fpCompare;
  // Logical Length/Size
  Length        fpLength;
  Size          fpSize;
  // Physical Length/Size
  PhyEmptySize  fpPhyEmptySize;
  PhyUsedSize   fpPhyUsedSize;
  // Persistence
  PersistSize   fpPersistSize;
  WriteTo       fpWriteTo;
  ReadFrom      fpReadFrom;
  
  LifeCycleBEFE(char const *name, UInt size); /*{
    MemoryZeroRaw((Byte *)this, sizeof(LifeCycleBEFE));
    sizeOf = size;
    className = name;    
    }*/
  
  void SetSizeOf        (UInt size)        {sizeOf          = size;}
  void SetClassName     (char const *name) {className = name;}
  
  void SetStartUp       (void *fp)  {fpStartUp       = (StartUp)fp;}
  void SetShutDown      (void *fp)  {fpShutDown      = (ShutDown)fp;}
  void SetReset         (void *fp)  {fpReset         = (Reset)fp;}
  void SetIsNull        (void *fp)  {fpIsNull        = (IsNull)fp;}
  void SetSetNull       (void *fp)  {fpSetNull       = (SetNull)fp;}
  void SetIsEmpty       (void *fp)  {fpIsEmpty       = (IsEmpty)fp;}
  void SetSetEmpty      (void *fp)  {fpSetEmpty      = (SetEmpty)fp;}
  void SetMoveFrom      (void *fp)  {fpMoveFrom      = (MoveFrom)fp;}
  void SetCopyFrom      (void *fp)  {fpCopyFrom      = (CopyFrom)fp;}
  void SetIsConsumable  (void *fp)  {fpIsConsumable  = (IsConsumable)fp;}
  void SetConsumable    (void *fp)  {fpConsumable    = (Consumable)fp;}
  void SetNotConsumable (void *fp)  {fpNotConsumable = (NotConsumable)fp;}
  void SetIsMutable     (void *fp)  {fpIsMutable     = (IsMutable)fp;}
  void SetMutable       (void *fp)  {fpMutable       = (Mutable)fp;}
  void SetImmutable     (void *fp)  {fpImmutable     = (Immutable)fp;}
  void SetIsValid       (void *fp)  {fpIsValid       = (IsValid)fp;}
  void SetHash          (void *fp)  {fpHash          = (Hash)fp;}
  void SetIsEqual       (void *fp)  {fpIsEqual       = (IsEqual)fp;}
  void SetCompare       (void *fp)  {fpHash          = (Hash)fp;}
  void SetLength        (void *fp)  {fpLength        = (Length)fp;}
  void SetSize          (void *fp)  {fpSize          = (Size)fp;}
  void SetPhyEmptySize  (void *fp)  {fpPhyEmptySize  = (PhyEmptySize)fp;}
  void SetPhyUsedSize   (void *fp)  {fpPhyUsedSize   = (PhyUsedSize)fp;}
  void SetPersistSize   (void *fp)  {fpPersistSize   = (PersistSize)fp;}
  void SetWriteTo       (void *fp)  {fpWriteTo       = (WriteTo)fp;}
  void SetReadFrom      (void *fp)  {fpReadFrom      = (ReadFrom)fp;}

}; // ...Struct LifeCycleBEFE

} // ...Namespace BEFE

#endif // ...LIFECYCLE_H
