//!befe-publish inc
//----------------------------------------------------------------------
// File: StringGeneral.h - Various general purpose String Funtions
//----------------------------------------------------------------------
//
// These functions are basically String related by we haven't put them
// into the String Class because they don't quite fit there in the
// typical sense of a "method".
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef STRINGGENERAL_H // STRINGGENERAL_H...
#define STRINGGENERAL_H

#include "Primitives.h"
#include "Arrays.h"

namespace BEFE { // Namespace BEFE...

// General NamedStrings substitution
Status Substitute(String       const &inStr,      // IN:  Input String
                  NamedStrings const &substVars,  // IN:  Named Substitution Variables
                  String       const &startTag,   // IN:  Start Tag (eg "%" "$(", etc)
                  String       const &endTag,     // IN:  End Tag (eg "%", ")", etc)
                  String             &outStr);    // OUT: Resulting String

// Escape character decode/encode
//
// *** See technical documentation for a more detailed description ***
//
// Note: We currently handle \t \b \n \r and \f and more, just like GNU
//       C++ and, mostly, Java as follows...
//
//         \0 ──► 0x00 - NUL - Null character
//         \a ──► 0x07 - BEL - Bell
//         \b ──► 0x08 - BS  - Backspace
//         \t ──► 0x09 - HT  - Horizontal Tab
//         \n ──► 0x0A - LF  - Line feed
//         \v ──► 0xOB - VT  - Vertical Tab
//         \f ──► 0x0C - FF  - Form feed
//         \r ──► 0x0D - CR  - Carriage return
//         \e ──► 0x1B - ESC - Escape[i]
//
//       See Column 'c' in the Wikipedia ASCII page at...
//
//          http://en.wikipedia.org/wiki/ASCII#ASCII_control_characters
//
//       We also handle \x<H...> and \X<H...> where <H...> is any number
//       of valid hexidecimal digits ('0',.'9', 'a'..'f', and 'A'..'F')
//       terminating at a non-hexidecimal digit.  This means you can
//       terminate with a space, end of string, another '\' et cetera.
//
//       Everything else after the '\' we just use literally and
//       and get rid of the leading '\'.  So \' and \" et cetera
//       come out just fine and '\\' comes out as '\'.
//
// Note: We DO NOT handle \u and \U.  This was intentional NOT an
//       oversight.  We simply substitute 'u' and 'U' instead because
//       we're adamantly against unicode surrogate pairs and the
//       outdated UCS-2 and UTF-16 way of thinking.
//
//       If you want them, do them yourself!!!  Otherwise, put a
//       proper UTF-8 bloody encoding in there for Unicode
//       characters. Basically. WAKE UP and GET WITH IT!!!!
//
// Note: Future BEFE versions intend to provide language specific
//       Encoding/Decoding of String Literals for your convenience
//       but, as of the current release, they're not written yet.
//

Status Escape(String const &inStr, String &outStr, Boolean escSpace);   // ◄── eg, turn '\' into '\\'
Status Unescape(String const &inStr, String &outStr); // ◄── eg, turn '\\' into '\'

} // ...Namespace BEFE

#endif // ...STRINGGENERAL_H