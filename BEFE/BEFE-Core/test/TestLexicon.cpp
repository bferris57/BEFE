//!befe-publish src
//----------------------------------------------------------------------
// File: TestLexicon.cpp - Implementation of the TestLexicon Class
//----------------------------------------------------------------------
//
// The TestLexicon Class tests the functionality of Lexical Analysis
// related classes.
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEWrappers.h"   // For general purpose BEFE "Wrapper" functions
#include "Globals.h"
#include "BEFEMacros.h"
#include "ToStrings.h"
#include "Test.h"

#include "Lex.h"
#include "LexLang.h"
#include "LexWorkspace.h"
#include "TestLexicon.h"
#include "LexFormat.h"
#include "Bmsg.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Function: LexPathWalker - Lexical Analysis path walker
//

Status LexPathWalker(String const &dir, Ptr context) { // LexPathWalker...
  
  Status   status;
  Strings *theFiles;
  String   curDir;
  Strings  fileNames;
  String   fileName;
  String   fileNameOnly;
  UInt     curIdx;
  
  theFiles = (Strings *)context;
  if (BEFE::IsNull(theFiles)) goto NULLPOINTER;
  curDir = dir;
  status = PathToGeneric(curDir);
  if (status) goto SOMEERROR;
  status = GetFiles(curDir, fileNames);
  if (status) goto SOMEERROR;

  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    
    // Get fhe file name...
    fileName = fileNames.Get(curIdx);
    status = PathToGeneric(fileName);
    if (status) goto SOMEERROR;
    fileNameOnly = FileOnly(fileName);

    // We want this file...
    if (fileName.Get(Range(-2,-1)) == ".h" || fileName.Get(Range(-4,-1)) == ".cpp") {
      status = theFiles->Append(fileName.Consumable());
      if (status) goto SOMEERROR;
    }

  }
  
  // Handle errors  
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...LexPathWalker

//----------------------------------------------------------------------
//
// Function: LexBlobPathWalker - Lexical Workspace Blob path walker
//

struct LexFilePathWalkerContext {
  LexWorkspace *ws;
  String        indent;
  UInt          counter;
};

Status LexFilePathWalker(String const &dir, Ptr context) { // LexFileWalker...
  
  Status  status;
  String  curDir;
  Strings fileNames;
  String  fileName;
  String  fileNameOnly;
  UInt    curIdx;
  Id      fileId;
  
  LexFilePathWalkerContext *ctxt;
  
  ctxt = (LexFilePathWalkerContext *)context;
  if (BEFE::IsNull(ctxt)) goto NULLPOINTER;
  curDir = dir;
  status = PathToGeneric(curDir);
  if (status) goto SOMEERROR;
  status = GetFiles(curDir, fileNames);
  if (status) goto SOMEERROR;

  for (curIdx=0; curIdx < fileNames.Length(); curIdx++) {
    
    // Get fhe file name...
    fileName = fileNames.Get(curIdx);
    status = PathToGeneric(fileName);
    if (status) goto SOMEERROR;
    fileNameOnly = FileOnly(fileName);

    // Ignore if we don't want this file...
    if (fileName.Get(Range(-2,-1)) != ".h" && fileName.Get(Range(-4,-1)) != ".cpp") continue;

    // Show what we're doing...
    if (gVeryVerbose)
      Cout << ctxt->indent << "File[" << ctxt->counter << "]: " << fileName << "...\n";
    ctxt->counter++;
      
    // Create the Workspace File Id...
    status = ctxt->ws->FileNew(fileName, fileId);
    if (status) goto SOMEERROR;
    if (gVeryVeryVerbose)
      Cout << ctxt->indent << "   File Id 0x" << ToHexString(fileId,8) << " for file '" << fileName << "'\n";
      
  }
  
  // Handle errors  
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;
  
} // ...LexBlobPathWalker

//----------------------------------------------------------------------
//
// Function: testLexLangCpp - Test C++ Lexical Analysis
//

