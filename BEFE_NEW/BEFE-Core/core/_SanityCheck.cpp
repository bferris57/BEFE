//----------------------------------------------------------------------
// File: _SanityCheck.cpp - Implementation of the "Sanity Check" function
//----------------------------------------------------------------------
//
// The _SanityCheck() function should be called every now and again
// to verify that static globals aren't being raped and other stuff
// we dearly depend on seems to be intact.
//
// *** See technical documentation for a more detailed description ***
//
// Note: The _SanityCheck function is currently called at least ONCE for
//       each instantiation of the Befe Class.  So, if you want to add
//       extra checks in here, please feel free to do so.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "_SanityCheck.h"

#include <limits.h>
#if INT_MAX != 2147483647
  #error BEFE cannot be compiled for non 32-bit Architectures
#endif

namespace BEFE {

//----------------------------------------------------------------------
//
// Function: _SanityCheck - Perform the check
//

void _SanityCheck() {

  Boolean ok;

  ok = _ValidateIntBits();
  if (!ok) BEFE_ABORT("_ValidateIntBits Sanity Check Failed");
  ok = _ValidateIntBitMax();
  if (!ok) BEFE_ABORT("_ValidateIntBitMax Sanity Check Failed");
  if (sizeof(UInt) != 4) BEFE_ABORT("BEFE Only works with 32-bit architectures at the moment");
  
  return;

}

} // Namespace BEFE