//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: BEFEMacros.h - Handy BEFE C++ Macro declarations
//----------------------------------------------------------------------
//
// Although we don't like the idea of C macros since they're one of the
// most evil things invented in the history of Software Development,
// they're still incredibly handy in certain circumstances when forced
// to use C or C++.  So...
//
// We've defined some Macros that typically are only used by BEFE code
// but you can use them if you wish.
//
// The most IMPORTANT thing is that this header isn't included in any
// other headers.  So, YOU'VE BEEN WARNED!!!
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef BEFEMACROS_H // BEFEMACROS_H...
#define BEFE_MACROS_H

#include "BEFE.h"
#include "_GlobalMacros.h"

namespace BEFE { // Namespace BEFE...

//
// Process control macros and functions
//

#define BEFE_STARTUP()      ( (TheBefe) ? TheBefe->StartUp()                  : (Status)Error::NoBefe )
#define BEFE_HEADER()       ( (TheBefe) ? TheBefe->Header()                   : (Status)Error::NoBefe )
#define BEFE_EXIT(s)        ( (TheBefe) ? TheBefe->Exit((Status)(s))          : (Status)Error::NoBefe )
#define BEFE_ABORT(s)       ( (TheBefe) ? TheBefe->Abort(s)                   : (Status)Error::NoBefe )
#define BEFE_WARN(s)        ( (TheBefe) ? TheBefe->Warn(s)                    : (Status)Error::NoBefe )
#define BEFE_CONGRATS(s)    ( (TheBefe) ? TheBefe->Congrats(s)                : (Status)Error::NoBefe )
#define BEFE_ERROR(f,s)     ( (TheBefe) ? TheBefe->ErrorStatus(f,(Status)(s)) : (Status)Error::NoBefe )
#define BEFE_ERRORSTRING(s) if (TheBefe) TheBefe->ErrorString(s); };
#define BEFE_DEBUG(s)       ( (TheBefe) ? TheBefe->Debug(s)                   : (Status)Error::NoBefe )
#define BEFE_DEPRECATED(s)  ( (TheBefe) ? TheBefe->Deprecated(s)              : (Status)Error::NoBefe )
#define BEFE_TRAILER()      ( (TheBefe) ? TheBefe->Trailer()                  : (Status)Error::NoBefe )
#define BEFE_SHUTDOWN()     ( (TheBefe) ? TheBefe->ShutDown()                 : (Status)Error::NoBefe )

//
// Handy internal Macros
//

#define NEED_TheBefe      if (!TheBefe)               return Error::NoBefe;
#define NEED_TheOS        if (!TheBefe->TheOS)        return Error::InternalNoTheOS;
#define NEED_TheProcess   if (!TheBefe->TheProcess)   return Error::InternalNoTheProcess;
#define NEED_ThePlatform  if (!TheBefe->ThePlatform)  return Error::InternalNoTheProcess;
#define NEED_TheProcessor if (!TheBefe->TheProcessor) return Error::InternalNoTheProcess;
#define NEED_TheNetwork   if (!TheBefe->TheNetwork)   return Error::InternalNoTheProcess;

//----------------------------------------------------------------------
//
// Macros available (and recommended) everywhere
//

#define Min(a,b) (((a)<(b))?(a):(b))
#define Max(a,b) (((a)>(b))?(a):(b))
#define Abs(a)   (((a)>=0)?(a):(-(a)))

} // ...Namespace BEFE

#endif // ...BEFEMACROS_H