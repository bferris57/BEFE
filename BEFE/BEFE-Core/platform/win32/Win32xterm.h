//!befe-publish inc
//----------------------------------------------------------------------
// File: Win32xterm.h - Declarations for the Windows 32-bit xterm functions
//----------------------------------------------------------------------
//
// The Win32xterm class is responsible mainly for reading single lines
// from stdin/stdout on the Microsoft Windows Platform.
//
// Along with Linux32xterm, they're both the beginnings of an abstract
// terminal implementation, but we haven't abstracted enough of it yet
// for them to do anything other than handle line input.
//
// Still, they both work via a BEFE Callback mechanism (see BEFE.h,
// BEFE.cpp and CommandShell.cpp).
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef WIN32XTERM_H // WIN32XTERM_H...
#define WIN32XTERM_H

#include "Primitives.h"  // EVERYTHING NEEDS THIS!!!

namespace BEFE { // Namespace BEFE...

//
// Class Win32xterm...
//

struct Win32xterm { // Struct Win32xterm...

  // C++ Lifecycle
  Win32xterm();
  ~Win32xterm();
  
	// BEFE Lifecycle
  Status StartUp();
	Status ShutDown();
	Status Reset();     // ◀── Note: NOT same as "Reset Screen"
	Status Clear();     // ◀── Note: NOT same as "Clear Screen"
	
  // General purpose
  Boolean IsValidTerminal();
  Status  EnterRawMode();
  Status  ExitRawMode();

  // Cursor/Screen
  Coord   GetCursorPosition();
  Status  SetCursorPosition(Coord const &pos);
  Coord   GetLinePosCoord();
  Status  ScrollUpOne();
  
  // Query
  Coord   GetScreenSize();
  String  GetTerminalType();
  Status  ResetInput();
  Byte    GetByte();
  
	// Debugging and such...
	void   DebugOut(String const &msg);
	void   Beep();
  Status ClearScreen();
	
  // Input Line
	//   Implemented in Linux32xtermLine.cpp
  Status  GetLine(String const &prompt, String &theLine);
  Status  RedrawLine(Boolean clear);
  Status  DoKey(Char key);
	Status  DoAction(String const &action);
	
  //
  // Members
  //
  
  Int          lastError;

  // Screen stuff
  Coord        screenSize;            // Current Screen size
  Coord        screenPos;             // Current Screen cursor position
	
	// Input buffer
	Byte         inBuf[256];
	Int          inBufLen;
	Int          inBufPos;
  
	// Input Line stuff
	// 
	// Note: Except for history, all the other Line stuff is only meaningful
	//       while in GetLine()
	//

  Boolean      isRaw;                 // true = "Is in RAW mode"
	Int          oldMode;               // Original Console mode (before GetLine called)
  Strings      history;               // Line history
  UInt         histMax;               // Maximum history length (Null="Unlimited")
  UInt         histPos;               // History position (<0 and Null="Not Set")
  String       curLine;               // Current input line
  Int          linePos;               // Current input line logical cursor position
	Coord        lineStart;             // Current input line first Screen Coordinate
	
}; // ...Class Win32xterm

} // ...Namespace BEFE

#endif // ...WIN32XTERM_H