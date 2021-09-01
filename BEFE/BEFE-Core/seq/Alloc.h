//!befe-publish inc
//----------------------------------------------------------------------
// File: Alloc.h - Declarations for the Alloc Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for <Positional Sequences>.
// As such, it appears throughout the product when manipulating
// Positional Sequences.
//
// The purpose of an Alloc class is to declare, for <Values>, how
// the memory/storage allocation is to be managed.
//
// This involves the declaration of dynamic <Size>/<Length>, if it
// is dynamic, and how changes in <Size>/<Length> are to be managed by
// the underlying implementation of the <Storage>
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ALLOC_H // ALLOC_H...
#define ALLOC_H

#include "Primitives.h"
#include "URange.h"

namespace BEFE { // Namespace BEFE...

// Forward class declarations
class String;

//----------------------------------------------------------------------
//
// Class: Alloc - Sequence allocation declaration
//

class Alloc {

  //
  // Public members
  //
  // Note: These are "private" because they're not really "safe" to
  //       query on their own, eg automatic casts to Int or UInt won't
  //       result in proper NaN/UNaN, et cetera, values.
  //
  //       So, use the query functions below instead.
  //
  private: UInt    minElms;
  private: UInt    maxElms;
  private: UInt    incElms:15;        // ◄── Private because it's not "safe" to autocast to a UInt
  private: Boolean incElmsPct:1;
  private: UInt    decElms:15;        // ◄── Private because it's not "safe" to autocast to a UInt
  private: Boolean decElmsPct:1;

  // C++ Lifecycle
  public: Alloc();
  public: Alloc(const char *str);
  public: Alloc(String const &str);
  public: Alloc(UInt fixedElms);
  public: Alloc(UInt initElms, UInt incElms);
  public: ~Alloc();

  // BEFE Lifecycle
  public: Status  StartUp();
  public: Status  ShutDown() {return Error::None;};
  public: Status  Reset()    {return StartUp();};
  public: Boolean IsNull() const;
  public: Status  SetNull();
  public: Boolean IsEmpty() const;
  public: Status  SetEmpty();
  public: Status  CopyFrom(Alloc const &that);
  public: Status  MoveFrom(Alloc const &that);

  // Query
  public: UInt    MinElms();
  public: UInt    MaxElms();
  public: UInt    IncElms();
  public: Boolean IncElmsPct();
  public: UInt    DecElms();
  public: Boolean DecElmsPct();
  
  //
  // Normalising
  //

  public: Alloc Normalise() const;
  public: Alloc Normalise(UInt elmSize) const;
  public: Alloc Normalise(UInt elmSize, UInt initElms) const;
  
  //
  // Alloc Setting
  //

  public: Status Set();
  public: Status Set(String &str);
  public: Status SetOpen();
  public: Status SetClosed();
  
  //
  // Alloc Query
  //

  public: Boolean IsFixed();                    // "Does this declare a fixed size sequence?"
  public: Boolean IsLimited();                  // "Does this declare a limited sized sequence?"
  public: UInt    GetMin();                     // Get minimum number of elements to initially allocate
  public: UInt    GetMax();                     // Get maximum number of elements to initially allocate
  public: UInt    GetInc();                     // Get number of elements when incrementing
  public: UInt    GetDec();                     // Get number of elements to decrease by
  public: UInt    GetMinElements(UInt len);     // Get initial elements for given size
  public: UInt    GetMaxElements(UInt len);     // Get maximum elements for a given size
  public: UInt    GetIncElements (UInt len);    // Get incremental elements for given size
  public: UInt    GetDecElements (UInt len);    // Get decremental elements for given size

  //
  // String methods
  //

  public: String  ToString() const;             // Alloc -> String
  public: Status  Parse(String const &str);     // String -> Alloc

};

} // ...Namespace BEFE

#endif // ...ALLOC_H