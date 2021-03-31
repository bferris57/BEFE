//----------------------------------------------------------------------
// File: SortMerge.h - Merge Sort implementation
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
// In-place Merge Sort
//

Status SortMerge(SortTarget const &target) {
  
  BEFE_ERROR("SortMerge",Error::NotCapable);  
  return Error::NotCapable;
  
}

//----------------------------------------------------------------------
//
// Out-of-place Merge Sort
//

Status SortMerge(SortTarget const &target, UInts &output) {

  Status  status;
  UInt    len;
  Int     cmp;
  UInts   lowInts[2];
  UInt   *uints[2];
  UInt    in;
  UInt    out;
  UInt    block1;
  UInt    block2;
  UInt    rem1;
  UInt    rem2;
  UInt    idx1;
  UInt    idx2;
  UInt    idxOut;
  UInt    i;
  UInt    j;
  UInt    m;
  
  //Get Length
  len = ((SortTarget *)&target)->Length();
  if (len <= 1) goto FINISHED;
    
  // Initialise output
  status = output.ShutDown();
  if (status) goto SOMEERROR;
  status = output.StartUp();
  if (status) goto SOMEERROR;
  
  // Build the two output buffers
  for (i=0; i < 2; i++) {
    status = lowInts[i]._BufAlloc(len);
    if (status) goto SOMEERROR;
    uints[i] = (UInt *)(lowInts[i]._GetBuf());
    if (IsNull(uints[i])) goto NULLPOINTER;
  }
  for (i=0, j=0; i < len; i++) {
    if (((SortTarget *)&target)->Selected(i)) {
      uints[0][j] = uints[1][j] = i;
      j++;
    }
  }
  len = j;
  status = lowInts[0].SetLength(len);
  if (status) goto SOMEERROR;
  status = lowInts[1].SetLength(len);
  if (status) goto SOMEERROR;

  in  = 1;
  out = 0;

  //
  // For each merge size...
  //

  //for (m=1; m < len; m<<=1) {
  for (m=1; m < len; m<<=1) {
    
    // Swap input and output lists
    in  = 1-in;    // Set input buffer index
    out = 1-out;   // Set output buffer index
    idxOut = 0;    // Set output index

    // For each pair of blocks of this size...
    for (block1=0; block1 < len; block1 += 2*m) { // For each pair...
      
      block2 = block1 + m;
      rem1 = Min(m, len-block1);
      if (rem1 == m)
        rem2 = Min(m, len-block2);
      else
        rem2 = 0;
      idx1 = block1;
      idx2 = block2;
      
      // Until we've exhausted the two blocks...
      while (rem1 && rem2) {
        cmp = ((SortTarget *)&target)->Compare(uints[in][idx1], uints[in][idx2]);
        if (cmp <= 0) {
          uints[out][idxOut++] = uints[in][idx1++];
          rem1--;
        }
        else {
          uints[out][idxOut++] = uints[in][idx2++];
          rem2--;
        }
      }
      
      // Fill in any remaining...
      // NOTE: Don't know which case happens but I think it's one of them!!!
      while (rem1) {
        uints[out][idxOut++] = uints[in][idx1++];
        rem1--;
      }
      while (rem2) {
        uints[out][idxOut++] = uints[in][idx2++];
        rem2--;
      }
      
    }  // ...For each pair
    
  }
  
  // Set the output
  status = output.MoveFrom(lowInts[out]);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED:    status = Error::None;                break;
    SOMEERROR:                                        break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }
      
  return status;
  
}

} // ...Namespace BEFE