//!befe-publish src
//----------------------------------------------------------------------
// File: Dictionary.cpp - Implementation of the Dictionary class
//----------------------------------------------------------------------
//
// The Dictionary Class implements an arbitrairily large word lookup
// mechanism.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Dictionary.h"
#include "Sort.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Class: DictSortTarg - Dictionary Sort target for sorting
//
//----------------------------------------------------------------------

struct DictSortTarg : SortTarget {

  // C++ Lifecycle
  DictSortTarg(Dictionary &theDict);
  virtual ~DictSortTarg();  
  // BEFE Lifecycle  
  virtual UInt   Length () const;
  // For Sorting
  virtual Int     Compare(UInt elm1, UInt elm2) const;
  virtual Status  Swap   (UInt elm1, UInt elm2) const;
  virtual Status  Move   (UInt elm1, UInt elm2) const;
  // For Out-of-place Selecting
  virtual Boolean Selected(UInt elm) const;
  
  // Members
  Dictionary *dict;
  UInt        numErrors;
  
};

DictSortTarg::DictSortTarg(Dictionary &theDict) {
  dict = &theDict;
  numErrors = 0;
}

DictSortTarg::~DictSortTarg() {
  return;
}

UInt DictSortTarg::Length () const {
  return dict->Length();
}

Int DictSortTarg::Compare(UInt elm1, UInt elm2) const {
  
  Status   status;
  Int      cmp;
  Byte    *theBuf;
  UInt    *theOffset;
  Byte    *line1;
  Byte    *line2;
  
  // NOTE: In case of failure we always return -1 so the offenders might bubble to
  //       the beginning of the sorted list for debugging purposes.  Who knows?
  cmp = -1;
  
  // Validate indexes
  if (elm1 >= dict->Length() || elm2 >= dict->Length()) goto OUTOFRANGE;
  theBuf = dict->contents.GetBuf();
  theOffset = (UInt *)dict->entryOffsets._GetBuf();
  if (IsNull(theBuf) || IsNull(theOffset)) goto NULLPOINTER;
  line1 = theBuf + theOffset[elm1];
  line2 = theBuf + theOffset[elm2];

  // Compare them
  cmp = Strcmpi(line1,line2);
  
  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE:  status = Error::IndexOutOfRange;     break;    
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }
  if (status) {
    if (!numErrors)
      BEFE_ERROR("DictSortTarg.Selected",status);
    ((DictSortTarg *)this)->numErrors++;
  }
  
  return cmp;
  
}

Status DictSortTarg::Swap(UInt elm1, UInt elm2) const {
  BEFE_ERROR("DictSortTarg.Swap",Error::NotCapable);
  return Error::NotCapable;
}

Status DictSortTarg::Move(UInt elm1, UInt elm2) const {
  BEFE_ERROR("DictSortTarg.Move",Error::NotCapable);
  return Error::NotCapable;
}

Boolean DictSortTarg::Selected(UInt elm) const {  
  return true;
}
  
//----------------------------------------------------------------------
//
// C++ Lifecycle
//

Dictionary::Dictionary() {
  StartUp();
}

Dictionary::Dictionary(Dictionary const &that) {
  CopyFrom(that);
}

Dictionary::~Dictionary() {
  ShutDown();
}

