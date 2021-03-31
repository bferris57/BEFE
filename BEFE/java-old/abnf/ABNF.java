//---------------------------------------------------------------------
//---------------------------------------------------------------------
//
// File:  ABNF.java - ABNF class declaration
//
// Class: ABNF - General ABNF object
//
// Notes: This class is a cheap and dirty attempt to implement the
//        IETF's RFC-5234.  It was written just enough to get by
//        until we can generate classes based on "real" ABNF as
//        specified in the RFC.
//
//        As such, it is subject to change as we encounter new
//        RFCs we want to implement and find it doesn't parse them
//        and generate proper code to do the real parsing.
//
//        The intent is to capture the ABNF as a data structure and
//        and generate proper Java, C/C++, etc. code to do proper
//        conformant parsing and implementation.
//
//        As such we expect to hack the sytax as required so we
//        can generate workable classes in all the destination
//        languages to do what we want.
//
//        The naive hope is that this, with other tools, will be able
//        to generate itself at some point so, once accomplished, it is
//        "throw away code" and, hence, not worth bothering with too
//        much.
//
//        Enough of that, on it with...
//
//        PS. Since it's "throw away", it's not worth documenting
//        too much, is it?
//
//---------------------------------------------------------------------
//---------------------------------------------------------------------

package bferris.abnf;

//import java.util.Stack;
import java.util.*;

public class ABNF {

	protected ABNFType   type;
  protected ABNFRule[] rules;
  
	public ABNF() {
		type = ABNFType.ABNF;
	}
	
	public boolean IsError() {
		return type == ABNFType.Error;
	}
	
	public boolean IsABNF() {
		return true;
	}
	
	//
	// Class members
	//

	// Representation
	static boolean               asJSON = true;
	
	// Parsing
	static protected boolean     conform = true;  // true = "Conform to RFC 4234", false = "Bruce's extensions to the RFC"
	static protected String[]    lines;
	static protected int         curline;
  static protected int         curcol;
  static protected ArrayList<Token> tokens;

  // ABNF Object types (Error isn't in the RFC but is implied or else it all turns into bullshit and wanking)
	static enum ABNFType {ABNF
		                   ,Error
		                   ,Terminal
		                   ,Range
		                   ,Rule
		                   ,RuleList
                       };

  // Character transformations (in RFC as quoted strings)
  static public char    Upper(char c)   {return (c>='a'&&c<='z') ? (char)(c-'a'+'A') : c;};
  static public char    Lower(char c)   {return (c>='A'&&c<='Z') ? (char)(c-'A'+'a') : c;};
  static public char[]  Upper(char[] c) {return new String(c).toUpperCase().toCharArray();}
  static public char[]  Lower(char[] c) {return new String(c).toLowerCase().toCharArray();}
  
  static void PushToken(Token tok) {
  	//String col = String.format("%d",tok.col);
  	//String lin = String.format("%d",tok.line);
  	//System.out.println(String.format("%3s/%-3s",lin, col)+": type = "+tok.type+", val = "+tok.val);
  	//System.out.println(tok);
  	tokens.add(tok);
  }
  
