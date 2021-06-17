//----------------------------------------------------------------------
// File: SortSelection.h - Selection Sort implementation
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

Status SortSelection(SortTarget const &target) {

  Status  status;
  UInt    len;
  UInt    i;
  UInt    j;
  UInt    k;
  
  // Get Length
  len = ((SortTarget *)&target)->Length();
  if (len <= 1) goto FINISHED;
  
  //
  // Do the sort...
  //
  
  for (i=0; i < len; i++) {
    k = i;
    for (j=i+1; j < len; j++)
      if (((SortTarget *)&target)->Compare(j, k) < 0)
        k = j;
    if (i != k) {
      status = ((SortTarget *)&target)->Swap(i,k);
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
// Out-of-place Selection Sort
//
//----------------------------------------------------------------------

Status SortSelection(SortTarget const &target, UInts &output) {

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
  status = output._BufAlloc(Alloc(len));
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
  
  //
  // Do the sort...
  //
  
  for (i=0; i < len; i++) {
    k = i;
    for (j=i+1; j < len; j++)
      if (((SortTarget *)&target)->Compare(uints[j], uints[k]) < 0)
        k = j;
    if (i != k) {
      uints[i] ^= uints[k];
      uints[k] ^= uints[i];
      uints[i] ^= uints[k];
    }
  }
  
  status = Error::None;
  while (false) {
    FINISHED:    status = Error::None; break;
    SOMEERROR:                         break;
    NULLPOINTER: BEFE_WARN("SortSelection: NULL Buffer");
                 status = Error::InternalNullPointer;
                 break;
  }
  
  return status;
  
}

} // ...Namespace BEFE