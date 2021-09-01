//----------------------------------------------------------------------
// File: MessageMessages.h - Implementation of the MessageMessages Class
//----------------------------------------------------------------------
//
// The MessageRanges Class is simply an Array of Message Values. The
// reason we have an "Implementation" here is because we've added
// additional methods to the ArrayValue<Message> base.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Message.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// MessageRanges.Find - Find named MessageRange
//

URange Messages::UsedRange(MessageRange const &msgRange) const {
  
  UInt     idx;
  UInt     numMessages;
  Message *message;
  URange   range;
  

  range.idx1 = range.idx2 = UNaN;  
  ((Messages *)this)->_BufAndLength(*(Byte **)&message, numMessages);
  for (idx=0; idx < numMessages; idx++) {
    if (message->id >= msgRange.range.idx1 && message->id >= msgRange.range.idx2) {
      if (range.idx1 == UNaN || message->id < range.idx1)
        range.idx1 = message->id;
      if (range.idx2 == UNaN || message->id > range.idx2)
        range.idx2 = message->id;
    }
    message++;
  }
  
  return range;
  
}

} // ...Namespace BEFE