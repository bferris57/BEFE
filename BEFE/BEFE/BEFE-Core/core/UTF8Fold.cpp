//----------------------------------------------------------------------
// File: UTF8Fold.cpp - Implementaion of the UTF8 class Folding
//----------------------------------------------------------------------
//
// These UTF8 Class methods are responsible for managing and performing
// Unicode Case Folding.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "UTF8.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Protected UTF8 Class static members
//
// Note: These actually need to be pushed into the Befe instance
//       instead of being kept as "static" here...
//

String      UTF8::foldFileName;
SparseChars UTF8::simpleFold;
SparseChars UTF8::simpleUnfold;
SparseChars UTF8::fullFold;

//----------------------------------------------------------------------
//
// Unicode Folding flags
//

Boolean UTF8::IsUnicodeFolding() {
  return gUnicodeFolding;
}

Boolean UTF8::IsTurkishFolding() {
  return gUnicodeTurkish;
}

Status UTF8::UnicodeFoldingOn() {
  
  Status status;
  
  if (!gUnicodeFolding) {
    status = LoadFolding();
    if (!status)
      gUnicodeFolding = true;
  }
  else
    status = Error::None;
  
  return status;
  
}

Status UTF8::UnicodeFoldingOff() {
  
  Status status;
  
  if (gUnicodeFolding) {
    gUnicodeFolding = false;
    status = UnloadFolding();
  }
  else
    status = Error::None;
  
  return status;
  
}

Status UTF8::TurkishFoldingOn() {
  gUnicodeTurkish = false;
  return Error::None;
}

Status UTF8::TurkishFoldingOff() {
  gUnicodeTurkish = true;
  return Error::None;
}

//----------------------------------------------------------------------
//
// Function: RemoveEmptyParts - Remove empty Ranges from list
//

