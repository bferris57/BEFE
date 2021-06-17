//----------------------------------------------------------------------
// File: Match.cpp - Struct Match implementation
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Match.h"

namespace BEFE { // Namespace BEFE...

// Workhorse Functions (these do the real work)
Status Match_Star(String const &theString, String const &thePattern);
Status Match_MinExp(String const &theString, String const &thePattern);
Status Match_SQL(String const &theString, String const &thePattern);
Status Match_PosixBRE(String const &theString, String const &thePattern);
Status Match_PosixERE(String const &theString, String const &thePattern);
Status Match_WildOne(String const &theString, String const &thePattern, Char wild, Char wildOne);

//----------------------------------------------------------------------
//
// C++ Lifecycle
//
// Note: The Match(String const &) constructor is a bit dodgy because
//       it only works properly IFF the pattern contains some indication
//       as to what type it is.  For example, the string "xxx*yyy" will
//       presume it's a TypeStar, "e[xit]" will presume it's a TypeMinExp
//       and "abc%def" and "abc_def" will presume it's TypeSQL.
//
//       Not just that, it looks in ASCENDING order of type, so the
//       String "ab[de*ef]" will turn it into a TypeStar, etc.
//
//       I mean, what else can it do than that?  Sort of mindless but
//       useful, sometimes.  ;-)
//

Match::Match(MatchType pType, String const &pPattern) {
  
  StartUp();
  type = pType;
  pattern = pPattern;
  
}
  
Match::Match(String const &pPattern) {

  if (!BEFE::IsNull(pPattern.Find('*')))
    type = TypeStar;
  else if (!BEFE::IsNull(pPattern.Find('[')))
    type = TypeMinExp;
  else if (!BEFE::IsNull(pPattern.Find('%')) ||
           !BEFE::IsNull(pPattern.Find('_')))
    type = TypeSQL;
  else
    type = TypeUnknown;
  pattern = pPattern;

}
  
// BEFE Lifecycle
Status Match::StartUp() {
  
  Status status;
  
  type = TypeUnknown;
  status = pattern.StartUp();
  
  return status;
  
}

Status Match::ShutDown() {
  
  Status status;
  
  status = pattern.ShutDown();
  
  return status;
  
}

Status Match::Reset() {
  
  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;

}  

Status Match::MoveFrom(Match const &that) {
  
  Status status;
  
  if (this != (Match *)&that) {
    
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(Match));
    status = ((Match *)&that)->StartUp();
    ((Match *)this)->NotConsumable();
    
  }
  else
    status = Error::None;
  
  return status;
  
}

Status Match::CopyFrom(Match const &that) {

  Status status;
  
  if (this != (Match *)&that) {

    if (that.IsConsumable())
      status = MoveFrom(that);
    else {
      type = that.type;
      status = pattern.CopyFrom(that.pattern);
      NotConsumable();
    }
  }
  else
    status = Error::None;
  
  return status;

}

Boolean Match::IsNull() const {
  return ( type == TypeUnknown && pattern.IsNull() );
}

Status Match::SetNull() {
  return Reset();
}

Boolean Match::IsEmpty() const {
  return (pattern.IsEmpty());
}

Status Match::SetEmpty() {
  
  Status status;
  
  status = pattern.SetEmpty();
  
  return status;
  
}

UInt Match::Length() const {
  return pattern.Length();
}

UInt Match::Size() const {
  return pattern.Size();
}

Boolean Match::IsConsumable() const {
  return pattern.IsConsumable();
}

Match &Match::Consumable() const {
  pattern.Consumable();
  return *((Match *)this);
}

Match &Match::NotConsumable() const {
  pattern.NotConsumable();
  return *((Match *)this);
}

//----------------------------------------------------------------------
//
// Operational methods
//

Boolean Match::Matches(String const &that) const {

  Status status;

  switch (type) {
    case TypeStar:     status = Match_Star    (that, pattern); break;
    case TypeMinExp:   status = Match_MinExp  (that, pattern); break;
    case TypeSQL:      status = Match_SQL     (that, pattern); break;
    case TypePosixBRE: status = Match_PosixBRE(that, pattern); break;
    case TypePosixERE: status = Match_PosixERE(that, pattern); break;
    default:           status = Error::MatchDoesNotMatch;      break;
  };
  
  return (status == Error::None);
  
}

//----------------------------------------------------------------------
//
// Workhorse Functions (these do the real work)
//
// Note: Once we get this working we have to rewrite it to make special
//       case of thePattern.isByte and theString.isByte because that
//       would run significantly faster...
//
//       Not just that, we need to rewrite the isChar case to use the
//       UTF8 functions to do the work instead of allocating temporary
//       Strings, etc.
//
//       I mean, the Strings aren't moving around in memory so we need
//       to get down to it with pointers...
//

Status Match_Star(String const &theString, String const &thePattern) {
  
  Status status;

  status = Match_WildOne(theString, thePattern, '*', '?');
  
  return status;

}

