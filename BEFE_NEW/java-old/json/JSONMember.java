package bferris.json;

//
// Class: JSONMember - Object/Array members (Member/Element)
//

public class JSONMember extends JSONElement {

	JSON name;

	public JSONMember() {
		type = JSONType.Member;
	}
	
	public JSONMember(String name, JSON value) {
		this.type = JSONType.Member;
		this.name = new JSONString(name);
		this.value = value;
		this.finalised = true;
	}
	
	@Override
	public boolean IsEquivalentTo(JSON that) {
		JSONMember athat;
		if (super.IsEquivalentTo(that)) {
			athat = (JSONMember)that;
			return (name.IsEquivalentTo(athat.name));
		}
		return EquivalencyFailed();
	}

	public JSON SetName(JSON name) {
		JSON result;
		if (name == null || !name.IsString())
			result = new JSONError("Expected string for member name");
		else {
  		this.name = name;
  		result = this;
		}
		return result;
	}
	
	public JSON GetName() {
		if (name == null)
			return new JSONNull();
		else
			return name;
	}
	
	@Override
	public JSON Finalise() {
		if (name == null)
			return new JSONError("Object member must have name");
		if (value == null)
			return new JSONError("Object member must have value");
		if (conform) {
			if (!name.IsString())
				return new JSONError("JSON members must have a string name");
		}
		else {
			if (!name.IsPrimitive())
				return new JSONError("JSON members must string, number, null, true, or false");
		}
		return super.Finalise();
 	}

	@Override
	public String ToString() {
		
		String json;
		
		if (name == null)
			json = "null";
		else
			json = name.ToString();
		json = json + ":";
		if (value == null)
			json = json + "null";
		else
			json = json + value.ToString();

		return json;	
	}
	
  @Override
  public String toString() {
  	return GetName().toString()+":"+GetValue().toString();
  }

}
