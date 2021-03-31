//----------------------------------------------------------------------
// File: Win32xterm.cpp - Win32xterm struct methods
//----------------------------------------------------------------------
//
// The Win32xterm struct is currently used as a context for terminal
// (or xterm) operations on a single line.  We implemented this along
// the lines of Linux32xterm and, hopefully, they'll pull together
// pretty soon into a port specific implementation of a more general
// Terminal Class.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "UTF8.h"
#include "Win32.h"
#include "Win32Console.h"
#include "Win32xterm.h"

#include "wincon.h"  // For Console functions

namespace BEFE { // Namespace BEFE...

//
// Handy typedefs
//

typedef CONSOLE_SCREEN_BUFFER_INFO ConInfo;
typedef INPUT_RECORD               InputRec;

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Win32xterm::Win32xterm()  { StartUp();  }
Win32xterm::~Win32xterm() {	ShutDown(); }

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Win32xterm::StartUp() {
		
	screenPos.x  = screenPos.y  = NaN;
	screenSize.x = screenSize.y = NaN;
	
	inBufLen = 0;
	inBufPos = 0;

  isRaw = false;
  history.StartUp();
  SetNull(histMax);
  histPos = 0;
  curLine.StartUp();

  return Error::None;
	
}

Status Win32xterm::ShutDown() {
 
	ExitRawMode();
  history.ShutDown();
  curLine.ShutDown();

  return Error::None;
	
}

Status Win32xterm::Reset() {

	Status retStatus;
	Status status;
	
	retStatus = ShutDown();
	status    = StartUp();
	if (status && !retStatus) retStatus = status;
	
	return retStatus;
	
}

Status Win32xterm::Clear() {

	Status status;
	
	status = Reset();
	inBufLen = 0;
	
	return status;
	  	
}

//----------------------------------------------------------------------
//
// General Purpose...
//

Boolean Win32xterm::IsValidTerminal() {	
	return (Cin.IsInteractive() && Cout.IsInteractive());
}

Status Win32xterm::EnterRawMode() {
	
	Status status;
	Int    ok;
	Int    newMode;
	
	if (isRaw) goto OK;
	
  ok = GetConsoleMode(Win32Console::stdout, (PDWORD)&oldMode);
  if (!ok) goto OSERROR;
  
  newMode = oldMode;
  newMode &= ~(ENABLE_ECHO_INPUT        |
               ENABLE_LINE_INPUT        |
               ENABLE_INSERT_MODE       |
               //ENABLE_PROCESSED_OUTPUT  |
               ENABLE_WRAP_AT_EOL_OUTPUT);
  //newMode &= ~(ENABLE_ECHO_INPUT | ENABLE_INSERT_MODE);
	ok = SetConsoleMode(Win32Console::stdout, newMode);
	if (!ok) goto OSERROR;

  isRaw = true;
	
  // Handle errors	
	status = Error::None;
	while (false) {
		OK:      status = Error::None;    break;
		OSERROR: status = Error::OSError; break;
	}

	return status;

}

Status Win32xterm::ExitRawMode() {
	
	Status status;
	Int    ok;
	
	if (!isRaw) goto OK;

  ok = SetConsoleMode(Win32Console::stdout, oldMode);
  if (!ok) goto OSERROR;

  isRaw = false;
	
  // Handle errors	
	status = Error::None;
	while (false) {
		OK:      status = Error::None;    break;
		OSERROR: status = Error::OSError; break;
	}

	return status;
	
}

//----------------------------------------------------------------------
//
// Cursor/Screen...
//

Coord Win32xterm::GetCursorPosition() {

	Coord   coord;
	ConInfo info;

  if (GetConsoleScreenBufferInfo(Win32Console::stdout, &info)) {
		coord.x = info.dwCursorPosition.X;
		coord.y = info.dwCursorPosition.Y;
		screenPos = coord;
	}
	
  return coord;
  
}

Status Win32xterm::SetCursorPosition(Coord const &pos) {
  
	Status status;
	COORD  winPos;
	
	winPos.X = pos.x;
	winPos.Y = pos.y;
	if (SetConsoleCursorPosition(Win32Console::stdout, winPos)) {
		screenPos = pos;
		status = Error::None;
	}
	else
		status = Error::OSError;
		
	return status;
	
}

Coord Win32xterm::GetLinePosCoord() {
  
  Coord theCoord;
  Int   midLines;
  
  theCoord = lineStart;
  
  if (linePos > (Int)curLine.Length())
    linePos = curLine.Length();
  else if (linePos < 0)
    linePos = 0;
    
  theCoord.x += linePos;

  // Adjust for wrap
  if (theCoord.x >= screenSize.x) {
    // Adjust for first line
    theCoord.x -= screenSize.x;
    theCoord.y++;
    // Adjust for midde lines...
    midLines = theCoord.x/screenSize.x;
    theCoord.y += midLines;
    theCoord.x -= screenSize.x * midLines;
  }
  
  return theCoord;
  
}

Status Win32xterm::ScrollUpOne() {
  
  Status status;
  CONSOLE_SCREEN_BUFFER_INFO sbInfo; 
  SMALL_RECT                 scroll;
  SMALL_RECT                 clip;
  CHAR_INFO                  fill;
  COORD                      dest; 
  Boolean                    ok;
  
  // Get the screen buffer info...
  ok = GetConsoleScreenBufferInfo(Win32Console::stdout, &sbInfo);
  if (!ok) goto OSERROR;
  
  // Form the scrollRect and clipRect...
  scroll.Top    = 1; 
  scroll.Bottom = sbInfo.dwSize.Y - 1; 
  scroll.Left   = 0; 
  scroll.Right  = sbInfo.dwSize.X - 1; 
  clip = scroll;
  
  // Form the dectination
  dest.X = 0; 
  dest.Y = 0; 
 
  // Fill the bottom row withblanks. 
  fill.Attributes     = sbInfo.wAttributes; 
  fill.Char.AsciiChar = (char)' '; 
 
  // Scroll up one line.
  ok = ScrollConsoleScreenBuffer(Win32Console::stdout, &scroll, &clip, dest, &fill);
  if (!ok) goto SOMEERROR;
  
  // Adjust screen and line positions...
  if (lineStart.y)
    lineStart.y--;
  if (screenPos.y)
    screenPos.y--;
  status = SetCursorPosition(screenPos);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    OSERROR:   status = Error::OSError; break;
    SOMEERROR:                          break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Query...
//

Coord Win32xterm::GetScreenSize() {

	Coord   coord;
	ConInfo info;

  if (GetConsoleScreenBufferInfo(Win32Console::stdout, &info)) {
		coord.x = info.dwSize.X;
		coord.y = info.dwSize.Y;
	}
	
  return coord;
  
}

//----------------------------------------------------------------------
//
// Debugging and such...
//

void Win32xterm::DebugOut(String const &msg) {

  Coord oldPos;

  if (gShowDebug)	{
	  oldPos = GetCursorPosition();
		SetCursorPosition(Coord(0,0));
		Cout << msg;
		SetCursorPosition(oldPos);
		Cout.Flush();
	}
	
	return;
	
}

void Win32xterm::Beep() {
  if (gConsoleBeep)
    (Cout << BEL).Flush();
}

Status Win32xterm::ClearScreen() {
	
	Status status;
	String cmd;
	
	cmd = GetEnvironmentValue("SYSTEMROOT")+"\\system32\\cmd.exe /c cls";
	
	status = _Invoke(cmd);
  if (!status)
		screenPos.x = screenPos.y = 0;

	return status;
	
}

//----------------------------------------------------------------------
//
// Method: Win32xterm.GetLine
//

Status Win32xterm::GetLine(String const &prompt, String &theLine) { // Win32xterm.GetLine...

  Status   status;
  Int      ok;
  Int      numevents;
  Int      numread;
  String   uchar;
  Boolean  gotit;
  Char     utf;
  Byte     utfbuf[4];
  UInt     utfbufl;
  String   msg;
  InputRec input;
  String   action;
	UInt     ctrlKeys;
	
	ctrlKeys = LEFT_ALT_PRESSED + LEFT_CTRL_PRESSED + RIGHT_ALT_PRESSED + RIGHT_CTRL_PRESSED;
	
  ok = FlushConsoleInputBuffer(Win32Console::stdin);
  status = EnterRawMode();
  
  curLine.Reset();
  SetNull(histPos);

  // Prompt if we're supposed to
  if (prompt.Length()) {
    Cout << prompt;
    Cout.Flush();
  }
	
  // Get Window Size and Position...
  screenSize = GetScreenSize();
  screenPos  = GetCursorPosition();
  
	// Initialise line stuff...
	curLine   = String();
	linePos   = 0;
	lineStart = screenPos;
  histPos   = 0;
  
  // Push an empty line (if we're doing history)...
  if (!IsNull(gConsoleHistory) && gConsoleHistory > 1) {
    if (history.Length() == 0) {
      status = history.Append(curLine);
      if (status) goto SOMEERROR;
    }
    while (history.Length() >= gConsoleHistory) {
      status = history.Remove(-1);
      if (status) goto SOMEERROR;
    }
    status = history.Insert(0, curLine);
    if (status) goto SOMEERROR;
  }
  

  //
	// Until we get an EOS...
	//
	
  do {

    // Wait until there's something, anything...
    ok = GetNumberOfConsoleInputEvents(Win32Console::stdin,(LPDWORD)&numevents);
    if (!ok) goto GETEVENTSFAILED;
    if (numevents == 0) {
      ok = WaitForSingleObject(Win32Console::stdin,2000);
      if ((UInt)ok == WAIT_FAILED) goto WAITFAILED;
      if (ok == WAIT_TIMEOUT) {
        //*this << '.';
        continue;
      if (ok != WAIT_OBJECT_0) goto WAITFAILED;
      }
      if (numevents == 0) continue;
    }

    // Have something... Read and handle until they're exhausted...
    gotit = false;

    while (numevents) {

      // Get the event
      ok = ReadConsoleInputW(Win32Console::stdin,&input,1,(LPDWORD)&numread);
      if (!ok) goto READFAILED;
      if (numread == 0) break;
      numevents--;

      if (input.EventType != KEY_EVENT ||
          !input.Event.KeyEvent.bKeyDown) {
        continue;
      }

      // Handle special keys...
			action.Reset();
			if (!(input.Event.KeyEvent.dwControlKeyState & ctrlKeys)) {
				if      (input.Event.KeyEvent.wVirtualKeyCode == VK_LEFT)   action = "Left";
				else if (input.Event.KeyEvent.wVirtualKeyCode == VK_RIGHT)  action = "Right";
				else if (input.Event.KeyEvent.wVirtualKeyCode == VK_UP)     action = "Up";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_DOWN)   action = "Down";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_HOME)   action = "Home";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_END)    action = "End";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_PRIOR)  action = "PgUp";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_NEXT)   action = "PgDn";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_INSERT) action = "Ins";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_DELETE) action = "Del";
  			else if (input.Event.KeyEvent.wVirtualKeyCode == VK_BACK)   action = "BS";
			}
			if (!action.IsNull()) {
			  status = DoAction(action);
				if (status) goto SOMEERROR;
				continue;
			}
			
			// Handle normal keys...
      //   Ignore if it's not Unicode
      utf = (UInt)input.Event.KeyEvent.uChar.UnicodeChar;
      if (utf == 0u) continue;

      // Get it in UTF-8...
      status = UTF8::EncodeChar(Char(utf),utfbuf,utfbufl);
      if (status) goto ENCODEFAILED;

      // Finished if it's '\r'...
			if (utf == '\r') {
				gotit = true;
				(Cout << "\n").Flush();
				break;
			}
			
      // Do the key...
      status = DoKey(utf);
      if (status) goto SOMEERROR;
			
    } // Main event loop

    if (gotit) break;

  } while (true); // Main loop

  // Update history
  if (!IsNull(gConsoleHistory) && gConsoleHistory > 1) {
    if (history.Length())
      status = history.Set(0, curLine);
    else
      status = history.Append(curLine);
    if (status) goto SOMEERROR;
  }
  
  //
  // Handle errors
  //
  
  status = Error::None;
  while (false) {
    GETEVENTSFAILED: msg = "GetNumberOfConsoleInputEvents failed"; break;
    WAITFAILED:      msg = "WaitForSingleObject failed";           break;
    READFAILED:      msg = "ReadConsoleInputW failed";             break;
    SOMEERROR:                                                     break;
    ENCODEFAILED:    Cout << '\n';
                     Cout << ErrorString(status) << '\n';
                     msg = "UTF8::Encode Failed";
                     break;
  }

  // If error, display it if we've got a message
  if (status && msg) {
    Cout << '\n';
    Cout << msg;
    Cout << '\n';
  }  
		
  // Finish up
  if (!status)
    status = theLine.CopyFrom(curLine);

  if (!IsNull(gConsoleHistory) && gConsoleHistory > 1) {
    if (IsNull(curLine.FirstNonWhite()))
      history.Remove(0);
    else
      history.Set(0, curLine);
  }

  ExitRawMode();
  
  return status;
 	
} // ...Win32xterm.GetLine

