//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: Sort.h - Sort class and function delarations
//----------------------------------------------------------------------
//
// This header file declares the Sort related classes and functions.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef SORT_H // SORT_H...
#define SORT_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Sort Namespace/Enums
//
// These Namespace/Enums are used to encapsulate Sort related types and
// enumerations.
//

namespace SortType { // Namespace SortType...

  enum UInt32 {

     None
    ,Insertion
    ,Selection
    ,Bubble
    ,Shell
    ,Merge
    ,Heap
    ,Quick
    ,Quick3
    
    ,NumTypes     // Number of sorts

  };

} // ...Namespace SortType

//----------------------------------------------------------------------
//
// SortTarget Classes
//
// The SortTarghet Struct is used to perform the low level operations
// required for all Sorting functions.
//
// This allows us to separate the specific sorting algorithm from the
// nature of the underlying data that is being sorted.
//

struct SortTarget {
  virtual        ~SortTarget()                  {};
  virtual UInt    Length () const               {return 0;};
  // For Sorting
  virtual Int     Compare(UInt elm1, UInt elm2) {return 0;};
  virtual Status  Swap   (UInt elm1, UInt elm2) {return Error::NotCapable;};
  virtual Status  Move   (UInt elm1, UInt elm2) {return Error::NotCapable;};
  // For Out-of-place Selecting
  virtual Boolean Selected(UInt elm) const      {return true;};
};

//
// Sort Functions
//

// General purpose sorts
Status Sort(SortTarget const &target, UInt32 sortNum);
Status Sort(SortTarget const &target, UInt32 sortNum, UInts &output);

// In-place Output sorting
Status SortInsertion (SortTarget const &target);
Status SortSelection (SortTarget const &target);
Status SortBubble    (SortTarget const &target);
Status SortMerge     (SortTarget const &target);

// Out-of-Place Output sorting
Status SortInsertion (SortTarget const &target, UInts &output);
Status SortSelection (SortTarget const &target, UInts &output);
Status SortBubble    (SortTarget const &target, UInts &output);
Status SortMerge     (SortTarget const &target, UInts &output);

} // ...Namespace BEFE

#endif // ...SORT_H
