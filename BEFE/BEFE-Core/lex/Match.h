//!befe-publish inc
//----------------------------------------------------------------------
// File: Match.h - Match Struct Declarations
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MATCH_H  // MATCH_H...
#define MATCH_H

#include "Primitives.h"
#include "MinExp.h"

namespace BEFE { // Namespace BEFE...

// Match specific return codes...
namespace Error { // Namespace Error...

  enum : Int { // enum...
  
    MatchDoesNotMatch = (Int)0xFFFFFFFF
    
  };
  
} // ...Namespace Error

//----------------------------------------------------------------------
//
// Struct Match
//

struct Match {
  
  // Enum MatchType...
  enum MatchType : Byte {
    TypeUnknown,
    TypeStar,
    TypeMinExp,
    TypeSQL,
    TypePosixBRE,
    TypePosixERE,
    NumTypes
  };
  
  //
  // Members
  //
  
  Byte   type;
  String pattern;
  
  //
  // Methods
  //

  // C++ Lifecycle
	BEFE_inline Match()                             {StartUp();}
  BEFE_inline Match(Match const &that)            {StartUp(); CopyFrom(that);}
              Match(MatchType pType, String const &pPattern);
              Match(String const &pPattern);
	BEFE_inline ~Match()                            {ShutDown();}
  BEFE_inline Match &operator=(Match const &that) {CopyFrom(that); return *this;}
  
  // BEFE Lifecycle
  Status   StartUp();
  Status   ShutDown();
  Status   Reset();
  Status   MoveFrom(Match const &that);
  Status   CopyFrom(Match const &that);
  Boolean  IsNull() const;
  Status   SetNull();
  Boolean  IsEmpty() const;
  Status   SetEmpty();
  UInt     Length() const;
  UInt     Size() const;
	Boolean  IsConsumable() const;
  Match   &Consumable() const;
  Match   &NotConsumable() const;
  
  // Operational methods
  Boolean  Matches(String const &that) const;
  
};

} // ...Namespace BEFE

#endif // ...MATCH_H
