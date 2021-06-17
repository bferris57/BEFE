//!befe-publish inc // Optional
//----------------------------------------------------------------------
// File: FormatOut.h - Utilities for formatting output information
//----------------------------------------------------------------------
//
// The static class functions here were placed here because they're all
// trying to do the same kind of things on different data.  That is they
// are expected to take internal representation of data and format it
// for display to the user.
//
// We've put them all here just to keep them in one managable place
// away from the internal classes, etc. that they look at.
//
// Essentially, each function is responsible for one of the following...
//
//   a) Formatting internal data into a single string
//
//   b) Formatting internal data into multiple strings to go in an array
//
// All these functions are passed the string/array to put the formatted
// representation into.
//
// Since it's not up to these functions to presume what the caller wants
// to do with the output we've made the decision to "append" instead of
// clearing and appending.  This means the caller must clear the output
// string or array before calling these functions IF and ONLY IF they
// want these functions to populate an empty string/array.
//
// This may seem counterintuitive at first but we've done it way
// because...
//
//   a) It's faster (no allocating new strings/arrays by copying)
//
//   b) It's easier when the caller's producing mixed output from
//      different functions.
//
// We chose to put these functions as members of a "FormatOut" class
// for some initially counterintuitive reasons...
//
//   a) This gives us the ability to have a shared context between
//      the functions if/when that becomes an issue.
//
//   b) We wanted to keep their names away from all the other possible
//      functions named the same thing.  We could have done this by
//      using namespaces but, quite honestly, it just makes more
//      sense to put them as instance methods in a class.
//
// 'Nuff said.  It's not rocket science but it deserved pointing that
// out before you start using these functions.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef FORMATOUT_H // FORMATOUT_H...
#define FORMATOUT_H

#include "Primitives.h"
#include "File.h"
//#include "Path.h"
#include "Platform.h"
#include "Process.h"
#include "Processor.h"
#include "OperatingSystem.h"
#include "Network.h"

namespace BEFE { // Namespace BEFE...

class FormatOut { // Class FormatOut...

  //
  // Everything's public from here on...
  //

public:

  // Constructors/destructors
  FormatOut();
  ~FormatOut();

  //
  // Miscellaneous non-object related stuff
  //

  Status FormatPageSeparator(Strings &formatted);
  Status FormatCopyrights(Strings &formatted);
  Status FormatHeader(Strings &formatted);
  Status FormatFooter(Strings &formatted);

  //
  // Formatting Stream info
  //

  Status FormatStreamBasics    (Stream &stream, Strings &formatted);
  Status FormatStreamCapability(Stream &stream, Strings &formatted);
  Status FormatStreamSizes     (Stream &stream, Strings &formatted);

  //
  // Formatting String info
  //

  Status DumpStringInfo(String *string);
  Status StringInfo(String *string,Strings &strinfo);

}; // ...Class FormatOut

} // ...Namespace BEFE

#endif // ...FORMATOUT_H