// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;

import java.nio.file.*; // For File, Path, etc.
import java.io.*;       // For PrintStream, etc.
import java.util.*;     // For ArrayList, List, Map, etc.
import BEFE.*;
import BEFE.Error;
import BEFE.SyntaxError;
import BEFE.InternalError;
import BEFE.Stack;
import BEFE.out;        // For proper output
import BEFE.Debug;      // For debug output

public class Rules {

  private enum   State {
                        /** NULL State, is able to process more files */
                        NULL,
                        /** ERROR State, Rules has Errors and is finished processing, can't go any further */
                        ERROR,
                        /** SYNTAX State, Rules have been Syntax Checked */
                        SYNTAX,
                        /** ANONYMISED State, Rules have been anonymised */
                        ANONYMISED,
                        /** RESOLVED, Rules is finished, resolved, and ready to be used */
                        RESOLVED,
                       }
  
  // Statics...
  private static String startOps = "([{";
  private static String endOps   = ")]}";
  
  // Members...
  private Map<String,Rule>   rules;
  private List<Error>        errors;
  private boolean            isValid;
  private State              state;   
  private List<String>       files;
  private int                anonID = 0;
  
  //
  // Constructors
  //
  
  public Rules() {
    rules   = new HashMap<String,Rule>();
    errors  = new ArrayList<Error>();
    isValid = false;
    state   = State.NULL;
    files   = new ArrayList<String>();
    
  }
  
  //
  // Utility methods
  //
  
  private void postError(Error err) {
    errors.add(err);
  }
  private void postError(String msg) {
    errors.add(new Error(msg));
  }
  private void postError(String fileName, String msg) {
    errors.add(new Error(fileName,msg));
  }
  public void postError(LineCol lineCol, String msg) {
    errors.add(new SyntaxError(lineCol, msg));
  }
  public void postError(String fileName, LineCol lineCol, String msg) {
    errors.add(new SyntaxError(fileName, lineCol, msg));
  }
  public void postError(Word word, String msg) {
    errors.add(new SyntaxError(word.fileName, word.lineCol, msg));
  }

  public void printErrors(String prefix) {
    Error[] errs = errors.toArray(new Error[errors.size()]);
    utils.printErrors(prefix,errs);
  }
  
  /**
  Processes Word[] array and returns list of Rule Spans in the array.
  */
  private Span[] getRuleSpans(Word[] words) {
    
    List<Span>    spans = new ArrayList<Span>();
    Span          span = new Span(0,words.length,1);
    Word          word;
    int           pos;
    int           expDepth = 0; // Expression depth
    
    if (words == null)
      return null;

    for (pos = 0; pos < words.length; pos++) {
      word = words[pos];
      if (word.type == Word.Type.OPERATOR) {

        // Manage expression counter...
        if (word.value.equals("{") ||
            word.value.equals("(") || 
            word.value.equals("[")) {
          expDepth++;
          continue;
        }
        if (word.value.equals("}") ||
            word.value.equals(")") || 
            word.value.equals("]")) {
          expDepth--;
          continue;
        }

        //  ";" and "::=" rules only apply if not within expression...
        if (expDepth >= 1)
          continue;          
        
        if (word.value.equals(";")) {
          span.end = pos;
          if (span.end > span.start)
            spans.add(new Span(span));
          pos++;
          span.start = pos;
          span.end = words.length;
          continue;
        }
        
        //
        if (word.value.equals("::=")) {
          span.end = pos - 1;
          if (span.end > span.start) {
            spans.add(new Span(span));
          }
          span.start = pos-1;
          span.end = words.length;
          continue;
        }
      }
    }

    // Add last one
    span.end = pos;
    if (span.start < words.length && span.end > span.start)
      spans.add(span);      
    
    // Get return value
    Span[] result = new Span[spans.size()];
    result = spans.toArray(result);
    
    return result;
      
  }

