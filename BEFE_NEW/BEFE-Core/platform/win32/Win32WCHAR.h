//----------------------------------------------------------------------
// File: Win32WCHAR.h - Win32 WCHAR utility Functions Declarations
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef WIN32WCHAR_H // WIN32WCHAR_H...
#define WIN32WCHAR_H

#include "Primitives.h"  // EVERYTHING NEEDS THIS!!!

namespace BEFE { // Namespace BEFE...

UInt   Win32W_Strlen(UShort *str);
UInt   Win32W_Strcpy(UShort *dst, UShort *src);
UInt   Win32W_UShortToByteLen(UShort *str);
UInt   Win32W_ByteToUShortLen(Byte *str, UInt numBytes);
String Win32W_ToString(UShort *str);
Status Win32W_FromString(String const &str, UShort *outStr, UInt maxLen, UInt &numUsed);

} // ...Namespace BEFE

#endif // ...WIN32WCHAR_H