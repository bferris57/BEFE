//----------------------------------------------------------------------
// File: LinuxFile.h - Declarations for the Win2File Storage Class
//----------------------------------------------------------------------
//
// The LinuxFile Class is responsible for implementing the abstract
// File Class on the Linux platform.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LINUXFILE_H // LINUXFILE_H...
#define LINUXFILE_H

#include "Primitives.h"
#include "Storage.h"
#include "File.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class LinuxFile - Linux platform File class implementation
//

class LinuxFile : public File { // File Class...

  // Friend classes
  friend class LinuxProcess;
  
  //
  // Public Instance Members
  //

  protected: String   name;           // File name
  protected: UInt     access;         // Access mode
  protected: UInt     fd;             // Open Linux File description
  protected: UInt     lastError;      // Last OS error

  // Current Size and position (only valid when open)
  protected: UInt     size;           // Current file size (in bytes)
  protected: UInt     pos;            // Current file position (in bytes)

  //
  // Instance Methods
  //

  // C++ Lifecycle
  public: LinuxFile();
  public: LinuxFile(char const *name);
  public: LinuxFile (String const &name);
  public: virtual ~LinuxFile();

  // BEFE Lifecycle
  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  public: virtual Status Clear();

  // Access...
  public: virtual Status  SetAccess(UInt mode);
  public: virtual Status  GetAccess(UInt &mode) const;

  // Naming...
  public: virtual String  GetName() const;
  public: virtual Status  SetName();
  public: virtual Status  SetName(char const *name);
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

  // Data Movement...
  public: virtual Status  Move  (StoreAddress src, Address      dst,  UInt numbytes, UInt &nummoved);
  public: virtual Status  Move  (Address      src, StoreAddress dst,  UInt numbytes, UInt &nummoved);
  public: virtual Status  Append(Address      src,                    UInt numbytes, UInt &nummoved);

}; // ...File Class

// File utility functions
String LinuxFileGetTempFileName(void);

} // ...Namespace BEFE

#endif // ...LINUXFILE_H
