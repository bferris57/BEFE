//!befe-publish inc
//----------------------------------------------------------------------
// File: Stream.h - Declarations for Stream and related classes
//----------------------------------------------------------------------
//
// This contains the generic declarations for all streams.  Unlike most
// other stream implementations we've made this one a workhorse instead
// of a whole bunch of interfaces you can never remember the name of
// or get their own peculiarities in you head.
//
// As such, it's a bit of overkill for simple cases but, once you get
// your head around it, this interface provides a lot of what you need
// to stick in your head about streams.  'Nuff said about that...
//
// Each stream has a set of capabilities described in the Capability
// enum.  It is a 32-bit word formed as follows:
//
//   Stream CAPABILITY
//
//    3      2 2 1 1 1 1│1 1 1 1 1 1    │
//    1 ...  1 0 9 8 7 6│5 4 3 2 1 0 9 8│7 6 5 4 3 2 1 0
//   ┌─┼...─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┐
//   │ │... │1│1│1│1│1│1│0│1│1│1│1│1│1│1│1 0│1│1│0 0│1│1│
//   └─┴...─┴─┼─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴─┴┼┴─┴┼┴┼┴─┴─┴┼┴┼┘
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   │ │     │ │
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   │ │     │ └── FixedElementSize
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   │ │     └── FixedTotalSize
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   │ │     └─┘   Known Size (0="Unknown")
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   │ │
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   │ └────────── HasBuffer
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │   └────────── CanBuffer
//           │ │ │ │ │ │   │ │ │ │ │ │ │ │
//           │ │ │ │ │ │   │ │ │ │ │ │ │ └──────────── IsInteractive (UI)
//           │ │ │ │ │ │   │ │ │ │ │ │ └──────────── CanPosition
//           │ │ │ │ │ │   │ │ │ │ │ └──────────── CanPut
//           │ │ │ │ │ │   │ │ │ │ └──────────── CanGet
//           │ │ │ │ │ │   │ │ │ └──────────── CanFlush
//           │ │ │ │ │ │   │ │ └──────────── CanTruncate
//           │ │ │ │ │ │   │ └──────────── CanLookAhead
//           │ │ │ │ │ │   └──────────── CanLookBehind
//           │ │ │ │ │ │
//           │ │ │ │ │ └─ IsFixedLength
//           │ │ │ │ └─ IsBigEndian
//           │ │ │ └─ IsRaw
//           │ │ └─ IsUTF8
//           │ └─ IsStructured
//           └─ IsBasedOnStream
//
// Essentially, a Stream is a Stream of Bytes.  It is NOT some kind
// of structured store.  It is a STREAM.
//
// The Persist class should be used to load and store persistent data
// that is structured.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef STREAM_H // STREAM_H...
#define STREAM_H

#include "Primitives.h"
#include "Integer.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: StreamStats - Stream statistics
//

struct StreamStats { // Struct StreamStats...

  UInt numLogReads;
  UInt numLogWrites;
  UInt numPhyReads;
  UInt numPhyWrites;
  UInt numBytesRead;
  UInt numBytesWritten;
  UInt numLineReads;
  UInt numLineWrites;
  
  StreamStats() { Reset(); }

  void Reset() {
    numLogReads     = 0;
    numLogWrites    = 0;
    numPhyReads     = 0;
    numPhyWrites    = 0;
    numBytesRead    = 0;
    numBytesWritten = 0;
    numLineReads    = 0;
    numLineWrites   = 0;
  }
  
}; // ...Struct StreamStats

//----------------------------------------------------------------------
//
// Class: Stream - Stream Base Class
//

class Stream { // Class Stream...

public:

  enum Capability {

     None

    ,FixedElementSize    = 0x00000001
    ,FixedTotalSize      = 0x00000002
    ,KnownSize           = 0x00000003

    ,HasBuffer           = 0x00000010
    ,CanBuffer           = 0x00000020

    ,IsInteractive       = 0x00000080

    ,CanPosition         = 0x00000100
    ,CanPut              = 0x00000200
    ,CanGet              = 0x00000400
    ,CanFlush            = 0x00000800
    ,CanTruncate         = 0x00001000
    ,CanLookAhead        = 0x00002000
    ,CanLookBehind       = 0x00004000

