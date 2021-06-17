//!befe-publish inc
//----------------------------------------------------------------------
// File: Sequence.h - Base C/C++ Sequence Declarations
//----------------------------------------------------------------------
//
// This header file declare Sequence specific data types.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef SEQUENCE_H // SEQUENCE_H...
#define SEQUENCE_H

#include "Alloc.h"
#include "Slice.h"
#include "Range.h"
#include "URange.h"
#include "Span.h"
#include "USpan.h"
#include "Cycle.h"

namespace BEFE { // Namespace BEFE...

namespace Sequence { // Namespace Sequence...
  
namespace RangeType { enum {

   Whole
  ,Range
  ,URange
  ,Span
  ,USpan
  ,Slice
  ,Alloc
  };}

namespace IndexType { enum {
  
   None
  ,LengthWrap
  ,Ring
  ,Stack
  ,Queue
  
  ,NumTypes
  
  };}

} // ...Namespace Sequence

} // ...Namespace BEFE

#endif // SEQUENCE_H