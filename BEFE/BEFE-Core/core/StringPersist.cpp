//----------------------------------------------------------------------
// File: StringPersist.cpp - String Class Persistence methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "SBuffer.h"
#include "UTF8.h"
#include "Persist.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Method: _PersistWriteAtomic - Write Atomic Persistent String
//

Status String::_PersistWriteAtomic(Persist &store) {

  Status   status;
  SBuffer *sbuf;
  Byte    *srcbuf;
  UInt     srcbufl;
  UInt     remmult;
  
  // Initialise
  srcbuf   = NULL;
  srcbufl  = UNaN;

  // Output it...
  switch (type) {
    case Null:
      break;
    case Const:
      if (data) {
        srcbuf = (Byte *)data;
        srcbufl = Strlen((const char *)srcbuf);
      }
      break;
    case ConstLen:
      srcbuf = (Byte *)data;
      srcbufl = constLen;
      break;
    case ASCII0:
    case ASCII1:
    case ASCII2:
    case ASCII3:
    case ASCII4:
      srcbuf = (Byte *)&data;
      srcbufl = type - ASCII0;
      break;
    case UtfInline:
      srcbuf = (Byte *)&data;
      srcbufl = constLen;
      break;
    case Buffer:
      if (data) {
        sbuf = (SBuffer *)&data;
        srcbuf = sbuf->GetBuf();
        if (BEFE::IsNull(srcbuf)) break;
        srcbufl = sbuf->GetUsedBytes();
      }
      break;
    default:
      goto INTERNALERROR;
  }

  // Write the length
  status = store.WriteAtomicLength(srcbufl);
  if (status) goto SOMEERROR;
  
  // Write the data
  remmult = mult;
  while ( !BEFE::IsNull(remmult) && remmult != 0 && !BEFE::IsNull(srcbufl) ) {
    status = store.WriteAtomicRaw(srcbuf,srcbufl);
    if (status) goto SOMEERROR;
    remmult--;
  }

  // NUL terminate
  status = store.WriteAtomic((UInt8)0x00);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                                          break;
    INTERNALERROR:    status = Error::Internal;         break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Method: _PersistReadAtomic - Read Atomic Persistent String
//

Status String::_PersistReadAtomic(Persist &store) {
  BEFE_WARN("String._PersistReadAtomic not implemented yet");
  return Error::NotImplemented;
}
  
} // ...Namespace BEFE