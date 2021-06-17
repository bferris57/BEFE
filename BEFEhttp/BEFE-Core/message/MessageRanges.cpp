//----------------------------------------------------------------------
// File: MessageRanges.h - Implementation of the MessageRanges Class
//----------------------------------------------------------------------
//
// The MessageRanges Class is simply an Array of MessageRange Values.
// The reason we have an "Implementation" here is because we've added
// additional methods to the ArrayValue<MessageRange> base.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Message.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// MessageRanges.Find(String const &) - Find MessageRange by Name
//

Int MessageRanges::Find(String const &findName) const {

  UInt          idx;
  UInt          numRanges;
  MessageRange *range;

  ((MessageRanges *)this)->_BufAndLength(*(Byte **)&range, numRanges);
  for (idx=0; idx < numRanges; idx++) {
    if (range->name == findName) break;
    range++;
  }
  if (idx >= numRanges)
    idx = -1;

  return idx;

}

//----------------------------------------------------------------------
//
// MessageRanges.Find(MessageId) - Find named MessageRange by MessageId
//

Int MessageRanges::Find(MessageId findId) const {

  UInt         idx;
  UInt          numRanges;
  MessageRange *range;

  ((MessageRanges *)this)->_BufAndLength(*(Byte **)&range, numRanges);
  for (idx=0; idx < numRanges; idx++) {
    if (findId >= range->range.idx1 && findId <= range->range.idx2) break;
    range++;
  }
  if (idx >= numRanges)
    idx = -1;

  return idx;

}

} // ...Namespace BEFE