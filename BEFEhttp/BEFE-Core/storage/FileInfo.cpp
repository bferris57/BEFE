//----------------------------------------------------------------------
// File: FileInfo.cpp - Implementation of the FileInfo struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "FileInfo.h"

namespace BEFE { // Namespace BEFE...

FileInfo::FileInfo() {
  StartUp();
}

FileInfo::FileInfo(FileInfo const &that) {
  StartUp();
  *this = that;
}

FileInfo::~FileInfo() {
  ShutDown();
}

FileInfo &FileInfo::operator=(FileInfo const &that) {
  CopyFrom(that);
  return *this;
}

Status FileInfo::StartUp() {

  Status retStatus;
  Status status;

  retStatus = Error::None;

  status = name.StartUp();
  if (!retStatus && status) retStatus = status;
  size = ULongNaN;
  creationTime.Reset();
  accessTime.Reset();
  updateTime.Reset();

  return retStatus;

}

Status FileInfo::ShutDown() {

  Status status;

  status = name.ShutDown();
  size = ULongNaN;
  creationTime.Reset();
  accessTime.Reset();
  updateTime.Reset();

  return status;

}

Status FileInfo::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();
  if (!retStatus && status) retStatus = status;

  return retStatus;
}

Boolean FileInfo::IsNull() const {
  return name.IsNull();
}

Status FileInfo::SetNull() {
  return Reset();
}
 
Status FileInfo::MoveFrom(FileInfo const &that) {

  Status status;

  if (this == &that) goto OK;

  status = Reset();
  if (status) goto SOMEERROR;

  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(FileInfo));
  status = ((FileInfo *)&that)->StartUp();
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
	SOMEERROR:                       break;
	OK:        status = Error::None; break;
  }

  return status;

}

Status FileInfo::CopyFrom(FileInfo const &that) {
  Status status;

  if (this == &that) goto OK;

  if (that.IsConsumable()) {
    status = MoveFrom(that);
    if (status) goto SOMEERROR;
    goto OK;
  }

  status = Reset();
  if (status) goto SOMEERROR;

  name         = that.name;
  size         = that.size;
  creationTime = that.creationTime;
  accessTime   = that.accessTime;
  updateTime   = that.updateTime;

  // Handle errors
  status = Error::None;
  while (false) {
	SOMEERROR:                       break;
	OK:        status = Error::None; break;
  }

  return status;

}

FileInfo &FileInfo::Consumable() const {
  ((FileInfo *)this)->name.Consumable();
  return *((FileInfo *)this);
}

FileInfo &FileInfo::NotConsumable() const {
  ((FileInfo *)this)->name.NotConsumable();
  return *((FileInfo *)this);
}  

Boolean FileInfo::IsConsumable() const {
  return name.IsConsumable();
}

} // ...Namespace BEFE