Dictionary &Dictionary::operator=(Dictionary const &that) {
  CopyFrom(that);
  return *this;
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle
//

Status Dictionary::StartUp() {

  Status retStatus;
  Status status;

  isConsumable = false;
  isSorted     = false;
  numWords     = 0;
  
  retStatus = fileName.StartUp();
  
  status = contents.StartUp();
  if (status && !retStatus) retStatus = status;  
  status = entryOffsets.StartUp();
  if (status && !retStatus) retStatus = status;
  status = sEntryOffsets.StartUp();
  if (status && !retStatus) retStatus = status;
  
  minWordLen = 0;
  maxWordLen = 0;
  totWordLen = 0;

  return retStatus;
  
}

Status Dictionary::ShutDown() {

  Status retStatus;
  Status status;
  
  retStatus = fileName.ShutDown();
  status = contents.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = entryOffsets.ShutDown();
  if (status && !retStatus) retStatus = status;
  status = sEntryOffsets.ShutDown();
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status Dictionary::Reset() {

  Status retStatus;
  Status status;
  
  retStatus = ShutDown();
  status = StartUp();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status Dictionary::MoveFrom(Dictionary const &that) {

  Status status;
  
  if (this == (Dictionary *)&that) goto FINISHED;
  
  isConsumable = that.isConsumable;
  isSorted     = that.isSorted;
  numWords     = that.numWords;
  
  status = fileName.MoveFrom(that.fileName);
  if (status) goto SOMEERROR;
  status = contents.MoveFrom(that.contents);
  if (status) goto SOMEERROR;
  
  status = entryOffsets.MoveFrom(that.entryOffsets);
  if (status) goto SOMEERROR;
  status = sEntryOffsets.MoveFrom(that.sEntryOffsets);
  if (status) goto SOMEERROR;
  
  minWordLen = that.minWordLen;
  maxWordLen = that.maxWordLen;
  totWordLen = that.totWordLen;

  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED:  status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

Status Dictionary::CopyFrom(Dictionary const &that) {

  Status status;
  
  if (this == (Dictionary *)&that) goto FINISHED;
  
  if (that.IsConsumable()) {
    status = MoveFrom(that);
    goto FINISHED;
  }
  
  isConsumable = false;
  isSorted     = that.isSorted;
  numWords     = that.numWords;
  
  status = fileName.CopyFrom(that.fileName);
  if (status) goto SOMEERROR;
  status = contents.CopyFrom(that.contents);
  if (status) goto SOMEERROR;
  
  status = entryOffsets.CopyFrom(that.entryOffsets);
  if (status) goto SOMEERROR;
  status = sEntryOffsets.CopyFrom(that.sEntryOffsets);
  if (status) goto SOMEERROR;
  
  minWordLen = that.minWordLen;
  maxWordLen = that.maxWordLen;
  totWordLen = that.totWordLen;

  // Handle errors
  status = Error::None;
  while (false) {
    FINISHED:  status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

Status Dictionary::WriteTo(Persist &store) {
  
  Status  status;
  Boolean started;
  String  string;
  Byte   *contentBuf;
  UInt32 *offsets;  
  
  started = false;
  
  // Start Instance Sequence
  status = store.StartSequenceInst(Class("Dictionary"));
  if (status) goto SOMEERROR;
  started = true;
  
  // Write main members...
  status = store.WriteScalar("isSorted",(Boolean)isSorted);
  if (status) goto SOMEERROR;
  
  // Get variable length member addresses
  contentBuf = contents.GetBuf();
  if (IsNull(contentBuf) && numWords) goto NULLPOINTER;
  offsets = (UInt *)entryOffsets._GetBuf();
  if (IsNull(offsets) && numWords) goto NULLPOINTER;
  
  // Write content strings
  status = store.WriteRaw("contents",contentBuf,contents.GetUsedBytes());
  if (status) goto SOMEERROR;
  
  // Write Entry Offsets...
  status = store.WriteScalars("wordOffsets",offsets,numWords);
  if (status) goto SOMEERROR;

  // Write extra members if sorted...
  
  // Handle errors
  while (false) {
    NULLPOINTER: BEFE_WARN("Dictionary.WriteTo: NULL pointer");
                 status = Error::InternalNullPointer;
                 break;
    SOMEERROR:   break;
  }
  
  // Finish Instance Sequence (if started)
  if (started) {
    if (status)
      store.AbortSequence();
    else
      store.EndSequence();
  }
  
  return status;
  
}

Status Dictionary::ReadFrom(Persist &store) {
  BEFE_WARN("Dictionary.ReadFrom not implemented yet");
  return Error::NotImplemented;
}

UInt Dictionary::Length() const {
  return numWords;
}

UInt Dictionary::Size() const {
  
  UInt size;
  
  size  = fileName.Size();
  size += contents.Size();
  size += entryOffsets.Size();
  size += sEntryOffsets.Size();

  return size;
  
}

Dictionary &Dictionary::Consumable() const {
  ((Dictionary *)this)->isConsumable = true;
  return *((Dictionary *)this);
}

Dictionary &Dictionary::NotConsumable() const {
  ((Dictionary *)this)->isConsumable = false;
  return *((Dictionary *)this);
}

Boolean Dictionary::IsConsumable() const {
  return (isConsumable)?true:false;
}

//----------------------------------------------------------------------
//
// More Persistence
//

Status Dictionary::ReadFromText(char const *fileName) {
  return ReadFromText(String(fileName));
}

Status Dictionary::ReadFromText(String const &theFileName) {

  Status   status;
  String   testPath;
  String   msg;

  File     file;
  String   tFileName;
  UInt     numBytes;
  Byte    *buf;
  UInt     numRead;
  Byte    *curByte;
  UInt     numRem;
  UInt    *entries;
  Byte    *lf;
  UInt     curLen;           // Current string length
  Byte    *cp;               // Temp: Character pointer
  UInt     wordLen;          // Length of current word
  Byte    *prevWord;         // Previous word (NULL=None)
  
  // Clear ourselves
  status = Reset();
  if (status) goto SOMEERROR;
  
  // Get and validate the test path
  testPath = TheBefe->TheProcess->GetEnvironmentValue("BEFE_TestData");
  testPath.Trim();
  if (testPath.Length() == 0) goto NOTESTDATA;
  if (!TheBefe->TheOS->Exists(testPath)) goto PNOTEXIST;
  if (!TheBefe->TheOS->IsDirectory(testPath)) goto PNOTDIR;
  fileName = TheBefe->TheOS->ApplyRelativePath(testPath,theFileName);
  if (!TheBefe->TheOS->Exists(fileName)) goto FNOTEXIST;
  if (!TheBefe->TheOS->IsFile(fileName)) goto FNOTFILE;

  // Load the file contents  
  file.SetName(fileName);
  numBytes = file.Size();
  status = contents.Allocate(numBytes+1);
  if (status) goto SOMEERROR;
  status = contents.SetSize(numBytes+1);
  if (status) goto SOMEERROR;
  buf = contents.GetBuf();
  if (IsNull(buf)) goto NULLBUF;  
  status = file.Read(0,buf, numBytes, numRead);
  if (status) goto SOMEERROR;
  if (numRead != numBytes) goto WRONGSIZE;
  buf[numBytes] = 0;
  
  // Count the lines
  curByte  = buf;
  numRem   = numBytes;
  numWords = 0;  
  while (numRem) {
    lf = (Byte *)Strchr(curByte, '\n');
    numWords++;
    if (!lf) break;
    numRem -= (lf - curByte) + 1;
    curByte = lf + 1;
    *lf = 0x00;
  }
  
  // Build the String pointers  (and check to see if it's sorted)
  status = entryOffsets.SetLength(numWords);
  if (status) goto SOMEERROR;
  entries = (UInt *)entryOffsets._GetBuf();
  if (IsNull(entries)) goto NULLBUF;
  curByte = buf;
  numRem  = numBytes;
  numWords   = 0;
  minWordLen = UNaN;
  maxWordLen = 0;
  totWordLen = 0;
  isSorted   = true;
  prevWord   = NULL;
  while (curByte-buf < (Int)numBytes) {
    // Set up this string
    entries[numWords] = curByte-buf;
    curLen = Strlen(buf+entries[numWords]);
    cp = curByte;
    // Skip leading whitespace (or any control characters)...
    while (*cp && *cp <= ' ') cp++;
    // If doesn't start with '//' or '#', keep it
    if (*cp && *cp != '#' && !(*cp == '/' && *(cp+1) == '/')) {
      // Truncate trailing whitespace (or any control characters)...
      if (curLen) {
        cp = curByte + curLen-1;
        while ((cp >= curByte) && *cp <= ' ') *cp-- = 0x00;
        wordLen = cp-curByte;
      }
      else
        wordLen = 0;
      // Accumulate stats
      if (wordLen) {
        totWordLen += wordLen;
        minWordLen = Min(minWordLen,wordLen);
        maxWordLen = Max(maxWordLen,wordLen);
      }
      // Bump to next word
      numWords++;
      // See if it's out of order
      if (isSorted && prevWord && Strcmpi(curByte,prevWord) < 0)
        isSorted = false;
    }
    // Position to next string
    prevWord = curByte;
    curByte += curLen + 1;
  }
  
  // Sort/select the lines if not sorted
  if (!isSorted) {
    status = SortMerge(DictSortTarg(*this),sEntryOffsets);
    if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOTESTDATA: msg = "Please set BEFE_TestData first";
                BEFE_WARN(msg);
                goto FAILED;
    PNOTEXIST:  msg = "Directory '"; msg += testPath; msg += "' does not exist";
                BEFE_WARN(msg);
                goto FAILED;
    PNOTDIR:    msg = "'"; msg += testPath; msg += "' is not a directory";
                BEFE_WARN(msg);
                goto FAILED;
    FNOTEXIST:  msg = "File '"; msg += testPath; msg += "' does not exist";
                BEFE_WARN(msg);
                goto FAILED;
    FNOTFILE:   msg = "'"; msg += testPath; msg += "' exists but is not a file";
                BEFE_WARN(msg);
                goto FAILED;
    SOMEERROR:  BEFE_ERROR("LoadStrings",status);
                goto FAILED;
    NULLBUF:    msg = "Buffer is NULL";
                BEFE_WARN(msg);
                goto FAILED;
    WRONGSIZE:  msg = "Expected ";
                msg += numBytes;
                msg += " Bytes, got ";
                msg += numRead;
                msg += '\n';
                BEFE_WARN(msg);
                goto FAILED;
    FAILED:     status = Error::Failed;
                break;
  }
  
  if (status) {
    BEFE_ERROR("MySortTarget: Building Strings",status);
    Reset();
  }
    
  return status;
  
}

//----------------------------------------------------------------------
//
// Entry query
//

Status Dictionary::Get(UInt index, String &entry) {
  
  Status  status;
  Byte   *entries;
  UInt   *offsets;
  UInt   *sOffsets;
  
  if (index > entryOffsets.Length()) goto OUTOFRANGE;
  entries = contents.GetBuf();
  if (IsNull(entries)) goto NULLPOINTER;
  offsets = (UInt *)entryOffsets._GetBuf();
  if (IsNull(offsets)) goto NULLPOINTER;
  if (isSorted) {
    entry = (char const *)(entries+offsets[index]);
  }
  else {
    sOffsets = (UInt *)sEntryOffsets._GetBuf();
    if (IsNull(sOffsets)) goto NULLPOINTER;
    entry = (char const *)(entries+offsets[sOffsets[index]]);
  }    
  status = entry._MorphTo(String::Buffer);
  if (status) goto SOMEERROR;
  
  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE:  status = Error::IndexOutOfRange;     break;
    SOMEERROR:                                        break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
  }
  
  return status;
  
}

String Dictionary::Get(UInt index) {
  
  //Status status;
  String string;
  
  /*status =*/ Get(index,string);
  
  return string.Consumable();
  
}

Status Dictionary::Find(String const &string, UInt &index) {
  BEFE_WARN("Dictionary.Find not implemented yet");
  return false;
}

Status Dictionary::FindInsensitive(String const &string, UInt &index) {
  BEFE_WARN("Dictionary.FindInsensitive not implemented yet");
  return false;
}

Boolean Dictionary::Exists(String const &string) {
  BEFE_WARN("Dictionary.Exists not implemented yet");
  return false;
}

Boolean Dictionary::ExistsInsensitive(String const &string) {
  BEFE_WARN("Dictionary.ExistsInsensitive not implemented yet");
  return false;
}
  
} // ...Namespace BEFE