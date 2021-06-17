package bferris.abnf;

public class ABNFRule extends ABNF {

	  String name;
	  
	  public String GetName() {
	  	
	  	if (name == null)
	  		return "";
	  	return name;
	  	
	  }
	  
	  public ABNF SetName(String name) {
	  	
	  	ABNF result;
	  	
	  	result = new ABNFError("ABNFRule.SetName not implemented yet");
	  	if (name == null) {
	  	  result = new ABNFError("Null rule name not allowed");	
	  	}
	  	
	  	return result;
	  }
}
