//!befe-publish inc
//----------------------------------------------------------------------
// File: Message.h - Declarations for Message and related Classes
//----------------------------------------------------------------------
//
// The Message Class implements a minimal idea of a general purpose
// "message" which, at minimum, has a UInt identifier member 'id' and
// a String member 'message' containing the textual equivalent of the
// message.
//
// While motivation for creating the Message Class was originally for
// general Error/Warning messages but, since that's a rather restrictive
// view of things, instead of calling it an ErrorMessage or
// WarningMessage, we simply expanded it to Message, and there you
// have it.
//
// Note: In the future the Message Class may be expanded to handle
//       parameterised messages and equivalent formatting functionality
//       but, for now, we've left it at the UInt->String mapping
//       for starters.
//
// Note: Do NOT confuse this Message Class with other things like
//       communication Messages, et cetera.  It's only purpose is
//       as stated above and we needed a simple enough name to make
//       it usable and readable for use in BEFE.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MESSAGE_H // MESSAGE_H...
#define MESSAGE_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Typedefs and other summary Message declarations
//

// Base Values
typedef UInt                     MessageId;         // Message Id
struct                           MessageRange;      // Message Id Range
struct                           Message;           // Message Instance

// Arrays/Maps
class                            MessageRanges;     // Array of MessageRanges
class                            Messages;          // Array of Messages
typedef UIntMapPrimitive<UInt>   MessageIds;        // MessageId->Index Map
typedef StringMapPrimitive<UInt> MessageNames;      // Name->Index Map of Messages

//----------------------------------------------------------------------
//
// Message Functions
//

Status MessageLoadFile(String const  &fileName,
                       MessageRanges &rngArray,
                       Messages      &msgArray,
                       MessageIds    &idIndexes,
                       MessageNames  &nameIndexes,
                       UInt          &numErrors);

//----------------------------------------------------------------------
//
// MessageRange Struct - Range of Message Ids
//

struct MessageRange {
  
  // C++ Lifecycle
  MessageRange();
  MessageRange(MessageRange const &that);
  ~MessageRange();
  MessageRange &operator=(MessageRange const &that);

  // BEFE Lifecycle
  Status        StartUp();
  Status        ShutDown();
  Status        Reset();
  Boolean       IsNull() const;
  Status        SetNull();
  Boolean       IsEmpty() const;
  Status        SetEmpty();
  Status        MoveFrom(MessageRange const &that);
  Status        CopyFrom(MessageRange const &that);
  UInt          Length();
  UInt          Size();
  MessageRange &Consumable() const;
  MessageRange &NotConsumable() const;
  Boolean       IsConsumable() const;  
  Boolean       IsMutable() const;
  Status        Mutable();

  // Members  
  String name;
  String shortName;
  URange range;
  
};

//----------------------------------------------------------------------
//
// MessageRanges Class - An array of MessageRanges
//

class MessageRanges : public ArrayValue<MessageRange> {
  public: Int Find(String const &name) const;
  public: Int Find(MessageId id) const;
};

//----------------------------------------------------------------------
//
// MessageRanges Class - An array of MessageRanges
//

class Messages : public ArrayValue<Message> {
  public: URange UsedRange(MessageRange const &msgRange) const;
};

//----------------------------------------------------------------------
//
// Message Struct - A single Message
//

struct Message {

  // C++ Lifecycle
  Message();
  ~Message();
  Message &operator= (Message const &that);
  
  // BEFE Lifecycle
  Status   StartUp();
  Status   ShutDown();
  Status   Reset();
  Status   MoveFrom(Message const &that);
  Status   CopyFrom(Message const &that);
  Boolean  IsNull() const;
  Status   SetNull();
  Boolean  IsEmpty() const;
  Status   SetEmpty();
  UInt     Length();
  UInt     Size();
  UInt     ImplementationSize();
  // Optional BEFE Lifecycle
  Message &Consumable() const;
  Message &NotConsumable() const;
  Boolean  IsConsumable() const;
  Int      Compare(Message const &that); 
  Boolean  Equals(Message const &that);
  // Members
  MessageId id;
  String    name;
  String    message;  
  
};

} // ...Namespace BEFE

#endif // ...MESSAGE_H