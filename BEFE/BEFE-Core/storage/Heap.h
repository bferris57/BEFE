//!befe-publish inc
//----------------------------------------------------------------------
// File: Heap.h - Declarations for the Heap Storage Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for memory Storage.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef HEAP_H // HEAP_H...
#define HEAP_H

#include "Primitives.h"
#include "Storage.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

class Heap { // Class Heap...

  //
  // Protected Instance Members
  //

  protected: Heap *deferee;         // Heap to defer the actions to
  protected: Int   deferDepth;      // Deferal depth

  //
  // Instance Methods
  //

  // C++ Lifecycle
  public:          Heap();
  public: virtual ~Heap();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  public: virtual Status MoveFrom(Heap const &that);
  public: virtual Status CopyFrom(Heap const &that);


}; // ...Class Heap

} // ...Namespace BEFE

#endif // ...HEAP_H
