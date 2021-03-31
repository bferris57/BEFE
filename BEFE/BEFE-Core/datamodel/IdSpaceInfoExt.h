//----------------------------------------------------------------------
// File: IdSpaceInfoExt.h - Declarations for the IdSpaceInfoExt Class
//----------------------------------------------------------------------
//
// This class is an <Impementation Class> for <Id Spaces>. As such, it
// should only be used within the IdSpace implementation code itself.
//
// The purpose of the IdSpaceInfoExt class is to maintain a complete
// set of <Implementation> values of an <IdSpace>.
//
// This is tthe "hidden" copy of the <IdSpaceInfo> values that are
// maintained under the covers by the IdSpace implementation itself,
// with additional values required for the implementation itself to
// behave properly.
//
// So, the consumer should not be exposed to these because they
// ARE the "real thing"!!!!!
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef IDSPACEINFOEXT_H // IDSPACEINFOEXT_H...
#define IDSPACEINFOEXT_H

#include "Primitives.h"
#include "Id.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: IdSpaceInfoExt - "Class Globals" for given IdSpace...
//

class IdSpaceInfoExt { // Class IdspaceInfoExt...

  // Friend classes
  friend class IdSpace;

  // "Public Info"
  public: IdSpaceInfo info;

  // "Private Info"
  public: _SparseBase liveIds;   // "Live Ids we're taking care of

  // C++ Lifecycle
  //
  // Note: We've made the destructor virtual because it acts like
  //       an IdSpace and, therefore, must have a vtable at the
  //       beginning of it

  public: IdSpaceInfoExt();
  public: IdSpaceInfoExt(const IdSpaceInfoExt &that);
  public: virtual ~IdSpaceInfoExt();

  // BEFE Lifecycle
  public: Status StartUp();
  public: Status ShutDown();

}; // ...Class IdSpaceInfoExt

} // ...Namespace BEFE

#endif // ...IDSPACEINFOEXT_H