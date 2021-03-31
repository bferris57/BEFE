//---------------------------------------------------------------------
//
// Unique Ids - Manage unique IDs
//
// Well, where to start?  The concept is pretty straightforward but
// I'll try to be precise here...
//
// We have the need to identify things.  Object, rows, files, just
// "things".  We want to ensure that no two things EVER have the same
// identity, otherwise, it's not an identity, is it?
//
// The standard implementation of UUID provides a 16-byte (128-bit) 
// random number that's pretty much guaranteed to be unique in the
// entire universe across all time.  That'll do.
//
// That's a start.  Unfortunately, while 16 bytes is pretty small
// for what it does, it's a bit large to be throwing about in memory
// all the time (i.e., it won't fit in a processor register, etc.)
//
// So, we'll relax our requirements a little but and look at what we
// absolutely need...  We only need the 16 bytes when for persistent
// storage or for interprocess/network communication.  For a single
// process, 32 bits will work just fine for the lifetime of the
// process.  If we allocate "unique 32 bit integers", 1000 per second,
// we won't run out until around 50 days.  That seems like a fair
// enough maximum lifetime for a given process to run, don't you
// think?  I do.  Even if it's not "fair enough" it's "reasonable
// enough" since, if you want transactions to last longer than that
// you should probably have some kind of persistent/network backup
// which would need to use 128-bit UUIDs anyway.
//
// The easiest way to create a "unique 32 bit integer" is by counting
// up.  Start with zero and go from there.
//
// The trick is... When talking to other processes or storing stuff
// persistently, the ID needs to be the 16-byte one.  So, we'll
// produce that when needed and we'll produce the integer when needed.
//
// The mapping between 32-bit and 128-bit "integers" is done by the
// Id object.
//
// There, we'll leave it there for now.  'nuff said.
//
//---------------------------------------------------------------------

package bferris.core;

import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.UUID;
import bferris.util.*;

public class Id {

	
	public  static boolean usebase64 = false;
	private static Id      nullid;
	private static int     counter = 0;
	
	//-------------------------------------------------------------------
	//
	// Instances
	//
	//-------------------------------------------------------------------
	
  // Instance Members
  byte[] bytes;
  int    uuidint;

  // Instance Methods
  public Id() {
  	uuidint = counter++;
  	bytes   = null;
  }

  public Id(Id id) {
  	bytes   = null;
  	if (id != null) {
    	FromBytes(id.bytes);
  		uuidint = id.uuidint;
  	}
  	else
  		uuidint = counter++;
  }

  public Id(String s) {
  	uuidint = counter++;
    if (s == null)
      throw new java.lang.Error("Id() expected non-null String");
    if (s.length() == 38) {
      if (s.charAt(0) != '{' || s.charAt(37) != '}')
        throw new java.lang.Error("Id() expected String starting with '{' and ending with '}'");
      try {
      	UUID uuid;
        uuid = UUID.fromString(s.substring(1,37));
        FromUUID(uuid);
      }
      catch (java.lang.Exception e) {
      	throw new RuntimeException("Id() expected valid UUID string");
      }
    }
    else if (s.length() == 22) {
    	byte[] uuidbytes;
    	uuidbytes = Base64Coder.decode(s+"==");
    	FromBytes(uuidbytes);
    }
    else
    	throw new RuntimeException("Id() expected 38 byte UUID (\"{...}\") or 22 byte base64 string");
  }

  public Id(UUID u) {
    FromUUID(u);
  }

  public static Id NullId() {
  	if (nullid == null)
  		nullid = new Id();
    return nullid;  	
  }
  
  public boolean Equals(Id that) {
    if (that != null) {
    	if (that.bytes != null && Arrays.equals(this.bytes,that.bytes))
      	return true;
    	if (that.uuidint == uuidint)
    		return true;
    }
    return false;
  }
  
  public void FromUUID(UUID uuid) {
  	ByteBuffer bb;
  	long msb;
  	long lsb;
    msb = uuid.getMostSignificantBits();
    lsb = uuid.getLeastSignificantBits();
    bb = ByteBuffer.allocate(16);
    bb.putLong(0,msb);
    bb.putLong(8,lsb);
    bytes = bb.array();  	
  }
  
  public void FromBytes(byte[] bytes) {
  	if (bytes != null)
    	this.bytes = bytes.clone();
  }
  
  public byte[] ToBytes() {
  	if (bytes != null)
      return bytes.clone();
		return null;
  }
  
  public int    ToInt() {
  	if (uuidint == -1)
  		uuidint = counter++;
  	return uuidint;
  }
  
  public String ToString() {
		String s;
		if (uuidint == -1)
			uuidint = counter++;
		if (bytes == null)
			return String.format("{%d}",uuidint);
		ByteBuffer bb = ByteBuffer.wrap(bytes);
		long   msb;
		long   lsb;
		int    time_low;
		short  time_mid;
		short  version_time_hi;
		short  variant_clock_seq;
		short  node_hi;
		int    node_low;
		msb = bb.getLong();
		lsb = bb.getLong();
		time_low = (int)((msb>>32)&0xffffffff);
		time_mid = (short)((msb>>16)&0xffff);
		version_time_hi  = (short)((msb)&0xffff);
		variant_clock_seq = (short)((lsb>>48)&0xffff);
		node_hi  = (short)((lsb>>32)&0xffff);
		node_low = (int)((lsb)&0xffffffff);
		s = String.format("{%08x-%04x-%04x-%04x-%04x%08x}",time_low,time_mid,version_time_hi,variant_clock_seq,node_hi,node_low);
		return s;  	
  }
  
  public String ToShortString() {
  	if (uuidint != -1)
  		return String.format("{%d}",uuidint);
  	else if (bytes != null) {
    	return ToString().substring(0,12)+"...}";
  	}
  	return "null";
  }
  
  public String ToBase64() {
		String encoded;
		if (bytes == null)
			FromUUID(UUID.randomUUID());
		encoded = new String(Base64Coder.encode(bytes));
		encoded = encoded.substring(0,encoded.length()-2);
		return encoded;  	
  }
  
  @Override
  public String toString() {
		if (!usebase64)
			return ToString();
		else
			return ToBase64();
  }

}
