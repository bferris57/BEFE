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
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------

#include "BEFE.h"
#include "Console.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Console::Console() {
  StartUp();
}


Console::~Console() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Console::StartUp() {
  deferTo        = NULL;
  lastByteOutput = 0x00;
  return Error::None;
}

Status Console::ShutDown() {
  Status status;
  if (deferTo)
    status = deferTo->ShutDown();  
  else
    status = Error::None;
  return status;
}

Status Console::Reset() {
  
  return Error::None;
  
}

//----------------------------------------------------------------------
//
// Stream overrides
//

// Capabilities
Int Console::GetCapability() {
  if (deferTo)
    return deferTo->GetCapability();
  return 0;
}

UInt Console::GetElementSize() {
  if (deferTo)
    return deferTo->GetElementSize();
  return 0;
}

Status Console::SetElementSize(UInt elmsiz) {
  if (deferTo)
    return deferTo->SetElementSize(elmsiz);
  return Error::NotCapable;
}

UInt Console::Length() {
  if (deferTo)
    return deferTo->Length();
  return 0;
}

UInt Console::Size() {
  if (deferTo)
    return deferTo->Size();
  return 0;
}

Boolean Console::IsInteractive() {
  if (deferTo)
    return deferTo->IsInteractive();
  return false;
}

Boolean Console::InputIsRedirected() {
  if (deferTo)
    return deferTo->InputIsRedirected();
  return true;
}

Boolean Console::OutputIsRedirected() {
  if (deferTo)
    return deferTo->OutputIsRedirected();
  return true;
}

// Positional Range
UInt Console::GetPos() {
  return 0;
}

Status Console::SetPos(UInt pos) {
  return Error::NotCapable;
}

Range Console::PosRange() {
  return Range(0,-1);
}

Status Console::NextPos() {
  return Error::NotCapable;
}

Status Console::PreviousPos() {
  return Error::NotCapable;
}

Status Console::RewindPos() {
  return Error::NotCapable;
}

Status Console::EndPos() {
  return Error::NotCapable;
}

// Flushing
Status Console::Flush() {
  if (deferTo)
    return deferTo->Flush();
  return Error::NotCapable;
}

// Cursor
Coord Console::GetCursor() {
  Coord result;
  if (deferTo && !deferTo->OutputIsRedirected())
    return deferTo->GetCursor();
  result.x = (lastByteOutput == '\n') ? 0 : NaN;
  result.y = NaN;
  return result;
}

// Base Buffering
UInt Console::GetBufferSize() {
  if (deferTo)
    return deferTo->GetBufferSize();
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::SetBufferSize(UInt size) {
  if (deferTo)
    return deferTo->SetBufferSize(size);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

//----------------------------------------------------------------------
//
// Exposed Gets/Puts overrides...
//

Status Console::GetElement(Byte *b) {
  if (deferTo)
    return deferTo->GetElement(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::GetByte(Byte &b) {
  if (deferTo)
    return deferTo->GetByte(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::GetBytes(Byte *buf, UInt buflen, UInt &numread) {
  if (deferTo)
    return deferTo->GetBytes(buf, buflen, numread);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::PutElement(Byte *b) {
  if (deferTo)
    return deferTo->PutElement(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::PutByte(Byte b) {
  if (deferTo)
    return deferTo->PutByte(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::PutBytes(Byte *buf, UInt buflen, UInt &numwritten) {

  if (deferTo) {
    if (buf && buflen)
      lastByteOutput = *(buf+buflen-1);
    else
      lastByteOutput = 0x00;
    return deferTo->PutBytes(buf, buflen, numwritten);
  }
  if (!TheBefe)
    return Error::NoBefe;

  return Error::NotCapable;

}

Status Console::PutBytes(String const &str) {
  
  Status  status;
  Byte   *buf;
  UInt    bufLen;
  UInt    numWritten;
  
  str._BufAndSize(buf, bufLen);
  status = PutBytes(buf, bufLen, numWritten);

  return status;
  
}

Status Console::PutBytes(char const *buf) {

  Status status;
  UInt   bufl;
  UInt   numout;

  if (IsNull(buf)) goto OK;
  bufl = Strlen(buf);
  status = PutBytes((Byte *)buf,bufl,numout);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
  	OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

Status Console::FlushOutput() {

  if (deferTo)
    return deferTo->FlushOutput();
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;

}

//----------------------------------------------------------------------
//
// Base Get/Put overrides...
//

Status Console::_GetElement(Byte *b) {
  if (deferTo)
    return deferTo->GetElement(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::_GetByte(Byte &b) {
  if (deferTo)
    return deferTo->GetByte(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::_GetBytes(Byte *buf, UInt buflen, UInt &numread) {
  if (deferTo)
    return deferTo->GetBytes(buf, buflen, numread);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::_PutElement(Byte *b) {
  if (deferTo)
    return deferTo->PutElement(b);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::_PutByte(Byte b) {

  if (deferTo)
    return deferTo->PutByte(b);

  if (!TheBefe)
    return Error::NoBefe;

  return Error::NotCapable;

}

Status Console::_PutBytes(Byte *buf, UInt buflen, UInt &numwritten) {

  if (deferTo) {
    if (buf && buflen)
      lastByteOutput = *(buf+buflen-1);
    else
      lastByteOutput = 0x00;
    return deferTo->PutBytes(buf, buflen, numwritten);
  }

  if (!TheBefe)
    return Error::NoBefe;

  return Error::NotCapable;

}

Status Console::_PutBytes(char const *buf) {

  Status status;
  UInt   bufl;
  UInt   numout;

  if (IsNull(buf)) goto OK;
  bufl = Strlen(buf);
  status = PutBytes((Byte *)buf,bufl,numout);
  if (status) goto SOMEERROR;
  
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
// Line I/O
//

Status Console::GetLine(String &line) {
  if (deferTo)
    return deferTo->GetLine(line);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

Status Console::PutLine(String &line) {
  if (deferTo)
    return deferTo->PutLine(line);
  if (!TheBefe)
    return Error::NoBefe;
  return Error::NotCapable;
}

//----------------------------------------------------------------------
//
// Foreground/Background colours
//
// NOTE: For Console itself, it ignores colour settings and
//       treats the background as Black and the foreground
//       as White.  END OF STORY!
//
//       It is up to the specific port implementation to
//       implement foreground and background colours if it
//       is applicable.
//

UInt Console::GetForegroundColour() {
  if (deferTo)
    return deferTo->GetForegroundColour();
  return White;
}

UInt Console::SetForegroundColour(UInt colour) {
  if (deferTo)
    return deferTo->SetForegroundColour(colour);
  return White;
}

UInt Console::GetBackgroundColour() {
  if (deferTo)
    return deferTo->GetBackgroundColour();
  return Black;
}

UInt Console::SetBackgroundColour(UInt colour) {
  if (deferTo)
    return deferTo->SetBackgroundColour(colour);
  return Black;
}

Status Console::Clear() {
  if (deferTo)
    return deferTo->Clear();
  return Error::NotCapable;
}

} // ...Namespace BEFE