//----------------------------------------------------------------------
//
// Method: Win32xterm.RedrawLine
//

Status Win32xterm::RedrawLine(Boolean clear) { // Win32xterm.RedrawLine...
	
	Status status;
  Int    tLinePos;
	Int    remInRow;
  String eraseEOS;
  
	// Position to beginning of line and erase it...
	status = SetCursorPosition(lineStart);
	if (status) goto SOMEERROR;
  if (!clear) {
    status = DoAction("EraseToEOL");
    if (status) goto SOMEERROR;	
  }
  
	// Until we're finished...
	tLinePos = 0;
	while (tLinePos < (Int)curLine.Length()) {
    if (tLinePos == 0)
  		remInRow = screenSize.x - screenPos.x;
    else
      remInRow = screenSize.x;
    remInRow = Min((Int)(curLine.Length() - tLinePos), remInRow);
    if (!remInRow) break;
    if (clear)
      Cout << String(' ')*screenSize.x;
    else
  	  Cout << curLine.Get(Span(tLinePos,tLinePos+remInRow));
    screenPos.x += remInRow;
		tLinePos += remInRow;
		if (screenPos.x < screenSize.x)
  	  Cout << String(' ')*(screenSize.x - screenPos.x);			
		Cout.Flush();
    if (screenPos.x >= screenSize.x) {
      screenPos.x = 0;
      screenPos.y++;
      if (screenPos.y >= screenSize.y) {
        status = ScrollUpOne();
        if (status) goto SOMEERROR;
      }
    }
    status = SetCursorPosition(screenPos);
    if (status) goto SOMEERROR;
	}
	
  // Reposition where we are in line...
  status = SetCursorPosition(GetLinePosCoord());
  if (status) goto SOMEERROR;
  
	// Handle errors
	status = Error::None;
	while (false) {
		SOMEERROR:                                          break;
	}
	
	return status;

} // ...Win32xterm.RedrawLine
	