  /**
  Determines if a given Rule name is Anonymous
  */
  boolean isAnonymous(String name) {
    if (name == null)
      return false;
    // Find last non-digit
    int lnd;
    for (lnd=name.length()-1; lnd >= 0; lnd--) {
      char c = name.charAt(lnd);
      if (c < '0' || c > '9')
        break;
    }
    // Check it...
    String anon = "_anon_";
    if (lnd != 5 || !name.regionMatches(0,anon,0,anon.length()))
      return false;
    return true;
  } 
  
  /**
  Determines if a given Rule is anonymous AND was (...)
  */
  boolean isParen(String ruleName) {
    if (isAnonymous(ruleName) && rules.containsKey(ruleName)) {
      Rule that = rules.get(ruleName);
      if (that.minOccur == 1 && that.maxOccur == null)
        return true;
    }
    return false;
  }
  
  /**
  Determines if a given Rule is anonymous AND was (...)
  */
  boolean isBracket(String ruleName) {
    if (isAnonymous(ruleName) && rules.containsKey(ruleName)) {
      Rule that = rules.get(ruleName);
      if (that.minOccur == 0 && that.maxOccur == 1)
        return true;
    }
    return false;
  }
  
  //
  // Query methods
  //
  
  public Error[] getErrors() {
    if (errors.size() == 0)
      return null;
    Error errs[] = errors.toArray(new Error[errors.size()]);
    return errs;
  }

  public boolean hasErrors() {
    return (errors != null && errors.size() != 0);
  }  

  //
  // Print methods
  //

  public void printRules(String prefix) {
    printRules(prefix,false);
  }
  
  public void printRules(String prefix,boolean verbose) {
    
    String[] keys = new String[0];
    
    if (rules == null) {
      out.println(prefix+"***No Rules***");
      return;
    }
    
    // Get longest rule name...
    int maxLen = 0;
    for (String key : rules.keySet())
      maxLen = Math.max(maxLen,key.length());
    
    // Get the list of keys and sort it
    keys = rules.keySet().toArray(keys);
    Arrays.sort(keys);

    // Print them...
    for (String key : keys) {

      String output = key;
      Rule   rule;
      Span   occur;
      int    numWords;

      rule = rules.get(key);

      output += utils.replicateChar(' ',maxLen-key.length());
      output += ": ";
      occur = new Span(rule.minOccur,rule.maxOccur);
      output += rule.type;
      output += " ("+occur+") ";
      if (rule.words == null)
        numWords = 0;
      else
        numWords = rule.words.length;
      for (int i = 0; i < numWords; i++) {
        Word word = rule.words[i];
        if (i != 0)
          output += " ";
        if (word.type == Word.Type.DQUOTE || word.type == Word.Type.SQUOTE) {
          if (word.value.equals("'"))
            output += "'''";
          else if (word.value.equals("\""))
            output += "\"\"\"";
          else
            output += "\"" + utils.javaStringLiteral(word.value) + "\"";
        }
        else
          output += word.value;
      }
      out.println(prefix+output);

      // If verbose, detail the words...
      if (verbose) {
        String[] lines = Word.outputWordsFiltered(rule.words,new Word.OptionsNominal());
        for (String line : lines)
          out.println("         "+line);
      }

    } 
    
  }
  
  //
  // Syntax Methods
  //
  
