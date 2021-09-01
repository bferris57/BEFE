//----------------------------------------------------------------------
// File: Strings.cpp - Implementation of the Strings class
//----------------------------------------------------------------------
//
// This file implements the String class, an Array of String instances.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Strings.h"
#include "Sort.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// StringsSortTarget Class - For sorting
//

// C++ Lifecycle
StringsSortTarget::StringsSortTarget() {
	target    = NULL;
	buf       = NULL;
	numElms   = 0;
	sensitive = false;
}

StringsSortTarget::~StringsSortTarget() {
}

// SortTarget overrides...
UInt StringsSortTarget::Length () const {
	return numElms;
}

Int StringsSortTarget::Compare(UInt elm1, UInt elm2) {

	Int answer;

	if (buf) {
		if (sensitive)
			answer = (buf+elm1)->CompareSensitive(*(buf+elm2));
		else
			answer = (buf+elm1)->CompareInsensitive(*(buf+elm2));
	}
	else
		answer = 0;

	return answer;

}

Status StringsSortTarget::Swap(UInt elm1, UInt elm2) {
	if (buf && elm1 != elm2)
		MemoryExchangeRaw((Byte *)(buf+elm1), (Byte *)(buf+elm2), sizeof(String));
	return Error::None;
}

Status StringsSortTarget::Move(UInt elm1, UInt elm2) {
	return Error::NotCapable;
}

Boolean StringsSortTarget::Selected(UInt elm) const {
	return true;
}
  
//----------------------------------------------------------------------
//
// Strings specific methods
//

String Strings::Join(char sep) const {
  return Join(Char(sep));
}

String Strings::Join(Char sep) const {

  String answer;
  String cur;
  Int    len;
  Int    i;

  len = Length();
  for (i=0;i<len;i++) {
    Get(i,cur);
    if (i != 0)
      answer += sep;
    answer += cur;
  }

  return answer;

}

