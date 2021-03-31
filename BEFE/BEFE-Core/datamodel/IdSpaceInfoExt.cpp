//----------------------------------------------------------------------
// File: IdSpaceInfoExt.cpp - Implementation of the IdSpaceInfoExt Class
//----------------------------------------------------------------------
//
// The IdSpace contains all the implementation informatation required
// by a Base Implementation of the IdSpace class.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "IdSpace.h"
#include "IdSpaceInfoExt.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// IdSpace C++ Lifecycle
//

IdSpaceInfoExt::IdSpaceInfoExt() {
  StartUp();
}

IdSpaceInfoExt::IdSpaceInfoExt(const IdSpaceInfoExt &that) {
  StartUp();
  this->info = that.info;
  //this->deadIds = that.deadIds;
  this->liveIds = that.liveIds;
}

IdSpaceInfoExt::~IdSpaceInfoExt() {
  ShutDown();
}

Status IdSpaceInfoExt::StartUp() {

  info.shortName.StartUp();
  info.description.StartUp();

  info.nextId = 0;

  //info.creationTime.Set();
  //info.accessTime.Set();
  //info.updateTime.Set();

  info.idRange = URange("0..");
  info.idCount = 0;
  liveIds.StartUp();

  return Error::None;

}

Status IdSpaceInfoExt::ShutDown() {

  info.shortName.ShutDown();
  info.description.ShutDown();
  liveIds.ShutDown();

  return Error::None;

}

} // ...NamespaceBEFE
