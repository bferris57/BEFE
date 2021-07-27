//!befe-publish inc
//----------------------------------------------------------------------
// File: _SanityCheck.h - Declarations for basic "Sanity Check" functions
//----------------------------------------------------------------------
//
// This header file pulls together all the weird and wonderful functions
// throughout BEFE that are there only to provide "Sanity Checks".
//
// Examples are functions which verify static globals aren't raped,
// etc.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef _SANITY_CHECK_H // _SANITY_CHECK_H...
#define _SANITY_CHECK_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//
// _SanityCheck - Perform sanity checks and abort of any fail
//

extern void _SanityCheck();

//
// Miscellaneous functios
//

extern Boolean _ValidateIntBits();
extern Boolean _ValidateIntBitMax();

} // ...Namespace BEFE

#endif // ..._SANITY_CHECK_H