//----------------------------------------------------------------------
// File: Console.cpp - Base implementation of the Console class
//----------------------------------------------------------------------
//
// This is the basic implementation of the Console class.
//
// This should be specialised by each specific Platform implementation.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "_GlobalMacros.h"
#include "win32.h"
#include "Win32Console.h"
#include "UTF8.h"
#include "ToString.h"

#include "wincon.h"  // For Console functions

namespace BEFE { // Namespace BEFE...

//
// External functions
//

String ErrorString(Status error);

//----------------------------------------------------------------------
//
// Static Class Members
//

//Boolean Win32Console::outIsRedirected = true;
Boolean Win32Console::inIsRedirected  = true;
Boolean Win32Console::outIsRedirected = true;
Boolean Win32Console::errIsRedirected = true;

HANDLE  Win32Console::stdout = NULL;
HANDLE  Win32Console::stdin  = NULL;
HANDLE  Win32Console::stderr = NULL;

UInt    Win32Console::oldConAttr;
UInt    Win32Console::oldConOutCp;
UInt    Win32Console::oldConInpCp;

//----------------------------------------------------------------------
//
// Windows Console Control Handler
//

extern void Exit(Status status);

BOOL WINAPI Win32ConsoleControlHandler(DWORD CEvent) {
  if (!gQuiet) {
    CancelIo(Win32Console::stdout);
    TheBefe->TheConsole.SetForegroundColour(Console::Yellow);
    TheBefe->TheConsole << "\n\n***Canceled by user request, aborting***\n";
  }
  SetConsoleTextAttribute(Win32Console::stdout,Win32Console::oldConAttr);
  SetConsoleOutputCP(Win32Console::oldConOutCp);
  SetConsoleCP(Win32Console::oldConInpCp);
  TerminateProcess(GetCurrentProcess(),-1);
  return TRUE;
}

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Win32Console::Win32Console() {
  _StartUp();
}

Win32Console::~Win32Console() {
  _ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Win32Console::StartUp() {
  return _StartUp();
}

Status Win32Console::ShutDown() {
  return _ShutDown();
}

Status Win32Console::_StartUp() {

  CONSOLE_SCREEN_BUFFER_INFO oldinfo;
  Int                        newattr;
  Int                        newcp;
  Int                        mode;
	
  // If no vtable, make one...
  if (*(UInt *)this == 0) {
    Win32Console x;
    *(Int32 **)this = *(Int32 **)&x;
  }

  // Initialise TheConsole's deferTo...
  TheBefe->TheConsole.deferTo = this;

  // Set the Windows Console Control Handler
  SetConsoleCtrlHandler( (PHANDLER_ROUTINE)Win32ConsoleControlHandler,TRUE);
  
  // Get the standard output handle
  stdout = GetStdHandle(STD_OUTPUT_HANDLE);
  stdin  = GetStdHandle(STD_INPUT_HANDLE);
  stderr = GetStdHandle(STD_ERROR_HANDLE);

  // Get the current buffer info
  oldinfo.wAttributes = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
  GetConsoleScreenBufferInfo(stdout,&oldinfo);
  oldConAttr = oldinfo.wAttributes;
  oldConOutCp = GetConsoleOutputCP();
  oldConInpCp = GetConsoleCP();
  // Set the new attributes...
  newattr = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
  newcp   = 65001;
  SetConsoleTextAttribute(stdout,newattr);
  SetConsoleOutputCP(newcp);
  SetConsoleCP(newcp);

  Console::StartUp();

  // See if they're redirected
  outIsRedirected  = !GetConsoleMode(stdout,(LPDWORD)&mode);
  inIsRedirected   = !GetConsoleMode(stdin,(LPDWORD)&mode);
  errIsRedirected  = !GetConsoleMode(stderr,(LPDWORD)&mode);
	
  return Error::None;

}

Status Win32Console::_ShutDown() {

  // Restore original console settings...
  if (this == TheBefe->TheConsole.deferTo && stdout) {
    SetConsoleTextAttribute(stdout,oldConAttr);
    SetConsoleOutputCP(oldConOutCp);
    SetConsoleCP(oldConInpCp);
    stdout = NULL;
  }

  return Error::None;

}

//----------------------------------------------------------------------
//
// Console Overrides
//

Int Win32Console::GetCapability() {

  Int answer;

  answer =  Stream::FixedElementSize |
            Stream::CanPut           |
            Stream::CanGet           |
            Stream::CanFlush         |
            Stream::IsFixedLength    |
            Stream::IsUTF8           |
            Stream::IsBigEndian;

  if (!inIsRedirected && !outIsRedirected)
    answer |= Stream::IsInteractive;

  return answer;

}

Boolean Win32Console::IsInteractive() {

  return (!inIsRedirected && !outIsRedirected);

}

Boolean Win32Console::InputIsRedirected() {
  return inIsRedirected;
}

Boolean Win32Console::OutputIsRedirected() {
  return outIsRedirected;
}

Status Win32Console::_GetBytes(Byte *buf, UInt bufLen, UInt &numRead) {

	Status status;
	Int    ok;
	
  // Read the bytes
  ok = ReadFile((HANDLE)stdin, (void *)buf, bufLen, (DWORD *)&numRead, NULL);
  if (ok == 0) goto READFAILED;

  // Make sure we're not at the end
  if (numRead == 0) goto ATEND;

  // Handle errors
	status = Error::None;
	while (false) {
		READFAILED: status = Error::OSError;     break;
    ATEND:      status = Error::StreamAtEnd; break;
	}
	
	return status;
	
}

Status Win32Console::GetBytes(Byte *buf, UInt bufLen, UInt &numRead) {

	Status status;
	Int    ok;
	
  // Read the bytes
  ok = ReadFile((HANDLE)stdin, (void *)buf, bufLen, (DWORD *)&numRead, NULL);
  if (ok == 0) goto READFAILED;

  // Make sure we're not at the end
  if (numRead == 0) goto ATEND;

  // Handle errors
	status = Error::None;
	while (false) {
		READFAILED: status = Error::OSError;     break;
    ATEND:      status = Error::StreamAtEnd; break;
	}
	
	return status;
	
}

Status Win32Console::_PutBytes(Byte *buf, UInt buflen, UInt &numwritten) {

  Int status;
  Int numout;

  numout = 0;
  status = WriteFile(stdout,buf,buflen,(LPDWORD)&numout,NULL);
  // For some odd reason, WriteFile puts one in numout if the buffer
  // contains UTF-8, Hmmm, so we'll set numout to buflen if status OK...
  if (status)
    numout = buflen;
    
  numwritten = numout;
  
  return Error::None;

}

Status Win32Console::FlushOutput() {

	FlushFileBuffers(stdout);

	return Error::None;

}

Status Win32Console::PutBytes(Byte *buf, UInt buflen, UInt &numwritten) {
  return _PutBytes(buf, buflen, numwritten);
}

//----------------------------------------------------------------------
//
// Line I/O
//

Status Win32Console::GetLine(String &line) {
  
  Status status;
  Char   theChar;
  Byte   buf;
  UInt   numRead;
  
  // If input/output not redirected, do a proper one...
  if (!inIsRedirected && !outIsRedirected) {
    status = _GetInteractiveLine(line);
    if (status && status != Error::NotCapable) goto SOMEERROR;
		if (!status) goto OK;
  }
  
  // It's not "proper" keyboard input...
  line.Reset();
  while (true) {
    status = GetBytes(&buf, 1, numRead);
    if (status && status != Error::StreamAtEnd) goto SOMEERROR;
    if (numRead == 0 || buf == '\n') break;
    status = line.Append(&buf, 1);
    if (status) goto SOMEERROR;
  }
  
  // Remove bogus Windows '\r' because we DO NOT WANT THAT CRAP!!!
  if (line[-1] == '\r') {
   status = line.Remove(-1);
   if (status) goto SOMEERROR;
  }

  // Handle end of input
  if (numRead == 0 && line.Length() == 0) goto ATEND;

  // Handle errors
  status = Error::None;
  while (false) {
		OK:        status = Error::None;        break;
    ATEND:     status = Error::StreamAtEnd; break;
    SOMEERROR:                              break;
  }

  return status;

}

Status Win32Console::PutLine(String &line) {

  Status status;
  Byte   *buf;
  UInt    bufSize;
  UInt    numout;
  
  line._BufAndSize(buf,bufSize);
  status = PutBytes(buf,bufSize,numout);
  if (status) goto SOMEERROR;
  status = PutByte(0x0a);
  if (status) goto SOMEERROR;

  goto OK;

SOMEERROR:
  goto DONE;
OK:
  status = Error::None;
  goto DONE;

DONE:

  return status;

}

// Cursor
Coord Win32Console::GetCursor() {

  Coord answer;
  Int   status;

  CONSOLE_SCREEN_BUFFER_INFO info;

  answer.x = NaN;
  answer.y = NaN;
  if (!TheBefe->TheConsole.deferTo) goto DONE;

  status = GetConsoleScreenBufferInfo(stdout,&info);
  if (!status) goto DONE;

  answer.x = info.dwCursorPosition.X;
  answer.y = info.dwCursorPosition.Y;

DONE:

  return answer;

}

// Foreground/Background colours
UInt Win32Console::GetForegroundColour() {

  UInt colour;

  CONSOLE_SCREEN_BUFFER_INFO binfo;

  // Get the current buffer info
  if (!GetConsoleScreenBufferInfo(stdout,&binfo)) {
    colour = Console::White;
    goto DONE;
  }

  // Translate to our colours...
  colour = Console::Black;
  if (binfo.wAttributes & FOREGROUND_RED)
    colour |= Console::Red;
  if (binfo.wAttributes & FOREGROUND_GREEN)
    colour |= Console::Green;
  if (binfo.wAttributes & FOREGROUND_BLUE)
    colour |= Console::Blue;

DONE:

  return colour;

}

UInt Win32Console::SetForegroundColour(UInt colour) {

  UInt oldcolour;

  CONSOLE_SCREEN_BUFFER_INFO binfo;

  // Get the current buffer info
  if (!GetConsoleScreenBufferInfo(stdout,&binfo)) {
    oldcolour = Console::White;
    goto DONE;
  }

  // Translate to our colours...
  oldcolour = Console::Black;
  if (binfo.wAttributes & FOREGROUND_RED)
    oldcolour |= Console::Red;
  if (binfo.wAttributes & FOREGROUND_GREEN)
    oldcolour |= Console::Green;
  if (binfo.wAttributes & FOREGROUND_BLUE)
    oldcolour |= Console::Blue;

  // If new colour is invalid, don't do anything...
  if (oldcolour < 0 || oldcolour > Console::White) goto DONE;

  // Form and set the new attributes...
  binfo.wAttributes &= ~(FOREGROUND_INTENSITY|BACKGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);
  if (colour & Console::Red)
    binfo.wAttributes |= FOREGROUND_RED;
  if (colour & Console::Green)
    binfo.wAttributes |= FOREGROUND_GREEN;
  if (colour & Console::Blue)
    binfo.wAttributes |= FOREGROUND_BLUE;
  if (gConsoleIntensity) {
    binfo.wAttributes |= FOREGROUND_INTENSITY;
    if ((binfo.wAttributes & (BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE)) != 0)
      binfo.wAttributes |= BACKGROUND_INTENSITY;
  }

  if (gConsoleColour)
    SetConsoleTextAttribute(stdout,binfo.wAttributes);

DONE:

  return oldcolour;

}

UInt Win32Console::GetBackgroundColour() {

  UInt colour;

  CONSOLE_SCREEN_BUFFER_INFO binfo;

  // Get the current buffer info
  if (!GetConsoleScreenBufferInfo(stdout,&binfo)) {
    colour = Console::Black;
    goto DONE;
  }

  // Translate to our colours...
  colour = Console::Black;
  if (binfo.wAttributes & BACKGROUND_RED)
    colour |= Console::Red;
  if (binfo.wAttributes & BACKGROUND_GREEN)
    colour |= Console::Green;
  if (binfo.wAttributes & BACKGROUND_BLUE)
    colour |= Console::Blue;

DONE:

  return colour;

}

UInt Win32Console::SetBackgroundColour(UInt colour) {

  UInt oldcolour;

  CONSOLE_SCREEN_BUFFER_INFO binfo;

  if (!stdout)
    stdout = GetStdHandle(STD_OUTPUT_HANDLE);

  // Get the current buffer info
  if (!GetConsoleScreenBufferInfo(stdout,&binfo)) {
    oldcolour = Console::White;
    goto DONE;
  }

  // Translate to our colours...
  oldcolour = Console::Black;
  if (binfo.wAttributes & BACKGROUND_RED)
    oldcolour |= Console::Red;
  if (binfo.wAttributes & BACKGROUND_GREEN)
    oldcolour |= Console::Green;
  if (binfo.wAttributes & BACKGROUND_BLUE)
    oldcolour |= Console::Blue;

  // If new colour is invalid, don't do anything...
  if (oldcolour < 0 || oldcolour > Console::White) goto DONE;

  // Form and set the new attributes...
  binfo.wAttributes &= ~(BACKGROUND_INTENSITY|BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE);
  if (colour & Console::Red)
    binfo.wAttributes |= BACKGROUND_RED;
  if (colour & Console::Green)
    binfo.wAttributes |= BACKGROUND_GREEN;
  if (colour & Console::Blue)
    binfo.wAttributes |= BACKGROUND_BLUE;
  if (gConsoleIntensity && (binfo.wAttributes & (BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE)) != 0)
      binfo.wAttributes |= BACKGROUND_INTENSITY;

  if (gConsoleColour)
    SetConsoleTextAttribute(stdout,binfo.wAttributes);

DONE:

  return oldcolour;

}

Status Win32Console::Clear() {

	Status                     status;
	HWND                       conWindow;
  COORD                      home = { 0, 0 };
	DWORD                      numWritten;
  DWORD                      conSize;
  CONSOLE_SCREEN_BUFFER_INFO info;

  // Sanity checks...
  conWindow = GetConsoleWindow();
  if (!conWindow) goto NOTCAP;
  
  // Do it...
  if (!GetConsoleScreenBufferInfo( stdout, &info ))                                        goto INTERNAL;
  conSize = info.dwSize.X * info.dwSize.Y;
  if (!FillConsoleOutputCharacter( stdout, (TCHAR) ' ', conSize, home, &numWritten ))      goto INTERNAL;
  if(!GetConsoleScreenBufferInfo( stdout, &info ))                                         goto INTERNAL;
  if (!FillConsoleOutputAttribute( stdout, info.wAttributes, conSize, home, &numWritten )) goto INTERNAL;
  if (!SetConsoleCursorPosition( stdout, home ))                                           goto INTERNAL;
  
  // Handle errors...
  status = Error::None;
  while (false) {
  	NOTCAP:   status = Error::NotCapable; break;
		INTERNAL: status = Error::None;   break;
		//INTERNAL: status = Error::Internal;   break;
  }

	return status;
  
}


/*
void cls( HANDLE hConsole )
{
   COORD coordScreen = { 0, 0 };    // home for the cursor
   DWORD cCharsWritten;
   CONSOLE_SCREEN_BUFFER_INFO csbi;
   DWORD dwConSize;

   // Get the number of character cells in the current buffer.

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
   {
      return;
   }

   dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

   // Fill the entire screen with blanks.

   if( !FillConsoleOutputCharacter( hConsole,        // Handle to console screen buffer
                                    (TCHAR) ' ',     // Character to write to the buffer
                                    dwConSize,       // Number of cells to write
                                    coordScreen,     // Coordinates of first cell
                                    &cCharsWritten ))// Receive number of characters written
   {
      return;
   }

   // Get the current text attribute.

   if( !GetConsoleScreenBufferInfo( hConsole, &csbi ))
   {
      return;
   }

   // Set the buffer's attributes accordingly.

   if( !FillConsoleOutputAttribute( hConsole,         // Handle to console screen buffer
                                    csbi.wAttributes, // Character attributes to use
                                    dwConSize,        // Number of cells to set attribute
                                    coordScreen,      // Coordinates of first cell
                                    &cCharsWritten )) // Receive number of characters written
   {
      return;
   }

   // Put the cursor at its home coordinates.

   SetConsoleCursorPosition( hConsole, coordScreen );
}

}*/

//----------------------------------------------------------------------
//
// Special internal private use
//

void Win32Console::Warn(char const *message) {

  Int oldcolour;

  if (gShowWarnings) {
    oldcolour = SetForegroundColour(Console::Yellow);
    if (GetCursor().x != 0)
      *this << '\n';
    if (IsNull(message))
      *this << "!!! WARNING: No reason given\n";
    else
      *this << "!!! WARNING: " << message << '\n';
    SetForegroundColour(oldcolour);
  }

}

//----------------------------------------------------------------------
//
// Method: _GetInteractiveLine - Get a proper keyboard/crt line from user
//
// Note:   This method is specially designed to handle "proper" keyboard
//         and Xterm based user input lines.
//
//         As such, it handles the Home, End, PageUp, PageDown, UpArrow,
//         DownArrow, LeftArrow, and RightArrow keys (and some others)
//         as a sane person would expect if they're used to using a
//         "proper shell".
//

Status Win32Console::_GetInteractiveLine(String &line) {
	
	Status          status;
	GetLineCallback cb;
	
	status = FlushOutput();
	if (status) goto BAD;

	while (true) {
		status = TheBefe->FindCallback("BEFE.Console.GetLine", (void **)&cb);
		if (status)
			status = Error::NotCapable;
		else {
			status = (*cb)(line);
			if (status) break;
			// Ignore bogus Eclipse/GDB bugs...
			if (gShowDebug) {
				static char const *frags[] = {"-thread","stopped,reason",0};
				int i;
				for (i=0;frags[i];i++) {
					if (!IsNull(line.Find(frags[i])))
						break;
				}
				if (frags[i]) {
					Cout << "Ignoring Eclipse/gdb: " << line << '\n';
					continue;
				}
			}
		}
		break;
	}


	// Handle errors...
	while (false) {
		BAD: break;
	}

  return status;
	
}

} // ...Namespace BEFE
