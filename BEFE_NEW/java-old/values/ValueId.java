package bferris.values;

import java.nio.ByteBuffer;
import java.util.UUID;

public class ValueId extends Value {

	private byte[] bytes;
	
	//-------------------------------------------------------------------
	//
  // Method: ValueId - Constructors
	//
	//-------------------------------------------------------------------

	public ValueId() {
		bytes = null;
	}
	
	public ValueId(String value) {
    if (value == null)
      return;
    if (value.length() == 38 && value.charAt(0) == '{' && value.charAt(37) == '}') {
      try {
      	UUID uuid;
        uuid = UUID.fromString(value.substring(1,37));
        FromUUID(uuid);
      }
      catch (java.lang.Exception e) {
      }
    }
    
    return;
    
	}

	//-------------------------------------------------------------------
	//
	// Method: GetType() - Get a Value's Type
	//
	//-------------------------------------------------------------------

	public Value.Type GetType() {return Type.Id;}
	
	//-------------------------------------------------------------------
	//
	// Method: Length() - Get a Value's length in entries
	//
	//-------------------------------------------------------------------

	public Value Length() {
		return Value.New(Type.Number).Set(38);
	}
	
	//-------------------------------------------------------------------
	//
	// Method: Set() - Set a value
	//
	//-------------------------------------------------------------------

	@Override
	public Value Set(String value) {

		if (value == null)
      return NullValue;
		if (bytes != null)
			return New(Type.Error).Set("Id is not mutable");
    if (value.length() == 38 && value.charAt(0) == '{' && value.charAt(37) == '}') {
      try {
      	UUID uuid;
        uuid = UUID.fromString(value.substring(1,37));
        FromUUID(uuid);
      }
      catch (java.lang.Exception e) {
      }
      return this;
    }
    else
    	return New(Type.Error).Set("Invalid Id string");
	}
	
	@Override
	public Value Set(char[] value) {
		String svalue;
		if (value == null)
			return NullValue;
		svalue = new String(value);
		return Set(svalue);
	}
	
	@Override
	public Value Set(long[] value) {
		if (value == null || value.length != 2)
	  	return NullValue;
		FromUUID(new UUID(value[0],value[1]));
		return this;
	}
	
	@Override
	public Value Set(String[] value) {
		if (value == null)
			return NullValue;
		if (value.length != 1)
			return New(Type.Error).Set("Can only set Id from single element string array");
		return Set(value[0]);
	}

	//-------------------------------------------------------------------
	//
	// Method: Get_<javatype>() - Get Java base types
	//
	//-------------------------------------------------------------------

  public String Get_String() {
		String s;
		ByteBuffer bb;
		long   msb;
		long   lsb;
		int    time_low;
		short  time_mid;
		short  version_time_hi;
		short  variant_clock_seq;
		short  node_hi;
		int    node_low;
		
		// Make bytes if we haven't got any
		if (bytes == null)
			FromUUID(UUID.randomUUID());

		bb = ByteBuffer.wrap(bytes);
		msb = bb.getLong();
		lsb = bb.getLong();
		time_low = (int)((msb>>32)&0xffffffff);
		time_mid = (short)((msb>>16)&0xffff);
		version_time_hi  = (short)((msb)&0xffff);
		variant_clock_seq = (short)((lsb>>48)&0xffff);
		node_hi  = (short)((lsb>>32)&0xffff);
		node_low = (int)((lsb)&0xffffffff);
		s = String.format("{%08X-%04X-%04x-%04X-%04X%08X}",time_low,time_mid,version_time_hi,variant_clock_seq,node_hi,node_low);
		return s;  	
  }

  @Override
	public byte[] Get_byteArray()   {
  	
		// Make bytes if we haven't got any
		if (bytes == null)
			FromUUID(UUID.randomUUID());

		return bytes.clone();
  }
  
  @Override
	public char[] Get_charArray() {
  	return Get_String().toCharArray();
  }

  @Override
	public long[] Get_longArray() {

  	long[]     longs;
  	ByteBuffer bb;
  	
  	// Make bytes if we haven't got any
		if (bytes == null)
			FromUUID(UUID.randomUUID());

		// Populate a byte buffer
		bb = ByteBuffer.allocate(16);
		bb.put(bytes);
		
		// Get longs from the byte buffer
		longs = new long[2];
		longs[0] = bb.getLong();
		longs[1] = bb.getLong();
  		
  	return longs;
  	}

  @Override
	public String[]  Get_StringArray() {
  	String[] strings = new String[1];
  	strings[0] = Get_String();
  	return strings;
  }

	//-------------------------------------------------------------------
	//
	// Java standard methods
	//
	//-------------------------------------------------------------------
  
  @Override
  public String toString() {
  	if (bytes == null)
  		return "null";
		return Get_String();
  }

  //-------------------------------------------------------------------
	//
	// Private Methods
	//
	//-------------------------------------------------------------------

  private void FromUUID(UUID uuid) {
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
  
}
