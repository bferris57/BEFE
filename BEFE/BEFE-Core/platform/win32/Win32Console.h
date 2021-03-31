//----------------------------------------------------------------------
// File: Win32Console.h - Declarations for the Win2Console Storage Class
//----------------------------------------------------------------------
//
// The Win2Console Class is responsible for implementing the abstract
// Console Class on the Windows32 platform.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef WIN32CONSOLE_H // WIN32CONSOLE_H...
#define WIN32CONSOLE_H

#include "Primitives.h"
#include "Storage.h"
#include "Console.h"

namespace BEFE { // Namespace BEFE...

//
// Crap we have to throw in from MSDN because MinGW doesn't declare it...
//

//
// Handy typedefs
//
typedef CONSOLE_SCREEN_BUFFER_INFO W32ConInfo;

//
// Class Win32Console...
//

class Win32Console : public Console { // Console Class...

  //
  // Public Class members
  //

  public: static Boolean outIsRedirected;
  public: static Boolean inIsRedirected;
  public: static Boolean errIsRedirected;

  public: static HANDLE stdout;
  public: static HANDLE stdin;     // <-- Just for now!!!!
  public: static HANDLE stderr;

  public: static UInt   oldConAttr;
  public: static UInt   oldConOutCp;
  public: static UInt   oldConInpCp;
  public: static UInt   oldForeground;
  public: static UInt   oldBackground;

  //
  // C++ Lifecycle
  //
  
  public:          Win32Console();
  public: virtual ~Win32Console();
  
  //
  // BEFE Lifecycle
  //

  public: virtual Status StartUp();
  public: virtual Status ShutDown();
  public:         Status _StartUp();
  public:         Status _ShutDown();
  
  //
  // Console Overrides
  //

  // Capabilities
  public: virtual Int     GetCapability();      // Get capabilities
  public: virtual Boolean IsInteractive();      // "Is Console in Interactive mode?"
  public: virtual Boolean InputIsRedirected();  // "Is Console input redirected?"
  public: virtual Boolean OutputIsRedirected(); // "Is Console output redirected?"

  // Base Gets/Puts (including overrides)
  public: virtual Status _GetBytes  (Byte *buf, UInt buflen, UInt &numread);
  public: virtual Status GetBytes   (Byte *buf, UInt buflen, UInt &numread);
  public: virtual Status _PutBytes  (Byte *buf, UInt buflen, UInt &numwritten);
  public: virtual Status PutBytes   (Byte *buf, UInt buflen, UInt &numwritten);
  public: virtual Status FlushOutput();

  // Line I/O
  public: virtual Status GetLine  (String &line);
  public: virtual Status PutLine  (String &line);

  // Cursor
  public: virtual Coord  GetCursor();         // Get current cursor position (x/y)

  // Foreground/Background colours
  public: virtual UInt GetForegroundColour();
  public: virtual UInt SetForegroundColour(UInt colour);
  public: virtual UInt GetBackgroundColour();
  public: virtual UInt SetBackgroundColour(UInt colour);

  // Other...
  public: virtual Status Clear();

  // Special internal use
  private: void   Warn(String const &message);
  private: void   Warn(char const *message);
	
	// Interactive Input
  private: Status _GetInteractiveLine(String &curLine); // ◄── Line Input
	
}; // ...Console Class

} // ...Namespace BEFE

#endif // ...WIN32CONSOLE_H
