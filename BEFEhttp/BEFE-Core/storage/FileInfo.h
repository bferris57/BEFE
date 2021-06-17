//!befe-publish inc
//----------------------------------------------------------------------
// File: FileInfo.h - Declarations for the FileInfo struct
//----------------------------------------------------------------------
//
// This Struct is used to identify and maintain interesting information
// about Storage Files.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef FILEINFO_H // FILEINFO_H...
#define FILEINFO_H

#include "Primitives.h"
#include "Storage.h"
#include "Time.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class FileInfo
//

struct FileInfo { // Class FileInfo...

  // Members
  String name;
  Long   size;
  Time   creationTime;
  Time   accessTime;
  Time   updateTime;

  // C++ Lifecycle
  FileInfo();
  FileInfo(FileInfo const &that);
  ~FileInfo();
  FileInfo &operator=(FileInfo const &that);

  // BEFE Lifecycle
  Status   StartUp();
  Status   ShutDown();
  Status   Reset();
  Boolean  IsNull() const;
  Status   SetNull();
  Status   MoveFrom(FileInfo const &that);
  Status   CopyFrom(FileInfo const &that);
  FileInfo &Consumable() const;
  FileInfo &NotConsumable() const;
  Boolean  IsConsumable() const;
  
}; // ...Class FileInfo

//----------------------------------------------------------------------
//
// Class FileInfos
//

typedef ArrayValue<FileInfo> FileInfos;

} // ...Namespace BEFE

#endif // ...FILEINFO_H