static Status RemoveEmptyParts(Spans &parts) {
  
  Status status;
  UInt   numParts;
  UInt   partNo;
  Span   curPart;
  
  numParts = parts.Length();
  for (partNo=0; partNo < numParts;) {
    curPart = parts.Get(partNo);
    if (curPart.Length() == 0) {
      status = parts.Remove(partNo);
      if (status) goto SOMEERROR;
      numParts--;
    }
    else
      partNo++;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;
}

//----------------------------------------------------------------------
//
// Function: LineError - Report Fold file syntax error
//

static void LineError(String const &line, Int lineNo, Int colNo, String const &msg) {
  
  String lead;
  UInt   oldColour;
  
  if (gQuiet || !gShowWarnings) return;

  oldColour = Cout.SetForegroundColour(Console::Cyan);
  Cout << ToString(lineNo+1,4) << ": ";
  Cout.SetForegroundColour(oldColour);
  Cout << line << '\n';
  if (colNo >= 0) {
    lead = ' ';
    lead *= colNo;
    Cout << "      " << lead << "^\n";
  }
  Cout.SetForegroundColour(Console::Yellow);
  Cout << "UTF-8 Fold Warning: ";
  Cout.SetForegroundColour(oldColour);
  Cout << msg;
  Cout.SetForegroundColour(Console::Yellow);
  Cout << " (ignored)\n\n";
  Cout.SetForegroundColour(oldColour);
  
  return;
  
}

//----------------------------------------------------------------------
//
// Function: ParseChar - Parse a single Unicode character specification
//

static UInt ParseChar(String const &line, Span span) {
  
  String charString;
  String string;
  UInt   numChars;
  UInt   curPos;
  Char   curChar;
  UInt   theUInt;
  
  string = line.Get(span);
  if (string.Get(-1) == ';')
    string = string.Get(Slice(0,-1));
  numChars = string.Length();
  theUInt = 0;
  for (curPos=0; curPos < numChars; curPos++) {
    if (curPos > 5) goto BADCHAR;
    curChar = string.Get(curPos);
    theUInt <<= 4;
    if (curChar >= '0' && curChar <= '9')
      theUInt |= (char)curChar - '0';
    else if (curChar >= 'a' && curChar <= 'f')
      theUInt |= (char)curChar - 'a' + 10;
    else if (curChar >= 'A' && curChar <= 'F')
      theUInt |= (char)curChar - 'A' + 10;
    else
      goto BADCHAR;
  }
  
  while (false) {
    BADCHAR: theUInt = UNaN; break;
  }
  
  return theUInt;
}

//----------------------------------------------------------------------
//
// Externalising Unicode Case Folding (including Turkish)
//

Status UTF8::LoadFolding() {
  return LoadFolding(String());
}

Status UTF8::LoadFolding(String const &fileName) {

  Status status;
  String testFile;
  File   file;
  
  Lines  lines;  
  UInt   numLines;
  UInt   lineNo;
  String curLine;
  Int    hashPos;
  Span   hashSpan;
  Span   parseSpan;
  Spans  parseParts;
  String hashPart;
  String parsePart;
  UInt   numParts;
  UInt   partNo;
  UInt   numKept;

  String part1;
  Char   char0;
  Char   char2;
  
  SparseChars foldCommon;
  SparseChars unfoldCommon;
  SparseChars foldSimple;
  SparseChars unfoldSimple;
  SparseChars foldFull;
  UInt        fullMaxChars(sizeof(UInt));
  
  UInt   oldColour;
  UInt   numWarn;
  Time   t1;
  Time   t2;
  UInt   foldNo;
  
  oldColour = Cout.GetForegroundColour();

  // Get default file if none specified...
  testFile = fileName;
  if (testFile.Length() == 0) {
    testFile = "BEFE_CaseFolding.txt";
    status = TheBefe->FindConfigurationFile(testFile);
    if (status) goto SOMEERROR;
    if (testFile.Length() == 0) goto CANTFINDFILE;
  }

  // Display what we're doing
  if (gShowInfo) {
    String tString;
    tString = testFile.MidEllipsis(40);
    Cout << "INFO: UTF8.LoadFolding file '" << tString << "'...\n";
  }
  
  // Read the lines
  t1.Set();
  if (!TheBefe->TheOS->Exists(testFile)) goto FILENOTEXIST;
  if (!TheBefe->TheOS->IsFile(testFile)) goto FILENOTFILE;
  file.SetName(testFile);
  status = file.ReadLines(lines);
  numLines = lines.Length();

  // Pre-allocate Array/Sparse sizes based on rough guess...
  status = foldCommon.SetLength(numLines);
  if (status) goto SOMEERROR;  
  status = unfoldCommon.SetLength(numLines);
  if (status) goto SOMEERROR;  
  status = foldSimple.SetLength(numLines);
  if (status) goto SOMEERROR;  
  status = unfoldSimple.SetLength(numLines);
  if (status) goto SOMEERROR;  
  status = foldFull.SetLength(numLines);
  if (status) goto SOMEERROR;  

  //
  // Process the lines
  //
  
  numKept = 0;
  numWarn = 0;
  parseParts.SetLength(10); // ◄── To keep from too many extra allocations below
  fullMaxChars = 0;
  
  for (lineNo=0; lineNo < numLines; lineNo ++) {
    
    // Get the current Line
    curLine = lines.GetLine(lineNo);
    
    // Strip off trailing '\r' if any
    if (curLine.Get(-1) == '\r')
      curLine = curLine.Get(Slice(0,-1));
    
    // Get Hash Part and Parse Part
    hashPos = curLine.Find('#');
    if (hashPos >= 0) {
      hashSpan  = Span(hashPos,NaN);
      parseSpan = Span(0,hashPos);
    }
    else {
      hashSpan  = Span(curLine.Length(),NaN);
      parseSpan = Span(0,curLine.Length());
    }
    hashPart = curLine.Get(hashSpan);
    parsePart = curLine.Get(parseSpan);
    
    // Ignore empty lines
    if (parseSpan.Length() == 0) continue;

    // Split the line and remove empty parts
    status = parsePart.SplitToSpans(' ',parseParts);
    if (status) goto SPLITERROR;
    status = RemoveEmptyParts(parseParts);
    if (status) goto REMOVEEMPTY;
    numParts = parseParts.Length();
    if (numParts < 3) {
      LineError(curLine, lineNo, parseParts.Get(numParts-1).idx2+1, "Expected at least three parts in the Folding specification");
      numWarn++;
      continue;
    }

    // Get the basic chars and parts
    char0 = ParseChar(curLine,parseParts.Get(0));
    part1 = curLine.Get(parseParts.Get(1));
    char2 = ParseChar(curLine,parseParts.Get(2));
    if (!char0.IsUnicode()) {
      LineError(curLine, lineNo, parseParts.Get(0).idx1, "Invalid Unicode Character Specification");
      numWarn++;
      continue;
    }
    if (!char2.IsUnicode()) {
      LineError(curLine, lineNo, parseParts.Get(2).idx1, "Invalid Unicode Character Specification");
      numWarn++;
      continue;
    }
    
    // Handle Common Folding
    if (part1 == "C;") {
      
      // Make sure it's not already there
      if (foldCommon.Exists(char0)) {
        LineError(curLine, lineNo, parseParts.Get(0).idx1, "Character already Common Folded");
        numWarn++;
        continue;
      }
      
      // Add it to the Common Folding Sparse...
      status = foldCommon.Append(char0, char2);
      if (status) goto APPENDERROR;
      
      // Add to Common Unfolding Sparse (Warn if alreay there)...
      if (!unfoldCommon.Exists(char2)) {
        status = unfoldCommon.Append(char2, char0);
        if (status) goto APPENDERROR;
      }
      else {
        LineError(curLine, lineNo, parseParts.Get(2).idx1, "Character already Common Unfolded");
        numWarn++;
      }
            
    }
    
    // Handle Simple Folding
    else if (part1 == "S;") {

      // Make sure it's not already there
      if (foldCommon.Exists(char0) || foldSimple.Exists(char0)) {
        LineError(curLine, lineNo, parseParts.Get(0).idx1, "Character already Simple/Common Folded");
        numWarn++;
        continue;
      }
      
      // Add it to the Simple Folding Sparse...
      status = foldSimple.Append(char0, char2);
      if (status) goto APPENDERROR;
      
      // Add to Special Unfolding Sparse (Warn if alreay there)...
      if (!unfoldSimple.Exists(char2)) {
        status = unfoldSimple.Append(char2, char0);
        if (status) goto APPENDERROR;
      }
      else if (!gQuiet && gVeryVerbose) {
        LineError(curLine, lineNo, parseParts.Get(2).idx1, "Character already Simple/Common Unfolded");
        numWarn++;
      }
      
    }
    
    // Handle Full Folding
    else if (part1 == "F;") {
      
      fullMaxChars = Max(fullMaxChars, numParts - 2);
      
      // Validate characters before we try inserting
      for (partNo=2; partNo < numParts; partNo++) {
        char2 = ParseChar(curLine,parseParts.Get(partNo));
        if (!char2.IsUnicode()) {
          LineError(curLine, lineNo, parseParts.Get(partNo).idx1, "Invalid Unicode Character Specification");
          numWarn++;
          continue;
        }
      }
      
      // Make sure it's not already there
      if (foldFull.Exists(char0)) {
        LineError(curLine, lineNo, parseParts.Get(0).idx1, "Character already Full Folded");
        numWarn++;
        continue;
      }
      
      // Add each of them
      for (partNo=2; partNo < numParts; partNo++) {
        char2 = ParseChar(curLine,parseParts.Get(partNo));
        status = foldFull.Append(char0, char2);
        if (status) goto APPENDERROR;
      }
      
    }
    
    // Handle Turkish Folding
    else if (part1 == "T;") {

      continue;
      
    }
    
    // Unknown Folding...
    else {
      LineError(curLine,lineNo,parseParts.Get(1).idx1,"Unknown Unicode Folding Type");
      continue;
    }
    
    // Display info
    if (gShowInfo && gVeryVerbose) {
      Cout << "INFO: ";
      Cout.SetForegroundColour(Console::Cyan);
      Cout << ToString(lineNo+1,4) << ": ";
      Cout.SetForegroundColour(oldColour);
      Cout << curLine << " (" << parseParts.Length() << " parts)\n";
    }
    
    // Keep this line    
    numKept++;
  }

  // Now we know their sizes...
  status = foldCommon.SetLength(foldCommon.Length());
  if (status) goto SOMEERROR;  
  status = unfoldCommon.SetLength(unfoldCommon.Length());
  if (status) goto SOMEERROR;  
  status = foldSimple.SetLength(foldSimple.Length());
  if (status) goto SOMEERROR;  
  status = unfoldSimple.SetLength(unfoldSimple.Length());
  if (status) goto SOMEERROR;  
  status = foldFull.SetLength(foldFull.Length());
  if (status) goto SOMEERROR;  

  // Everything's okay, save the Simple Folding map...
  simpleFold.Reset();
  status = simpleFold.SetLength(foldCommon.Length() + foldSimple.Length());
  if (status) goto SOMEERROR;
  for (foldNo=0; foldNo < foldCommon.Length(); foldNo++) {
    status = foldCommon.GetPhysical(foldNo, char0.value, char2);
    if (status) goto SOMEERROR;
    status = simpleFold.Append(char0.value, char2);
    if (status) goto SOMEERROR;
  }
  for (foldNo=0; foldNo < foldSimple.Length(); foldNo++) {
    status = foldSimple.GetPhysical(foldNo, char0.value, char2);
    if (status) goto SOMEERROR;
    status = simpleFold.Append(char0.value, char2);
    if (status) goto SOMEERROR;
  }
  
  // Save the Simple Unolding map...
  simpleUnfold.Reset();
  status = simpleUnfold.SetLength(unfoldCommon.Length() + unfoldSimple.Length());
  if (status) goto SOMEERROR;
  for (foldNo=0; foldNo < unfoldCommon.Length(); foldNo++) {
    status = unfoldCommon.GetPhysical(foldNo, char0.value, char2);
    if (status) goto SOMEERROR;
    status = simpleUnfold.Append(char0.value, char2);
    if (status) goto SOMEERROR;
  }
  for (foldNo=0; foldNo < unfoldSimple.Length(); foldNo++) {
    status = unfoldSimple.GetPhysical(foldNo, char0.value, char2);
    if (status) goto SOMEERROR;
    status = simpleUnfold.Append(char0.value, char2);
    if (status) goto SOMEERROR;
  }
  
  // Save the Full Folding map...
  fullFold.Reset();
  status = fullFold.SetLength(foldCommon.Length() + foldFull.Length());
  if (status) goto SOMEERROR;
  for (foldNo=0; foldNo < foldCommon.Length(); foldNo++) {
    status = fullFold.Append(char0.value, char2);
    if (status) goto SOMEERROR;
  }
  for (foldNo=0; foldNo < foldFull.Length(); foldNo++) {
    status = fullFold.Append(char0, char2);
    if (status) goto SOMEERROR;
  }
  
  t2.Set();

  // Save Global Settings...
  TheBefe->TheGlobals.uints.UnicodeCurFullFoldChars = fullMaxChars;
  
  // Display stats
  if (gShowInfo && gVerbose) {
    Cout << "INFO:   Common.......Folds..: " << foldCommon.Length()       << '\n';
    Cout << "INFO:   Common.......Unfolds: " << unfoldCommon.Length()     << '\n';
    Cout << "INFO:   Simple.......Folds..: " << foldSimple.Length()       << '\n';
    Cout << "INFO:   Simple.......Unfolds: " << unfoldSimple.Length()     << '\n';
    Cout << "INFO:   Full.........Folds..: " << foldFull.DistinctLength() << " (" << foldFull.Length() << " entries)\n";
    Cout << "INFO:   Full.....Max Chars..: " << fullMaxChars              << '\n';
    Cout << "INFO: \n";
    Cout << "INFO:   Total Simple Folds..: " << simpleFold.Length()       << '\n';
    Cout << "INFO:   Total Simple Unfolds: " << simpleUnfold.Length()     << '\n';
    Cout << "INFO:   Total Full...Folds..: " << fullFold.DistinctLength() << '\n';
    Cout << "INFO: \n";
    Cout << "INFO:   Elapsed Time........: " << ToString(t1,t2) << '\n';
  }
    
  // Handle errors
  status = Error::None;
  while (false) {
    CANTFINDFILE: BEFE_WARN("Cannot locate file 'BEFE_CaseFolding.txt'");
                  status = Error::Failed;
                  break;
    FILENOTEXIST: Cout << "Error opening UTF-8 Folding file '" << testFile << "'...\n";
                  BEFE_WARN("File does not exist");
                  status = Error::Failed;
                  break;
    FILENOTFILE:  Cout << "Error opening UTF-8 Folding file '" << testFile << "'...\n";
                  BEFE_WARN("File exists but is not a file");
                  status = Error::Failed;
                  break;
    SPLITERROR:   BEFE_ERROR("UTF8.LoadFolding->String.SplitToRanges",status); break;
    REMOVEEMPTY:  BEFE_ERROR("UTF8.LoadFolding->RemoveEmptyParts",status);     break;
    APPENDERROR:  BEFE_ERROR("UTF8.LoadFolding->Sparse.Append",status);        break;
    SOMEERROR:    BEFE_ERROR("UTF8.LoadFolding",status);                       break;
  }
  
  // If error, clear the UTF8 folds
  if (status) {
    UTF8::simpleFold.Reset();
    UTF8::simpleUnfold.Reset();
    UTF8::fullFold.Reset();
  }
  
  return status;
  
}

Status  UTF8::UnloadFolding() {
  
  Status retStatus;
  Status status;
  
  retStatus = foldFileName.Reset();
  status = simpleFold.Reset();
  if (status && !retStatus) retStatus = status;
  status = simpleUnfold.Reset();
  if (status && !retStatus) retStatus = status;
  status = fullFold.Reset();
  if (status && !retStatus) retStatus = status;

  // Clear Globals
  TheBefe->TheGlobals.uints.UnicodeCurFullFoldChars = UNaN;
  
  return retStatus;
  
}

//----------------------------------------------------------------------
//
// Handy Unicode Case Folding
//

Char UTF8::Fold(Char in) {
  Char out;
  FoldSimple(in, out);
  return out;
}

Char UTF8::Unfold(Char in) {
  Char out;
  UnfoldSimple(in, out);
  return out;
}

//----------------------------------------------------------------------
//
// Base Unicode Case Folding
//

Status UTF8::FoldSimple(Char in, Char &out) {
  
  Status status;
  
  // Special case if no input file...
  if (foldFileName.Length() == 0) {
    out = in;
    if (in.value >= 'A' && in.value <= 'Z')
      out.value = in.value - 'A' + 'a';
    goto OK;
  }
  
  // If Turkish...
  if (gUnicodeTurkish) {
    if (in == (Char)0x0130u) {
      out = (Char)0x0131u;
      goto OK;
    }
  }
  
  // Try Simple...
  status = simpleFold.Get(in.value,out);
  if (status && status != Error::IndexNotFound) goto SOMEERROR;
  if (status)
    out = in;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  
  return status;

}

Status UTF8::FoldSimple(String const &in, String &out) {
  
  Status status;
  UInt   numChars;
  UInt   charNo;
  Char   curChar;
  Char   newChar;
  Byte   utfBuf[MaxLength];
  UInt   utfBufLen;
  
  numChars = in.Length();
  status = out.Reset();
  if (status) goto SOMEERROR;
  //status = out.SetLength(numChars);
  //if (status) goto SOMEERROR;
  
  // For each Char...
  for (charNo=0; charNo < numChars; charNo++) {
    curChar = in.Get(charNo);
    status = FoldSimple(curChar,newChar);
    if (status) goto SOMEERROR;
    status = EncodeChar(newChar, utfBuf, utfBufLen);
    if (status) goto SOMEERROR;
    status = out.Append(utfBuf,utfBufLen);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;
  
}

Status UTF8::FoldFull(Char in, String &out) {
  BEFE_WARN("UTF8.FoldFull(Char, String &) not implemented yet");
  return Error::NotImplemented;
}

Status UTF8::FoldFull(String const &in, String &out)  {
  BEFE_WARN("UTF8.FoldFull(String const &, String &) not implemented yet");
  return Error::NotImplemented;
}

//----------------------------------------------------------------------
//
// Base Unicode Case Unfolding
//

Status UTF8::UnfoldSimple(Char in, Char &out) {
  
  Status status;

  // Special case if no input file...
  if (foldFileName.Length() == 0) {
    out = in;
    if (in.value >= 'a' && in.value <= 'z')
      out.value = in.value - 'a' + 'A';
    goto OK;
  }
  
  // If Turkish...
  if (gUnicodeTurkish) {
    if (in == (Char)0x0131u) {
      out = (Char)0x0130u;
      goto OK;
    }
  }
  
  // Try Simple...
  status = simpleUnfold.Get(in.value,out);
  if (status && status != Error::IndexNotFound) goto SOMEERROR;
  if (status)
    out = in;
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR:                       break;
    OK:        status = Error::None; break;
  }
  
  return status;

}

Status UTF8::UnfoldSimple(String const &in, String &out) {
  
  Status status;
  UInt   numChars;
  UInt   charNo;
  Char   curChar;
  Char   newChar;
  Byte   utfBuf[MaxLength];
  UInt   utfBufLen;
  
  numChars = in.Length();
  status = out.Reset();
  if (status) goto SOMEERROR;
  
  // For each Char...
  for (charNo=0; charNo < numChars; charNo++) {
    curChar = in.Get(charNo);
    status = UnfoldSimple(curChar,newChar);
    if (status) goto SOMEERROR;
    status = EncodeChar(newChar, utfBuf, utfBufLen);
    if (status) goto SOMEERROR;
    status = out.Append(utfBuf,utfBufLen);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;
  
}

Status UTF8::UnfoldFull(Char in, String &out) {
  BEFE_WARN("UTF8.UnfoldFull(Char, String &) not implemented yet");
  return Error::NotImplemented;
}

Status UTF8::UnfoldFull(String const &in, String &out)  {
  BEFE_WARN("UTF8.UnfoldFull(String const &, String &) not implemented yet");
  return Error::NotImplemented;
}

} // ...Namespace BEFE