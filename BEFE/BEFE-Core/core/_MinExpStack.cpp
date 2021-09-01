//----------------------------------------------------------------------
// File: _MinExpStack.cpp - MinExp helper classes and structs
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------

#include "BEFE.h"         // For output
#include "BEFEMacros.h"   // For output
#include "BEFEWrappers.h" // For output

#include "_MinExpStack.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// _MinExpSortTarget - Used to special sort output of _MinExpStack...
//

class _MinExpSortTarget : public StringsSortTarget {
  public: virtual Int Compare(UInt elm1, UInt elm2) {
            String *str1;
            String *str2;
            Int     len1;
            Int     len2;
            Int     answer;
            str1 = &buf[elm1];
            str2 = &buf[elm2];
            len1 = str1->Length();
            len2 = str2->Length();
            if (len1 == len2) {
              if (*str1 < *str2)
                answer = -1;
              else if (*str1 > *str2)
                answer = 1;
              else
                answer = 0;
            }
            else
              answer = (len1 > len2)?-1:1;
            return answer;
          }
};

//----------------------------------------------------------------------
//
// _MinExpStack: Public Methods...
//

_MinExpStackEntry *_MinExpStack::TOS() {
  return (_MinExpStackEntry *)_GetAddress(-1);
}

_MinExpStackEntry _MinExpStack::Pop() {
	
  _MinExpStackEntry  that;
	
  that.MoveFrom(*(_MinExpStackEntry *)_GetAddress(-1));
  Remove(-1);
  return that.Consumable();
}

_MinExpStackEntry *_MinExpStack::Push(_MinExpStackEntry &that) {
  Append(that);
  return TOS();
}

Status _MinExpStack::ProcessChars(String &input, Strings &output) {
  
  Status             status;
  _MinExpStackEntry  entry;
  _MinExpStackEntry *tos;
  _MinExpSortTarget  sortIt;
  Int                pos;
  Int                len;
  Char               c;
  
  output.SetEmpty();
  
  // Normalise to ensure matching '(...)' and '[...]'...
  status = input.Mutable();
  if (status) goto ERROR;
  status = Normalise(input);
  if (status) goto ERROR;
  
  // Push empty first stack entry...
  entry.isOpen = true;
  status = Append(entry);
  if (status) goto ERROR;
  
  //
  // Process all characters...
  //
  
  pos = -1;
  c   = 0u;
  len = input.Length()-1;
  
  while (pos < len) {
    
    prevChar = c;
    c = input[++pos];
    
    // If previous Char wasn't '\', handle special characters...
    if (prevChar != '\\') {
      
      // Handle '[' and '('...
      if (c == '[' || c == '(') {
        status = ProcessBegin(c=='[');
        if (status) goto ERROR;
        continue;
      }
      
      // Handle ']' and ')'...
      if (c == ']' || c == ')') {
        status = ProcessEnd();
        if (status) goto ERROR;
        continue;
      }
      
      // Handle '|'...
      if (c == '|') {
        status = ProcessOr();
        if (status) goto ERROR;
        continue;
      }
      
      // Handle '\'...
      if (c == '\\') {
        continue;
      }
      
    }
    
    // Handle normal characters...
    status = ProcessChar(c);
    if (status) goto ERROR;
    
  }
  
  // Remove TOS if NULL...
  tos = TOS();
  if (tos->IsNull()) {
    status = Remove(-1);
    if (status) goto ERROR;
  }
  
  // Finish string processing...
  status = ProcessEOS(output);
  if (status) goto ERROR;
  if (output.Length() > 1) {
    //status = output.Sort(sortIt);
  	status = output.Sort();
    if (status) goto ERROR;
  }
  
  // Finally, remove duplicates...
  pos = 0;
  while (pos < (Int)output.Length()) {
  	pos++;
  	if (output[pos] == output[pos-1])
  		output.Remove(pos--);
  }

  // Handle errors
  status = Error::None;
  while (false) {
    ERROR: break;
  }
  
  return status;
}

