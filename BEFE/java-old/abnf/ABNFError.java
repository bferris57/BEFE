package bferris.abnf;

public class ABNFError extends ABNF {

	public int    line;
	public int    col;
	public String message = "Unknown";

	public ABNFError() {
		type = ABNFType.Error;
    line = ABNF.curline;
    col  = ABNF.curcol;
	}
	
	public ABNFError(String message) {
		type = ABNFType.Error;
    line = ABNF.curline;
    col  = ABNF.curcol;
	  this.message = message;
	}
	  
  @Override
  public String toString() {
  	String s = "";
  	if (line > 0) {
  		s = "(Line "+line;
  		if (col > 0) {
  			s = s + "/" + col;
  		s = s + ")";
  		}
  		s = "ABNF Error "+s+": "+message;
  	}
  	else
  		s = "ABNF Error: "+message;
  	return s;
  }

}
