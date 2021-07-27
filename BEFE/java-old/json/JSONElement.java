package bferris.json;

//
//Class: JSONElement - Array element
//

public class JSONElement extends JSON {

	protected JSON value;
	
	public JSONElement() {
		type = JSONType.Element;
		value = null;
		finalised = false;
	}
	
	public JSONElement(JSON value) {
		type = JSONType.Element;
		this.value = value;
		this.finalised = true;
	}
	
	@Override
	public boolean IsEquivalentTo(JSON that) {
		JSONElement athat;
		if (super.IsEquivalentTo(that)) {
			athat = (JSONElement)that;
			return (value.IsEquivalentTo(athat.value));
		}
		return EquivalencyFailed();
	}

	public JSON SetValue(JSON value) {
		JSON result;
		
		if (value == null || !value.IsFinal())
		  result = new JSONError("Can only append terminated values to arrays");	
		else if (value.IsError())
			result = value;
		else if (value.DoesConform()) {
  		this.value = value;
  		result = this;
		}
		else
			result = new JSONError("Can only append RFC 4627 conforman values to arrays");
		return result;
	}

	@Override
	public JSON GetValue() {
		if (value == null)
			return new JSONNull().GetValue();
		else
			return value.GetValue();
	}
	
	@Override
	public JSON Finalise() {
		if (value == null)
			return new JSONError("Member/Element must have value");
		return super.Finalise();
 	}

  @Override
  public String ToString() {
  	String vs;
  	if (value == null)
  		vs = "null Element";
  	else
  		vs = value.toString();
  	return vs;
  }

  @Override
  public String toString() {
  	return ToString();
  }

}
