//----------------------------------------------------------------------
// File: SortInsertion.h - Insertion Sort implementation
//----------------------------------------------------------------------
//
// This source contains the functions that implement the BEFE version
// of an "Insertion Sort".
//
// An Insertion Sort is one of the "worst case" sorts with regards to
// number of computations (N**N) required.
//
// We've only included it here for demonstration purposes, completeness
// sake, and because in certain limited cases, it's "good enough to get
// the job done".
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
// In-place Insertion Sort
//

Status SortInsertion(SortTarget const &target) {

  Status  status;
  UInt    len;
  UInt    i;
  UInt    k;

  // Get Length
  len = ((SortTarget *)&target)->Length();
  if (len <= 1) goto FINISHED;
  
  // Do the sort
  for (i=1; i < len; i++) {
    for (k=i; k > 0 && ((SortTarget *)&target)->Compare(k, k-1) < 0; k--) {
      status = ((SortTarget *)&target)->Swap(k,k-1);
      if (status) goto SOMEERROR;
    }
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
// Out-of-place Insertion Sort
//

Status SortInsertion(SortTarget const &target, UInts &output) {

  Status  status;
  UInt    len;
  UInt    i;
  UInt    j;
  UInt    k;
  UInt   *uints;

  // Get Length
  len = ((SortTarget *)&target)->Length();
  if (len <= 1) goto FINISHED;
  
  // Initialise output
  status = output.ShutDown();
  if (status) goto SOMEERROR;
  status = output.StartUp();
  if (status) goto SOMEERROR;
  status = output.SetLength(len);
  if (status) goto SOMEERROR;
  uints = (UInt *)output._GetBuf();
  if (IsNull(uints)) goto NULLPOINTER;
  for (i=0, j=0; i < len; i++) {
    if (target.Selected(i))
      uints[j++] = i;
  }
  len = j;
  status = output.SetLength(len);
  if (status) goto SOMEERROR;
  
  // Do the sort
  for (i=1; i < len; i++) {
    for (k=i; k > 0 && ((SortTarget *)&target)->Compare(uints[k], uints[k-1]) < 0; k--) {
      uints[k]   ^= uints[k-1];
      uints[k-1] ^= uints[k];
      uints[k]   ^= uints[k-1];
    }
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED:    status = Error::None; break;
    SOMEERROR:                         break;
    NULLPOINTER: BEFE_WARN("SortInsertion: NULL Buffer");
                 status = Error::InternalNullPointer;
                 break;
  }
  
  return status;
  
}

} // ...Namespace BEFE