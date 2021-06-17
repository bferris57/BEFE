//!befe-publish inc
//----------------------------------------------------------------------
// File: Ids.h - Declarations for the Ids Class
//----------------------------------------------------------------------
//
// The Ids Class is sort of like an array of Ids and sort of like a
// Sparse of Ids but it's sort of different...
//
// Actually, you really could classify it as an "Ordered Collection of Ids".
// Unfortunately, we haven't modelled that yet in BEFE and need this
// Class in place in order to do the Lexical processing properly to
// get it into the BEFE Model.
//
// So, we have to "hard code" this Class for now until we get further
// along in the BEFE development.
//
// In an Ids instance, the Id itself serves as the "Index" and the "Value".
// And, since Ids may be removed from the list, it's Sparse.
//
// Enough of that...
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef IDS_H // IDS_H...
#  define IDS_H

#include "Sparse.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

class Ids {
  
  //
  // Public Instance methods
  //

  // C++ Lifecycle methods
  public: Ids();                // Default constructor
  public: Ids(Ids const &that); // Copy constructor
  public: ~Ids();               // Deconstructor
  public: Ids &operator=(Ids const &that);
  
  // BEFE Lifecycle methods
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Boolean  IsNull()  const;
  public: Status   SetNull();  
  public: Boolean  IsEmpty() const;
  public: Status   SetEmpty();
  public: Status   MoveFrom(Ids const &that);
  public: Status   CopyFrom(Ids const &that);
  //public: Status  &NewIterator(UInt iterType, Iterator *&newIter); // ◄── Not implemented yet

  public: Ids     &Consumable() const;
  public: Ids     &NotConsumable() const;
  public: Boolean  IsConsumable() const;
  public: Status   SetLength(UInt newElms) const;
  public: Boolean  Contains(Id const &id) const;
  public: Status   Contains(Id const &id, Boolean &answer) const;
  
  // Contents Info Methods
  public: UInt     Length() const;
  public: UInt     Size() const;
  
  // Contents Access Methods
  public: Id       Get(Int index);
  public: Status   Get(Int index, Id &that);
  public: Status   Insert(Id);
  public: Status   Remove(Id that);
  
  //
  // Protected Methods
  //
  
  protected: Status Find(Id that, UInt &idx) const;
  
  //
  // Public Members
  //
  
  public: ArrayPrimitive<Id> _ids;
  
};

//----------------------------------------------------------------------
//
// BEFE Lifecycle Functions
//


} // ...Namespace BEFE

#endif // ...IDS_H