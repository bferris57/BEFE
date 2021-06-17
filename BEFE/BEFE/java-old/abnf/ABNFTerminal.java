package bferris.abnf;

public class ABNFTerminal extends ABNF {

	protected char[] value;

	public ABNFTerminal() {
		super();
		type = ABNFType.Terminal;
		value = null;
	}
	
	public ABNFTerminal(char c) {
    super();
    type = ABNFType.Terminal;
    SetValue(c);
	}
	
	public ABNFTerminal(int c) {
    super();
    type = ABNFType.Terminal;
    SetValue(c);
	}
	
	public ABNFTerminal(char[] carray) {
		super();
		
		ABNF result;

    type = ABNFType.Terminal;
		result = SetValue(carray);
		if (result.IsError())
			throw new RuntimeException(result.toString());
	}
	
	public ABNFTerminal(String s) {
		super();
		
		ABNF result;

    type = ABNFType.Terminal;
		result = SetValue(s);
		if (result.IsError())
			throw new RuntimeException(result.toString());
	}

	public ABNF SetValue(char c) {
    char[] localvalue = {c};
    value = localvalue;
    return this;
	}
	
	public ABNF SetValue(int c) {
    char[] localvalue = {(char)c};
    value = localvalue;
    return this;
	}
	
	public ABNF SetValue(char[] carray) {
    if (carray == null || carray.length < 1)
    	return new ABNFError("RFC 4234 demands terminals are 1 or more characters");
    value = new String(carray).toCharArray();  // Take a copy of it
    return this;
	}

	public ABNF SetValue(String s) {
		return SetValue(s.toCharArray());
	}

	public ABNF SetValueASCII(String ascii) {
	  return new ABNFError("ABNFTerminal.SetValueASCII not implemented yet");
	}
	
  public boolean Equals(char c) {
	  return (value.length == 1 && value[0] == c);	
	}

	public boolean Equals(char[] carray) {
	  return value.equals(carray);	
	}

	@Override
	public String toString() {
		String s;
		int    c;
		int    i;
		
		s = "";
		for (i=0;i<value.length;i++) {
			c = (int)value[i];
			if (i != 0)
				s = s + '.';
			if (c <= 0x7f)
  			s = s + String.format("%02x", c);
			else
				s = s + String.format("%x",c);
		}
		s = s.toUpperCase();
		s = "%x"+s;
		return s;
	}
}