// Note: Once we're finished here we should rewrite this to not
//       instantiate a MinExp and do it in place because we don't
//       really need it to compute ALL THE POSSIBILITIES when we
//       could simply walk each character and do it in one pass
//       on the fly.
Status Match_MinExp(String const &theString, String const &thePattern) {
  
  Status status;
  MinExp me;
  
  me.Set(thePattern);
  if (me.Match(theString))
    status = Error::None;
  else
    status = Error::MatchDoesNotMatch;
    
  return status;
  
}

Status Match_SQL(String const &theString, String const &thePattern) {
  
  Status status;

  status = Match_WildOne(theString, thePattern, '%', '_');
  
  return status;

}

Status Match_PosixBRE(String const &theString, String const &thePattern) {
  return Error::MatchDoesNotMatch;
}

Status Match_PosixERE(String const &theString, String const &thePattern) {
  return Error::MatchDoesNotMatch;
}

// Note: Once we get this working we have to rewrite it to make special
//       case of thePattern.isByte and theString.isByte because that
//       would run significantly faster...
//
//       Not just that, we need to rewrite the isChar case to use the
//       UTF8 functions to do the work instead of allocating temporary
//       Strings, etc.
//
//       I mean, the Strings aren't moving around in memory so we need
//       to get down to it with pointers...
//

Status Match_WildOne(String const &theString, String const &thePattern, Char wild, Char wildOne) { // Match_WildOne...
  
  Status status;
  UInt   patPos;
  UInt   patLen;
  UInt   strPos;
  UInt   strLen;
  Span   patCmp;
  Span   strCmp;
  Int    nextWild;
  UInt   i;
  Char   patChar;
  Char   strChar;
  
  // Initialise for the loop...
  patPos = 0;
  patLen = thePattern.Length();
  strPos = 0;
  strLen = theString.Length();
  nextWild = thePattern.Find(wild, Span(patPos, patLen));
  if (BEFE::IsNull(nextWild))
    nextWild = patLen;
    
  // Some special pre-checks...
  if (patLen == 0 && strLen == 0) goto MATCH;
  if (patLen == 0) goto NOMATCH;
  
  // Combine contiguous Wild Chars...
  while (patPos < patLen &&
         thePattern.Get(nextWild+1) == wild) {
    nextWild++;
    patPos++;
    patLen--;
  }
  
  // Check for weird special cases...
  if (patLen == 1 && thePattern.Get(0) == wild) goto MATCH;
  if ((UInt)nextWild >= patLen) {
    if (patLen != strLen) goto NOMATCH;
    for (i=0; i < patLen; i++) {
      patChar = thePattern.Get(i);
      strChar = theString.Get(strPos+i);
      if (patChar != wildOne && strChar != patChar)
        break;
    }
    if (i < (UInt)patLen) goto NOMATCH;    
    goto MATCH;
  }

  // Make sure leading non-wild part matches...
  for (i=patPos; i < (UInt)nextWild; i++) {
    patChar = thePattern.Get(i);
    strChar = theString.Get(strPos+i);
    if (patChar != wildOne && strChar != patChar)
      break;
  }
  if (i < (UInt)nextWild) goto NOMATCH;    
    
  // Matches, bump to next Wild...
  strPos += i - patPos;
  patPos  = nextWild+1;
    
  // At this point patPos points to a Wild Char and there's
  // no duplicate contiguous Wild Chars.
  //
  // Process until we're finished...
  
  do {
    
    // Find next Wild Char...
    nextWild = thePattern.Find(wild, Span(patPos, patLen));
    if (BEFE::IsNull(nextWild))
      nextWild = patLen;
    else
      while ((UInt)nextWild < patLen && thePattern.Get(nextWild+1) == wild)
        nextWild++;
        
    // Find the matching non-wild part in theString...
    if (patPos >=(UInt)nextWild) goto MATCH;
    while (strPos < strLen) {
      for (i=patPos; i < (UInt)nextWild && strPos < strLen; i++) {
        patChar = thePattern.Get(i);
        strChar = theString.Get(strPos);
        if (patChar == wildOne || patChar == strChar)
          strPos++;
        else
          break;
      }
      if (i < (UInt)nextWild) {
        strPos++;
        continue;
      }
      if (strPos >= strLen) {
        if ((UInt)nextWild >= patPos)
          goto MATCH;
        else
          goto NOMATCH;      
      }
      else
        break;
    }
    if (strPos >= strLen)
      goto NOMATCH;
      
    // Matches so far, so update patPos...
    patPos  = nextWild+1;
    
  } while ((UInt)patPos < patLen);
  
  // Make nothing left over...
  if (strPos < strLen && patPos > patLen) goto NOMATCH;
  
  // Handle errors
  status = Error::None;
  while (false) {
    MATCH:   status = Error::None;              break;
    NOMATCH: status = Error::MatchDoesNotMatch; break;    
  }
  
  return status;

} // ...Match_WildOne

} // ...Namespace BEFE
