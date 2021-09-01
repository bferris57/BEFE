//!befe-publish inc
//----------------------------------------------------------------------
// File: FileStream.h - Declarations for the FileStream Storage Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for persistent Storage.
//
// Basically, the File class implements, you guessed it...
//
//    a File appearing like a Stream!
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef FILESTREAM_H // FILESTREAM_H...
#define FILESTREAM_H

#include "Primitives.h"
#include "Storage.h"
#include "Stream.h"
#include "File.h"
#include "Array.h"

namespace BEFE { // Namespace BEFE...

class FileStream : public Stream { // Class FileStream...

  // Friends
  friend class Cache;
  
  // Class (static) Methods
  public: static StreamStats ClassStats();
  public: static Status      ClassStats(StreamStats &stats);
  
  // C++ Lifecycle
  public: FileStream();
  public: FileStream(char const *name);
  public: FileStream(String const &name);
  public: FileStream(FileStream const &fileStream);
  public: virtual ~FileStream();
  public: FileStream &operator=(char const *name);
  public: FileStream &operator=(String const &name);

  // BEFE Lifecycle
  public: virtual Status      StartUp();
  public: virtual Status      ShutDown();
  public: virtual Status      Reset();
  public: virtual Boolean     IsNull() const;
  public: virtual Status      SetNull();
  public: virtual Status      MoveFrom(FileStream const &that);
  public: virtual Status      CopyFrom(FileStream const &that);
  public: virtual FileStream &Consumable()    const;
  public: virtual FileStream &NotConsumable() const;
  public: virtual Boolean     IsConsumable()  const;

  // Access...
  public: virtual Status  GetAccess(UInt &mode) const;
  public: virtual UInt    GetAccess() const;
  public: virtual Status  SetAccess(UInt mode);
  public: virtual Status  ValidateBOM();
  
  // Naming...
  public: virtual String  GetName() const;
  public: virtual Status  SetName();
  public: virtual Status  SetName(const char *name);
  public: virtual Status  SetName(String const &name);

  // Query
  public: virtual Boolean IsNameValid() const;  // "Is the name valid?"
  public: virtual UInt    GetMaxLength() const; // Get Maximum length (in elements)
  public: virtual UInt    GetMaxSize() const;   // Get Maximum size (in bytes)
  public: virtual Boolean IsOpen() const;       // "Is this FileStream open?"
  
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
  public: virtual UInt    GetBufferSize();
  public: virtual Status  SetBufferSize(UInt size);

  // Gets and Puts (including Overrides)...
  public: virtual Status  _GetBytes (Byte *buf, UInt buflen, UInt &numread);
  public: virtual Status  GetElement(Byte *b);
  public: virtual Status  GetBytes  (Byte *buf, UInt buflen, UInt &numread);
  public: virtual Status  PutElement(Byte *b);
  public: virtual Status  _PutBytes (Byte *buf, UInt buflen, UInt &numwritten);
  public: virtual Status  PutBytes  (Byte *buf, UInt buflen, UInt &numwritten);

  // Line I/O
  public: virtual String  GetLine   ();
  public: virtual Status  GetLine   (String &line);
  public: virtual Status  PutLine   (String &line);

  // Multiple Line I/O
  public: virtual Status  GetLines  (Strings &lines);

  // Flushing if buffered and Putting...
  public: virtual Status  Flush();

  //
  // Public Class Members
  //

  private: static StreamStats classStats;

  //
  // Protected members
  //

  protected: File   file;
  protected: UInt   fileSize;
  protected: UInt   curPos;
  protected: UInt   bufPos;
  protected: Buffer buffer;

  // Flags
  protected: UInt    isDirty:1;
  protected: UInt    setPosCalled:1;

  //
  // Protected methods
  //

  protected: Status AllocBuffer();
  protected: Status PositionBuffer(UInt newPos);
  protected: Status FlushBuffer();
  
}; // ...Class FileStream

//----------------------------------------------------------------------
//
// Class FileStreams
//

typedef ArrayValue<FileStream> FileStreams;

} // ...Namespace BEFE

#endif // ...FILESTREAM_H