Status testLexLangCpp() { // testLexLangCpp...
  
  Status status;
  Lex          lex;
  LexStream    stream;
  TestLexicon  test;
  String       indent;

  indent = ' ';
  indent *= 2;
  
  if (!gQuiet)
    Cout << indent << "Testing C++ language...\n";

  indent *= 4;
  
  // Specify C++ LexLanguage...
  {
    String       searchPath;
    Lex          lex;
    LexLanguage  lang;
    LexStream    stream;
    LexToken     token;
    UInt         tokNo;
    LexFormatOut fmt;
    
    status = LexLangCpp(lang);
    
    //
    // Parse something...
    //
    
    if (true) {
      char const *types[] = {
          "Unknown"        // 00: Unknown (initial value)
         ,"EndOfInput"     // 01: Input exhausted
         ,"EndOfLine"      // 02: Typically '\n'
         ,"Error"          // 03: Typically Error::None
         ,"Unexpected"     // 04: Character not recognised
         ,"Warning"        // 05: Typically Error::None
         ,"WhiteSpace"     // 06: Whitespace (typically 0x00..0x20)
         ,"Operator"       // 07: Some operator
         ,"BracketLeft"    // 08: Beginning of some grouping
         ,"BracketRight"   // 09: End of some grouping
         ,"Separator"      // 10: List separator
         ,"Comment"        // 11: Comment
         ,"Name"           // 12: The "Name" of something
         ,"Number"         // 13: Number literal
         ,"DLiteral"       // 14: Double-quote literal
         ,"SLiteral"       // 15: Single-quote literal
      	 ,"BLiteral"       // 16: Back-quote literal
         ,"MultiQuote"     // 17: Multi-line String literal
      	 ,"NameLiteral"    // 18: Befe Name literal
         ,"Preprocessor"   // 19: C/C++/Befe Preprocessor Directive
				 ,0
      };
      UInt    maxTypeLen;
      Time    start;
      Time    finish;
      Strings theFiles;
      UInt    i;
      UInt    thisFileLines;
      String  tStr;
      
      UInt    totFileSize;
      UInt    totFileLines;
      
      maxTypeLen = 0;
      for (i=0; types[i]; i++)
        maxTypeLen = Max(maxTypeLen,Strlen(types[i]));

      if (true) {
        searchPath = GetEnvironmentValue("BEFE_GIT");
        status = WalkPath(searchPath, LexPathWalker, (Ptr)&theFiles);
        if (status) goto SOMEERROR;
      }
      else
        theFiles.Append("C:/GIT/BEFE-Core/lang/Blang.h");
        
      if (gVerbose){
        i = theFiles.Length();
        Cout << indent 
             << "There " << ((i==1)?"is":"are") << ' ' << i
             << " file" << ((i==1)?"":"s") << " to process...\n";
        Cout.Flush();
      }
      
      status = lex.RegisterLanguage(lang);
      if (status) goto SOMEERROR;
      start.Set();
      status = lex.Attach(stream);
      if (status) goto SOMEERROR;
      status = stream.SetLanguageName("C++");
      //status = stream.SetLanguageName(".btyp");
      if (status) goto SOMEERROR;

      tokNo = 0;
      totFileSize = 0;
      totFileLines = 0;
      
      for (i=0; i < theFiles.Length(); i++) { // For each file...

        if (gVeryVerbose)
          Cout << indent << "File: " << theFiles.Get(i) << "...\n";
        
        status = stream.SetInputFileName(theFiles.Get(i));
        if (status) goto SOMEERROR;
        
        status = stream.CountInputLines(thisFileLines);
        if (status) goto SOMEERROR;
        totFileLines += thisFileLines;
        
        while (true) {
          status = stream.NextToken(token);
          if (status) {
            UInt fg;
            fg = Cout.SetForegroundColour(Console::Red);
            if (!BEFE::IsNull(token.lineNo)) token.lineNo++;
            if (!BEFE::IsNull(token.colNo))  token.colNo++;
            Cout << indent 
                 << "Error at line/col " << token.lineNo << '/' << token.colNo
                 << ": "
                 << ErrorString(status) << '\n';
            Cout.SetForegroundColour(fg);
            goto OK;
          }
          if (token.type == LexToken::EndOfInput) {
            if (!BEFE::IsNull(stream.GetInputSize()))
              totFileSize += stream.GetInputSize();
            break;
          }
          if (gVeryVeryVerbose) {

          	String typeName;

            if (token.type >= LexToken::LexTokenType::NumBaseTypes)
              typeName = String("0x") << ToHexString((UInt)token.type,2);
            else
              typeName = types[token.type];

            if (!BEFE::IsNull(token.lineNo)) token.lineNo++;
            if (!BEFE::IsNull(token.colNo))  token.colNo++;
            Cout << indent 
                 << "Token[" << tokNo
                 << "]: type = "
                 << typeName
                 << String(' ')*(maxTypeLen-Strlen(types[token.type]))
                 << " line/col = " << token.lineNo << '/' << token.colNo
                 << " size = "
                 << ToString(token.span.Length())
                 << " span = "
                 << ToString(token.span);
            if (gVeryVerbose) {
              if (!token.value.IsNull())
                tStr = String("->'") + token.value + '\'';
              else {
                tStr = stream.GetInputSpan(token.span);
                tStr.Escape(tStr, false);
                tStr = String('\'') + tStr + '\'';
              }
              Cout << " value = " << tStr;
            }
            Cout << '\n';

          }
          tokNo++;
          
          // Special for big.xml...
          if (false) {
            if (tokNo%25000 == 0)
              (Cout << '.').Flush();
          }
        }
   
      } // ...For each file
   
      finish.Set();
      if (gVerbose) {
        Cout << indent << "Parsing Elapsed:        " << ToString(start,finish) << '\n';
        Cout << indent << "Total Tokens:           " << tokNo << '\n';
        Cout << indent << "Total Bytes Processed:  " << totFileSize << '\n';
        Cout << indent << "Total Lines Proccessed: " << totFileLines << '\n';
      }
      
    }

  }
    
  // Start doing something
  if (false) {
    if (gVeryVerbose)
      Cout << indent << "Before lex.Attach: stream.blkSize = " << stream.blkSize << '\n';
    status = lex.Attach(stream);
    if (status) BEFE_ERROR("Lex.Attach",status);
    if (gVeryVerbose)
      Cout << indent << "After  lex.Attach: stream.blkSize = " << stream.blkSize << '\n';
    status = lex.Detach(stream);
    if (status) BEFE_ERROR("Lex.Detach",status);
  }
  
  // Do some tests...
  if (false) {
    status = test.Run();
    if (status) BEFE_ERROR("testLexLangCpp", status);
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  if (status) BEFE_ERROR("testLexLangCpp", status);
  
  return status;
  
} // ...testLexLangCpp

//----------------------------------------------------------------------
//
// Function: testLexWorkspace - Test LexWorkspace Class
//

Status testLexWorkspace() { // testLexWorkspace...

  Status       status;
  String       befeRoot;
  String       befeGIT;
  String       wsFileName;
  String       inFileName;
  Lex          lex;
  LexStream    stream;
  LexWorkspace ws;
  String       indent;
  UInt         i;           // Temp: Loop Counter
  
  indent = ' ';
  indent *= 2;
  if (!gQuiet)
    Cout << indent << "Testing LexWorkspace...\n";  
  
  // Get and validate settings...  
  befeRoot = GetEnvironmentValue("BEFE_Root");
  if (befeRoot.Length() == 0) goto NOROOT;
  if (!IsDirectory(befeRoot)) goto NOTDIR;
  befeGIT = GetEnvironmentValue("BEFE_GIT");
  if (befeGIT.Length() == 0) goto NOGIT;
  if (!IsDirectory(befeGIT)) goto NOTDIR;
  status = PathToGeneric(befeRoot);
  if (status) goto SOMEERROR;
  status = PathToGeneric(befeGIT);
  if (status) goto SOMEERROR;
  
  // Setup Input and Workspace file names...  
  wsFileName = befeRoot + "/testLexWorkspace.befe";
  inFileName = befeGIT + "/core/String.h";
  
  // Delete workspace if already there...
  if (Exists(wsFileName)) {
    status = DeleteFile(wsFileName);
    if (status) goto SOMEERROR;
  }
  
  // Set up the LexWorkspace...
  status = ws.SetFileName(wsFileName);
  if (status) goto SOMEERROR;
  
  // Make sure input file is there...
  if (!Exists(inFileName)) goto NOTEXIST;
  if (!IsFile(inFileName)) goto NOTFILE;
  
  // Stitch all the Lex stuff together...
  status = lex.Attach(stream);
  if (status) goto SOMEERROR;
  status = lex.Attach(ws);
  if (status) goto SOMEERROR;
  
  //
  // Test LexWsBlockHead...
  //
  
  if (false) {

    LexWsBlockHead blockHead;
    Strings        strs;
    UInt           i;
    
    indent *= 2;
    Cout << indent << "Testing LexWsBlockHead...\n";    
    indent *= 4;
    
    status = ws._Open();
    if (status) goto SOMEERROR;
    blockHead.type = LexWs::BlockTypeBlob;
    if (gVeryVerbose) {
      Cout << indent  << "blockHead.PersistSize() = " << blockHead.PersistSize() << '\n';
      Cout << indent  << "Fake Block Header...\n";
      strs = blockHead.ToStrings();
      for (i=0; i < strs.Length(); i++)
        Cout << indent  << "  " << strs.Get(i) << '\n';
    }
    
  }

  //
  // Test LexWorkspace NewFileBlock and FreeFileBlock methods...
  //
  
  if (false) {
    
    UInt blockNo;
    
    indent *= 2;
    Cout << indent << "Testing LexWorkspace NewFileBlock and FreeFileBlock...\n";    
    indent *= 4;
    
    status = ws.NewFileBlock(LexWs::BlockTypeBlob, blockNo);
    if (status) goto SOMEERROR;
    Cout << indent  << "New block no: 0x" << ToHexString(blockNo) << '\n';
    status = ws.FreeFileBlock(blockNo);
    if (status) goto SOMEERROR;
    
  }

  // Test LexWorkspace Blob methods
  if (false) {
    
    String         sData;
    Bytes          bData;
    Id             blobId;
    LexWsArrayHead aHead;
    Boolean        exists;
    
    indent *= 2;
    Cout << indent << "Testing LexWorkspace Blobs...\n";    
    indent *= 4;
    
    sData = "Hello world!";
    bData = sData.MorphToBytes();
    Cout << indent << "bData.Length()      = " << bData.Length() << '\n';
    Cout << indent << "Initial Blobs       = " << ws.BlobCount() << '\n';
    Cout << indent << "blobs.PersistSize() = " << aHead.PersistSize() << '\n';
    // Make some blobs
    for (i=0; i < 200; i++) {
      status = ws.BlobNew(blobId);
      if (status)
        BEFE_ERROR(String("ws.BlobNew[") << i << ']', status);
      if (status) goto SOMEERROR;
      //if (gVeryVerbose)
      //  Cout << indent << "blob[" << i << "].Id = 0x" << ((PtrInt)blobId) << '\n';
    }
    
    exists = ws.BlobExists((Id)5);
    Cout << indent << "blob[id=0x00000005] exists = " << (exists?CONSTR_True:CONSTR_False) << '\n';
    exists = ws.BlobExists((Id)(i*2));
    Cout << indent << "blob[id=0x" << ToHexString(i*2,8) << "] exists = " << (exists?CONSTR_True:CONSTR_False) << '\n';

    // Write a little data to blob Id=0x00000001
    status = ws.BlobAppend((Id)1, bData.GetBuf(), bData.Size());
    if (status) goto SOMEERROR;
    
  }
  
  // Test Workspace File methods...
  if (false) {

    String fileName;
    Id     fileId;
    
    indent *= 2;
    Cout << indent << "Testing LexWorkspace Files...\n";    
    indent *= 4;

    fileName = "C:/GIT/BEFE-Core/core/String.h";
    status = ws.FileNew(fileName, fileId);
    if (status) goto SOMEERROR;
    if (gVerbose)
      Cout << indent << "File Id 0x" << ToHexString(fileId,8) << " for file '" << fileName << "'\n";
      
  }
  
  // Test More Workspace File methods...
  if (true) {

    String                   dirName;
    LexFilePathWalkerContext ctxt;
    
    indent *= 2;
    Cout << indent << "Testing LexWorkspace Files (Many Files)...\n";    
    indent *= 4;

    dirName = "C:/GIT/BEFE-Core";
    
    ctxt.ws      = &ws;
    ctxt.indent  = indent;
    ctxt.counter = 0;
    
    status = WalkPath(dirName, LexFilePathWalker, (Ptr)&ctxt);
    if (status) goto SOMEERROR;

    Cout << indent << "Files processed: " << ctxt.counter << '\n';
        
  }

  if (false) { // Test Workspace String methods...

    char const *inputStrings[] = {
      "Hello World!",
      "Hello again!",
      "And another string",
      "***Hey there ◄─► this is fun***",
      "And another one",
      "String 6",
      "String 7",
      "String 8",
      "String 9",
      "String 10",
      0};
    UInt   i;
    Id     theIds[10];
    UInt   numInputStrings;
    UInt   numFileStrings;
    Id     strId;
    String theString;
    
    indent *= 2;
    Cout << indent << "Testing LexWorkspace String...\n";    
    indent *= 4;
    
    numFileStrings = ws.StringCount();
    if (gVerbose)
      Cout << indent << "Before New:    count = " << numFileStrings << '\n';

    // Create the Workspace Strings...      
    numInputStrings = 0;
    for (i=0; inputStrings[i]; i++) {
      numInputStrings++;
      theString = inputStrings[i];
      status = ws.StringNew(theString, theIds[i]);
      if (status) goto SOMEERROR;
      if (gVerbose)
        Cout << indent << "New String Id: 0x" << ToHexString(theIds[i],8) << '\n';
    }
    
    // Display count
    numFileStrings = ws.StringCount();
    if (gVerbose)
      Cout << indent << "After New:     count = " << numFileStrings << '\n';

    // Get the Workspace Strings...
    for (i=0; i < numInputStrings; i++) {
      status = ws.StringGet(theIds[i], theString);
      if (status) goto SOMEERROR;
      if (gVerbose)
        Cout << indent << "string[" << i << "]:     '" << theString << "'\n";
    }
    
  } // ...Test Workspace String methods
  
  // Display current LexWorkspace
  if (!gQuiet) {
    
    LexFormatOut fmt;
    
    indent *= 2;
    fmt.SetIndent(indent.Length()*indent.Multiplier());
    fmt.Display(ws);
    
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    NOROOT:    status = Error::NoBEFE_Root;         break;
    NOGIT:     status = Error::NoBEFE_GIT;          break;
    NOTDIR:    status = Error::FileDirDoesNotExist; break;
    NOTEXIST:  status = Error::FileDoesNotExist;    break;
    NOTFILE:   status = Error::FileNameIsNotFile;   break;
    SOMEERROR:                                      break;
  }
  if (status) BEFE_ERROR("testLexWorkspace",status);
  
  return status;
  
} // ...testLexWorkspace

//----------------------------------------------------------------------
//
// Function: testBmsg - Test Bmsg Class
//

Status testBmsg() { // testBmsg...
  
  Status status;
  Bmsg   bmsg;
  String fileName;
  String indent;
  Time   start;
  Time   end;
  
  indent = ' ';
  indent *= 2;
  
  if (!gQuiet)
    Cout << indent << "Testing .bmsg language...\n";

  start.Set();
  if (true) {
    fileName = "BEFE_ErrorMessages.bmsg";
    status = FindConfigurationFile(fileName);
    if (status) goto SOMEERROR;
  }
  else
    fileName = "c:/befe/test.bmsg";
  status = bmsg.ProcessFile(fileName);
  if (status) goto SOMEERROR;
  end.Set();
  
  // Show what we got...
  if (gVerbose) {
    UInt numInTokens;
    UInt numOutTokens;
    UInt numRanges;
    UInt numMessages;
    UInt numErrors;
    UInt numWarnings;
    
    bmsg.GetStatistics(numInTokens, numOutTokens);
    bmsg.GetCounts(numRanges, numMessages, numErrors, numWarnings);
    indent *= 4;
    Cout << indent << "Input LexTokens:   " << numInTokens << '\n';
    Cout << indent << "Output BmsgTokens: " << numOutTokens << '\n';
    Cout << indent << "Ranges:            " << numRanges << '\n';
    Cout << indent << "Messages:          " << numMessages << '\n';
    Cout << indent << "Errors:            " << numErrors << '\n';
    Cout << indent << "Warnings:          " << numWarnings << '\n';
    Cout << indent << "Elapsed:           " << ToString(start, end) << '\n';
  }
  
  // Handle errors
  while (false) {
    SOMEERROR: break;
  }
  if (status) BEFE_ERROR("testBmsg",status);

  return status;

}
    
//----------------------------------------------------------------------
//
// Function: TestLexicon.Run - Test Lexical Analysis
//

Status TestLexicon::Run() { // TestLexicon.Run...

  Status  status;
  Boolean failed;
  
  if (!gQuiet)
    out << indent << "***Testing Lex (Lexical Analysis)..." << Endl;

  failed = false;
  status = Error::None;
  
  if (true) status = testLexLangCpp();
    if (status) failed = true;
  if (false) status = testLexWorkspace();
    if (status) failed = true;
  if (true) status = testBmsg();
    if (status) failed = true;
  
  return (failed) ? Error::Failed : Error::None;
  
} // ...TestLexicon.Run

} // ...Namespace BEFE
