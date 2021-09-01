//----------------------------------------------------------------------
// File: MinExp.cpp - Implementation of the MinExp lexical class
//----------------------------------------------------------------------
//
// The MinExp class implements a lexical "variable term"
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "MinExp.h"
#include "_MinExpStack.h"

namespace BEFE { // Namespace BEFE...

// Class statics...

String MinExp::bogusIndex; // Undefined indexes end of pointing here

//-----------------------------------S----------------------------------
//
// C++ Lifecycle methods
//

MinExp::MinExp() {
  StartUp();
}

MinExp::MinExp(String const &str) {
  StartUp();
  theSpec = str;
  Parse();
}

MinExp::MinExp(char const *str) {
  StartUp();
  theSpec = str;
  Parse();
}

MinExp::~MinExp() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle methods
//

Status MinExp::StartUp() {

  theSpec.StartUp();
  theStrings.StartUp();
  shortIdx = NaN;
  longIdx  = NaN;
  
  return Error::None;

}

Status MinExp::ShutDown() {

  theSpec.ShutDown();
  theStrings.ShutDown();

  return Error::None;

}

Status MinExp::Reset() {

  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;

}

Boolean MinExp::IsNull() const {
  return theSpec.IsNull();
}

Status MinExp::SetNull() {
  return Reset();
}

Boolean MinExp::IsEmpty() const {
  return (theSpec.IsEmpty());
}

Status MinExp::SetEmpty() {
  return Reset();
}

Status MinExp::MoveFrom(MinExp const &that) {
  
  Status retStatus;
  Status status;
  
  retStatus = Error::None;
  if ((MinExp *)&that != this) {
    retStatus = Reset();
    if (retStatus) goto SOMEERROR;
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(MinExp));
    status = ((MinExp *)&that)->StartUp();
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  retStatus = Error::None;
  while (false) {
    SOMEERROR: if (!retStatus) retStatus = status; break;
  }
  
  return retStatus;
  
}

Status MinExp::CopyFrom(MinExp const &that) {

  Status status;
  
  if ((MinExp *)&that == this)
    status = Error::None;
  if (that.IsConsumable()) {
    status = MoveFrom(that);
    if (status) goto SOMEERROR;
  }
  else {
    status = theSpec.CopyFrom(that.theSpec);
    if (status) goto SOMEERROR;
    status = theStrings.CopyFrom(that.theStrings);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

MinExp &MinExp::Consumable() const {
  return *(MinExp *)this;
}

MinExp &MinExp::NotConsumable() const {
  return *(MinExp *)this;
}

Boolean MinExp::IsConsumable() const {
  return false;
}

Int MinExp::Length() const {
  return theStrings.Length();
}

Int MinExp::Count() const {
  return theStrings.Length();
}

//----------------------------------------------------------------------
//
// Getting/Setting
//

Status MinExp::Set(String const &str) {
  
  Status status;
  
  status = Reset();
  if (status) goto SOMEERROR;
  status = theSpec.CopyFrom(str);
  if (status) goto SOMEERROR;
  status = Parse();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
  
}

Status MinExp::Set(char const *str) {
  Status status;
  
  status = Reset();
  if (status) goto SOMEERROR;
  status = theSpec.CopyFrom(str);
  if (status) goto SOMEERROR;
  status = Parse();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;
  
}

//----------------------------------------------------------------------
//
// Query
//

String MinExp::Get(Int i) const {
  
  String term;

  Get(i, term);
  
  return term.Consumable();
}

Status MinExp::Get(Int i, String &term) const {
  
  Status status;
  
  status = theStrings.Get(i, term);
  
  return status;
  
}

String const &MinExp::Longest() {

	Int     stringsLen = (Int)Length();
	Int     curIdx;
	String *thisOne;
	String *thatOne;

	if (!stringsLen)
		return (String const &)bogusIndex;

	if (longIdx ==  NaN) {
    for (curIdx=0; curIdx < stringsLen; curIdx++) {
    	thisOne = &theStrings[curIdx];
    	if (!thisOne->Length())
    		continue;
    	thatOne = &theStrings[longIdx];
    	if (thisOne->Length() > thatOne->Length()) {
    		longIdx = curIdx;
    		continue;
    	}
    	if (thisOne->Length() < thatOne->Length())
    		continue;
    	if (*thisOne < *thatOne)
    		longIdx = curIdx;
    }
	}

	return (String const &)theStrings[longIdx];

}

String const &MinExp::Shortest() {

	Int     stringsLen = (Int)Length();
	Int     curIdx;
	String *thisOne;
	String *thatOne;

	if (!stringsLen)
		return (String const &)bogusIndex;

	if (shortIdx ==  NaN) {
    for (curIdx=0; curIdx < stringsLen; curIdx++) {
    	thisOne = &theStrings[curIdx];
    	if (!thisOne->Length())
    		continue;
    	thatOne = &theStrings[shortIdx];
    	if (!thatOne->Length() || thisOne->Length() < thatOne->Length()) {
    		shortIdx = curIdx;
    		continue;
    	}
    	if (thisOne->Length() > thatOne->Length())
    		continue;
    	if (*thisOne < *thatOne)
    		shortIdx = curIdx;
    }
	}

	return (String const &)theStrings[shortIdx];

}

//----------------------------------------------------------------------
//
// Matching
//

Boolean MinExp::Contains(String const &that) const {
  return Match(that);
}

Boolean MinExp::Contains(char const *that) const {
	return Match(String(that));
}

Boolean MinExp::Match(String const &that) const {
  
  Boolean  answer;
  String  *strings;
  UInt     numStrings;
  UInt     i;
  
  numStrings = theStrings.Length();
  if (numStrings == 0) goto NOPE;  
  strings = (String *)((Strings *)&theStrings)->_GetAddress(0);
  if (BEFE::IsNull(strings)) goto NOPE;
  for (i=0; i < numStrings; i++)
    if (strings[i].CompareEquals(that)) goto YEP;
  answer = false;
  
  // Handle errors
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }
  
  return answer;
  
}

Boolean MinExp::MatchSensitive(String const &that) const {
  
  Boolean  answer;
  String  *strings;
  UInt     numStrings;
  UInt     i;
  
  numStrings = theStrings.Length();
  if (numStrings == 0) goto NOPE;  
  strings = (String *)((Strings *)&theStrings)->_GetAddress(0);
  if (BEFE::IsNull(strings)) goto NOPE;
  for (i=0; i < numStrings; i++)
    if (strings[i].CompareEqualsSensitive(that)) goto YEP;
  answer = false;
  
  // Handle errors
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }
  
  return answer;
  
}

Boolean MinExp::MatchInsensitive(String const &that) const {
  
  Boolean  answer;
  String  *strings;
  UInt     numStrings;
  UInt     i;
  
  numStrings = theStrings.Length();
  if (numStrings == 0) goto NOPE;  
  strings = (String *)((Strings *)&theStrings)->_GetAddress(0);
  if (BEFE::IsNull(strings)) goto NOPE;
  for (i=0; i < numStrings; i++)
    if (strings[i].CompareEqualsInsensitive(that)) goto YEP;
  answer = false;
  
  // Handle errors
  while (false) {
    NOPE: answer = false; break;
    YEP:  answer = true;  break;
  }
  
  return answer;
  
}

//----------------------------------------------------------------------
//
// C++ Operators
//

String const &MinExp::operator[] (Int index) const {

	String *that;

	that = (String *)((Strings *)&theStrings)->_GetAddress(index);
  if (!that)
  	that = (String *)&bogusIndex;
	return *that;

}

//----------------------------------------------------------------------
//
// Formatting
//

Status MinExp::ToStrings(Strings &info) {

  Status status;
  String str;
  String leadIn;
  String leadOut;
  UInt   i;
  String tmpStr;
  
  status  = info.SetEmpty();
  leadIn  = GetEnvironmentValue("BEFE_QuoteLeadIn");
  leadOut = GetEnvironmentValue("BEFE_QuoteLeadOut");
  
  // Header...
  str = "Normalised: ";
  if (BEFE::IsNull(theSpec))
    str += CONSTR_Null;
  else
    str += leadIn + theSpec + leadOut;
  status = info.Append(str);
  if (status) goto ERROR;

  str = "Longest:    ";
  tmpStr = Longest();
  if (BEFE::IsNull(tmpStr))
  	str += CONSTR_Null;
  else
  	str += leadIn + tmpStr + leadOut;
  status = info.Append(str);
  if (status) goto ERROR;

  str = "Shortest:   ";
  tmpStr = Shortest();
  if (BEFE::IsNull(tmpStr))
  	str += CONSTR_Null;
  else
  	str += leadIn + tmpStr + leadOut;
  status = info.Append(str);
  if (status) goto ERROR;

  status = info.Append(String(""));
  if (status) goto ERROR;
  
  // Matches...
  if (BEFE::IsEmpty(theStrings)) {
    str = "Matches: ";
    str += CONSTR_StarNone;
    status = info.Append(str);
  }
  else for (i=0; i < theStrings.Length(); i++) {
    str = String(" Match[") << i << "] = ";
    if (theStrings[i].IsNull())
    	str += "NULL";
    else
      str += leadIn + theStrings.Get(i) + leadOut;
    status = info.Append(str);
  }

  // Handle errors
  status = Error::None;
  while (false) {
    ERROR: break;
  }
  
  return status;

}

Strings MinExp::ToStrings() {

	Strings localStrings;

  ToStrings(localStrings);

  return localStrings.Consumable();

}

//----------------------------------------------------------------------
//
// Protected instance methods
//
//----------------------------------------------------------------------

//----------------------------------------------------------------------
//
// Method: Parse - Parse the specification and produce the strings
//

Status MinExp::Parse() {

  Status        status; // Return status
  _MinExpStack  stack;  // Processing stack
  
  status = stack.ProcessChars(theSpec,theStrings);

  return status;

}

} // ...Namespace BEFE
