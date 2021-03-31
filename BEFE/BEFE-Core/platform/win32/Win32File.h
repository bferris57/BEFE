//----------------------------------------------------------------------
// File: Win32File.h - Declarations for the Win2File Storage Class
//----------------------------------------------------------------------
//
// The Win2File Class is responsible for implementing the abstract
// File Class on the Windows32 platform.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef WIN32FILE_H // WIN32FILE_H...
#define WIN32FILE_H

#include "Primitives.h"
#include "Storage.h"
#include "File.h"
#include "String.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class Win32File - Windows32 platform File class implementation
//

class Win32File : public File { // File Class...

  // Friend classes
  friend class Win32Process;
  
  //
  // Public Instance Members
  //

  protected: String   name;           // File name
  protected: UInt     access;         // Access mode
  protected: HANDLE   hFile;          // Open Win32 File Handle (NULL="Not Open")
  protected: UInt     lastError;      // Last OS error

  // Current Size and position (only valid when open)
  protected: UInt     size;           // Current file size (in bytes)
  protected: UInt     pos;            // Current file position (in bytes)

  //
  // Instance Methods
  //

  // C++ Lifecycle
  public: Win32File();
  public: Win32File(char const *name);
  public: Win32File (String const &name);
  public: virtual ~Win32File();

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

  //
  // Protected Class Methods
  //

  protected: static void ModeToStyle(UInt mode, UInt &access, UInt &shared);// Convert our access to Windows32 CreateFile Style

}; // ...File Class


//----------------------------------------------------------------------
//
// Win32File utility functions
//

String Win32FileGetTempFileName();
String Win32FileGetTempPath();

} // ...Namespace BEFE

#endif // ...WIN32FILE_H