//----------------------------------------------------------------------
// File: LexRange.h - Declarations for the BEFE LexRange Class
//----------------------------------------------------------------------
//
// A LexRange describes a Char range and a LexAction ID to invoke when
// a Char in that range is encountered.
//
// Each LexState of type LexState.TypeChar has a list of zero or more
// of these "transitions" which, given a Char X, the list is searched
// from the beginning (element 0) to the end until a match is found.
//
// If a match is found, that LexAction.handler is invoked.  Otherwise
// the character is treated as "unexpected".
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LEXRANGE_H // LEXRANGE_H...
#define LEXRANGE_H

#include "Primitives.h"
#include "LexAction.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Struct LexRange - A Lexical Analyser Char Range Transition Declaration
//

struct LexRange {

  // Members
  URange range;
  Byte   actionNo;

	// C++ Lifecycle
	LexRange();
  LexRange(LexRange const &that);
	~LexRange();
  LexRange &operator=(LexRange const &that);
	
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
  LexRange &Consumable();
  LexRange &NotConsumable();
  Status    MoveFrom(LexRange const &that);
  Status    CopyFrom(LexRange const &that);
  
};

//----------------------------------------------------------------------
//
// LexRange Typedefs
//

typedef ArrayValue<LexRange> LexRanges;

} // ...Namespace BEFE

#endif // ...LEXRANGE_H