package bferris;

import java.io.*;
import java.util.Arrays;
import bferris.core.*;
import bferris.core.Object;

//---------------------------------------------------------------------
//
// Class: MSLink - Implementation of Microsoft .lnk file
//
// Notes: This class is a bit dodgy at the moment, it reads the header
//        ok, but from there can only reasonably get NUL terminated
//        C strings.  So, to get the full path of the target it
//        just looks for the first occurrence of CString "DATA" or
//        "TestOS", followed by a CString full path.
//
//        Quite honestly, we don't have any current need for other info
//        for now.  We may, however, do more in the future if we
//        need to get the command line parameters specified when it
//        resolves to an executable.
//
//        This was coded using the semi-readable but not nearly
//        comprehensive pages at the following link:
//
//          http://www.stdlib.com/art6-Shortcut-File-Format-lnk.html
//

public class MSLink extends Object {

	//
	// Shortcut flags...
	//
	//   Bit	Meaning
	//    0 	Shell item id list is present
	//    1 	Target is a file or directory
	//    2 	Has a description
	//    3 	Has a relative path
	//    4 	Has a working directory
	//    5 	Has command line arguments
	//    6 	Has a custom icon.
	//	
	static final int ShortcutShellItemIdListIsPresent = 0x01;
	static final int ShortcutTargetIsFileOrDirectory  = 0x02;
	static final int ShortcutHasDescription           = 0x04;
	static final int ShortcutHasRelativePath          = 0x08;
	static final int ShortcutHasWorkingDirectory      = 0x10;
	static final int ShortcutHasCommandLineArguments  = 0x20;
	static final int ShortcutHasCustomIcon            = 0x40;
	
	// Target flags...
	//
  //	Bit	Meaning
	//   0	Target is read only.
	//   1	Target is hidden.
	//   2	Target is a system file.
	//   3	Target is a volume label. (Not possible)
	//   4	Target is a directory.
	//   5	Target has been modified since last backup. (archive)
	//   6	Target is encrypted (NTFS partitions)
	//   7	Target is Normal
	//   8	Target is temporary.
	//   9	Target is a sparse file.
	//  10	Target has reparse point data.
	//  11	Target is compressed.
	//  12	Target is offline.
	//
	static final int TargetIsReadOnly          = 0x0001;
	static final int TargetIsHidden            = 0x0002;
	static final int TargetIsSystemFile        = 0x0004;
	static final int TargetIsVolumeLabel       = 0x0008;
  static final int TargetIsDirectory         = 0x0010;
  static final int TargetModified            = 0x0020;
  static final int TargetEncrypted           = 0x0040;
  static final int TargetNormal              = 0x0080;
  static final int TargetTemporary           = 0x0100;
  static final int TargetIsSparseFile        = 0x0200;
  static final int TargetHasReparsePointData = 0x0400;
  static final int TargetIsCompressed        = 0x0800;
  static final int TargetIsOffline           = 0x1000;

  //
  // Instance members
  //
  
  private Bytes  contents;

	private int    always;
	private byte[] guid;

	//private int    shortcutflags;
	//private int    targetfileflags;
	//private int    creationtime;
	//private int    accesstime;
	//private int    modificationtime;
	//private int    filelength;
	//private int    iconnumber;
	//private int    showwindow;
	//private int    reserved0;
	private int    reserved1;

	//
	// Constructors
	//
	
  public MSLink(String msfilename) {
	
  	File            file;
    FileInputStream istream;
    byte[]          bcontents;

    // Make sure it exists
  	file = new File(msfilename);
  	if (!file.exists())
  	  throw new RuntimeException("Link file '"+msfilename+"' does not exist");

  	try {
      
      // Read the entire contents
      istream = new FileInputStream(file);
	    if (istream.available() != file.length())
		  throw new RuntimeException("All of file contents are not available without blocking");
	    bcontents = new byte[(int)file.length()];
	    istream.read(bcontents);
	    contents = new Bytes(bcontents);
	    contents.MSB();
	  } 
	  catch (java.lang.Exception e) {
	    throw new RuntimeException("Error reading .lnk file");
	  }
	
  	//
  	// Read the header
  	//
	  
  	byte[] guidshouldbe = {0x01,0x14,0x02,0x00
                          ,0x00,0x00
                          ,0x00,0x00
                          ,(byte)0xc0,0x00
                          ,0x00,0x00,0x00,0x00,0x00,0x46};
    
  	contents.LSB();
    always           = contents.GetInt();
  	guid             = contents.GetN(16);
    /*shortcutflags    =*/ contents.GetInt();
    /*targetfileflags  =*/ contents.GetInt();
    /*creationtime     =*/ contents.GetInt();
    /*accesstime       =*/ contents.GetInt();
    /*modificationtime =*/ contents.GetInt();
    /*filelength       =*/ contents.GetInt();
    /*iconnumber       =*/ contents.GetInt();
    /*showwindow       =*/ contents.GetInt();
    /*reserved0        =*/ contents.GetInt();
    reserved1        = contents.GetInt();

    if (always != 0x4c)
    	throw new RuntimeException("Expected 0x4c000000 at beginning");
    
    if (!Arrays.equals(guid,guidshouldbe))
    	throw new RuntimeException("Unknown GUID in .lnk file");
    
    if (reserved1 != 0)
    	throw new RuntimeException("Reserved1 not zero");
    if (reserved1 != 0)
    	throw new RuntimeException("Reserved1 not zero");

  }

 public String GetFullPath() {

   String data;

	 // Search for "DATA" followed by a NUL...
   for (;;) {
   	data = contents.GetCString();
   	if (data == null || data.equals("DATA") || data.equals("TestOS"))
   		break;
   }
   
   // If we found it, the next string is the path
   if (data == null)
  	 data = new String("");
   else
     data = contents.GetCString();

   return data;
 }

}
