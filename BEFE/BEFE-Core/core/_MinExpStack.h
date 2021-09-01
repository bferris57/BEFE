//!befe-publish inc
//----------------------------------------------------------------------
// File: _MinExpStack.h - Declarations for the MinExpStack (Internal)
//----------------------------------------------------------------------
//
// The MinExpStack class is used to perform analysis for MinExp.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef _MINEXPSTACK_H // _MINEXPSTACK_H...
#define _MINEXPSTACK_H

#include "Primitives.h"
#include "Array.h"

namespace BEFE { // Namespace BEFE...

//
// MinExpEntry - These are the stack entries
//

struct _MinExpStackEntry {

  Strings list;   // Current list of possibilities
  String  value;  // Current value being built (Char by Char)
  Boolean isOr;   // True = "Is |"
  Boolean isOpt;  // True = "Is optional ('[...]')"
  Boolean isOpen; // True = "Group is still open (hasn't finished yet)"

  // BEFE Lifecycle methods
  Status             StartUp();                               // ◄── Initialise
  Status             ShutDown();                              // ◄── Destroy
  Status             MoveFrom(_MinExpStackEntry const &that); // ◄── Move from another, possibly consumable, String
  Status             CopyFrom(_MinExpStackEntry const &that); // ◄── Copy from another, possibly consumable, String
  _MinExpStackEntry &Consumable() const;                      // ◄── Called mainly before returning local _MinExpStackEntry
  _MinExpStackEntry &NotConsumable() const;                   // ◄── Called mainly from MoveFrom
  Boolean            IsNull() const;                          // ◄── Is this entry null?

  // C++ LifeCycle methods
                   _MinExpStackEntry()                              {StartUp();}
                   _MinExpStackEntry(_MinExpStackEntry const &that) {StartUp(); CopyFrom(that);}
                   _MinExpStackEntry(_MinExpStackEntry &&that)      {StartUp(); MoveFrom(that);}
                    ~_MinExpStackEntry()                            {ShutDown();}
  _MinExpStackEntry &operator =(_MinExpStackEntry const &that)      {ShutDown(); StartUp(); CopyFrom(that); return *this;}

};

//
// MinExpStack - Stack (array) of MinExpStackEntry
//

class _MinExpStack : public ArrayValue<_MinExpStackEntry> {

  friend class MinExp;
  
  // Constructor...
  //public: _MinExpStack() {lastChar=0;}
  // Protected members...
  protected: Char lastChar = Char((UInt)0);
  
	// Method additions to ArrayValue...
  public: _MinExpStackEntry *TOS();
  public: _MinExpStackEntry  Pop();
  public: _MinExpStackEntry *Push(_MinExpStackEntry &that);
  public: Status             ProcessChars(String &input, Strings &output);

  // Internal helper methods...
  protected: Status ProcessChar(Char c);
  protected: Status ProcessBegin(Boolean isOptional);
  protected: Status ProcessEnd();
  protected: Status ProcessOr();
  protected: Status ProcessEOS(Strings &strList);
  protected: Status FlushValue();
  protected: Status Combine();

  protected: Status Normalise(String &str);                         // ◄── Normalise a MinExp declaration String
  protected: Status AppendStringToList(Strings &list, String &str); // ◄── Append String to String Array
  protected: Status JoinLists();                                    // ◄── Join Stack[-1] (TOS) to Stack[-2] and remove TOS

  // Protected Members
  protected: Char    prevChar;

};

} // ...Namespace BEFE

//
// MinExpStack - MinExp Processing Stack
//
// This is used to create and manage the processing of a MinExp, producing
// the String list 
#endif // ..._MINEXPSTACK_H
