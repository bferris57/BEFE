//!befe-publish inc
//----------------------------------------------------------------------
// File: Console.h - Declarations for Console class
//----------------------------------------------------------------------
//
// This Class is responsible for providing the default behariour of a
// "Console".  The underlying implementations of this class are
// provided by specific implementations of Console.
//
// The most important implementations of Console are provided by
// platform specific code.
//
// A Console is a sub-class of Stream.  As such, this layer of Console
// provides the basic Stream behaviour of a Console but with the
// underlying guts of the Class being provided by other implementations.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef CONSOLE_H // CONSOLE_H...
#define CONSOLE_H

#include "Primitives.h"
#include "Coord.h"
#include "Stream.h"

namespace BEFE { // Namespace BEFE...

class Console : public Stream { // Class Console...

  //
  // Console Colours
  //

  public: enum Colour {
     Black   = 0
    ,Red     = 1
    ,Green   = 2
    ,Blue    = 4
    ,Yellow  = Red | Green
    ,Magenta = Red |         Blue
    ,Cyan    =       Green | Blue
    ,White   = Red | Green | Blue
  };

  // Callback typedef...
	typedef Status (*GetLineCallback)(String &line);
	
  //
  // C++ Lifecycle
  //

  public: Console();
  public: virtual ~Console();

  //
  // BEFE Lifecycle
  //

  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public: virtual Status Reset();
  
  //
  // Stream overrides
  //

  // Capabilities
  public: virtual Int     GetCapability();      // Get capabilities
  public: virtual UInt    GetElementSize();     // Get current element size (in StorageUnits)
  public: virtual Status  SetElementSize(UInt elmsiz);// Set desired element size (in StorageUnits
  public: virtual UInt    Length();             // Total length of Stream (in Elements)
  public: virtual UInt    Size();               // Total size of Stream (in bytes)
  public: virtual Boolean IsInteractive();      // "Is Console running in interactive mode?"
  public: virtual Boolean InputIsRedirected();  // "Is Console input redirected?"
  public: virtual Boolean OutputIsRedirected(); // "Is Console output redirected?"

  // Positional Range
  public: virtual UInt    GetPos();             // Get current positional index (in element index)
  public: virtual Status  SetPos(UInt pos);     // Set current index (0..Length())
  public: virtual Range   PosRange();           // Get available positional range (-N..N)
  public: virtual Status  NextPos();            // Position to next element
  public: virtual Status  PreviousPos();        // Position to previous element
  public: virtual Status  RewindPos();          // Set to beginning
  public: virtual Status  EndPos();             // Set to end

  // Flushing
  public: virtual Status  Flush();              // Flush if buffered and putting
  
  // Cursor
  public: virtual Coord  GetCursor();           // Get current cursor position (x/y)

  // Base Buffering
  public: virtual UInt   GetBufferSize();
  public: virtual Status SetBufferSize(UInt size);

  // Gets/Puts
  public: virtual Status GetElement  (Byte *b);
  public: virtual Status GetByte     (Byte &b);
  public: virtual Status GetBytes    (Byte *buf, UInt buflen, UInt &numread);
  public: virtual Status PutElement  (Byte *b);
  public: virtual Status PutByte     (Byte  b);
  public: virtual Status PutBytes    (Byte *buf, UInt buflen, UInt &numwritten);
  public: virtual Status PutBytes    (String const &str);
  public: virtual Status PutBytes    (char const *buf);
  public: virtual Status FlushOutput ();
  
  // Base Get overrides...
  public: virtual Status _GetElement(Byte *b);
  public: virtual Status _GetByte   (Byte &b);
  public: virtual Status _GetBytes  (Byte *buf, UInt buflen, UInt &numread);

  // Base Put overrides...
  public: virtual Status _PutElement(Byte *b);
  public: virtual Status _PutByte   (Byte  b);
  public: virtual Status _PutBytes  (Byte *buf, UInt buflen, UInt &numwritten);
  public: virtual Status _PutBytes  (char const *buf);

  // Line I/O
  public: virtual Status GetLine  (String &line);
  public: virtual Status PutLine  (String &line);

  //
  // Additional Console device functions
  //

  // Foreground/Background colours
  public: virtual UInt GetForegroundColour();
  public: virtual UInt SetForegroundColour(UInt colour);
  public: virtual UInt GetBackgroundColour();
  public: virtual UInt SetBackgroundColour(UInt colour);

  // Other stuff...
  public: virtual Status Clear(); // Does a "clear screen"

  //
  // Protected members
  //

  public: Console *deferTo;
  public: Byte     lastByteOutput;    // Last byte output (for '\n' check)
  
}; // ...Class Console

} // ...Namespace BEFE

#endif // ...CONSOLE_H
