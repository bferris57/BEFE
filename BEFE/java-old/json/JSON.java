//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// Class: JSON - Bruce's implementation of JSON (RFC 4627) and
//               his optional extensions to it to make it more useful.
//
// Class JSON is the superclass of all JSON objects.  It's intended to
// provide a short and fast java implementation of RFC 4627.
//
// Notes: With the intent of being as fast as I can make it in java,
//        there's lots of static members and methods in the JSON class
//        because most of its work has to do with parsing and
//        validating Json textual input.  So, it synchronising on the
//        JSON java class object itself instead of JSON instances
//        when parsing.  On the down side this means that no two java
//        threads running in the same JVM can be parsing at the same
//        time.  On the up side this means it's thread safe and faster
//        then it would be if it had to do all kinds of instance object
//        locks.  Alternatively, I may not have my head on straight
//        as to how java threads work.  ;)
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

package bferris.json;

import java.util.Stack;

import java.io.File;

public abstract class JSON {

	JSONType type;
  boolean  finalised = false;
  
	public JSON() {
		type = JSONType.JSON;
	}
	
	public boolean DoesConform() {
	  switch (type) {
	    case False:
	    case True:
	    case Null:
	    case Object:
	    case Array:
	    case Number:
	    case String:
	      return true;
	    default:
	  	  return false;
	  }
	}
	
	public boolean IsFinal() {
		return finalised;
	}
	
	public boolean IsError() {
		return type == JSONType.Error;
	}
	
	public boolean IsJSON() {
		return true;
	}
	
	public boolean IsPrimitive() {
		
		return (type == JSONType.Null   ||
				    type == JSONType.True   ||
				    type == JSONType.False  ||
		        type == JSONType.String ||
            type == JSONType.Number );    
	}

	public boolean IsNumber() {
		return (type == JSONType.Number);
	}
	
	public boolean IsString() {
		return (type == JSONType.String);
	}
	
	public boolean IsObject() {
		return (type == JSONType.Object);
	}

	public boolean IsArray() {
		return (type == JSONType.Array);
	}
	
	public boolean IsMember() {
		return (type == JSONType.Member);
	}
	
	public boolean IsEquivalentTo(JSON that) {
		if (that == null) {
			if (this == NullObject)
				return true;
			else
			return EquivalencyFailed();
		}
		if (that.type != this.type)
			return EquivalencyFailed();
		if (that.finalised != this.finalised)
			return EquivalencyFailed();
		if (that.getClass() != this.getClass())
			return EquivalencyFailed();
		return true;
	}

	protected boolean EquivalencyFailed() {
		return false;
	}
	
	public JSON ProcessChar(char c) {
		return new JSONError(String.format("Unexpected character 0x%8x",(int)c));
	}

	public JSON Finalise() {
		JSON that = this;
		if (finalised)
			 that = new JSONError("JSON already finalised");
		else
			finalised = true;
		return that;
	}

	@Override
	public boolean equals(Object that) {
		if (JSON.class.isInstance(that)) {
			return IsEquivalentTo((JSON)that);
		}
		return super.equals(that); 
	}
	public abstract JSON     GetValue();
	public          String   ToJSON() {
		return ToString();
	}
	public abstract String   ToString();
	public          String[] ToStringArray() {
		String[] sarray = {ToString()};
		return sarray;
		}
	public abstract String   toString();
	
	//
	// Class members
	//

	// Parsing
	static protected boolean     conform = true;  // true = "Conform to RFC 4627", false = "Bruce's extensions to the RFC"
	static protected Stack<JSON> tokens;
	static protected String      source;
	static protected int         length;
	static protected int         curpos;
	static protected int         curline;
  static protected int         curcol;
  static protected int         depth = 0;
  static protected boolean     usestringfragments = true;

  // JSON Object types (Error isn't in the RFC but is implied or else it all turns into bullshit and wanking)
	public static enum JSONType {JSON
		                          ,Error
                              ,Object
                              ,Member
                              ,Array
                              ,Element
                              ,Null
                              ,True
                              ,False
                              ,String
                              ,Number
                              ,LastType // Placeholder to make arrays from
                              };
  
  // The one (and only one) JSON.Null object
  public static JSON NullObject = new JSONNull();
  
	//
	// Class methods
	//
 
  private static boolean IsWhite(char c) {
  	return (" \t\r\n".indexOf(c) >= 0);
  }
  
