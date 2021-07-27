//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: SBuffer.h - Declarations for the SBuffer class
//----------------------------------------------------------------------
//
// The SBuffer class is an "smart class", based on Buffer.
//
//  *** see Buffer.h for a better description ***
//
// The reason we're different is because we store N logical Unicode
// characters (type Char) in UTF-8 form.  So, if only ASCII is used,
// it's the same as Buffer.  It's when Unicode is used that we
// differ because Length() and Size() differ, and there's other
// methods provided to manipulate with Chars instead of Bytes.
//
// NOTE: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//       !!! It is IMPORTANT for the caller to realise the above and
//       !!! distinction between "self" and "this".  This means they
//       !!! cannot assume it makes any sense to have a SBuffer
//       !!! local variable or do a "new SBuffer()" EVER.
//       !!!
//       !!! NEVER DO THIS!!!
//       !!!
//       !!! Instead, use the SBuffer class itself to allocate and
//       !!! free string buffers.
//       !!!
//       !!! Thank you!
//       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef SBUFFER_H // SBUFFER_H...
#define SBUFFER_H

#include "Primitives.h"
#include "Buffer.h"

namespace BEFE { // Namespace BEFE...

class SBuffer : public Buffer { // Class SBUffer...

  //
  // Public instance methods
  //

  // Current chars/bytes in buffer
  public: UInt  Length();             // Get the number of characters in buffer
  public: UInt  Size();               // Get the number of bytes in buffer
  public: Char  GetChar(Int cIndex);  // Get given character in buffer

  // Manipulating Char contents
  public: Status AppendChar  (Char theChar);
  public: Status InsertChar  (Int cIndex, Char theChar);
  public: Status RemoveChar  (Int cIndex);

}; // ...Class SBuffer

} // ...Namespace BEFE

#endif // ...SBUFFER_H