  /**
  Checks Syntax recording any malformed Rule declarations for a given rule.
  
  Return true if syntax okay and false if syntax not okay.
  */
  private boolean syntax(Rule rule) {

    Word[]      words        = rule.words;
    Stack<Word> stack        = new Stack<Word>();
    int         startErrors  = errors.size();
    String[][]  matching     = {{"(",")"},
                                {"[","]"},
                                {"{","}"}
                               };
    String[]    startOps     = {"(","[","{",};

    class BREAK extends Exception {BREAK(){}} // Sneeky bollocks for flow control
    BREAK Break = new BREAK();
    
    try { // For flow control NOT exceptions...
    
      if (state != State.NULL) {
        String msg = "Rules.syntax cannot be called unless in the NULL State";
        if (rule == null || rule.words == null || rule.words.length == 0)
          postError(new InternalError(msg));
        else
          postError(rule.words[0].lineCol,msg);
        throw Break;
      }
      
      // Validate it's not a reserved name form...
      if (isAnonymous(rule.name)) {
        postError(new SyntaxError(rule.lineCol, "Anonymous name form reserved"));
        throw Break;
      }

      for (int wordIdx = 0; wordIdx < words.length; wordIdx++) { // For each Word
        
        Word word = words[wordIdx];
        
        // We're only interested in OPERATOR Words here
        if (word.type != Word.Type.OPERATOR)
          continue;

        // Validate '..' Ranges
        if (word.value.equals("..")) { // Range Checks...
          Word prevWord = (wordIdx == 0)              ? null : words[wordIdx-1];
          Word nextWord = (wordIdx+1 >= words.length) ? null : words[wordIdx+1];

          if (prevWord == null || nextWord == null) {
            if (prevWord == null)
              postError(word,"Expected word before '..'");
            if (nextWord == null)
              postError(word,"Expected word after '..'");
            continue;
          }
          
          if (prevWord.type != Word.Type.HEX    &&
              prevWord.type != Word.Type.INT    &&
              prevWord.type != Word.Type.DQUOTE &&
              prevWord.type != Word.Type.SQUOTE) {
            postError(prevWord,"Expected hex or single character string literal here");
          }
          if (prevWord.type != Word.Type.HEX && prevWord.type != Word.Type.INT && prevWord.len != 1)
            postError(prevWord,"Expected single character string literal here");

          if (nextWord.type == Word.Type.HEX    &&
              nextWord.type == Word.Type.INT    &&
              nextWord.type != Word.Type.DQUOTE &&
              nextWord.type != Word.Type.SQUOTE &&
              nextWord.type != Word.Type.OPERATOR)
            postError(nextWord,"Expected hex, single character string literal, or '('  here");
            
          else if (nextWord.type != Word.Type.HEX &&
                   nextWord.type != Word.Type.INT &&
                   nextWord.type != Word.Type.OPERATOR &&
                   nextWord.len  != 1)
            postError(nextWord,"Multiple character string literals invalid here");
            
          if (nextWord.type == Word.Type.OPERATOR && !nextWord.value.equals("("))
            postError(nextWord,"Only operator '(' allowed here");
          
          continue;
        } // ...Range Checks
        
        // Push onto stack if '(', '[' and '{'...
        if (Arrays.asList(startOps).contains(word.value)) {
          stack.push(word);
          continue;
        }
        
        // Handle '|'...
        if (word.value.equals("|")) {
          if (wordIdx == 0) {
            postError(word,"Expected something before '|'");
            continue;
          }
          if (wordIdx == words.length-1) {
            postError(word,"Expected something after '|'");
            continue;
          }
          Word prevWord = words[wordIdx-1];
          Word nextWord = words[wordIdx+1];
          String badOpBefore = "([{|::=";
          if (prevWord.type == Word.Type.OPERATOR && badOpBefore.contains((CharSequence)prevWord.value) ) {
            postError(word,"'|' not valid after '"+prevWord.value+"'");
            continue;
          }
          continue;
        }
        
        // Handle mismatched parens...
                                     
        boolean foundEndParen = false;
        
        for (String[] check : matching) {
          
          String end   = check[1];
          String start = check[0];
          Word   TOS   = stack.TOS();
          
          if (word.value.equals(end)) {
            foundEndParen = true;
            if (TOS == null)
              postError(word,"'"+end+"' without matching '"+start+"'");
            else if (!TOS.value.equals(start))
              postError(word,"'"+end+"' conflicts with preceeding '"+TOS.value+"' at "+TOS.lineCol.to1String());
            else {
              if (stack.size() <= 1 && TOS.fileSeq == word.fileSeq-1)
                postError(TOS,"Empty expression");
              stack.pop();
              break;
            }
          }
        }
        
      } // ...For each Word
      
      // Handle unterminated parens
      if (stack.size() != 0) {
        for (int i = stack.size()-1; i >= 0; i--) {
          Word word = stack.get(i);
          postError(word,"Unterminated '"+word.value+"'");
        } 
      }

      throw Break;

    } // ...For flow control NOT exceptions
    catch (BREAK b) {}
    
    return (errors.size() == startErrors);
    
  }