//------------------------------------------------------------------------------
//
// _MinExpStack: Internal Protected Methods...
//

Status _MinExpStack::ProcessChar(Char c) {
  
  _MinExpStackEntry *tos;
  
  if (Length() < 1)
    return Error::MinExpStackEmpty;
  
  tos = TOS();
  if (tos->value.IsNull() || tos->value.Length() == 0)
    tos->value = c;
  else
    tos->value += c;
  
  prevChar = c;

  return Error::None;
}

Status _MinExpStack::ProcessBegin(Boolean isOptional) {
  
  Status            status;
  _MinExpStackEntry empty;
  
  status = FlushValue();
  if (status) return status;
  empty.isOpt  = isOptional;
  empty.isOpen = true;
  Push(empty);
  
  return status;
  
}

Status _MinExpStack::ProcessEnd() {

  Status             status;
  _MinExpStackEntry *open;
  _MinExpStackEntry *tos;
  Int                openIdx;
  _MinExpStackEntry  empty;
  
  // Flush any open value...
  status = FlushValue();
  if (status) goto ERROR;
  
  // Remove TOS if NULL...
  tos = TOS();
  if (tos->IsNull()) {
    status = Remove(-1);
    if (status) goto ERROR;
  }
  if (Length() == 0) goto EMPTY;
  
  // Find last open one...
  for (openIdx=Length()-1; openIdx; openIdx--) {
    open = (_MinExpStackEntry *)_GetAddress(openIdx);
    if (open->isOpen)
      break;
    status = Combine();
    if (status) goto ERROR;
  }
  if (openIdx < 0) goto NONEOPEN;
  open = (_MinExpStackEntry *)_GetAddress(openIdx);
  
  // Close and combine...
  if (open->isOpt && !open->list.Contains("")) {
    status = open->list.Append("");
    if (status) goto ERROR;
  }

  status = Combine();
  if (status) goto ERROR;
  
  // If previous character was '|' and not '\|') put '' in list if not already...
  tos = TOS();
  if (prevChar == '|' && !tos->list.Contains("")) {
    status = tos->list.Append("");
    if (status) goto ERROR;
  }

  // Push empty entry for follow-on stuff...
  status = Append(empty);
  if (status) goto ERROR;
  
  // Error cases...
  while (false) {
    EMPTY:    status = Error::MinExpStackEmpty;             break;
    NONEOPEN: status = Error::MinExpStackExpectedOpenGroup; break;
    ERROR:                                                  break;
  }
  
  return status;
  
}

Status _MinExpStack::ProcessEOS(Strings &strList) {

  _MinExpStackEntry *tos;
  Status             status;

  // Sanity checks...
  if (Length() == 0) goto EMPTY;
  
  // Before we start, handle dangling '|' if any...
  tos = TOS();
  if (tos->value.IsNull() && prevChar == '|' && !tos->list.Contains("")) {
    status = tos->list.Append("");
    if (status) goto ERROR;
  }
  
  // Close/End entire MinExp...
  status = ProcessEnd();
  if (status) goto ERROR;
  
  // Remove NULL entry ProcessEnd puts there...
  tos = TOS();
  if (tos->IsNull()) {
    status = Remove(-1);
    if (status) goto ERROR;
  }

  // Final sanity checks...
  if (Length() != 1) goto EXPECT1;
  tos = TOS();
  
  // Move the list...
  strList = tos->list.Consumable();
  tos->list.StartUp();

  // Error cases...
  while (false) {
    EMPTY:    status = Error::MinExpStackEmpty;            break;
    EXPECT1:  status = Error::MinExpStackExpected1;        break;
    ERROR:                                                 break;
  }
  
  return status;

}

