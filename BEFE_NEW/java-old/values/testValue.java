package bferris.values;

import java.io.PrintStream;

import bferris.values.Value.Type;

public class testValue {

  //----------------------------------------------------------------------------
  //
  // Method: test - Test Value class
  //
  //----------------------------------------------------------------------------

	public static void test() {
		test("");
	}

	public static void testNumber(String indent) {

		String      ind;
    PrintStream out;
    
    ValueNumber number;
    String[]    invalid = {""
    		                  ,"x"
    		                  ,"+"
    		                  ,"-"
    		                  ,"."
    		                  ,"-123-"
    		                  ,"+-123"
    		                  ,"- 1"
    		                  ,"1 2"
    		                  ,"123.456.789"
    		                  ,"123.456e"
    		                  ,"-0"
    		                  ,"+0"
    		                  ,"-.0"
    		                  ,"+.0"
                          };
    String[]    valid   = {"123"
    		                  ,"-123"
    		                  ,"+123"
    		                  ,"123."
    		                  ,"123.456"
    		                  ,"0."
    		                  ,".0"
    		                  ,"-123.e+10"
    		                  ,"-123.456e10"
    		                  ,"-123.0e10"
    		                  ,"-123.1e10"
    		                  ,"inf"
    		                  ,"-inf"
    		                  ,"+inf"
    		                  ,"NaN"
    		                  ,"nan"
                          };
    int         maxlen;
    String      pad = " ";
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(indent+"Number...");

    // Null number
    out.println(ind+"Value.New(Number) = "+Value.New(Type.Number));
    out.println(ind+"new ValueNumber() = "+new ValueNumber());

    // Setting constants
    out.println();
    out.println(ind+"Value.New(Number).Set(1)      = "+Value.New(Type.Number).Set(1));
    out.println(ind+"Value.New(Number).Set('0')    = "+Value.New(Type.Number).Set('0'));
    out.println(ind+"Value.New(Number).Set(true)   = "+Value.New(Type.Number).Set(true));
    out.println(ind+"Value.New(Number).Set(-1e10)  = "+Value.New(Type.Number).Set(-1e10));
    out.println(ind+"Value.New(Number).Set(-1.)    = "+Value.New(Type.Number).Set(-1.));
    out.println(ind+"Value.New(Number).Set(+1.)    = "+Value.New(Type.Number).Set(+1.));
    out.println(ind+"Value.New(Number).Set(+5.000) = "+Value.New(Type.Number).Set(+5.000));
    out.println(ind+"Value.New(Number).Set(+5.020) = "+Value.New(Type.Number).Set(+5.020));
    out.println(ind+"Value.New(Number).Set(\"x\")    = "+Value.New(Type.Number).Set("x"));
    
    // Calculate valid/invalid padding
    maxlen = 0;
    for (String string : invalid)	if (string.length() > maxlen)	maxlen = string.length();
    for (String string : valid)	  if (string.length() > maxlen)	maxlen = string.length();
    for (int i=maxlen;i>0;i--)   	pad = pad + " ";

    // For each Invalid string...
    number = new ValueNumber();
    out.println(ind+"Invalid...");
    for (String string : invalid) {
      if (number.IsValid(string))
      	out.println(ind+"  \""+string+"\""+pad.substring(0,maxlen-string.length())+" = Valid");
    }
    
    // For each Valid string...
    out.println(ind+"Valid...");
    for (String string : valid) {
      if (!number.IsValid(string))
      	out.println(ind+"  \""+string+"\""+pad.substring(0,maxlen-string.length())+" = Invalid");
    }
    
	}
	
