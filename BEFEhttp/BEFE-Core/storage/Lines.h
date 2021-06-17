//!befe-publish inc
//----------------------------------------------------------------------
// File: Lines.h - Declarations for the Lines class
//----------------------------------------------------------------------
//
// The Lines class implements a, currently, read-only representation
// of a text file as lines.  The entire file contents are stored in a
// single SBuffer, and an ArrayString is maintained with, each element
// of which points to the contents of the string buffer.
//
// This makes parsing and error reporting much easier in the long
// run.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef LINES_H // LINES_H...
#define LINES_H

#include "Primitives.h"
#include "File.h"
#include "Buffer.h"

namespace BEFE { // Namespace BEFE...

class Lines { // Class Lines...

  //
  // Public methods
  //
  
  // C++ Lifecycle
  public: Lines();
  public: ~Lines();

  // BEFE Lifecycle
  public: Status StartUp();
  public: Status Reset();
  public: Status ShutDown();
  public: UInt   Length();
  
  // Loading
  public: Status Load(const char *fname);
  public: Status Load(const char *fname, Boolean trimwhite);
  public: Status Load(String const &fname);
  public: Status Load(String const &fname, Boolean trimwhite);
  public: Status Load(File &file);
  public: Status Load(File &file, Boolean trimwhite);

  // Line info
  public: String GetLine(UInt index);                      // ◄── Safe!
  public: Status GetLine(UInt index,String &line);         // ◄── Safe!

  //
  // Protected and Internal use methods
  //
  
  protected: Status _Build(Boolean trimwhite);
  public:    void   _BufAndSize(Byte *&buf, UInt &size);
  public:    Status _LineAndColumn(UInt &bufOff, UInt &lineNo, UInt &colNo);

  //
  // Protected members
  //
  
  protected: String  fileName;
  protected: Buffer  buffer;
  protected: UInt    numBytes;
  protected: UInt    numLines;
  protected: Buffer  lineOffsets;
  
}; // ...Class Lines

} // ...Namespace BEFE

#endif // ...LINES_H