  /**
  Anonymise all rules by turning multiple Word expressions into separate, anonymous, rules
  named "_anon_<N>" where <N> is a unique number.
  <p>Note: Puts Rules into the ANONYMISED state.
  <p>Note: Previous calls to syntax() ensure all expressions ( (...), [...], and {...}) 
  are terminatesd and non-empty AND all '|' occurrences are not empty on the left and right.
  */  
  private void anonymise() {
    
    ArrayList<String> ruleNames;
    
    class Expression {
      char      value;      // Expression start character (0 = Beginning of Words)
      int       start;      // Index (in ArrayList<Word>) of start of this expression)
      int       pos;        // Index of current position in expression
      boolean   hasOR;      // Current expression contains '|'
      Occurs    occurs;     // Optionsl Occurs
      Word      occursWord; // Where occurs was declared, if anywhere
      public Expression(char value, int start) {
        this.value      = value;
        this.start      = start;
        this.pos        = start;
        this.hasOR      = false;
        this.occurs     = null;
        this.occursWord = null;
      }
      public String toString() {
        String str = "";
        str += "value = '"+value+"'";
        str += ", start = "+start;
        str += ", pos = "+pos;
        str += ", hasOR = "+hasOR;
        str += ", occurs = "+occurs;
        str += ", occursWord = "+occursWord;
        return str;
      }
    };

    Stack<Expression> stack = new Stack<Expression>();
    
    if (state != State.NULL) {
      postError(new InternalError("Rules.anonymise cannot be called unless in the NULL State"));
      return;
    }

    // Put all the rule names into the list...
    ruleNames = new ArrayList<String>(rules.keySet());

    // For each rule...
    for (String ruleName : ruleNames) { // For each Rule...
      
      Rule              rule    = rules.get(ruleName);
      int               curPos  = 0;
      int               actionCounter = 0;

      // Initialise stack
      stack.clear();
      stack.push(new Expression((char)0x00,0));

      // Ignore TYPE rules...
      if (rule.type == Rule.Type.TYPE)
        continue;
      
      while (curPos < rule.words.length) { // For each Word...
      
        Expression TOS   = stack.TOS();        
        Word       word  = rule.words[curPos];
        Word[]     words = rule.words;
        Rule       anon;

        if (word.type == Word.Type.OPERATOR) { // OPERATOR...

          Span   rSpan = new Span(TOS.pos,curPos+1); // Word Span to remove
          Span   wSpan = new Span(TOS.pos,curPos);     // Word Span to anonymise
          char   curChar = word.value.charAt(0);
          
          // Handle actions seperately...
          if (curChar == '{' && actionCounter == 0) {
             stack.push(new Expression('{',curPos));
             actionCounter++;
          }
        
          // If in action...
          else if (actionCounter != 0) {
            if (curChar == '{')
              actionCounter++;
            else if (curChar == '}')
              actionCounter--;
            if (actionCounter == 0) {
              TOS.occurs = new Occurs(1,1);
              wSpan.end = curPos+1;
              anon = newAnon(rule,new LineCol(rule.words[TOS.start].lineCol),TOS.occurs,wSpan,rSpan);
              stack.pop();
            }
            curPos = rSpan.start + 1;
            continue;

          }
          
          // Handle beginning of expression...
          else if (curChar == '(' || curChar == '[' || curChar == '{') {
            
            Expression exp = new Expression(curChar,curPos);
            Word       prev = null;
            exp.pos = curPos+1;
            stack.push(exp);
            TOS = stack.TOS();
            
            // Get previous word if any
            if (curPos > 0)
              prev = rule.words[curPos-1];
            
            // Consume leading/butting range, if any and we're a '('...
            if (prev != null && curChar == '('
                && (prev.type == Word.Type.RANGE ||
                     (prev.type == Word.Type.OPERATOR &&
                      prev.value.equals("*")
                     )
                   )
                && (int)prev.span.end == (int)word.span.start) {
              TOS = stack.TOS();
              if (prev.type == Word.Type.OPERATOR)
                TOS.occurs = new Occurs(0,null);
              else {
                TOS.occurs     = new Occurs(rule.words[curPos-1].value);
                TOS.occursWord = new Word(rule.words[curPos-1]);
              }
              TOS.start--;
              TOS.pos--;
              rule.words = Word.ArrayRemove(rule.words,curPos-1);
              curPos--;
            }
            else
              TOS.occurs = new Occurs(1,1);

          }
          
          // Handle end of expressions...            
          else if (curChar == ')' || curChar == ']') {

            // Figure out Occurs...
            if (curChar == ')') {
              if (TOS.occurs == null)
                TOS.occurs = new Occurs(1,1);
            }
            else
              TOS.occurs = new Occurs(0,1);
            
            if (TOS.value == '|') { // We're in a '|'...
              if (curPos - TOS.pos > 1) {
                rSpan = new Span(TOS.pos,curPos);
                anon = newAnon(rule,new LineCol(rule.words[TOS.start].lineCol),TOS.occurs,wSpan,rSpan);
                curPos   -= rSpan.size()-1;
              }
              stack.pop();
              TOS = stack.TOS();
              TOS.hasOR = true;
              rSpan = new Span(TOS.start,curPos+1); // Word Span to remove
              wSpan = new Span(TOS.start+1,curPos); // Word Span to anonymise
            }
            else { // We're on our own here...
              rSpan = new Span(TOS.start,curPos+1);
              wSpan = new Span(TOS.start+1,curPos);
            }

            // Handle trailing '*' or RANGE...
            if (curPos < rule.words.length-1) {
              Word    next   = rule.words[curPos+1];
              Range   occurs = null;
              boolean doit   = false;
              if (next.type == Word.Type.RANGE && next.span.start == word.span.end)
                occurs = new Range(next.value);
              else if (next.type == Word.Type.OPERATOR && next.value.equals("*"))
                occurs = new Range(0,null);
              if (occurs != null) { // Have Occurrence...
                // Sanity checks...
                if (TOS.occurs != null && TOS.occurs.equals(occurs)) {
                  postError(next,"Redundant occurance spec");
                  break;
                }
                else if (TOS.occursWord != null) {
                  postError(TOS.occursWord,"Conflicting occurence specification");
                  postError(next,"Conflicting occurence specification");
                  break;
                }
                else
                  doit = true;
              } // ...Have Occurrence
              if (doit && occurs != null) {
                rule.words = Word.ArrayRemove(rule.words,curPos+1);
                TOS.occurs = new Occurs(occurs);
              }
              
            }
            
            // Normal case...
            if (rSpan.size() <= rule.words.length) { // Anonymise if only part of the words...
              if (rSpan.size() == rule.words.length) { // Expression is entire rule.words...
                rule.words = Word.slice(rule.words,wSpan);
                rule.minOccur = TOS.occurs.min;
                rule.maxOccur = TOS.occurs.max;
              }
              else { // Expression part of words...
                anon = newAnon(rule,new LineCol(rule.words[TOS.start].lineCol),TOS.occurs,wSpan,rSpan);
                if (TOS.hasOR)
                  anon.type = Rule.Type.OR;
              }
              curPos = rSpan.start;
            }
            else { // It's the whole words, substitute instead of anonymising...
              // Compute combined minOccur/maxOccur
              char c1 = stack.get(1).value;
              char c0 = stack.get(0).value;
              if (c1 == c0) { // Sanity check
                postError(new SyntaxError(rule.fileName,rule.words[0].lineCol, "Multiple nesting of '"+c1+"' achieves nothing"));
                break;
              }
              rule.minOccur = 0;
              rule.maxOccur = null;
              // Replace words with inner content
              rule.words = Word.slice(rule.words,wSpan);
            }

            // Get rid of TOS 
            stack.pop();

          }
          
          // Handle '|'
          else if (curChar == '|') {

            // If we're inside something, post new '|' on stack...
            if (TOS.value != '|') {
              TOS.hasOR = true;
              stack.push(new Expression(curChar,TOS.start));
              TOS = stack.TOS();
              TOS.pos = TOS.start+1;
              TOS.hasOR = true;
            }
            // Anonymise to the left if more than one term...
            if (curPos-TOS.pos > 1) { // Anonymise...
              anon = newAnon(rule,new LineCol(rule.words[TOS.start].lineCol),new Occurs(1,1),wSpan,rSpan);
              curPos    = rSpan.start;
              TOS.start = curPos+1;
              TOS.pos   = curPos+1;
            }
            else { // Get rid of '|'
              rule.words = Word.ArrayRemove(rule.words,curPos);
              TOS.start = curPos;
              TOS.pos   = curPos;
            }
          }
          
          // Handle '..' ...
          else if (word.value.equals("..")) {
            Word newWord = new Word(rule.words[curPos-1]);
            newWord.type = Word.Type.RANGE;
            newWord.value += "..";
            if (rule.words[curPos+1].type != Word.Type.OPERATOR) { 
              newWord.value    += rule.words[curPos+1].value;
              newWord.span.end  = rule.words[curPos+1].span.end;
              rSpan = new Span(curPos-1,curPos+2);
            }
            else {
              newWord.span.end = rule.words[curPos].span.end;
              rSpan = new Span(curPos-1,curPos+1);
            }
            newWord.len = newWord.value.length();
            rule.words = Word.ArrayRemove(rule.words,rSpan);
            rule.words = Word.ArrayInsert(rule.words,newWord,curPos-1);
            curPos = curPos-1;
          }
          
          // Handle '*'...
          else if (word.value.equals("*") && curPos + 1 < rule.words.length) {
            Word next = rule.words[curPos+1];
            if (next.type == Word.Type.OPERATOR &&
                (int)next.span.start == (int)word.span.end) {
              // Nothing to do, will get consumed shortly
            }
            else
              postError(new SyntaxError(word.lineCol,"Invalid context for operator '"+word.value+"'"));
          }

          // Handle '.'...
          else if (curChar == '.') { // Handle TYPE."..."
            
            // If not in right context, error...
            if (curPos <= 0)
              postError(word,"Operator '.' makes no sense here");
            else if (curPos >= words.length-1)
              postError(word,"Dangling operator '.'");
            else if (words[curPos-1].type != Word.Type.NAME)
              postError(words[curPos-1],"Expected name here");
            else if (curPos >= rule.words.length)
              postError(word,"Dangling '.'");
            else if (words[curPos+1].type != Word.Type.SQUOTE && words[curPos+1].type != Word.Type.DQUOTE)
              postError(words[curPos+1],"Expected string literal here");
            else if (!rules.containsKey(words[curPos-1].value))
              postError(words[curPos-1],"Expected Type name here");
            else if (rules.get(words[curPos-1].value).type != Rule.Type.TYPE)
              postError(words[curPos-1],"Expected Type name here");
            else { // Combine them...
              char quoteChar   = (words[curPos+1].type == Word.Type.SQUOTE)?'\'':'"';
              Rule newRule     = new Rule();
              newRule.fileName = rule.fileName;
              newRule.lineCol  = words[curPos-1].lineCol;
              newRule.name     = words[curPos-1].value + "."+ quoteChar + words[curPos+1].value + quoteChar;
              newRule.type     = Rule.Type.TOKEN;
              newRule.minOccur = 1;
              newRule.maxOccur = 1;
              newRule.words    = new Word[1];
              newRule.words[0] = new Word(words[curPos+1]);
              
              // Create new TOKEN Rule if it doesn't already exist
              if (!rules.containsKey(newRule.name))
                rules.put(newRule.name,newRule);

              // Adjust words
              rule.words = Word.ArrayRemove(rule.words,new Span(curPos,curPos+2));
              curPos--;
              rule.words[curPos].value = newRule.name;
              rule.words[curPos].len   = newRule.name.length();
            } // ...Combine them
                 
          } // ...Handle '.'
          
          // Handle other operators...
          else {
            postError(new SyntaxError(word.lineCol,"Invalid context for operator '"+word.value+"'"));
          }
          
        } // ...OPERATOR

        curPos++;
          
      } // ...For each Word
      
      // Update Rule type
      Expression TOS = stack.TOS();
      
      if (TOS.start != 0 && TOS.start < rule.words.length) {
        
        rule.type = Rule.Type.OR;

        // Anonymise remainder if more than one word...
        if (rule.words.length - TOS.start > 1 && TOS.start != 0) {
          Span wSpan = new Span(TOS.start,rule.words.length);
          Span rSpan = new Span(TOS.start,rule.words.length);
          Rule anon = newAnon(rule,new LineCol(rule.words[TOS.start].lineCol),new Occurs(1,1),wSpan,rSpan);
        }
      }
      else if (rule.type == null)
        rule.type = Rule.Type.AND;

    } // ...For each Rule

    // We're anonymised now
    if (errors != null && errors.size() != 0)
      state = State.ERROR;
    else
      state = State.ANONYMISED;
    
    return;
    
  }