Status _MinExpStack::FlushValue() {
  
  _MinExpStackEntry *tos;
  Status             status;
  
  // Nothing to do if empty...
  if (Length() == 0) goto OK;
  
  // Do it...
  tos = TOS();
  if (!tos->value.IsNull()) {
    status = AppendStringToList(tos->list,tos->value);
    if (status) goto ERROR;
    tos->value.SetNull();
  }
  
  // Error cases...
  goto OK;
  while (false) {
    OK:      status = Error::None;                 break;
    ERROR:                                         break;
  }
  
  return status;

}

Status _MinExpStack::ProcessOr() {
  
  _MinExpStackEntry *tos;
  _MinExpStackEntry  empty;
  _MinExpStackEntry *open;
  _MinExpStackEntry *entry0;
  Int                openIdx;
  Status             status;
  
  // Sanity checks...
  if (Length() == 0) goto EMPTY;
  
  // Find target OpenGroup...
  entry0 = (_MinExpStackEntry *)_GetAddress(0);
  openIdx = Length();
  while (openIdx > 0) {
    open = &entry0[--openIdx];
    if (open->isOpen)
      break;
  }
  if (openIdx < 0) goto NONEOPEN;
  
  // If no value we'll add "" because it's a leading/trailing '|'...
  tos = TOS();
  if (tos->value.IsNull())
    tos->value = "";
  status = FlushValue();
  if (status) goto ERROR;
  
  // Combine everything until open's on the top...
  while (Length() > 1u && (Int)Length()-1 > openIdx) {
    status = Combine();
    if (status) goto ERROR;
  }
  status = Append(empty);
  if (status) goto ERROR;
  
  // NOW we're an '|'
  entry0 = (_MinExpStackEntry *)_GetAddress(0);
  entry0[openIdx].isOr = true;
  prevChar = '|';
  
  // Error cases...
  while (false) {
    EMPTY:    status = Error::MinExpStackEmpty;             break;
    NONEOPEN: status = Error::MinExpStackExpectedOpenGroup; break;
    ERROR:                                                  break;
  }
  
  return status;

}

Status _MinExpStack::Combine() {

  _MinExpStackEntry *tos1;
  _MinExpStackEntry *tos2;
  Status             status;
  
  // Nothing to do if we're less than 2 entries...
  if (Length() < 2) goto OK;
  
  tos1 = (_MinExpStackEntry *)_GetAddress(-1);
  tos2 = (_MinExpStackEntry *)_GetAddress(-2);
  
  // Do it...
  if (tos2->isOr) {
    Int len = tos1->list.Length();
    for (Int idx=0; idx < len; idx++) {
      status = tos2->list.Append(tos1->list[idx]);
      if (status) goto ERROR;
    }
  }
  else if (tos1->list.Length() == 0) {
  }
  else if (!tos2->isOr) {
    status = JoinLists();
    if (status) goto ERROR;
  }
  else {
    status = tos2->list.Append(tos1->list);
    if (status) goto ERROR;
  }

  // Don't need TOS any more, it's been combined...
  status = Remove(-1);
  if (status) goto ERROR;
  
  // Error cases...
  goto OK;
  while (false) {
    OK:      status = Error::None;                 break;
    ERROR:                                         break;
  }
  
  return status;

}
 
//----------------------------------------------------------------------
//
// Method: Normalise - Normalise the input string
//

