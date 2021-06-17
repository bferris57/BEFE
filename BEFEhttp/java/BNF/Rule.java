// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;

import java.util.ArrayList; // For ArrayList
import BEFE.*;
import BEFE.Error;

public class Rule {

  public enum Type {AND, OR, TYPE, TOKEN, ACTION,}
  
  // Members...
  public String    fileName;     // File defined in
  public LineCol   lineCol;      // Line/Col defined at
  public String    name;         // Rule name (may have '<...>' format)
  public Type      type;         // Rule Type (see above)
  public Integer   minOccur;     // Limits: Minimum occurrences
  public Integer   maxOccur;     // Limits: Maximum occurrences
  public Word[]    words;        // Words in the Rule
  public Integer[] recursion;    // List of words indexes identifying recursive Rules
  public String[]  referencedBy; // List of Rule.name indicating which rules reference the Rule
  public Error[]   errors;       // Syntax errors
  
  //
  // Constructors
  //
  
  private void init() {
    fileName     = null;
    lineCol      = null;
    name         = null;
    type         = null;
    minOccur     = null;
    maxOccur     = null;
    words        = null;
    recursion    = null;
    referencedBy = null;
  }
  
  public Rule() {
    init();
  }
  
  public Rule(String fileName) {
    init();
    this.fileName = fileName;
  }
  
  public Rule(Rule that) {
    this.fileName     = that.fileName;
    this.lineCol      = new LineCol(that.lineCol);
    this.name         = that.name;
    this.type         = that.type;
    this.minOccur     = that.minOccur;
    this.maxOccur     = that.maxOccur;
    this.words        = utils.ArrayCopy(that.words);
    this.recursion    = utils.ArrayCopy(that.recursion);
    this.referencedBy = utils.ArrayCopy(that.referencedBy);
  }
  
  //
  // Queries
  //
  
  public boolean IsEmpty() {
    return (fileName == null && words == null)?true:false;
  }
  
  public boolean IsRecursive() {
    return (recursion != null && recursion.length != 0);
  }
  
  public boolean IsHeadRecursive() {
    return (recursion != null && recursion[0] == 0);
  }

  public String[] References() {
    ArrayList<String> refs = new ArrayList<String>();
    if (words != null) {
      for (int i=0; i < words.length; i++) {
        Word word = words[i];
        if (word != null && (word.type == Word.Type.NAME || word.type == Word.Type.QNAME))
          refs.add(word.value);
      }
    }
    String[] result = new String[refs.size()];
    result = refs.toArray(result);
    
    return result;
  }
  
  @Override
  public String toString() {
    String result = "";
    if (words != null) {
      for (Word word : words) {
        if (result.length() != 0)
          result += " ";
        if (word.value == null)
          result += ">null<";
        else {
          if (word.type == Word.Type.DQUOTE)
            result += "\"" + utils.javaStringLiteral(word.value) + "\"";
          else if (word.type == Word.Type.SQUOTE)
            result += "'" + utils.javaStringLiteral(word.value) + "'";
          else
            result += word.value;
        }
      }
    }
    else
      result = "***Empty***";
    if (name == null)
      result = "***Unnamed*** ::=" + result;
    else
      result = name + " ::= " + result;
    return result;
  }

  // DEBUG...
  public String toDebugString() {
    String result = "Rule '"+name+"': occurs="+minOccur+":"+maxOccur+", words =";
    return result + wordsToDebugString();
  }
  
  public String wordsToDebugString() {
    String result = "";
    if (words == null)
      result += "**null**";
    else {
      for (int i = 0; i < words.length; i++) {
        Word word = words[i];
        result += " " + i + ":";
        if (word.type == Word.Type.SQUOTE)
          result += "'" + word.value + "'";
        else if (word.type == Word.Type.DQUOTE)
          result += "\"" + word.value + "\"";
        else
          result += word.value;
      }
    }
    return result;    
  }
  // ...DEBUG
  
}