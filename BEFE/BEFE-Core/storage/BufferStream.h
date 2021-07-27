//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: BufferStream.h - Declarations for the BufferStream Storage Class
//----------------------------------------------------------------------
//
// This class provides a Stream capability for Memory maintained by a
// Buffer instance.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BUFFERSTREAM_H // BUFFERSTREAM_H...
#define BUFFERSTREAM_H

#include "Primitives.h"
#include "Storage.h"
#include "Stream.h"
#include "Buffer.h"

namespace BEFE { // Namespace BEFE...

class BufferStream : public Stream { // Class BufferStraem...

  // Class (static) Methods
  public: static StreamStats ClassStats();
  public: static Status      ClassStats(StreamStats &stats);
  
  // C++ Lifecycle
  public: BufferStream();
  public: BufferStream(BufferStream const &that);
  public: virtual ~BufferStream();
  public: BufferStream &operator=(BufferStream const &that);

  // BEFE Lifecycle
  public: virtual Status  StartUp();
  public: virtual Status  ShutDown();
  public: virtual Status  Reset();
  public: virtual Boolean IsNull() const;
  public: virtual Status  SetNull();
  public: virtual Boolean IsEmpty() const;
  public: virtual Status  SetEmpty();
  
  // Extra BufferStream methods
  public: virtual Byte    *GetBuf() const;
  
  // Capabilities
  public: virtual Int     GetCapability() const;// Get capabilities
  public: virtual UInt    GetElementSize() const;// Get current element size (in StorageUnits)
  public: virtual Status  SetElementSize(UInt elmsiz);// Set desired element size (in StorageUnits
  public: virtual UInt    Length() const;       // Total length of Stream (in Elements)
  public: virtual UInt    Size() const;         // Total size of Stream (in bytes)

  // Positional Range
  public: virtual UInt    GetPos() const;       // Get current positional index (in element index)
  public: virtual Status  SetPos(UInt pos);     // Set current index (0..Length())
  public: virtual Range   PosRange() const;     // Get available positional range (-N..N)
  public: virtual Status  NextPos();            // Position to next element
  public: virtual Status  PreviousPos();        // Position to previous element
  public: virtual Status  RewindPos();          // Set to beginning
  public: virtual Status  EndPos();             // Set to end
  public: virtual Status  SetSize(UInt newSize); // Set size of Stream

  // Buffering
  public: virtual Byte   *GetBuffer() const;
  public: virtual UInt    GetBufferSize() const;
  public: virtual Status  SetBufferSize(UInt size);
  
  // Gets and Puts...
  public: virtual Status  GetElement(Byte *b);
  public: virtual Status  GetBytes  (Byte *buf, UInt buflen, UInt &numread);
  public: virtual Status  PutElement(Byte *b);
  public: virtual Status  PutBytes  (Byte *buf, UInt buflen, UInt &numwritten);

  // Line I/O
  public: virtual String  GetLine   ();
  public: virtual Status  GetLine   (String &line);
  public: virtual Status  PutLine   (String &line);

  // Flushing if buffered and Putting...
  public: virtual Status  Flush();

  //
  // Public Class Members
  //

  public: static StreamStats classStats;

  //
  // Protected members
  //

  protected: UInt   setPosCalled:1;
  protected: UInt   curPos;
  protected: Buffer buffer;

  //
  // Protected methods
  //

  protected: Status AllocBuffer();
  protected: Status AllocBuffer(UInt size);

}; // ...Class BufferStream

} // ...Namespace BEFE

#endif // ...BUFFERSTREAM_H