  private static char SourceChar(int index) {
  	if (source == null || index < 0 || index > source.length())
  		return 0;
  	return source.charAt(index);
  }
  
  private static void Push(JSON json) {
    tokens.push(json);
    if (tokens.size() > depth)
    	depth = tokens.size();
  }
  
  private static JSON Pop() {
    if (tokens.isEmpty())
    	return new JSONError("JSON token stack is empty");
    return tokens.pop();
  }

  private static JSON Peek() {
  	if (tokens == null || tokens.size() == 0)
  		return null;
  	return tokens.peek();
  }
  
	private static JSON ProcessChars() {
		
		char   c;
		JSON   that;
    
		tokens     = new Stack<JSON>();
		that = null;
		//System.out.println("JSON.ProcessChars() source: "+source);

		c = 0;
		
		for (;;) {

			// Finished if at end of string
			if (curpos >= length)
				break;
		
			// Get next character
			c = SourceChar(curpos++);
			curcol++;
			
			// Peek at topmost token
			if (tokens.isEmpty())
				that = null;
			else {
				that = Peek();
				if (that.IsError())
					break;
			}

			// Handle end of line
	  	if (c == '\r' || c == '\n') {
	  		if (that != null && that.IsPrimitive() && !that.IsFinal()) {
	  			Pop();
		  		Push(that.Finalise());	  			
	  		}
	  		if (c == '\n') {
	  			curline += 1;
	  			curcol = 1;
	  		}
	  		continue;
	  	}

	  	// If building string...
			if (!tokens.isEmpty()) {
			  that = Peek();
			  if (!that.IsFinal() && that.IsString()) {
			  	
			  	that = Pop();
  		   	that = that.ProcessChar(c);
  			 	Push(that);
  			 	if (that.IsError())
  			 		break;
  			 	if (usestringfragments && !that.IsFinal()) {
  			  	int    dq,fraglen;
  		      dq = source.indexOf('\"',curpos);
  		      fraglen = source.length() - curpos;
  		      if (dq >= curpos) fraglen = dq-curpos;
  		      if (fraglen > 0) {
  		      	that = Pop();
  		      	Push(((JSONString)that).ProcessFrag(source.substring(curpos,curpos+fraglen)));
  		      	if (Peek().IsError())
  		      		break;
  		      	curpos = curpos + fraglen;
  		      	curcol = curcol + fraglen;
  		      }
  			 	}
  			 	continue;
			  }
			}
			
			//
			// Depending on the character...
			//
			
			// Ignore non <EOL> whitespace
			if (IsWhite(c))
	  		continue;
	  	
	  	else if (c == '"') {
	  		that = new JSONString();
	  	  Push(that);
	  		that.ProcessChar(c);
	  		if (that.IsError())
	  			break;
	  		continue;
	  	}

	  	else if (c == 'f') {
	  		if (curpos > length-4 || !source.substring(curpos,curpos+4).equals("alse")) {
	  			Push(new JSONError("Expected 'false' here"));
	  			break;
	  		}
	  		Push(new JSONFalse());
	  		curpos += 4;
	  		curcol += 4;
	  		continue;
	  	}
	  	
	  	else if (c == 't') {
	  		if (curpos > length-3 || !source.substring(curpos,curpos+3).equals("rue")) {
	  			Push(new JSONError("Expected 'true' here"));
	  			break;
	  		}
	  		Push(new JSONTrue());
	  		curpos += 3;
	  		curcol += 3;
	  		continue;
	  	}
	  	
	  	else if (c == 'n') {
	  		if (curpos > length-3 || !source.substring(curpos,curpos+3).equals("ull")) {
	  			Push(new JSONError("Expected 'null' here"));
		  		Push(that);
	  			break;
	  		}
	  		Push(new JSONNull());
	  		curpos += 3;
	  		curcol += 3;
	  		continue;
	  	}
	  	
	  	else if ("+-0123456789eE.".indexOf(c) >=0) {
	  		if (that == null || !that.IsPrimitive()) {
	  			Push(new JSONNumber());
	  		}
	  		that = Pop();
	  		if (that.IsFinal()) {
	  			Push(new JSONError("Numeric character '"+c+"' not valid in this context"));
	  			break;
	  		}
	  		if (!that.IsNumber()) {
	  			Push(new JSONNumber().ProcessChar(c));  			
	  		}
	  		else
  	  		Push(that.ProcessChar(c));
	  		continue;
	  	}
	  	
	  	else if (",:{[]}".indexOf(c) >= 0) {
	  		
	  		// Handle starting ones
		  	if (c == '[') {
		  		that = new JSONArray();
		  		Push(that);
		  		continue;
		  	}

		  	else if (c == '{') {
		  		that = new JSONObject();
		  		Push(that);
		  		continue;
		  	}
		  	
		  	// Invalid if nothing on the stack
	  		if (tokens == null || tokens.isEmpty()) {
	  		  Push(new JSONError("Operator '"+c+"' can only be applied inside an Array or Object"));
	  		  break;
	  		}
	  		
		  	// Terminate top element on token stack if not already terminated
	  		
		  	that = Pop();
		  	
	  		if (c == ':') {
	  			if (Peek() != null && !Peek().IsObject()) {
	  				Push(new JSONError("':' only valid within Objects"));
	  				break;
	  			}
	  			that = new JSONMember().SetName(that);
   				Push(that);
	  		  continue;
	  		}

  			if (c == ']') {

  				if (!that.IsFinal()) {
  					that = that.Finalise();
  					if (that.IsError())
  						break;
  					if (that.IsArray()) {
  						Push(that);
  						continue;  						
  					}
  				}
  				if (Peek() != null && Peek().IsArray() && !Peek().IsFinal()) {
  					((JSONArray)Peek()).AppendElement(that);
  					Peek().Finalise();
  				}
  				else {
  					Push(new JSONError("End of array not applicable here (no matching start array)"));
  				  break;
  				}
  				continue;
  			}
  			
  			else if (c == '}') {
  				if (!that.IsFinal()) {
  					that = that.Finalise();
  					if (that.IsError()) {
  						Push(that);
  						break;
  					}
  					if (that.IsObject()) {
    					Push(that);
  						continue;
  					}
  				}
  				if (Peek() != null && !Peek().IsFinal()) {
  					if (!that.IsMember() && Peek().IsMember()) {
  					  ((JSONMember)Peek()).SetValue(that);
  					  Peek().Finalise();
  					  that = Pop();
  					  if (that.IsMember()) {
  					  	if (Peek() != null && !Peek().IsFinal() && Peek().IsObject()) {
  					  		((JSONObject)Peek()).SetMember((JSONMember)that);
  					  		Peek().Finalise();
  					  	}
  					  }
  					}
 					  else {
 					  	Push(new JSONError("Can only place name/value pairs in Objects"));
 					  	break;
 					  }
  				}
  				else {
  					Push(new JSONError("End of '}' not applicable here (no matching '{')"));
  				  break;
  				}
  			}
  			
  			else if (c == ',') {
  				if (!that.IsFinal()) {
  					that = that.Finalise();
  					if (that.IsError()) {
  						Push(that);
  						break;
  					}
  				}
  				if (Peek() == null)
  					Push(new JSONError("Operator ',' not applicable here"));
  				if (Peek().IsArray())
    	  		that = ((JSONArray)Peek()).AppendElement(that);
  				else if (Peek().IsMember()) {
  					that = ((JSONMember)Peek()).SetValue(that);
  					that = Pop();
  					that = that.Finalise();
  					if (that.IsError()) {
  						Push(that);
  						break;
  					}
  				}
  				if (that.IsError()) {
  					Push(that);
  					break;
  				}
	  			if (Peek().IsObject()) {
	  				if (that.IsMember())
  	  				that = ((JSONObject)Peek()).SetMember((JSONMember)that);
	  				else
	  					that = new JSONError("Operator ':' missing");
	  			}
	  			else if (!Peek().IsArray())
	  				that = new JSONError("Operator ',' only applicable within Objects or Arrays");
	  			if (that.IsError()) {
	  				Push(that);
	  				break;
	  			}
	  		}

  			if (Peek() != null && Peek().IsError())
  				break;
  			
  			continue;
  			
	  	} // End of if ',:]}'

	  	else {
	  		that = new JSONError("Invalid character in this context");
	  		Push(that);
	  		break;
	  	}
		} // for (...)

		//
		// Finished parsing all characters...
		//

		that = null;
		
		while (!tokens.isEmpty()) {
		
			that = Pop();
			
			if (that.IsError())
				break;
			
			if (!that.IsFinal()) {
				Push(that.Finalise());
				continue;
      }
			
    	if (tokens.size() > 0) {
  		  that = new JSONError("Array/Object not terminated properly");
    	}

    	break;
	  }

		// Clean up after parsing
		curline = 0;
		curcol  = 0;
		
		return that;
  }

