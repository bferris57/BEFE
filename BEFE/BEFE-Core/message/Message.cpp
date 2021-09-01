//----------------------------------------------------------------------
// File: Message.cpp - Message Functions and Class Implementation
//----------------------------------------------------------------------
//
// These functions are used to process Messages and Message declarations
// specified in a file.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Message.h"
#include "Bmsg.h"

namespace BEFE { // Namespace BEFE...

// Forward function prototypes...
Status MessageError(String const &fileName, String const &line, BmsgToken errTok);

//----------------------------------------------------------------------
//
// Message Class: C++ Lifecycle
//

Message::Message() {
  StartUp();
}

Message::~Message() {
  ShutDown();
}

Message &Message::operator= (Message const &that) {
  CopyFrom(that);
  NotConsumable();
  return *this;
}

//----------------------------------------------------------------------
//
// Message Class: BEFE Lifecycle
//

Status Message::StartUp() {
  BEFE::SetNull(id);
  name.StartUp();
  message.StartUp();
  return Error::None;
}

Status Message::ShutDown() {
  
  Status retStatus;
  Status status;
  
  retStatus = name.ShutDown();
  status = message.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status Message::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Boolean Message::IsNull() const {
  return BEFE::IsNull(id) && message.IsNull();
}

Status Message::SetNull() {
  
  Status retStatus;
  Status status;
  
  BEFE::SetNull(id);
  retStatus = name.SetNull();
  status = message.SetNull();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Boolean Message::IsEmpty() const {
  return name.IsEmpty() && message.IsEmpty();
}

Status Message::SetEmpty() {
  Status retStatus;
  Status status;
  
  retStatus = name.SetEmpty();
  status = message.SetEmpty();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Status Message::MoveFrom(Message const &that) {
  Reset();
  MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(Message));
  return ((Message *)&that)->StartUp();
}

Status Message::CopyFrom(Message const &that) {
  
  Status status;
  
  if (IsConsumable())
    status = MoveFrom(that);
  else {
    status = Reset();
    id      = that.id;
    name    = that.name;
    message = that.message;
  }

  return status;

}
  
UInt Message::Length() {
  return message.Length();
}

UInt Message::Size() {
  return message.Size();
}

UInt Message::ImplementationSize() {
  return message.Size();
}

//----------------------------------------------------------------------
//
// Message Class: Optional BEFE Lifecycle
//

Message &Message::Consumable() const {
  name.Consumable();
  message.Consumable();
  return *((Message *)this);
}

Message &Message::NotConsumable() const {
  name.NotConsumable();
  message.NotConsumable();
  return *((Message *)this);
}

Boolean Message::IsConsumable() const {
  return name.IsConsumable() || message.IsConsumable();
}

Int Message::Compare(Message const &that) {
  
  Int answer;
  
  if (id < that.id)
    answer = -1;
  else if (id > that.id)
    answer = 1;
  else
    answer = 0;
    
  return answer;

}

Boolean Message::Equals(Message const &that) {
  return id == that.id;
}

//----------------------------------------------------------------------
//
// Function: MessageError - Display Message Error (or not)
//
// Usage: status = MessageError(fileName, line, msgTok)
//
// Where: status     - OUT: Return status
//        fileName   - IN:  File Name
//        line       - IN:  Line String
//        msgTok     - IN:  MessageToken type=TypeError or TypeWarning
//
// This function displays a TypeError or TypeWarning MessageToken
// if we're supposed to, and if it's TypeError returns the error
// as the return value.
//
// Note: Until we get all this message stuff working and plugged into the
//       Befe Class, we only display the general error code NOT the
//       proper entire message string.
//
// Note: And YES!!!, I know we call them "Errors" but we treat them as
//       Warnings because they don't really compromise the functioning
//       of BEFE as a whole.
//

Status MessageError(String const &fileName, String const &line, BmsgToken errTok) {

  Status status;
  UInt   oldColour;
  String pad;
  
  if (errTok.type != BmsgToken::TypeError && errTok.type != BmsgToken::TypeWarning) goto INTERNAL;

  if (!gQuiet && gShowWarnings) {
    oldColour = Cout.GetForegroundColour();
    if (gVeryVerbose) {
      if (Cout.GetCursor().x != 0) Cout << '\n';
      Cout.SetForegroundColour(Console::Cyan);
      Cout << "Line " << errTok.lineNo+1 << ": ";
      Cout.SetForegroundColour(Console::White);
      Cout << line << '\n';
      pad = ' ';
      pad *= errTok.colNo + 7 + IntLog10(errTok.lineNo+1) + 1;
      Cout << pad << "^\n";
    }
    if (errTok.type == BmsgToken::TypeWarning) {
      Cout.SetForegroundColour(Console::Yellow);
      Cout << CONSTR_Warning;
    }
    else {
      Cout.SetForegroundColour(Console::Red);
    }
    Cout << CONSTR_Syntax << errTok.value;
    Cout.SetForegroundColour(oldColour);
    Cout << '\n';
  }
  
  // Figure out what to return
  if (errTok.type == BmsgToken::TypeError)
    status = errTok.type;
  else
    status = Error::None;
    
  // Handle errors
  while (false) {
    INTERNAL: status = Error::Internal; break;
  }
  
  return status;
  
}

  
//----------------------------------------------------------------------
//
// Function: MessageLoadFile - Parse an entire Message File
//
// Usage: status = MessageLoadFile(fileName, rangeArray, messageMap)
//
// Where: status      - OUT:   Return status
//        rangeArray  - INOUT: Array of MessageRanges
//        msgArray    - INOUT: Array of Messages
//        idIndexes   - INOUT: Id->Index Map
//        nameIndexes - INOUT: Name->Index Map
//        numErrors   - INOUT: Number of errors (incremented)
//
// This function parses a single Message File, and appends all
// valid MessageRanges and Messages to the MessageRanges and
// MessageMap.
//

Status MessageLoadFile(String const  &fileName,
                       MessageRanges &rngArray,
                       Messages      &msgArray,
                       MessageIds    &idIndexes,
                       MessageNames  &nameIndexes,
                       UInt          &numErrors) {
  
  Status status;         // Return status
  UInt   rangeCount;
  UInt   messageCount;
  UInt   warningCount;
  Bmsg   bmsg;
 
  // Do it...
  status = bmsg.ProcessFile(fileName);
  if (status) goto SOMEERROR;
  
  // Get the info...
  status = bmsg.GetCounts(rangeCount, messageCount, numErrors, warningCount);
  if (status) goto SOMEERROR;
  status = bmsg.TakeResults(rngArray, msgArray, idIndexes, nameIndexes);  
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

} // ...Namespace BEFE