//----------------------------------------------------------------------
//
// Method: Win32xterm.DoKey
//

Status Win32xterm::DoKey(Char key) {
	
	Status  status;
  Coord   before;
  Coord   after;
	
	// Handle BS...
	if (key == '\b') {
		status = DoAction("BS");
		if (status) goto SOMEERROR;
		goto OK;
	}
	
	// Normal key...
  // Insert if not at end, otherwise just append to end...
  before = GetLinePosCoord();
  if (linePos < (Int)curLine.Length()) {
    status = curLine.Insert(linePos, key);
    if (status) goto SOMEERROR;
  }
  else {
		status = curLine.Append(key);
		if (status) goto SOMEERROR;
  }	
  
  // Adjust the position...
  linePos++;
  after = GetLinePosCoord();
  
  histPos = 0;

  // Redraw the line
  if (linePos != (Int)curLine.Length()) {
    status = RedrawLine(false);
    if (status) goto SOMEERROR;
  }
  else {
    Cout << key;
    Cout.Flush();
  }
  
	// Handle errors
	status = Error::None;
	while (false) {
		OK:        status = Error::None; break;
		SOMEERROR:                       break;
	}
	
	return status;

}

//----------------------------------------------------------------------
//
// Method: Win32xterm.DoAction
//

Status Win32xterm::DoAction(String const &action) {
	
	Status status;
  String emptyLine;
	Coord  before;
	Coord  after;
	Coord  tPos;
	
  // Handle BS...
	if (action == "BS") {
    tPos = GetCursorPosition();
		if (linePos > 0) { // Need to delete to the left and do a "Del"...
      if (linePos >= (Int)curLine.Length())
        linePos = curLine.Length();
	    linePos--;
      status = curLine.Remove(linePos);
      if (status) goto SOMEERROR;        
      status = RedrawLine(false);
      if (status) goto SOMEERROR; 
		}
    else
      Beep();
    histPos = 0;
	}
	
	// Handle EraseToEOL...
	else if (action == "EraseToEOL") {
		before = screenPos;
		emptyLine = String(' ') * (screenSize.x - screenPos.x);
		(Cout << emptyLine).Flush();
		screenPos.x += emptyLine.Length();
		// Reposition to current Position
		status = SetCursorPosition(before);
		if (status) goto SOMEERROR;
	}
	
  // Left - Move 1 character to the left
  else if (action == "Left") {
    if (linePos) {
      linePos--;
      status = RedrawLine(false);
      if (status) goto SOMEERROR; 
    }
    else
      Beep();
    histPos = 0;
  }
  
  // Handle ScrollUp...
  else if (action == "ScrollUp") {
    before = screenPos;
    status = ScrollUpOne();
    if (status) goto SOMEERROR;
    if (before.y)
      before.y--;
    status = SetCursorPosition(before);
    if (status) goto SOMEERROR;
    if (lineStart.y)
      lineStart.y--;
  }
  
  // Right  - Move 1 character to the right
  else if (action == "Right") {
    if (linePos < (Int)curLine.Length()) {
      linePos++;
      status = RedrawLine(false);
      if (status) goto SOMEERROR; 
    }
    else
      Beep();
    histPos = 0;
  }
  
  // Up     - History Up
  else if (action == "Up") {

    // Only if handle if we're doing history...
    if (IsNull(gConsoleHistory) || gConsoleHistory < 1) { Beep(); goto OK; }
    
    // Update the history with current line if we're at bottom line...
    if (histPos == 0) {
      status = history.Set(0, curLine);
      if (status) goto SOMEERROR;
    }
    
    // Clear current line first...
    status = RedrawLine(true);
    if (status) goto SOMEERROR;
    
    // If past max, nothing to do...
    if (histPos+1 >= history.Length()) { Beep(); goto OK; }

    // Get the line...
    histPos++;
    curLine = history.Get(histPos);
    
    // Redraw it...
    linePos = curLine.Length();
    status = RedrawLine(false);
    if (status) goto SOMEERROR;
    
  }
  
  // Down   - History Down
  else if (action == "Down") {

    // Only if handle if we're doing history...
    if (IsNull(gConsoleHistory) || gConsoleHistory < 1) { Beep(); goto OK; }
    
    // Update the history with current line if we're at bottom line...
    if (histPos == 0) {
      status = history.Set(0, curLine);
      if (status) goto SOMEERROR;
    }
   
    // Do it... 
    if (histPos > 0) {

      // Clear current line first...
      status = RedrawLine(true);
      if (status) goto SOMEERROR;
    
      // Get the line...
      --histPos;
      curLine = history.Get(histPos);
      linePos = curLine.Length();
    
      // Redraw it...
      status = RedrawLine(false);
      if (status) goto SOMEERROR;
        
    }
    else
      Beep();
  }
  
  // Home   - Beginning of Line
  else if (action == "Home") {
		linePos = 0;
		status = RedrawLine(false);
		if (status) goto SOMEERROR;
    histPos = 0;
  }
  
  // End    - End of Line
  else if (action == "End") {
		linePos = curLine.Length();
		status = RedrawLine(false);
		if (status) goto SOMEERROR;
    histPos = 0;
    histPos = 0;
  }
	
  // Ins - Insert
  else if (action == "Ins") {
    if (linePos < (Int)curLine.Length()) {
      status = curLine.Insert(linePos, ' ');
      if (status) goto SOMEERROR;
      status = RedrawLine(false);
      if (status) goto SOMEERROR;
    }
    histPos = 0;
  }
  
  // Del    - Del key
  else if (action == "Del") {
    if (linePos < (Int)curLine.Length()) {
      curLine.Remove(linePos);
      status = RedrawLine(false);
      if (status) goto SOMEERROR; 
    }
    else
      Beep();
    histPos = 0;
  }
  	
	// Unknown action
	else {
  	Beep();
	}
	
	// Handle errors
	status = Error::None;
	while (false) {
		OK:        status = Error::None; break;
		SOMEERROR:                       break;
	}
	
	return status;
	
}

} // ...namespace BEFE