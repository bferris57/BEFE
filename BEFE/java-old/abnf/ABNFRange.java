package bferris.abnf;

public class ABNFRange extends ABNF {

	private char[] range;
	
	public ABNFRange(char lower) {
		super();
		Init(lower,lower);
	}
	
	public ABNFRange(char lower, char upper) {

		super();
		Init(lower,upper);
	}
	
	private void Init(char lower, char upper) {
		type = ABNFType.Range;
		
		char[] r = {lower,upper};
		range = r;		
	}
	
	public boolean Contains(char c) {
		return (c >= range[0] && c <= range[1]);
	}
	
	@Override
	public String toString() {
		String ls;
		String us;
		if (range[0]<=0xff)
			ls = String.format("%2x",range[0]);
		else
			ls = String.format("%x",range[0]);
		if (range[0]<=0xff)
			us = String.format("%2x",range[1]);
		else
			us = String.format("%x",range[1]);
		return "%x"+ls+"-"+us;
	}
}
