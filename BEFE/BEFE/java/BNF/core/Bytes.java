//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// File: Bytes.java - N bytes
//
// Author: bferris
//
// Copyright: 2011, Bruce Ferris
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

package bferris.core;

//
// Imports
//

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//
// Class:            Bytes - <Description>
//
// Class Members:    ***None***
//
// Class Methods:    Allocate - Allocate N bytes
//
// Instance Members: ***None***
//
// Instance Methods: Length - Return length in bytes
//                   Set    - Set N bytes at a given offset
//                   Get    - Get N bytes at a given offset
// <Notes>
//
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

public class Bytes {

  //
  // Class Members
  //

  //
  // Class Methods
  //

  //
  // Instance Members
  //

  private boolean msbfirst;
  private int     size;
  private byte[]  bytes;
  private int     curpos;
  
  //
  // Instance Methods
  //

  public Bytes(int numbytes) {

  	if (numbytes < 0)
  		throw new RuntimeException("Expected zero or positive number of bytes");
  	size = numbytes;
  	msbfirst = true;
  	bytes = new byte[numbytes];
  	curpos = 0;
  	
    return;
  }

  public Bytes(byte[] bytes) {
	
	  if (bytes == null) {
		  size = 0;
		  this.bytes = new byte[0];
	  }
	  else {
		  size = bytes.length;
		  this.bytes = bytes;
	  }
  }

  public int GetPos() {
  	return curpos;
  }
  
  public int SetPos(int index) {
  	int was;
  	
  	was = curpos;
  	if (index < 0)
  		curpos = 0;
  	else if (index >= bytes.length)
  		curpos = bytes.length;
  	else
  		curpos = index;
  	return was;
  }
  
  public boolean MSB() {
  	boolean was;
  	was = msbfirst;
	  msbfirst = true;
	  return was;
  }
  
  public boolean LSB() {
  	boolean was;
  	was = msbfirst;
	  msbfirst = false;
	  return was;
  }

  public int GetInt() {
	
	int myint;
	
	if (curpos + 4 > size)
	  throw new RuntimeException("Out of bounds");
	
	if (msbfirst) {
    myint =              (bytes[curpos++] & 0xff);
	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
	}
	else {
	  myint =              (bytes[curpos+3] & 0xff);
	  myint = (myint<<8) | (bytes[curpos+2] & 0xff);
	  myint = (myint<<8) | (bytes[curpos+1] & 0xff);
	  myint = (myint<<8) | (bytes[curpos] & 0xff);
	  curpos += 4;
	}

	return myint;
  }
  
  public long GetLong() {
  	
  	long myint;
  	
  	if (curpos + 8 > size)
  	  throw new RuntimeException("Out of bounds");
  	
  	if (msbfirst) {
      myint =              (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos++] & 0xff);
  	}
  	else {
  	  myint =              (bytes[curpos+7] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos+6] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos+5] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos+4] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos+3] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos+2] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos+1] & 0xff);
  	  myint = (myint<<8) | (bytes[curpos]   & 0xff);
  	  curpos += 8;
  	}

  	return myint;
  }
    
  public byte[] GetN(int len) {
    
  	byte[] rbytes;
    int    i;
    
    if (len < 0 || curpos + len > size)
    	throw new RuntimeException("Specified bytes not available");
    rbytes = new byte[len];
    for (i=0;i<len;i++) {
    	rbytes[i] = bytes[curpos++];
    }
    
    return rbytes;
  }
  
  public byte GetByte() {
  	if (curpos >= bytes.length)
  		return 0;
  	return bytes[curpos++];
  }
  
  // Get NUL terminated string starting at current location (returns null if none)
  public String GetCString() {
  	byte barray[];
  	int  oldpos;
  	int  eos;
  	
  	if (curpos >= bytes.length)
  		return null;
  	oldpos = curpos;
    for(eos=curpos;eos < bytes.length && bytes[eos] != 0;eos++);
    barray = new byte[eos-curpos];
    while (curpos<eos)
    	barray[curpos-oldpos] = bytes[curpos++];
    if (curpos < bytes.length)
    	curpos++;
  	return new String(barray);
  }
}