  /**
   Creates a new anonymous Rule based on word span and startChar.
  
   Note: keepSpan and removeSpan are EXCLUSIVE   
  */  
  private Rule newAnon(Rule rule,LineCol lineCol,Occurs occurs,Span wordSpan,Span removeSpan) {

    if (wordSpan == null       ||
        rule == null           ||
        rule.words == null     ||
        rule.words.length == 0 ||
        lineCol == null        || 
        wordSpan == null       ||
        removeSpan == null     ||
        rules == null
       )
      return null;

    Rule anon  = new Rule();
    Span wSpan = new Span(wordSpan).normalise();
    Span rSpan = new Span(removeSpan).normalise();
    
    
    anon.name = "_anon_"+(anonID++);
    anon.fileName = rule.words[0].fileName;
    anon.type = Rule.Type.AND;
    if (occurs != null) {
      anon.minOccur = occurs.min;
      anon.maxOccur = occurs.max;
    }
    else {
      anon.minOccur = 1;
      anon.maxOccur = 1;
    }
    
    anon.words = Word.ArrayGetSpan(rule.words,wSpan);
    
    // Special check for actions (was easiest to put it here)
    // Note: Did this so, later, ACTION rules exist and "unresolved" isn't applied to contents
    if (rSpan != null && rSpan.start >= 0                  &&
        rSpan.start < rule.words.length                    &&
        rule.words[rSpan.start].type == Word.Type.OPERATOR &&
        rule.words[rSpan.start].value.equals("{")
       )
      anon.type = Rule.Type.ACTION;
    
    // Create the anonymous rule
    rules.put(anon.name,anon);

    Word newWord    = new Word();
    newWord.type    = Word.Type.NAME;
    newWord.lineCol = new LineCol(lineCol);
    newWord.len     = anon.name.length();
    newWord.value   = new String(anon.name);
    
    if (removeSpan != null)
      rule.words = Word.ArrayRemove(rule.words,rSpan);
    rule.words = Word.ArrayInsert(rule.words,newWord,rSpan.start);
    
    return anon;
    
  }

