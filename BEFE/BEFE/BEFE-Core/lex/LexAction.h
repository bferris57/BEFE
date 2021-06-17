//----------------------------------------------------------------------
// File: LexAction.h - Declarations for the BEFE LexAction Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXACTION_H // LEXACTION_H...
#define LEXACTION_H

#include "Primitives.h"
#include "Array.h"
#include "Map.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

// Forward prototypes...
class LexStream;
class LexLanguage;

// Typedef LexCharAction...
typedef Status (*LexCharAction)(LexStream &stream, Char theChar);

//----------------------------------------------------------------------
//
// Struct LexAction - A Lexical Analyser State Action Table Primitive
//

struct LexAction {
  
  // Members
  Id             id;            // Id
  String         name;          // Name
  LexCharAction  charHandler;   // Handler for input Char values
  
	// C++ Lifecycle
	public: LexAction();
  public: LexAction(LexAction const &that);
	public: ~LexAction();
  public: LexAction &operator=(LexAction const &that);
	
	// BEFE Lifecycle
  public: Status    StartUp();
  public: Status    ShutDown();
  public: Status    Clear();
  public: Status    Reset();
  public: Boolean   IsNull() const;
  public: Status    SetNull();
  public: Boolean   IsEmpty() const;
  public: Status    SetEmpty();
  public: Boolean   IsConsumable() const;
  public: LexAction &Consumable();
  public: LexAction &NotConsumable();
  public: Status    MoveFrom(LexAction const &that);
  public: Status    CopyFrom(LexAction const &that);

  // ToString methods
  public: String    ToString() const;
  
};

//----------------------------------------------------------------------
//
// LexAction Typedefs
//

typedef ArrayValue<LexAction>   LexActions;
typedef StringMapPrimitive<Id>  LexActionName2Id;
typedef UIntMapValue<String>    LexActionId2Name;

//----------------------------------------------------------------------
//
// LexAction BEFE Lifecycle...
//
// NOTE: The following declarations that aren't BEFE_INLINE, are NOT
//       implemented yet.  There are listed for consistency.  If and
//       when needed, this header will updated with appropriate inlines.
//

            Status     StartUp       (LexAction &rThis)       ;
            Status     ShutDown      (LexAction &rThis)       ;
            Status     Reset         (LexAction &rThis)       ;
            Status     Clear         (LexAction &rThis)       ;
BEFE_INLINE Boolean    IsNull        (LexAction const &rThis) {return rThis.IsNull();}
BEFE_INLINE Status     SetNull       (LexAction &rThis)       {return rThis.SetNull();}
            Boolean    IsEmpty       (LexAction const &rThis) ;
            Status     SetEmpty      (LexAction &rThis)       ;
            Boolean    IsConsumable  (LexAction const &rThis) ;
            LexAction &Consumable    (LexAction &rThis)       ;
            LexAction &NotConsumable (LexAction &rThis)       ;
            Boolean    IsMutable     (LexAction const &rThis) ;
            LexAction &Mutable       (LexAction &rThis)       ;
            LexAction &Immutable     (LexAction &rThis)       ;
BEFE_INLINE UInt       Length        (LexAction const &rThis) {return sizeof(rThis);}
BEFE_INLINE UInt       Size          (LexAction const &rThis) {return sizeof(rThis);}

} // ...Namespace BEFE

#endif // ...LEXACTION_H
