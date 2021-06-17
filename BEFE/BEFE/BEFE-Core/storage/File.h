//!befe-publish inc
//----------------------------------------------------------------------
// File: File.h - Declarations for the File Storage Class
//----------------------------------------------------------------------
//
// This class is a <Foundation Class> for persistent Storage.
//
// Basically, the File class implements, you guessed it, a File!
//
// Unlike most File-like viewpoints, we take the approach that a File
// is a Storage mechanism very much like Memory itself is.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef FILE_H // FILE_H...
#define FILE_H

#include "Primitives.h"
#include "Storage.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

class Lines;

class File { // Class File...

  // Friends...
  friend class FileStream;
  friend class Cache;
  
  //
  // File Access Mode...
  //
  // Note: The ModeAppend automatically turns on ModeWrite since it
  //       doesn't make sense to "ReadAppend".  ;-)
  //
  //       ModeAppend is intended for things like Log Files which, are
  //       perfectly okay to read randomly (e.g. given an Address in the
  //       file) while you have it open but, when writing, the data
  //       MUST go, atomically, to the end of the file since other
  //       processes may be Appending to it as well.
  //
  //       On file systems that don't support this operation, we
  //       typically open it write exclusive, to get the same
  //       behaviour.  The only side effect is you have to be
  //       aware that it may not open with WriteAppend, so you have
  //       to use a different, distinct, log file instead.
  //
  // Note: Come to think about it, we may deprecate this if it becomes
  //       an issue because, quite honestly, what's wrong with opening
  //       an exclusive, distinctly named, log file???
  //

  public: enum Mode {

     ModeNone           = 0x0000
    ,ModeRead           = 0x0001      // For Read Random
    ,ModeWrite          = 0x0002      // For Write Random
    ,ModeAppend         = 0x0004      // For Append
    ,ModeWriteThrough   = 0x0008      // Force Writes to flush
    ,ModeTransient      = 0x0010      // File is transient (Okay to delete on Close)
    
    ,ModeReadWrite      = ModeRead | ModeWrite
    ,ModeAll            = ModeRead | ModeWrite | ModeAppend
    ,ModeDefault        = ModeRead

    ,ModeMask           = 0x001F      // <-- All the used bits in Mode

  };

  //
  // Protected Instance Members
  //

  protected: File   *deferee;         // File to defer the actions to
  protected: Int     deferDepth;      // Deferal depth

  //
  // Instance Methods
  //

  // C++ Lifecycle
  public: File();
  public: File(char const *name);
  public: File(String const &name);
  public: File(File const &file);
  public: virtual ~File();
  public: File &operator=(char const *name);
  public: File &operator=(String const &name);
  public: File &operator=(File const &name);

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  public: virtual Status MoveFrom(File const &that);
  public: virtual Status CopyFrom(File const &that);

  // Access...
  public: virtual Status  SetAccess(UInt mode);
  public: virtual UInt    GetAccess() const;
  public: virtual Status  GetAccess(UInt &mode) const;
  public: virtual Boolean CanRead() const;
  public: virtual Boolean CanWrite() const;
  
  // Naming...
  public: virtual String  GetName() const;
  public: virtual Status  SetName();
  public: virtual Status  SetName(const char *name);
  public: virtual Status  SetName(String const &name);

  // Query
  public: virtual Boolean IsNameValid() const;  // "Is the name valid?"
  public: virtual UInt    Length() const;       // Length (in elements)
  public: virtual UInt    Size() const;         // Size (in storage units)
  public: virtual UInt    GetMaxLength() const; // Get Maximum length (in elements)
  public: virtual UInt    GetMaxSize() const;   // Get Maximum size (in bytes)

  // Existence
  public: virtual Boolean Exists() const;       // "Does this exist?"
  public: virtual Status  Create();             // ◄── "Create new file" (new file must NOT exist)
  public: virtual Status  Delete();             // ◄── "Delete file" (must exist)
  public: virtual Status  Rename(String const &newName); // ◄── "Rename file" (must exist, new file must NOT exist)
  public: virtual Status  Move(String const &newPath);   // ◄── "Move file" (must exist, new file must NOT exist)
  public: virtual Status  SetLength(UInt len);  // Set length (in elements)
  public: virtual Status  SetSize(UInt size);   // Set size (in storage units)

  // Opening/Closing
  public: virtual Boolean IsOpen() const;
  public: virtual Status  Open();
  public: virtual Status  Open(Int access);
  public: virtual Status  Close();

  // Low level Data Movement...
  public: virtual Status  Read  (StoreAddress src, Address      dst, UInt numbytes, UInt &nummoved);
  public: virtual Status  Write (Address      src, StoreAddress dst, UInt numbytes, UInt &nummoved);
  public: virtual Status  Move  (StoreAddress src, Address      dst, UInt numbytes, UInt &nummoved);
  public: virtual Status  Move  (Address      src, StoreAddress dst, UInt numbytes, UInt &nummoved);
  public: virtual Status  Append(Address      src,                   UInt numbytes, UInt &nummoved);

  // High level Data Movement...
  public: virtual Status  ReadLines(Lines &lines);
  public: virtual Status  ReadContents(Bytes &bytes);
  public: virtual Bytes   ReadContents();
  //public: virtual UInt    CountLines();
  //public: virtual Strings ReadLines();
  //public: virtual String  Line(Int i);

//
// Protected internal functions
//

  protected: virtual File   *GetDeferee() const;
  protected: virtual Status  ValidateBOM(Byte *buf, UInt bufLen) const;
  
}; // ...Class File

} // ...Namespace BEFE

#endif // ...FILE_H