  /**
  Check for unresolved rules
  */
  private void checkUnresolved() {
    
    if (state != State.ANONYMISED) {
      postError(new InternalError("Rules.checkUnresolved cannot be called unless in the ANONYMISED State"));
      return;
    }

    // For each rule...
    String[] ruleNames = rules.keySet().toArray(new String[0]);
    Arrays.sort(ruleNames);
    
    for (String ruleName : ruleNames) {
      Rule rule = rules.get(ruleName);
      if (rule.type == Rule.Type.ACTION || rule.type == Rule.Type.TYPE)
        continue;
      for (Word word : rule.words) {
        String tValue = word.value;
        if (word.type != Word.Type.NAME)
          continue;
        // Trim trailing '.' so we can handle TYPE."..."
        if (tValue.charAt(tValue.length()-1) == '.')
          tValue = tValue.substring(0,tValue.length()-1);
        // Check it...
        if (!rules.containsKey(tValue))
          postError(word.fileName,word.lineCol,"Rule '"+word.value+"' not declared");
      }      
    }
    
  }
  //
  // Processing Methods
  //
  
  public Error[] processFile(String fileName) {

    byte[] data;
    Word[] words;
    Word[] ruleWords;
    
    if (state != State.NULL) {
      postError(new InternalError("Rules.processFile: Can only be invoked in NULL state"));
      return getErrors();
    }
    if (fileName == null) {
      postError("Rules.processFile() expected file name, got null");
      return getErrors();
    }
    
    // Get the file contents
    {
      Path path = Paths.get(fileName);
      if (!Files.exists(path))
        postError("File \""+fileName+"\" does not exist");
      else if (!Files.isReadable(path))
        postError("File \""+fileName+"\" cannot be read");
      else if (!Files.isRegularFile(path))
        postError("File \""+fileName+"\" is not a file");
      if (errors.size() != 0)
        return getErrors();      
      data = utils.fileReadBytes(fileName);
    }

    // Save this file name
    if (!files.contains(fileName))
      files.add(fileName);
    
    // Get the Rule words...
    { // Get Rule Words...
      FileWords fw = new FileWords(fileName);
      String    ruleName;
      Word[]    wds;
      Span[]    spans; // Rule Spans
      Rule      rule;
      
      words = fw.getWords();
      if (fw.hasErrors()) {
        Error[] errs;
        errs = fw.getErrors();
        for (Error e : errs) errors.add(e);
        return getErrors();        
      }
      // Get the Rule Spans....
      spans = getRuleSpans(words);
      if (spans == null)
        return getErrors();
 
      // For each Rule Span...
      for (Span span : spans) { // For each Rule...

        Word word;
        
        if (span.size() <= 0) {
          postError(fileName,"***Unexpected empty Span("+span+").size()="+span.size()+" ***");
          continue;
        }
        if (span.size() < 2) {
          postError(fileName,words[span.start].lineCol,"Invalid Syntax");
          continue;
        }
        word = words[span.start];
        if (word.type != Word.Type.NAME && word.type != Word.Type.QNAME) {
          postError(fileName,word.lineCol,"Expected name here");
          continue;
        }
        word = words[span.start+1];
        if (word.type != Word.Type.OPERATOR && !word.value.equals("::=")) {
          postError(fileName,word.lineCol,"Expected \"::=\" here");
          continue;
        }
        if (span.size() == 2) {
          postError(fileName,words[span.start+1].lineCol,"Empty Rule");
          continue;
        }
        
        // Make sure it's not already defined...
        word = words[span.start];
        ruleName = word.value;
        if (rules != null && rules.containsKey(ruleName)) {
          String msg = "Rule '"+ruleName+"' already declared at";
          Rule that = rules.get(ruleName);
          if (that.fileName != fileName)
            msg += that.fileName+" ";
          msg += " line/col "+that.lineCol.to1String();
          postError(fileName,word.lineCol,msg);
          continue;
        }

        // Create the rule...
        rule = new Rule();
        rule.name     = ruleName;
        rule.fileName = fileName;
        rule.minOccur = 1;
        rule.maxOccur = 1;
        rule.lineCol  = words[span.start].lineCol;
        rule.words    = Word.ArrayGetSpan(words,new Span(span.start+2,span.end));
        if (rules == null)
          rules = new HashMap<String,Rule>();

        // Add to Rule Map if no initial syntax errors...
        boolean syntaxOkay = syntax(rule);
        if (!syntaxOkay)
          continue;
        
        if (!ruleName.equals("TYPES")) {
          rules.put(ruleName,rule);
          continue;
        }

        // Special case for TYPES...
        for (int i=0; i < rule.words.length; i++) {
          word = rule.words[i];
          Rule temp = new Rule(rule);
          temp.lineCol = word.lineCol;
          temp.name = word.value;
          temp.type = Rule.Type.TYPE;
          temp.minOccur = 1;
          temp.maxOccur = 1;
          temp.words    = null;
          rules.put(temp.name,temp);
        }

      } // ...For each Rule
      
    } // ...Get Rule Words

    return getErrors();

  }

  public Error[] finishProcessing() {
  
    if (state != State.NULL) {
      postError("Rules.finishProcessing() not in NULL state");
    }
    
    // Anonymise the rules...
    anonymise();
    if (hasErrors())
      return getErrors();
    
    // Check unresolved references...
    checkUnresolved();
    if (hasErrors())
      return getErrors();    
    
    return getErrors();
    
  }
}