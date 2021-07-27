package bferris.abnf;

public class ABNFChar extends ABNF {

	private char thechar;
	
	public ABNFChar(char c) {
		super();
		//type = ABNFType.Char;
		
	  thechar = c;
	}

	public boolean In(ABNFRange range) {
	  return range.Contains(thechar);	
	}
	
	@Override
	public String toString() {
		char[] s = {thechar};
		return new String(s);
	}
}