	public static JSON Parse(File file) {
		return new JSONError("JSON.ParseFile not implemented yet");
	}

	public static JSON Parse(String in) {

		JSON result;
		
		// Strip off trailing CR and LF
		while (in != null && in.length() > 1 && (in.charAt(in.length()-1) == '\n' || in.charAt(in.length()-1) == '\r'))
			in = in.substring(0,in.length()-1);
		
		// Parse it...
		synchronized (JSON.class) {
  		// Initialise parsing state
  		source  = in;
  		length  = source.length();
  		curline = 1;
  		curcol  = 0;
  		curpos  = 0;
  		depth   = 0;
		  result  = ProcessChars();
		  source  = null;
		}
		return result;
	}

  //-------------------------------------------------------------------
	//
	// Class: Analysis - Analysis of a JSON object
	//
	//-------------------------------------------------------------------
	
	public static class Analysis {
	
		// Total based on type
		public int[] total;  // Useful for all JSONType enum values

		// Info for all JSONArrays encountered
		public int   totalArrayElements   = 0;
		public int   meanArrayElements    = 0;
		public int   maxArrayElements     = 0;

		// Info for all JSONObjects encountered
		public int   totalObjectMembers   = 0;
		public int   meanObjectMembers    = 0;
		public int   maxObjectMembers     = 0;
		
