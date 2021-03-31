//----------------------------------------------------------------------
// File: Error.cpp - Implementation of Error specific functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// NOTE: We currently only implement ErrorToString() which mindlessly
//       turns it into 0x<nnnnnnnn>.  We expect to expand on this
//       in the future once we get strings for the errors and, later,
//       when we translate them and provide parameter passing to the
//       errors.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

String ErrorString(Status error) {

  //Status  status;
  Byte    buf[9];
  Byte   *bufp;
  Int     i;
  String  theString;
  Boolean append;

  //
  // If the messages aren't loaded, try loading them and use that...
  //

  if (TheBefe && !TheBefe->ErrorMessagesAreLoaded())
    /*status =*/ TheBefe->LoadErrorMessages();
    
  if (TheBefe && TheBefe->ErrorMessagesAreLoaded()) {
    theString = TheBefe->ErrorMessage(error);
    goto GOTIT;
  }
  
  //
  // Nope, they're not loaded, do it ourselves...
  //
  
  bufp = &buf[8];
  *bufp-- = 0;
  for (i=0;i<8;i++) {
    *bufp = ( error>>(i*4) ) & 0x0f;
    if (*bufp < 0x0a)
      *bufp-- += '0';
    else {
      *bufp += 'A';
      *bufp-- -= 10;
    }
  }

  append = true;
  switch (error&Error::MaskType) {
    case ErrorBase::General:    theString += "GEN+"; break;
    case ErrorBase::Buffer:     theString += "BUF+"; break;
    case ErrorBase::Sequence:   theString += "SEQ+"; break;
    case ErrorBase::Encode:     theString += "UTF+"; break;
    case ErrorBase::Array:      theString += "ARR+"; break;
    case ErrorBase::String:     theString += "STR+"; break;
    case ErrorBase::Stream:     theString += "STM+"; break;
    case ErrorBase::File:       theString += "FIL+"; break;
    case ErrorBase::Lexicon:    theString += "LEX+"; break;
    case ErrorBase::Crypt:      theString += "SEC+"; break;
    case ErrorBase::CryptKey:   theString += "CKY+"; break;
    case ErrorBase::Command:    theString += "CMD+"; break;
    case ErrorBase::PortWin32:  theString += "W32+"; break;
    case ErrorBase::Utility:    theString += "UTL+"; break;
    case ErrorBase::UtilBuild:  theString += "UBL+"; break;
		case ErrorBase::UtilLex:    theString += "ULX+"; break;
    case ErrorBase::Internal:   theString += "!!!+"; break;

    default:                    theString  = "\?\?\?-0x";
                                theString += (char *)&buf[0];
                                append = false;
                                break;
  }

  if (append)
    theString += ToString((Int)(error & Error::MaskCode)).LeftPad(3,'0');

  // Done...
  while (false) {
    GOTIT: break;
  }
  
  return theString;

}

} // ...Namespace BEFE
