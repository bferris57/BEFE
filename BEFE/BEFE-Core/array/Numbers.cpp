//----------------------------------------------------------------------
// File: Numbers.cpp - Implementation of the Numbers class
//----------------------------------------------------------------------
//
// This file implements the String class, an Array of Number instances.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Numbers.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Finding
//

Boolean Numbers::Contains(Number const &num) {
  
  UInt    curIdx;
  UInt    numNumbers;
  Number *curNumber;

  _BufAndLength((Byte *&)curNumber, numNumbers);
  
  for (curIdx=0; curIdx < numNumbers; curIdx++)
    if (curNumber[curIdx] == num) break;
  
  return curIdx < numNumbers;
  
}

} // ...Namespace BEFE
