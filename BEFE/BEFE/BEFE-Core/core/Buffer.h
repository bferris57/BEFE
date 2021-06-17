//!befe-publish inc
//----------------------------------------------------------------------
// File: Buffer.h - Declarations for the Buffer class
//----------------------------------------------------------------------
//
// *** REWRITE THIS DESCRIPTION ***
//
// The Buffer class is a "smart class" that MUST NEVER be
// instantiated via a local variable.  (We make sure by declaring the
// constructors and destructors private).
//
// This is because it is responsible for maintaining the buffer of
// bytes for the caller, which may change size at runtme. It knows and
// maintains the allocated memory buffer size, bytes allocated vs. used.
//
// Essentially, the Buffer class methods themselves work as static
// class methods that control where the bytes are in memory.
// So, they take "ownership" control of the pointer to the bytes.
// This is unlike the typical C++ concept of "this".  So, we've
// introduced the Python-like concept of "self" in these static class
// methods to take control of the pointer.
//
// NOTE: !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//       !!! It is IMPORTANT for the caller to realise the above and
//       !!! distinction between "self" and "this".  This means they
//       !!! cannot assume it makes any sense to have a Buffer
//       !!! local variable or do a "new Buffer()" EVER.
//       !!!
//       !!! NEVER DO THIS!!!
//       !!!
//       !!! Instead, use the Bytes class itself.  That one proxies to
//       !!! Buffer to take the headache away from the caller.
//       !!!
//       !!! Thank you!
//       !!!
//       !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// NOTE: This "self" behaviour is "sort of like" but NOT "sort of like"
//       the C++0x (C++11) "move" constructor with the "&&".  The main
//       this is it has little to do with "constructing" and, as said
//       above, is like Python's "self" rubbish.  So, don't worry about
//       confusing those concepts and, for the BEFE experience, just
//       go along with our way of thinking while using our code, okay?
//
//       Not just that, we're staying away from most C++0x stuff because
//       we're highly suspicious of majority of these "enhancements"
//       except where they properly model behaviour we're willing to
//       concede are actually useful.
//
//       'Nuff said. ;-)
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BUFFER_H  // BUFFER_H...
#define BUFFER_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

class Buffer { // Class Buffer...

  //
  // Public Class (static) methods
  //
  
  public: static UInt GetTotalAllocations();
  public: static UInt GetActiveAllocations();
  
  //
  // Public Instance methods
  //
  
  // C++ Lifecycle
  public: Buffer();
  public: Buffer(Buffer const &that);
  public: Buffer(UInt initBytes, UInt incBytes);
  public: ~Buffer();
  public: Buffer &operator=(Buffer const &that);
  
  // BEFE Lifecycle
  public: Status   StartUp();
  public: Status   ShutDown();
  public: Status   Reset();
  public: Boolean  IsNull() const  { return pointer == NULL; };
  public: Status   SetNull()       { return Reset();         };
  public: Boolean  IsEmpty() const { return pointer == NULL; };
  public: Status   SetEmpty();
  public: Status   MoveFrom(Buffer const &that);
  public: Status   CopyFrom(Buffer const &that);
  public: Buffer  &Consumable() const;
  public: Buffer  &NotConsumable() const;
  public: Status   SetSize(UInt newBytes);
  public: Status   Resize (UInt newBytes);
  public: Status   Allocate(UInt initBytes);
  public: Status   Allocate(UInt initBytes, UInt incBytes);
  
  // Manipulating Byte contents
  public: Status   GetBytes     (UInt bIndex, Byte *theBytes, UInt bufMaxSize, UInt &usedSize);
  public: Status   SetBytes     (UInt bIndex, Byte *theBytes, UInt bufMaxSize, UInt &usedSize);
  public: Status   AppendBytes  (Byte *theBytes, UInt numBytes);
  public: Status   InsertBytes  (UInt bIndex, Byte *theBytes, UInt numBytes);
  public: Status   RemoveBytes  (UInt bIndex, UInt numBytes);
  public: Status   ReplaceBytes (UInt bIndex, UInt numBytes, Byte *newBytes, UInt newSize);
  
  // Handy C++ Manipulation
  public: Status   Append(char const *that);
  public: Status   Append(char *that);
  
  // Buffer info
  public: UInt     Size() const;                // Get size in bytes
  public: UInt     Length() const;              // Get length in bytes (same thing as size for this class)
  public: Boolean  IsConsumable() const;        // "Is this Buffer condumable?"
  
  public: UInt     GetAllocBytes() const;       // Get allocated bytes
  public: UInt     GetIncrementBytes() const;   // Get byte increment count
  public: UInt     GetUsedBytes() const;        // Get number of used bytes
  public: Status   SetUsedBytes(UInt newSize);  // Set number of used bytes
  public: Byte    *GetBuf() const;              // Get pointer to the bytes (transient)

  //
  // Public instance members
  //
  
  public: Byte *pointer;                        // ◄── Actually points to an internal BufferLow instance
  
  //
  // Protected Class (static) members
  //
  
  public: static UInt32 totalAllocations;
  public: static UInt32 activeAllocations;
  
};  // ...Class Buffer

//----------------------------------------------------------------------
//
// Buffer Functions
//

BEFE_INLINE Boolean IsNull(Buffer const &that)    {return that.IsNull();}
BEFE_INLINE Status  SetNull(Buffer &that)         {return that.SetNull();}
BEFE_INLINE Boolean IsEmpty(Buffer const &that)   {return that.IsEmpty();}
BEFE_INLINE Status  SetEmpty(Buffer &that)        {return that.SetEmpty();}
  
} // ...Namespace BEFE

#endif // ...BUFFER_H