//!befe-publish inc
//----------------------------------------------------------------------
// File: ToStrings.h - Various Multiple line ToStrings utility functions
//----------------------------------------------------------------------
//
// These functions are more verbose forms of the Python str() and
// Java toString() functions and, as such, give more details about the
// object/value being passed.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TOSTRINGS_H
#define TOSTRINGS_H

#include "Primitives.h"

#include "File.h"
#include "Platform.h"
#include "Process.h"
#include "Processor.h"
#include "OperatingSystem.h"
#include "Network.h"
#include "Stream.h"
#include "BufferStream.h"

namespace BEFE {

// Displaying to Cout
void    DisplayStrings(Strings &strings);
void    DisplayStrings(Strings &strings, UInt indent);

void    DisplayMemory(Byte *vbuf, size_t len, UInt startoffset,UInt indent);
void    DisplayMemory(Byte *vbuf, size_t len, UInt startoffset);

// Instance -> Strings handy form
Strings ToStrings(Process           &process);
Strings ToStrings(Platform          &platform);
Strings ToStrings(Processor         &processor);
Strings ToStrings(OperatingSystem   &os);
Strings ToStrings(Network           &net);
Strings ToStrings(IdSpaceInfo       &spaceid);
Strings ToStrings(FileInfo    const &info);
Strings ToStrings(StreamStats const &info);

// Instance -> Strings normal form
Status  ToStrings(Process           &process,   Strings &strings);
Status  ToStrings(Platform          &platform,  Strings &strings);
Status  ToStrings(Processor         &processor, Strings &strings);
Status  ToStrings(OperatingSystem   &os,        Strings &strings);
Status  ToStrings(Network           &net,       Strings &strings);
Status  ToStrings(IdSpaceInfo       &spaceid,   Strings &strings);
Status  ToStrings(FileInfo    const &info,      Strings &strings);
Status  ToStrings(StreamStats const &info,      Strings &strings);

// Memory -> Strings
Strings ToStrings(Byte *vbuf, size_t len, UInt startoffset);
Strings ToStrings(Buffer const &buf);
Strings ToStrings(Buffer const &buf, UInt startoffset);
Strings ToStrings(BufferStream const &stream);
Strings ToStrings(BufferStream const &stream, UInt startoffset);

// Memory -> Strings normal form
Status  ToStrings(Byte *vbuf, size_t len, UInt startoffset, Strings &strings);
Status  ToStrings(Buffer const &buf, Strings &strings);
Status  ToStrings(Buffer const &buf, UInt startoffset, Strings &strings);
Status  ToStrings(BufferStream const &buf, Strings &strings);
Status  ToStrings(BufferStream const &buf, UInt startoffset, Strings &strings);

// Globals -> Strings
Strings ToStrings(Globals const &globs);
Status  ToStrings(Globals const &globs, Strings &strings);

} // Namespace BEFE

#endif // TOSTRINGS_H
