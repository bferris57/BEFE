package bferris.json;

import java.util.HashMap;
import java.util.ArrayList;

public final class JSONObject extends JSON {

	HashMap<String,JSON>  members;
  ArrayList<JSONMember> orderedmembers;

  public static JSONObject New() {
  	return new JSONObject();
  }
  
  public static JSONObject NewOrdered() {
    JSONObject obj;
    obj = new JSONObject();
    obj.members = null;
    obj.orderedmembers = new ArrayList<JSONMember>();
    return obj;
  }
  
	public JSONObject() {
    type = JSONType.Object;
    members = new HashMap<String,JSON>();
	}

	@Override
	public boolean IsEquivalentTo(JSON that) {

		JSONObject athat;
		String[]   thisnames;
		String[]   thatnames;
		JSON       thisvalue;
		JSON       thatvalue;
		
		if (super.IsEquivalentTo(that)) {
			athat = (JSONObject)that;
			thisnames = this.GetMemberNames();
			thatnames = athat.GetMemberNames();
			if (thisnames.length != thatnames.length)
				return EquivalencyFailed();
			for (String thisname : thisnames) {
		    thisvalue = GetMember(thisname);
		    if (thisvalue == null)
		    	return EquivalencyFailed();
		    thatvalue = athat.GetMember(thisname);
		    if (thatvalue == null)
		    	return EquivalencyFailed();
		    if (!thisvalue.IsEquivalentTo(thatvalue))
		    	return EquivalencyFailed();
			}
			return true;
		}
		return false;
	}

	public JSON SetMember(JSONMember member) {
		int idx;
	  if (member == null || !member.IsMember())
	  	return new JSONError("JSON Object can only contain name/value pairs");
	  if (members != null)
  	  members.put(member.GetName().toString(),member.GetValue());
	  else if (orderedmembers != null) {
	  	for (idx=0;idx<orderedmembers.size();idx++) {
	  		if (orderedmembers.get(idx).GetName().ToString().equals(member.GetName()))
	  			break;
	  	}
	  	if (idx >= orderedmembers.size()) {
	  		orderedmembers.add(member);
	  	}
	  	else {
	  		orderedmembers.set(idx,member);
	  	}	  	
	  }
	  return this;
	}

	public JSON SetMember(String name, JSON value) {
		JSON json;
		int  idx;
		
		json = this;
	  if (name == null)
	  	json = new JSONError("JSON Object can not members with null names");
	  else if (members != null) {
  	  members.put(name,value);
	  }
	  else if (orderedmembers != null) {
	  	JSONMember member;
	  	for (idx=0;idx<orderedmembers.size();idx++) {
	  		if (orderedmembers.get(idx).GetName().equals(name))
	  			break;
	  	}
	  	if (idx >= orderedmembers.size()) {
	  		member = new JSONMember();
	  		member.SetName(new JSONString(name));
	  		orderedmembers.add(member);
	  	}
	  	else
	  		member = orderedmembers.get(idx);
	  	member.SetValue(value);
	  }
	  else
	  	json = new JSONError("JSON Object in unknown state");
	  	
	  return json;
	}

	public JSON GetMember(String name) {
	
		  JSON json;

		  if (members != null) {
  		  if (!members.containsKey(name))
	  	  	json = null;
   		  else
	  	  	json = members.get(name);
		  }
		  else if (orderedmembers != null) {
		  	json = null;
		  	for (JSONMember member: orderedmembers) {
		  		if (member.GetName().ToString().equals(name)) {
		  			json = member;
		  			break;
		  		}
		  	}
		  }
		  else
		  	json = null;
		  
		  return json;
	}
	
	public int GetSize() {
	  if (members == null)
	  	return 0;
	  else
	  	return members.size();
	}
	
	public JSON FindByName(String name) {
		JSON member = (JSON)NullObject;
		if (members != null) {
  		if (members.containsKey(name))
  			member = members.get(name);
		}
		else if (orderedmembers != null) {
			for (JSONMember omember:orderedmembers) {
				if (omember.GetName().toString().equals(name)) {
					member = omember.GetValue();
					break;
				}
			}
		}
		return member;
	}

	public JSON FindByName(JSONString name) {
		String sname = name.toString();
		return FindByName(sname);
	}

	public String[] GetMemberNames() {
		Object[] mkeys;
		String[] strings = {};

		if (members != null) {
  		mkeys = members.keySet().toArray();
      strings = new String[mkeys.length];
      System.arraycopy(mkeys,0,strings,0,mkeys.length);
		}
		else if (orderedmembers != null) {
			ArrayList<String> sarray = new ArrayList<String>();
			for (JSONMember omember: orderedmembers)
				sarray.add(omember.GetName().toString());
			strings = (String[])sarray.toArray(strings);
		}
		else
			strings = null;
		return strings;
	}

	@Override
	public JSON GetValue() {
		return this;
	}
	
	@Override
	public String ToString() {
	
		String   json;
		String[] names;
		
		int      i;

		synchronized (this) {
			json = "{";
	    names = GetMemberNames();
	    for (i=0;i<names.length;i++) {
	    	if (i != 0)
	    		json = json + ',';
	    	json = json + new JSONString(names[i]).ToJSON() + ":";
	    	json = json + FindByName(names[i]).GetValue().ToJSON();
	    }
			json = json + "}";
		}
		
		return json;
	}
	
	@Override
	public String[] ToStringArray() {
		
		ArrayList<String> sarray = new ArrayList<String>();
		String[]          keys   = GetMemberNames();
		int               maxkeyl;
		String            s;
		int               i;
		int               j;
	  int               pad;
	  String            spad;
	  
		// Compute maximum key length
		maxkeyl = 0;
		for (String key: keys)
			if (key.length() > maxkeyl)
				maxkeyl = key.length();
		
		// Populate the strings...
    if (keys.length == 0)
    	sarray.add("{}");
    else {
    	String[] mlines;
    	String   prefix;
    	String   lpad;
    	for (i=0;i<keys.length;i++) {
    		mlines = FindByName(keys[i]).ToStringArray();
    		spad = "";
    		for (pad=0;pad<(maxkeyl-keys[i].length());pad++)
    		  spad = spad + ' ';
    		if (i == 0)
    			prefix = "{" + ('"' + keys[i] + '"') + spad + " : ";
    		else
    			prefix = "," + ('"' + keys[i] + '"') + spad + " : ";
    		lpad = "";
    		for (pad=0;pad<prefix.length();pad++)
    			lpad = lpad + ' ';
    		for (j=0;j<mlines.length;j++) {
    			if (j == 0)
      			s = prefix + mlines[j];
    			else
    				s = lpad + mlines[j];
    			sarray.add(s);
    		}
    	}
    	sarray.add("}");
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