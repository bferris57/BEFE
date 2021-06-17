package bferris.json;

import java.util.ArrayList;
import java.util.List;

public final class JSONArray extends JSON {

	List<JSON> elements;
	
	public JSONArray() {
    type = JSONType.Array;
    elements = new ArrayList<JSON>();
	}
	
	@Override
	public boolean IsEquivalentTo(JSON that) {

		JSONArray athat;
		int       i;
		
		if (super.IsEquivalentTo(that)) {
		  athat = (JSONArray)that;
		  if (athat.elements.size() != this.elements.size())
		  	return EquivalencyFailed();
		  for (i=0;i<this.elements.size();i++) {
		  	if (!GetElement(i).IsEquivalentTo(athat.GetElement(i)))
		  		return EquivalencyFailed();
		  }
		  return true;
		}
		return EquivalencyFailed();
	}

	public JSON AppendElement(JSON element) {
		JSON result;
		if (element == null || !element.DoesConform())
			result = new JSONError("Expected RFC 4627 conformant value");
		else {
  		elements.add(element);
  		result = this;
		}
		return result;
	}
	
	public JSON GetElement(int index) {
		if (index < 0 && elements != null)
			index = elements.size()+index;
	  if (elements == null || index < 0 || index >= elements.size())
	  	return NullObject;
    return elements.get(index);    
	}
	
	public int  GetSize() {
		return elements.size();
	}
	
	@Override
	public JSON GetValue() {
		return this;
	}

	@Override
	public String ToString() {

		String result;
		
		result = "";
  	for (String s : ToStringArray())
  		result = result + s;
  	return result;
	}
	
	// TEMP: Old one...
	public String[] OLD_ToStringArray() {
		
		ArrayList<String> sarray = new ArrayList<String>();
	  String[]          result;
		String            s;
		int               i;
		
	  for (i=0;i<elements.size();i++) {
	  	if (i == 0)
	  		s = "[";
	  	else
	  		s = ",";
	  	s = s + elements.get(i).ToJSON();
	  	sarray.add(s);
	  }

	  if (elements.size() == 0)
	  	sarray.add("[]");
	  else if (elements.size() == 1)
	  	sarray.set(0,sarray.get(0)+"]");
	  else
	  	sarray.add("]");

	  // Form result
	  result = new String[sarray.size()];
	  for (i=0;i<sarray.size();i++)
	  	result[i] = sarray.get(i);

	  return result;
	}

	@Override
	public String[] ToStringArray() {
		
		ArrayList<String> sarray = new ArrayList<String>();
		String            s;
		int               i;
		int               j;
	  
		// Populate the strings...
    if (elements.size() == 0)
    	sarray.add("[]");
    else {
    	String[] mlines;
    	String   prefix;
    	String   lpad;
    	for (i=0;i<elements.size();i++) {
    		if (i == 0)
    			prefix = "[";
    		else
    			prefix = ",";
    		lpad = " ";
    		mlines = elements.get(i).ToStringArray();
    		for (j=0;j<mlines.length;j++) {
    			if (j == 0)
      			s = prefix + mlines[j];
    			else
    				s = lpad + mlines[j];
    			sarray.add(s);
    		}
    	}
    	sarray.add("]");
    }
    
	  // Form result
	  String[] result = new String[sarray.size()];
	  for (i=0;i<sarray.size();i++)
	  	result[i] = sarray.get(i);

	  return result;

	}

	@Override
  public String toString() {
  	return ToString();
  }
}