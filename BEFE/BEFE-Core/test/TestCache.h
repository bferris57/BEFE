//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: TestCache.h - Declarations for the TestClass Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE Cache Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TESTCACHE_H // TESTCACHE_H...
#define TESTCACHE_H

#include "Primitives.h"
#include "Test.h"
#include "Cache.h"

namespace BEFE { // Namespace BEFE...

class TestCache : public Test { // Class TestCache...

  public: Status Run();

  public: Status testEmpty();
  public: Status testAllocateBlocks();
  public: Status testStartCacheFiles();
  //public: Status testStopCacheFiles();
  public: Status testBlockLoad();
  public: Status testBlockLoadSource();
  
  // For internal use...
  protected: UInt   indent;
  protected: Cache  cache;
  protected: Status DisplayBEFE_GIT(UInt indent);
  protected: Status DisplayCache(UInt indent);
  
}; // ...Class TestCache

} // ...Namespace BEFE

#endif // ...TESTCACHE_H