	public static void testString(String indent) {

		String      ind;
    PrintStream out;
    
    Value       string;
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(indent+"String...");

    // Null string
    out.println(ind+"Value.New(Type.String)        = "+Value.New(Type.String));
    out.println(ind+"new ValueString()             = "+new ValueString());

    // Number -> String
    out.println(ind+"String from Number value      = "+Value.New(Type.String).Set(Value.New(Type.Number).Set(-1234.567)));
    out.println(ind+"new ValueString(null)         = "+new ValueString(null));
    out.println(ind+"new ValueString(true)         = "+new ValueString(true));
    out.println(ind+"new ValueString(null)         = "+new ValueString(null));
    out.println(ind+"new ValueString((byte)0x24)   = "+new ValueString((byte)0x24));
    out.println(ind+"new ValueString((char)0x24)   = "+new ValueString((char)0x24));
    out.println(ind+"new ValueString((short)0x24)  = "+new ValueString((short)0x24));
    out.println(ind+"new ValueString((int)0x24)    = "+new ValueString((int)0x24));
    out.println(ind+"new ValueString((long)0x24)   = "+new ValueString((long)0x24));
    out.println(ind+"new ValueString((float)0x24)  = "+new ValueString((float)0x24));
    out.println(ind+"new ValueString((double)0x24) = "+new ValueString((double)0x24));
    
    // String as an array...
    out.println(ind+"As Array...");
    ind = ind + "  ";
    
    string = Value.New(Type.String).Set("hello world");
    out.println();
    out.println(ind+"Value.Length() = "+string.Length());    
    out.println();
    for (int i=0;i<string.length();i++) {
    	out.println(ind+"string.Get("+i+").Get_char()    = '"+string.Get(i).Get_char()+"'");
    	out.println(ind+"string.toString().charAt("+i+") = '"+string.toString().charAt(i)+"'");
    }
    out.println();
    for (int i=-1;i>=-5;i--) {
    	out.println(ind+"string.Get("+i+").Get_char() = '"+string.Get(i).Get_char()+"'");
    }
    out.println();
    for (Integer i=0;i<string.length();i++) {
    	out.println(ind+"string.Get(\""+i+"\").Get_char() = '"+string.Get(i.toString()).Get_char()+"'");
    }
    out.println();
  	out.println(ind+"string.Get(\"1.5\").Get_char() = '"+string.Get("1.5").Get_char()+"'");
  	out.println(ind+"string.Get(\"-1.5\").Get_char() = '"+string.Get("-1.5").Get_char()+"'");
  	out.println(ind+"string.Get(\"-.5\").Get_char() = '"+string.Get("-.5").Get_char()+"'");
    out.println();
    out.println(ind+"string.charAt(0)             = '"+string.charAt(0)+"'");
    out.println(ind+"string.charAt(1)             = '"+string.charAt(1)+"'");
    out.println(ind+"string.charAt(2)             = '"+string.charAt(2)+"'");
    out.println(ind+"string.charAt(3)             = '"+string.charAt(3)+"'");
    out.println(ind+"string.charAt(-1)            = '"+string.charAt(-1)+"'");
    out.println(ind+"string.charAt(-2)            = '"+string.charAt(-2)+"'");
    out.println(ind+"string.charAt(-3)            = '"+string.charAt(-3)+"'");
    out.println(ind+"string.charAt(-4)            = '"+string.charAt(-4)+"'");
    out.println(ind+"string.toCharArray()         = "+new String(string.toCharArray()));
    
	}

	public static void testId(String indent) {

		String      ind;
    PrintStream out;
    
    Value       id;
    
    ind = indent + "  ";
    out = System.out;
    
    out.println(indent+"Id...");

    // Null string
    out.println(ind+"Value.New(Type.Id) = "+Value.New(Type.Id));
    out.println(ind+"new ValueId()      = "+new ValueId());

    // Create a new empty Id value
    id = Value.New(Type.Id);
    out.println(ind+"id.Length()        = "+id.Length());
    out.println(ind+"id.Get_String()    = "+id.Get_String());
    
    // Create a new non-empty value
    id = Value.New(Type.Id,"{80000000-ffff-ffff-ffff-ffffffffffff}");
    out.println(ind+"id.Get_String()    = "+id.Get_String());

    // Create a new invalid Id value
    id = Value.New(Type.Id,"{8x-ffff-ffff-ffff-ffffffffffff}");
    out.println(ind+"id.Get_String()    = "+id.Get_String());

    // Try setting it again, should get error
    id = Value.New(Type.Id,"{80000000-ffff-ffff-ffff-ffffffffffff}");
    id = id.Set("{80000000-ffff-ffff-ffff-ffffffffffff}");
    out.println(ind+"id.Set() mutated   = "+id);
    
	}

	public static void test(String indent) {

    PrintStream out;
    String      ind;
    
    out = System.out;
    ind = indent+"  ";
    
    out.println(indent+"***Testing Value...");

    testNumber(ind);
    testString(ind);
    testId(ind);
    
  }

}
