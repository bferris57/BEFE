package bferris.json;

public final class JSONNumber extends JSON {

	private String  value;
	
	public JSONNumber() {
	  type = JSONType.Number;
	  value = null;
	}

	public JSONNumber(int v) {
		type = JSONType.Number;
		value = String.format("%d",v);
		Finalise();
	}
	
	public JSONNumber(long v) {
		type = JSONType.Number;
		value = String.format("%d",v);
		Finalise();
	}
	
	@Override
	public boolean IsEquivalentTo(JSON that) {
		if (super.IsEquivalentTo(that)) {
			if (((JSONNumber)that).value.equals(value))
				return true;
		}
		return EquivalencyFailed();
	}

	@Override
	public JSON ProcessChar(char c) {
		
		JSON result;
		char c0;
		char cprev;
		
		if (finalised)
			result = new JSONError("JSON number is finalised, cannot process character");
		else {
			if (value == null)
				value = "";
			result = this;
			switch (c) {
  			case '+':
  			case '-':
  				if (value.length() > 1) {
  					c0 = value.charAt(0);
 						cprev = value.charAt(value.length()-1);
  					if ((c0 == '+' || c0 == '-') && (cprev != 'e')) {
  						result = new JSONError("Sign already present");
  					}
  				}
  				if (result == this)
  					value = value+c;
  				break;
  			case '0':
  			case '1':
  			case '2':
  			case '3':
  			case '4':
  			case '5':
  			case '6':
  			case '7':
  			case '8':
  			case '9':
	        value = value+c;
	        break;
  			case 'e':
  			case 'E':
  				if (value.length() == 0)
  					result = new JSONError("JSON Number cannot start with an exponent");
  				if (value.contains("E"))
  					result = new JSONError("JSON Number exponent already specified");
  				else
    				value = value + 'e';
  				break;
  			case '.':
  				if (value.contains("e"))
  					result = new JSONError("JSON Number decimal point not allowed in exponent");
  			  else if (value.contains("."))
  					result = new JSONError("JSON Number already has decimal point");
  				else
  					value = value + '.';
  				break;
			  default:
          result = new JSONError(String.format("Unexpected character 0x%x",(int)c));
          break;
			}
		}
		return result;
	}

  @Override
  public JSON Finalise() {
  	
  	char c0 = 0;
    char cN = 0;
    int  ipos;
    int  epos;
    int  dpos;
    int  i;
    
    if (finalised)
    	return new JSONError("JSON number already finalised");
    if (value == null || value.length() == 0)
    	return new JSONError("JSON number empty");
    c0 = value.charAt(0);
    
    // Strip leading '+' sign
    if (c0 == '+')
    	value = value.substring(1);
    if (value.length() == 0)
    	return new JSONError("JSON number missing leading digits");
    
    // Get trailing character
    cN = value.charAt(value.length()-1);
    if (cN == '+' || cN=='-' || cN == 'e')
    	return new JSONError("JSON number missing digits after exponent");
  	
    // Figure out where integer, decimal, and exponent digits start...
    ipos = 0;
    if (c0 == '-')
    	ipos++;
    epos = value.indexOf('e');
    if (epos < 0) epos = value.length();
    dpos = value.indexOf('.');
    if (dpos < 0) dpos = value.length();
    
    // Strip leading integer digits
    for (i=ipos; i<dpos-1 && value.charAt(i) == '0';) {
      value = value.substring(i+1);
      dpos--;
      epos--;
    }
    
    // Strip off trailing zeros
    for (i=epos-1; i>dpos && value.charAt(i) == '0';i--) {
    	value = value.substring(0,i)+value.substring(epos);
    	epos--;
    }
    
    // Strip off trailing empty '.'
    if (dpos == epos-1) {
    	epos--;
    	value = value.substring(ipos,dpos) + value.substring(dpos+1);
    }
    
    // Strip off leading zeros in exponent (this has to be done last
    // because it buggers with epos)...
    if (epos < value.length()-1) {
      if (value.charAt(epos+1) == '-')
    	epos++;
      for (i=epos+1;i < value.length()-1 && value.charAt(i) == '0';)
        value = value.substring(0,epos+1)+value.substring(epos+2);
    }
    
    return super.Finalise();
  }
  
  @Override
  public JSON GetValue() {
  	if (value == null)
  		return new JSONNull();
  	else
  		return this;
  }
  
  @Override
  public String ToString() {
  	if (value == null)
  		return "null";
  	else
  		return value;
  }

  @Override
  public String toString() {
  	return GetValue().toString();
  }
}
