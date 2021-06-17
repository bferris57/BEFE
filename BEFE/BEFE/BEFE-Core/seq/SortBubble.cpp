//----------------------------------------------------------------------
// File: SortBubble.h - Bubble Sort implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Sort.h"

namespace BEFE { // Namespace BEFE...
  
//----------------------------------------------------------------------
//
// In-place Selection Sort
//
//----------------------------------------------------------------------

Status SortBubble(SortTarget const &target) {

  Status  status;
  UInt    len;
  UInt    i;
  UInt    j;
  Int     cmp;
  Boolean swapped;
  
  // Get Length
  len = ((SortTarget *)&target)->Length();
  if (len <= 1) goto FINISHED;
  
  //
  // Do the sort...
  //
  
  for (i=0; i < len; i++) {
    swapped = false;
    for (j=len-1; j > i; j--) {
      cmp = ((SortTarget *)&target)->Compare(j, j-1);
      if (cmp < 0) {
        status = ((SortTarget *)&target)->Swap(j,j-1);
        if (status) goto SOMEERROR;
      }
    }
    if (!swapped) break;  
  }
  
  status = Error::None;
  while (false) {
    FINISHED:  status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Out-of-place Selection Sort
//
//----------------------------------------------------------------------

Status SortBubble(SortTarget const &target, UInts &output) {

  Status  status;
  UInt    len;
  UInt    i;
  UInt    j;
  Int     cmp;
  Boolean swapped;
  UInt   *uints;
  
  // Get Length
  len = ((SortTarget *)&target)->Length();
  if (len <= 1) goto FINISHED;
  
  // Initialise output
  status = output.ShutDown();
  if (status) goto SOMEERROR;
  status = output.StartUp();
  if (status) goto SOMEERROR;
  status = output._BufAlloc(Alloc(len));
  if (status) goto SOMEERROR;
  uints = (UInt *)output._GetBuf();
  if (IsNull(uints)) goto NULLPOINTER;
  for (i=0, j=0; i < len; i++) {
    if (((SortTarget *)&target)->Selected(i))
      uints[j++] = i;
  }
  len = j;
  status = output.SetLength(len);
  if (status) goto SOMEERROR;
  
  //
  // Do the sort...
  //
  
  for (i=0; i < len; i++) {
    swapped = false;
    for (j=len-1; j > i; j--) {
      cmp = ((SortTarget *)&target)->Compare(uints[j], uints[j-1]);
      if (cmp < 0) {
        uints[j-1] ^= uints[j];
        uints[j]   ^= uints[j-1];
        uints[j-1] ^= uints[j];
        swapped = true;
      }
    }
    if (!swapped) break;  
  }
  
  status = Error::None;
  while (false) {
    FINISHED:    status = Error::None; break;
    SOMEERROR:                         break;
    NULLPOINTER: BEFE_WARN("SortBubble: NULL Buffer");
                 status = Error::InternalNullPointer;
                 break;
  }
  
  return status;
  
}

} // ...Namespace BEFE