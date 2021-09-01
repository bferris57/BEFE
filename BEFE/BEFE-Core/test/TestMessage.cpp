//!befe-publish src
//----------------------------------------------------------------------
// File: TestMessage.cpp - Implementation of the TestMessage Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE Message and related Classes and Types
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Test.h"
#include "Message.h"

namespace BEFE { // Namespace BEFE...

Status TestMessage::Run() {

  Status        status;
  UInt          fg;
  String        fileName;
  MessageRanges rngArray;
  Messages      msgArray;
  MessageIds    idIndexes;
  MessageNames  nameIndexes;
  UInt          numErrors;
  UInt          msgIdx;
  Message       msg;
  Int           rngIdx;
  MessageRange  msgRange;
  UInt          errIdx;
  String        bldMsg;
  Status        errors[] = {
    0x12345789,
    Error::Failed,
    Error::NoBefe,
    Error::DivideByZero,
    Error::BufferAlreadyAllocated,
    Error::SeqSpanBadNumber,
    Error::NumberBadMantissa,
    Error::UTF8InvalidCodePoint,
    Error::ArraySizeFixed,
    Error::StringTruncated,
    Error::StreamAtEnd,
    Error::FileAlreadyExists,
    Error::FileDoesNotExist,
    Error::InternalNullPointer,
    Error::IdSpaceFull,
    Error::CommandAlreadyRegistered,
    Error::Win32RegKeyRootUnknown,
    Error::UtilityNotFound,
    0
  };
  
  if (!gQuiet)
    out << "***Testing Messages (Errors, etc.)..." << Endl;

  fg = out.GetForegroundColour();
  
  // Find the file if it exists...
  fileName = "BEFE_ErrorMessages.bmsg";
  status = TheBefe->FindConfigurationFile(fileName);
  if (status) goto SOMEERROR;

  if (gVerbose)
    out << "  Message file '" << fileName << "'...\n";
  
  status = MessageLoadFile(fileName, rngArray, msgArray, idIndexes, nameIndexes, numErrors);
  if (status)
    BEFE_ERROR("LoadMessages",status);
  if (gVerbose)
    out << "  There are " << msgArray.Length() << " Error Strings and " << rngArray.Length() << " Error Ranges\n";
  if (numErrors && gVerbose)
    out << "  There were " << numErrors << " errors\n";
    
  // Try finding a couple of them...
  for (errIdx=0; errors[errIdx]; errIdx++) {
    SetNull(msgIdx);
    idIndexes.Get(errors[errIdx], msgIdx);
    if (!IsNull(msgIdx))
      msg = msgArray.Get(msgIdx);
    else
      msg.message = "***Unknown***";
    rngIdx = rngArray.Find(errors[errIdx]);
    if (rngIdx >= 0) {
      msgRange = rngArray.Get(rngIdx);
      bldMsg = msgRange.shortName + '+' + ToString(errors[errIdx]-msgRange.range.idx1).LeftPad(3,'0');
    }
    else
      bldMsg = String("???+0x") + ToHexString(errors[errIdx],8);
    bldMsg += ": ";
    bldMsg += msg.message.Consumable();
    
    if (gVeryVerbose) {
      out << "  errIdx = "  << ToString(errIdx,2)
          << ": 0x"         << ToHexString(errors[errIdx],8)
          << " Message: ";
      out.SetForegroundColour(Console::Green);
      out << bldMsg << '\n';
      out.SetForegroundColour(fg);
    }
    
  }  
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: BEFE_ERROR("TestMessage.Run",status); break;
  }
  
  return status;

}

} // ...Namespace BEFE