Status _MinExpStack::Normalise(String &str) {

	ArrayPrimitive<Int> stack;    // Position Stack
	Int                 pos;      // Current position
	Status              result;   // Return result
  String              special = "()[]|";
  Char                c;        // Temp
  Char                shouldBe; // Temp
  Int                 tPos;     // Temp

	// Initialise
	result = Error::None;
	pos = -1;
  result = str.Mutable();
  if (result) goto DONE;
  
	while (true) {

    // Find next special character...
		pos += 1;
		pos = str.FindChars(special,pos);
    if (pos < 0)
      break;
    
    // Ignore if preceeded by '\'...
    if (pos && str[pos-1] == '\\')
      continue;

    // Handle cases...
    c = str[pos];
    if (c == '(' || c == '[') {
      stack.Append(pos);
      continue;
    }
    if (c == '|')
      continue;
    else { // Should be ']' or ')'...
      shouldBe = (c == ')') ? '(' : '[';
      if (stack.Length() && str[stack[-1]] != shouldBe) { // Missing match...
        tPos = stack[-1];
        result = str.Insert(tPos+1,shouldBe);
        pos += 1;
      }
      else if (stack.Length())
        stack.Remove(-1);
      else {
        str.Insert(0,shouldBe);
        pos += 1;
      }
    }
	}
  
  // Put missing cases on end...
  while (stack.Length()) {
    tPos = stack[-1];
    stack.Remove(-1);
    c = str[tPos];
    shouldBe = (c=='(') ? ')' : ']';
    str.Append(shouldBe);
  }

DONE:

	return result;

}

Status _MinExpStack::AppendStringToList(Strings &list, String &str) {
	
  Status             status;
  Int                len;
  _MinExpStackEntry *tos;
	
  tos = (_MinExpStackEntry *)_GetAddress(-1);
	len = tos->list.Length();
	if (!len)
    status = tos->list.Append(str);
	else {
    String *that = (String *)tos->list._GetAddress(0);
    for (Int i = 0; i < len; i++) {
      status = that[i].Append(str);
      if (status)
        break;
    }
  }

  return status;
	
}

Status _MinExpStack::JoinLists() {

	Status status;


  _MinExpStackEntry *entry1;
  int                len1;
  _MinExpStackEntry *entry2;
  int                len2;
  Strings            join;
  
	if (Length() < 2) goto EXPECT2;

  entry1 = (_MinExpStackEntry *)_GetAddress(-1);
  entry2 = (_MinExpStackEntry *)_GetAddress(-2);
  len1   = entry1->list.Length();
  len2   = entry2->list.Length();

  // Handle cases where one or both are empty...
  if (len1 == 0) goto OK;
  if (len2 == 0) {
  	entry2->list = entry1->list.Consumable();
  	goto OK;
  }

  // Both have contents...
  for (int i=0; i < len2; i++)
    for (int j=0; j < len1; j++)
      join.Append(entry2->list[i]+entry1->list[j]);
  entry2->list = join.Consumable();
  
  goto OK;

  // Handle end cases...
  while (false) {
  	EXPECT2: status = Error::MinExpStackExpected2; break;
  	OK:      status = Error::None;                 break;
  }

  return status;
  
}

//----------------------------------------------------------------------
//
// _MinExpStackEntry
//

Status _MinExpStackEntry::StartUp() {
  list.StartUp();
  value.StartUp();
  isOr   = false;
  isOpt  = false;
  isOpen = false;
  return Error::None;
}

Status _MinExpStackEntry::ShutDown() {
  list.ShutDown();
  value.ShutDown();
  return Error::None;
}

Status _MinExpStackEntry::MoveFrom(_MinExpStackEntry const &that) {
  ShutDown();
  MemoryCopyRaw((Byte *)&that,(Byte *)this,sizeof(_MinExpStackEntry));
  return Error::None;
}

Status _MinExpStackEntry::CopyFrom(_MinExpStackEntry const &that) {
  list   = Strings(that.list);
  value  = String(that.value);
  isOr   = that.isOr;
  isOpt  = that.isOpt;
  isOpen = that.isOpen;
  return Error::None;
}

_MinExpStackEntry &_MinExpStackEntry::Consumable() const {
	return *(_MinExpStackEntry *)this;
}

_MinExpStackEntry &_MinExpStackEntry::NotConsumable() const {
	return *(_MinExpStackEntry *)this;
}

Boolean _MinExpStackEntry::IsNull() const {
  return (list.IsNull() || list.Length() == 0) && value.IsNull() && !isOr && !isOpt && !isOpen;
}

} // ...Namespace BEFE