Status  Strings::Join(String const &sep, String &outString) const {
  
  Status  status;
  UInt    numParts;
  UInt    curIdx;
  Byte   *strBuf;
  UInt    strByteCount;
  UInt    strCharCount;
  Byte   *sepBuf;
  UInt    sepByteCount;
  UInt    sepCharCount;
  UInt    totSize;
  UInt    totLen;
  UInt    totSensitive;
  UInt    totStrings;
  String *strings;
  String  ourJoin;
  Buffer  ourBuf;
  
  // Nothing to do if no parts
  ((Strings *)this)->_BufAndLength(*(Byte **)&strings, numParts);
  if (numParts == 0) goto OK;
  
  // Special case for one string...
  if (numParts == 1) {
    status = ourJoin.CopyFrom(*strings);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // First pass, get Counts et cetera...
  totSize = 0;
  totLen  = 0;
  for (curIdx=0; curIdx < numParts; curIdx++) {
    status = strings[curIdx]._BufByteAndCharCount(strBuf, strByteCount, strCharCount);
    if (status) goto SOMEERROR;
    totSize += strByteCount;
    totLen  += strCharCount;
  }
  
  // Allocate the resulting Buffer
  status = ((String *)&sep)->_BufByteAndCharCount(sepBuf, sepByteCount, sepCharCount);
  if (status) goto SOMEERROR;
  status = ourBuf.Allocate(totSize + (numParts-1)*sepByteCount);
  if (status) goto SOMEERROR;
  
  // Second pass, build ourBuf...
  totSize      = 0;
  totLen       = 0;
  totSensitive = 0;
  totStrings   = 0;
  for (curIdx=0; curIdx < numParts; curIdx++) {
    status = strings[curIdx]._BufByteAndCharCount(strBuf, strByteCount, strCharCount);
    if (status) goto SOMEERROR;
    if (curIdx) {
      status = ourBuf.AppendBytes(sepBuf, sepByteCount);
      if (status) goto SOMEERROR;
      totSize += sepByteCount;
      totLen  += sepCharCount;
      if (sep.IsSensitive())
        totSensitive++;
      totStrings++;
    }
    status = ourBuf.AppendBytes(strBuf, strByteCount);
    if (status) goto SOMEERROR;    
    totSize += strByteCount;
    totLen  += strCharCount;
    if (strings[curIdx].IsSensitive())
      totSensitive++;
    totStrings++;
  }
  
  // We have our buffer now, setup ourJoin String...
  ourJoin.isMutable   = true;
  ourJoin.isByte      = (totSize == totLen);
  ourJoin.isChar      = !ourJoin.isByte;
  ourJoin.isSensitive = (totSensitive == totStrings);
  ourJoin.type        = String::Buffer;
  ourJoin.mult        = 1;
  ourJoin.constLen    = 0;
  
  // Transfer the buffer to ourJoin String
  ourJoin.data   = ourBuf.pointer;
  ourBuf.pointer = NULL;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  if (!status)
    outString = ourJoin.Consumable();
 
  return status;
  
}

Int Strings::Find(String const &str) const {
  
  Int     curIdx;
  UInt    numStrings;
  String *curString;
  
  ((Strings *)this)->_BufAndLength((Byte *&)curString, numStrings);
  
  // If isSorted==true we'll binary search if big enough to make sense...
  if (isSorted && Length() > 4) {

  	Int ll  = 0;
		Int ul  = Length()-1;
		Int mid;
		Int answer;

		while (ll <= ul) {
      mid = (ul+ll) >> 1;
      answer = NaN;
      curString[mid]._CompareRelative(str,answer);
      if (answer == NaN) {
      	ul = -1;
      	break;
      }
      if (answer < 0)
      	ll = mid+1;
      else if (answer > 0)
      	ul = mid-1;
      else
      	break;
		}

		curIdx = (ll <= ul) ? mid : NaN;

  }

  // Not sorted, search one by one...
  else {

    for (curIdx=0; curIdx < (Int)numStrings; curIdx++)
      if (curString[curIdx] == str) break;
  }
  
  if (curIdx >= (Int)numStrings)
    curIdx = NaN;
    
  return curIdx;
  
}

Boolean Strings::Contains(String const &str) const {
  
  Int curIdx = Find(str);
  
  return curIdx != NaN;
  
}

//----------------------------------------------------------------------
//
// Sorting and Sensitivity
//

Boolean Strings::IsSensitive() {
	return _reserved & 0x1;
}

Boolean Strings::IsInsensitive() {
	return !(_reserved & 0x1);
}

Status Strings::Sensitive() {
  
  Byte   *buf;
  UInt    numElms;
  String *curString;  

  _BufAndLength(buf, numElms);
  curString = (String *)buf;
  while (numElms) {
    curString->Sensitive();
    curString++;
    numElms--;
  }
  _reserved |= 0x01;
  isSorted = false;
  
  return Error::None;
  
}

Status Strings::Insensitive() {
  
  Byte   *buf;
  UInt    numElms;
  String *curString;  

  _BufAndLength(buf, numElms);
  curString = (String *)buf;
  while (numElms) {
    curString->Insensitive();
    curString++;
    numElms--;
  }
  
  _reserved &= 0x1e;
  isSorted = false;

  return Error::None;
  
}

Status Strings::Sort() {
  Status status;
  if (IsSensitive())
  	status = SortSensitive();
  else
    status = SortInsensitive();

  return status;
}

Status Strings::SortSensitive() {
  
  Status            status;
  StringsSortTarget targ;
  
  targ.buf       = (String *)_GetBuf();
  targ.numElms   = Length();
  
  status = SortInsertion((SortTarget const &)targ);

  isSorted = IsSensitive();
  
  return status;
  
}

Status Strings::SortInsensitive() {

  Status            status;
  StringsSortTarget targ;
  
  targ.buf       = (String *)_GetBuf();
  targ.numElms   = Length();
  
  status = SortInsertion((SortTarget const &)targ);

  isSorted = IsInsensitive();
  
  return status;

}

Status Strings::Sort(StringsSortTarget &target) {
	Status            status;

	target.buf       = (String *)_GetBuf();
  target.numElms   = Length();
  target.sensitive = false;

  status = SortInsertion(target);

  isSorted = false;

  return status;

}

} // ...Namespace BEFE