    ,IsFixedLength       = 0x00010000
    ,IsRaw               = 0x00020000
    ,IsUTF8              = 0x00040000
    ,IsBigEndian         = 0x00080000
    ,IsStructured        = 0x00100000
    ,IsOtherStream       = 0x00200000

  };

  //
  // Public instance methods
  //

  // C++ Lifecycle
  Stream();
  public: virtual ~Stream();

  // C++ Operator =
  public: virtual Stream &operator= (Stream const &that);
  public: virtual Stream &operator= (String const &that);
  public: virtual Stream &operator= (char const *that);
  public: virtual Stream &operator= (char *that);
  public: virtual Stream &operator= (char that);
  public: virtual Stream &operator= (Short  that);
  public: virtual Stream &operator= (UShort that);
  public: virtual Stream &operator= (Int  that);
  public: virtual Stream &operator= (UInt that);
  public: virtual Stream &operator= (void *that);

  // C++ Operator <<
  public: virtual Stream &operator<< (Char that);
  public: virtual Stream &operator<< (String const &that);
  public: virtual Stream &operator<< (Integer const &that);
  public: virtual Stream &operator<< (char const *that);
  public: virtual Stream &operator<< (char  *that);
  public: virtual Stream &operator<< (char   that);
  public: virtual Stream &operator<< (Short  that);
  public: virtual Stream &operator<< (UShort that);
  public: virtual Stream &operator<< (Int    that);
  public: virtual Stream &operator<< (UInt   that);
  public: virtual Stream &operator<< (Long   that);
  public: virtual Stream &operator<< (void   *that);

  // Capabilities
  public: virtual Int     GetCapability() const;// Get capabilities
  public: virtual UInt    GetElementSize()const;// Get current element size (in StorageUnits)
  public: virtual Status  SetElementSize(UInt elmsiz);// Set desired element size (in StorageUnits)
  public: virtual UInt    Length() const;      // Total length of Stream (in Elements)
  public: virtual UInt    Size() const;        // Total size of Stream (in bytes)

  // Positional Range
  public: virtual UInt    GetPos() const;      // Get current positional index (in element index)
  public: virtual Status  SetPos(UInt pos);    // Set current positional index (0..Length())
  public: virtual Status  SetRelativePos(Int pos); // Set relative position -- see PosRange()
  public: virtual Range   PosRange() const;    // Get available positional range (-N..N)
  public: virtual Status  NextPos();           // Position to next element
  public: virtual Status  PreviousPos();       // Position to previous element
  public: virtual Status  RewindPos();         // Set to beginning
  public: virtual Status  EndPos();            // Set to end
  public: virtual Status  SetSize(UInt newSize); // Set size of Stream (if supported)

  // Flushing if buffered and Putting...
  public: virtual Status  Flush();

  // Base Buffering
  //
  // NOTE: Address returned by GetBuffer is transient
  public: virtual Byte   *GetBuffer() const;
  public: virtual UInt    GetBufferSize() const;
  public: virtual Status  SetBufferSize(UInt size);

  // Base Gets... NOTE: It is up to the sub-class to define the Get()
  //                    methods as they wish, however...
  public: virtual Status  _GetElement(Byte *b);
  public: virtual Status  _GetChar   (Char &c);
  public: virtual Status  _GetByte   (Byte &b);
  public: virtual Status  _GetBytes  (Byte *buf, UInt buflen, UInt &numread);
  
  // Base Puts... NOTE: It is up to the sub-class to define the Put()
  //                    methods if they wish, however...
  public: virtual Status  _PutElement(Byte *b);
  public: virtual Status  _PutBytes  (Byte *buf, UInt buflen, UInt &numwritten);
  public: virtual Status  _PutByte   (Byte  b);

  // Line I/O
  public: virtual Status  GetLine    (String &line);
  public: virtual Status  PutLine    (String &line);

  // Multiple Line I/O
  public: virtual Strings GetLines  ();
  public: virtual Status  GetLines  (Strings &lines);

}; // ...Class Stream

} // ...Namespace BEFE

#endif // ...STREAM_H