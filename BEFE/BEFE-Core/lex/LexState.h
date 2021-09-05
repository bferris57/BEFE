//----------------------------------------------------------------------
// File: LexState.h - Declarations for the BEFE LexState Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXSTATE_H // LEXSTATE_H...
#define LEXSTATE_H

#include "Primitives.h"
#include "Array.h"
#include "Map.h"
#include "Id.h"

#include "LexRange.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct LexState - A Lexical Analyser State Action Table Primitive
//

struct LexState {

  // Friends
  friend class LexLanguage;
  
  enum StateType : Byte {
    TypeUnknown,       // State Type not specified
    TypeByte,          // State Type Byte
    TypeChar,          // State Type Char
    NumTypes
  };

  //
  // Members
  //
  
  Id        id;               // Id
  String    name;             // Name
  StateType type;             // State Type (See LexStateType above)
  Byte      byteActions[128]; // Lex.ActionNo per Byte ◄── (if type==TypeByte)
  LexRanges ranges;           // List of LexRanges     ◄── (if type==TypeChar)
  
  // Indeterminate members, useful for callers who know the LexState has not
  // changed during processing.
  //
  // Note: These are VOLATILE, and DEFINITELY NOT THREAD SAFE.  ;-)
  UInt      numRanges;        // ◄── Number of ranges (eg numRanges.Length())
  LexRange *range0;           // ◄── Pointer to first LexRange in ranges (if any)

  //
  // Methods
  //
  
	// C++ Lifecycle
	LexState();
  LexState(LexState const &that);
	~LexState();
  LexState &operator=(LexState const &that);
	
	// BEFE Lifecycle
  Status    StartUp();
  Status    ShutDown();
  Status    Clear();
  Status    Reset();
  Boolean   IsNull() const;
  Status    SetNull();
  Boolean   IsEmpty() const;
  Status    SetEmpty();
  Boolean   IsConsumable() const;
  LexState &Consumable();
  LexState &NotConsumable();
  Status    MoveFrom(LexState const &that);
  Status    CopyFrom(LexState const &that);
  
  // Member Access
  String    GetName() const;
  Status    SetName(String const &newName);
  UInt      GetType() const;
  Status    SetType(Byte pType);
  
  // String Methods
  Status ToStrings(Strings &strings);

  // Range Management
  Status PopulateRangesFromBytes();
  Byte   GetCharActionNo(Char c); // ◄── Only use this if you can't be bothered
                                  //     to denormalise its algorithm because
                                  //     it'll be a bit faster if you did it
                                  //     yourself using numRanges and range0 above.
  
};

//----------------------------------------------------------------------
//
// LexState Typedefs
//

typedef ArrayValue<LexState>    LexStates;
typedef StringMapPrimitive<Id>  LexStateName2Id;
typedef UIntMapValue<String>    LexStateId2Name;

//----------------------------------------------------------------------
//
// LexState BEFE Lifecycle...
//
// NOTE: The following declarations that aren't BEFE_INLINE, are NOT
//       implemented yet.  There are listed for consistency.  If and
//       when needed, this header will updated with appropriate inlines.
//

            Status    StartUp       (LexState &rThis)                     ;
            Status    ShutDown      (LexState &rThis)                     ;
            Status    Reset         (LexState &rThis)                     ;
            Status    Clear         (LexState &rThis)                     ;
BEFE_INLINE Boolean   IsNull        (LexState const &rThis)               {return rThis.IsNull();}
BEFE_INLINE Status    SetNull       (LexState &rThis)                     {return rThis.SetNull();}
            Boolean   IsEmpty       (LexState const &rThis)               ;
            Status    SetEmpty      (LexState &rThis)                     ;
            Boolean   IsConsumable  (LexState const &rThis)               ;
            LexState &Consumable    (LexState &rThis)                     ;
            LexState &NotConsumable (LexState &rThis)                     ;
            Boolean   IsMutable     (LexState const &rThis)               ;
            LexState &Mutable       (LexState &rThis)                     ;
            LexState &Immutable     (LexState &rThis)                     ;
BEFE_INLINE UInt      Length        (LexState const &rThis)               {return sizeof(rThis);}
BEFE_INLINE UInt      Size          (LexState const &rThis)               {return sizeof(rThis);}

} // ...Namespace BEFE

#endif // ...LEXSTATE_H