  static ABNF Parse(String str) {

  	String              line;
  	String              indent;
  	ABNF                abnf;
  	//ArrayList<ABNFRule> rulelist;
    char                c;                     // Current character
    String              rulename;              // Name of rule
    //ArrayList<Token>    tokenlist;             // Current list of tokens !!!Should go into a stream somewhere!!!
    Token               tok;                   // Current token
  	
    
    int    numrules = 0;
    int    numcomments = 0;
    int    numquotes   = 0;
    int    quotecol;
    
  	if (str == null)
  		return new ABNFError("Source is null");
    if (str.length() < 2 || !str.substring(str.length()-2).equals("\r\n"))
    	return new ABNFError("Source doesn't terminate in CR/LF");

    synchronized (ABNF.class) {
  		
	  	// Initialise
	  	lines    = str.split("\r\n");
      curline  = 0;
      curcol   = 0;
      abnf     = new ABNF();
      //rulelist = new ArrayList<ABNFRule>();
      
      // For each line...
      indent = null;
      tokens = new ArrayList<Token>();
      
      for (curline=1;curline<=lines.length;curline++) {

      	// Get current line and replace all tabs with space
      	line = lines[curline-1].replaceAll("\t"," ");

      	// Validate indentation
      	if (indent == null) {
      		for (curcol=1;curcol<=line.length();curcol++) {
      			c = line.charAt(curcol-1);
      			if (c != ' ')
      				break;
      		}
      		indent = line.substring(0,curcol-1);
      	}
      	else {
      		if (line.length() == 0)
      			continue;
    			curcol = indent.length();
    			if (curcol > line.length())
    				curcol = line.length();
      		if (!indent.substring(0,curcol).equals(line.substring(0,curcol))) {
      			abnf = new ABNFError("Indentation doesn't match first line");
      			break;
      		}
      		if (curcol < indent.length())
      			continue;
      	}
      	
      	// Make sure it only contains valid 7-bit ascii characters excluding NUL...
      	// While we're at it strip off trailing comments (that aren't in quotes)
      	quotecol = 0;
  	  	for (curcol=1;curcol<=line.length();curcol++) {
  	  		c = line.charAt(curcol-1);
  	  		if (c > 0x7f)
  	  			return new ABNFError(String.format("ABNF definition contains invalid 7-bit ascii character 0x%x",(int)c));
          if (c < 0x20 && c != '\n' && c != '\r' && c != '\t')
          	return new ABNFError(String.format("ABNF definitions does not allow control character 0x%x",(int)c));
          if (c == '"') {
          	if (quotecol > 0)
          		quotecol = 0;
          	else {
          		numquotes++;
          		quotecol = curcol;
          	}
          }
          if (c == ';' && quotecol == 0) {
        		line = line.substring(0,curcol-1);
        		numcomments += 1;
          }
  	  	}
        if (quotecol > 0) {
        	curcol = quotecol;
        	return new ABNFError("Unterminated string literal");        	
        }
        
        // Start at beginning of line
  	  	curcol = indent.length()+1;

  	  	//System.out.println("Line: "+curline+" = "+line);
  	  	if (curcol >= line.length())
  	  		continue;
  	  	
        //
  	  	// Parse rulename...
        //
  
  	  	c = line.charAt(curcol-1);
  	  	tok = new Token();
  	  	
  	  	if (c != ' ') {
        	
        	int endname;
        	
          numrules += 1;
          // Find next non-whitespace or '='...
          endname = curcol;
          for (endname=curcol;endname<line.length();endname++) {
          	c = line.charAt(endname-1);
          	if (c == ' ' || c == '=')
          		break;
          }
          if (endname-curcol <= 0) {
          	abnf = new ABNFError("Missing rule name");
          	break;
          }
          rulename = line.substring(curcol-1,endname-1);
          tok.type = "DEF";
          tok.val = rulename;
          PushToken(tok);
          
          // Validate the first character of the rule name
          c = rulename.charAt(0);
          if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))) {
          	abnf = new ABNFError("Rule names must start with an alphabetic character");
          	break;
          }
          
          // Skip till past trailing whitespace
          for (curcol=endname;curcol<line.length();curcol++) {
          	c = line.charAt(curcol-1);
          	if (c != ' ')
          		break;
          }
          
          if (curcol > line.length()) {
          	abnf = new ABNFError("Missing '=' after Rule name");
          	break;
          }
          curcol = curcol+1;
          
          // Handle "=/"
          if (curcol < line.length() && line.charAt(curcol-1) == '/') {
          	System.out.println("Line : "+curline+" rule =/ "+rulename+" =/ ...");
          	curcol++;
          }
          
    	  	// Skip trailing whitespace
          for (;curcol<=line.length();curcol++) {
          	c = line.charAt(curcol-1);
          	if (c != ' ')
          		break;
          }
          if (curcol >= line.length()) {
          	abnf = new ABNFError("Missing elements");
          	break;
          }
        }

  	  	//
  	  	// Parse elements...
  	  	//

