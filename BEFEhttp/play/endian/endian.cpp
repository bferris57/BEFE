//---------------------------------------------------------------------
// File: endian.cpp - Test out BEFE with big/little endian text files
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "CommandShell.h"
#include "UTF8.h"
using namespace BEFE;

int main(int argc, char **argv) {

  Status        status;
  Befe          theBefe;
  String        script;
  CommandShell  shell;
  String        fileName;
  char const   *fNames[] = {
    "endian.cpp",
    "endian-notepad-ANSI.txt",
    //"endian-notepad-Unicode.txt",
    //"endian-notepad-Unicode_big_endian.txt",
    "endian-notepad-UTF-8.txt",
    0};
  Id            fileId;
  Byte         *buf;
  UInt          bufLen;
  UInt          idx;
  String        theString;
  
  Cout << "Starting Endian text file test...\n";

  // Load '~/.beferc' if it exists...
	do {
    script = ApplyRelativePath(GetCurrentWorkingDirectory(), "~/.beferc");
    if (Exists(script) && IsFile(script)) {
      status = shell.ExecuteScript(script, "", false);
      if (status == Error::CommandExit) break;
    }
  }	while (false);	

  //
  // Process the files
  //
  
  SetCurrentWorkingDirectory("C:/SVN/play/endian");
  
  for (idx=0; fNames[idx]; idx++) {

    // Turn file name into a full path...
    fileName = ApplyRelativePath(GetCurrentWorkingDirectory(), fNames[idx]);
    Cout << "File '" << fileName << "'...\n";

    // Start caching the file and get its Id...
    status = TheBefe->TheCache.StartCacheFile(fileName);
    if (status) goto SOMEERROR;
    fileId = TheBefe->TheCache.GetFileId(fileName);
    if (IsNull(fileId)) goto INTERNAL;

    // Load the block and get it's address/len
    status = TheBefe->TheCache.LoadFileBlock(fileId, 0);
    if (status) goto SOMEERROR;

    // Display the file block
    status = TheBefe->TheCache.DisplayFileBlock(fileId, 0, 2);
    if (status) goto SOMEERROR;
    
    // If it's the "...UTF-8.txt" file, read the string and display it...
    if (!IsNull(fileName.Find("UTF-8.txt"))) {
      
      Cout << '\n';
      Cout << "  Processing the file as a UTF-8 string...\n";
      Cout << '\n';
      
      // Get the buf and size and turn it into a string...
      buf = TheBefe->TheCache._GetBlockPointer(fileId, 0);
      if (IsNull(buf)) goto NULLPOINTER;
      bufLen = TheBefe->TheCache.GetFileSize(fileId);
      bufLen = Min(bufLen, TheBefe->TheCache.GetBlockSize());
      theString = String(buf, bufLen);
      if (theString.Get(0).IsBOM())
        theString.Remove(0);
      Cout << "  theString    = '" << theString << "'\n";
      Cout << "  Length       = " << theString.Length() << '\n';
      Cout << "  theString[0] = 0x" << ToHexString((Int)theString.Get(0),8) << '\n';
    }
    
    // Uncache the file
    status = TheBefe->TheCache.StopCacheFile(fileName);
    if (status) goto SOMEERROR;

  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    INTERNAL:    status = Error::Internal;            break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  if (status) BEFE_ERROR(String("File '")+fileName+"'",status);
  
}