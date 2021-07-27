package bferris.abnf;

class Token extends ABNF {

	static boolean asJSONObject = false;
  static boolean asJSONArray  = true;
  static boolean asString     = false;
  
	String type;
	int    line;
	int    col;
	String val;
	
	public Token() {
		
		type = "???";
		line = curline;
		col  = curcol;
		val  = "";
	}

	public String ToJSONObject() {
	  String json;
	  String literal;
	  
	  json = "{\"type\":\""+type+"\"";
	  
	  if (line > 0)
  	  json = json + ", \"line\":" + line;
	  
	  if (col > 0)
	  	json = json + ", \"col\":" + col;

	  if (val.length() >= 2 && val.charAt(0) == '"')
	  	literal = "\"" + val.substring(1,val.length()-1).replaceAll("\"","\\\"") + "\"";
	  else
		  literal = "\"" + val + "\"";
	  json = json + ", \"value\":" + literal;
	  
	  json = json + "}";
	  
	  return json;
	}

	public String ToJSONArray() {

		String json;
	  String literal;
	  
	  json = "[\""+type+"\"";
	  
 	  json = json + "," + line;
  	json = json + "," + col;

	  if (val.length() >= 2 && val.charAt(0) == '"')
	  	literal = "\"" + val.substring(1,val.length()-1).replaceAll("\"","\\\"") + "\"";
	  else
		  literal = "\"" + val + "\"";
	  json = json + "," + literal;
	  
	  json = json + "]";
	  
	  return json;
	}

	@Override
	public String toString() {
		
		if (asJSONObject)
			return ToJSONObject();
		if (asJSONArray)
			return ToJSONArray();
		return "Type:" + type + ", val = " + ", line="+line+", col="+col;
	}
}