  	  	while (curcol<=line.length()) {
	  	  	
  	  		// Skip leading whitespace
	        for (;curcol<=line.length();curcol++) {
	        	c = line.charAt(curcol-1);
	        	if (c != ' ')
	        		break;
	        }
  	  		
	        // If nothing left, finished
	        if (curcol >= line.length())
	        	break;
	        
	        //
	        // Parse leading [repeat]
	        //

	        tok = new Token();
	        c = line.charAt(curcol-1);
	        if ( (c >= '0' && c <= '9') || c == '*') {
	        	tok.type = "rep";
	  	  		// Until end of leading digit...
	        	while (curcol<line.length()) {
	        		c = line.charAt(curcol-1);
	        		if ((c>='0' && c <= '9') || c == '*')
	        			curcol++;
	        		else
	        			break;
	        	}
	        	// Output token if any
	          tok.val = line.substring(tok.col-1,curcol-1);
	          if (tok.val.length() != 0)
	          	PushToken(tok);
	  	  	}
	  	  	
	        //
	        // Based on the first character...
	        //

	        tok = new Token();
	        tok.val = "Ref";
	        c = line.charAt(curcol-1);
	        
	        // Handle %...	
          if (c =='%') {
          	// Skip until next whitespace or terminal
          	for (;curcol<=line.length();curcol++) {
          		String s = line.substring(curcol-1,curcol);
          		c = line.charAt(curcol-1);
          		if (" *()[]".contains(s))
          			break;
          	}
          	tok.type = "lit";
          	tok.val = line.substring(tok.col-1,curcol-1);
          	PushToken(tok);
          }
           
          else if (c == '"') {

          	// Skip until next '"'...
          	curcol++;
          	for (;curcol<=line.length();curcol++) {
          		c = line.charAt(curcol-1);
          		if (c == '"')
          			break;
          	}
            curcol++;
            tok.val = line.substring(tok.col-1,curcol-1);
            tok.type = "lit";
            PushToken(tok);
          }
          	
          else if ("([])/".contains(line.substring(curcol-1,curcol))) {

          	curcol++;
          	tok.val = line.substring(tok.col-1,curcol-1);
          	tok.type = "opr";
          	PushToken(tok);

          }
          	
        	else {
        		
        		// Skip until whitespace...
          	for (;curcol<=line.length();curcol++) {
          		String s = line.substring(curcol-1,curcol);
          		c = line.charAt(curcol-1);
          		if (" *()[]".contains(s))
          			break;
          	}
        		
        		if (curcol-tok.col > 0) { 
          		tok.val = line.substring(tok.col-1,curcol-1);
          		tok.type = "Ref";
            	PushToken(tok);
        		}
        	}

	        if (abnf.IsError())
	        	break;
  	  	}
  	  	
  	  	if (abnf.IsError())
  	  		break;
      }
      
  	}
  	
    //System.out.println("*** "+curline+" lines, "+numrules+" rules, "+numcomments+" comments, "+numquotes+" string literals processed***");
    
  	return abnf;
  }

  public String ToJSON() {
  	
  	Token token;
  	int   i;
  	
  	String json;
  	int    defno = -1;
  	int    reltok = 0;
  	int    maxdeflen = 0;
  	String pad;
  	
  	// Computing rule name padding
  	for (i=0;i<tokens.size();i++) {
  		token = tokens.get(i);
  		if (token.type != "DEF")
  			continue;
      if (token.val.length() > maxdeflen)
      	maxdeflen = token.val.length();  		
  	}
  	for (pad="",i=0;i<maxdeflen;i++) pad = pad + " ";
  	
  	// Form the initial JSON
  	json = "{";

  	// For each token...
  	for (i=0;i<tokens.size();i++) {

  		token = tokens.get(i);
  		if (token.type == "DEF") {
  			defno++;
  			if (defno != 0)
  				json = json + "]\n,";
  			json = json + pad.substring(0,maxdeflen-token.val.length())+"\"" + token.val + "\":[";
  			reltok = 0;
  		}
  		else {
  			if (reltok != 0)
  				json = json + ",";
  			json = json + token.ToJSONArray();
  			reltok++;
  		}
  	}
  	
  	// Finish off last token
  	if (reltok != 0)
    	json = json + "]\n";
    json = json + "}";

    return json;
  	
  }
  
  @Override
  public String toString() {
  	if (asJSON)
  		return ToJSON();
  	return "ABNF Instance";
  }
}
