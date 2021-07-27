//!befe-publish inc
//----------------------------------------------------------------------
// File: Arrays.h - Commonly used Array/Sparse/Map Value Template Typedefs
//----------------------------------------------------------------------
//
// This header file declares various Array-like Template Typedefs that
// don't belong in other headers because of inter-dependencies.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ARRAYS_H // ARRAYS_H...
#define ARRAYS_H

#include "Primitives.h"
#include "Null.h"
#include "Map.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Commonly used <V>MapValue Template Uses
//

typedef UIntMapValue<String>   UIntStrings;
BEFE_INLINE Boolean IsNull(UIntStrings const &that) {return that.IsNull();}
BEFE_INLINE Boolean IsEmpty(UIntStrings const &that) {return that.IsEmpty();}

typedef StringMapValue<String> NamedStrings;
BEFE_INLINE Boolean IsNull(NamedStrings const &that) {return that.IsNull();}
BEFE_INLINE Boolean IsEmpty(NamedStrings const &that) {return that.IsEmpty();}

typedef StringMapPrimitive<Int> NamedInts;
BEFE_INLINE Boolean IsNull(NamedInts const &that) {return that.IsNull();}
BEFE_INLINE Boolean IsEmpty(NamedInts const &that) {return that.IsEmpty();}

typedef StringMapPrimitive<UInt> NamedUInts;
BEFE_INLINE Boolean IsNull(NamedUInts const &that) {return that.IsNull();}
BEFE_INLINE Boolean IsEmpty(NamedUInts const &that) {return that.IsEmpty();}


} // ...Namespace BEFE

#endif // ...ARRAYS_H