		// Depth info
		public int   maxDepth             = 0;
		
		public Analysis(JSON pjson) {

			class StackEntry {
				JSON     json;
				String[] membernames = null;
				int      index = -1;
			}
			
			Stack<StackEntry> stack = new Stack<StackEntry>();
			StackEntry        entry;
		  StackEntry        nentry;	
			JSON        json;
			JSONArray   a;
			JSONObject  o;
			int         i;
			
			total             = new int[JSONType.LastType.ordinal()];      
      
      for (i=0;i<JSONType.LastType.ordinal();i++)
      	total[i] = 0;

      entry = new StackEntry();
      entry.json = pjson;
      stack.push(entry);

      while (!stack.empty()) {

      	if (stack.size() > maxDepth)
      		maxDepth = stack.size();
      	
      	entry = stack.pop();
      	json = entry.json;
      	
      	if (json.type.ordinal() < 0 || json.type.ordinal() >= JSONType.LastType.ordinal())
      		throw new Error(String.format("Unknown JSONType %d",json.type));
      	if (entry.index < 0)
        	total[json.type.ordinal()]++;
      	
      	if (json.type == JSONType.Array) {
      		a = (JSONArray)json;

      		if (entry.index < 0) {
      			totalArrayElements += a.elements.size();
      			if (a.elements.size() > maxArrayElements)
      				maxArrayElements = a.elements.size();
      		}
      		entry.index++;
      		if (entry.index < a.elements.size()) {
      			stack.push(entry);
      			nentry = new StackEntry();
      			nentry.json = a.GetElement(entry.index);
      			stack.push(nentry);
      		}
      	}
      	
      	else if (json.type == JSONType.Object) {

      		o = (JSONObject)json;

      		if (entry.index < 0) {
      			entry.membernames = o.GetMemberNames();
      			totalObjectMembers += entry.membernames.length;
      			if (entry.membernames.length > maxObjectMembers)
      				maxObjectMembers = entry.membernames.length;
      		}
      		entry.index++;
      		if (entry.index < entry.membernames.length) {
      			total[JSONType.Member.ordinal()]++;
      			total[JSONType.String.ordinal()]++;
      			stack.push(entry);
      			nentry = new StackEntry();
      			nentry.json = o.GetMember(entry.membernames[entry.index]);
      			stack.push(nentry);
      		}      		
      	}
      }
      
      // Calculate Array and Object stats
      if (total[JSONType.Array.ordinal()] != 0) {
      	meanArrayElements = totalArrayElements/total[JSONType.Array.ordinal()];
      }
      if (total[JSONType.Object.ordinal()] != 0) {
      	meanObjectMembers = totalObjectMembers/total[JSONType.Object.ordinal()];
      }
		}
	}
	
}
