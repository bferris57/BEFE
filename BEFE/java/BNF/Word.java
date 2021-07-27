// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;

import java.io.*;   // For PrintStream, etc.
import java.util.*; // For ArrayList, etc.
import BEFE.*;      // For Span, etc.

public class Word {

  static PrintStream out = System.out; // For output and debugging
  
  public enum Type {
    WHITE,COMMENT,OPERATOR,DQUOTE,SQUOTE,INT,HEX,NAME,QNAME,ACTION,RANGE,EOL,EOF
  }
  
  // Members...
  public String  fileName;
  public Integer lineSeq;
  public Integer fileSeq;
  public LineCol lineCol;
  public Span    span;
  public Integer len;
  public Type    type;
  public String  value;
  
  // null Constructor...
  public Word() {
    fileName = null;
    lineSeq  = null;
    fileSeq  = null;
    lineCol  = null;
    span     = null;
    len      = null;
    type     = null;
    value    = null;
  }

  // Copy Constructor...
  public Word(Word that) {
    fileName = that.fileName;
    lineSeq  = that.lineSeq;
    fileSeq  = that.fileSeq;
    lineCol  = new LineCol(that.lineCol);
    span     = new Span(that.span);
    len      = that.len;
    type     = that.type;
    value    = that.value;
  }
  
  // typeToString...
  public String typeToString() {
    if (type == null)
      return "null";
    return ""+type;
  }
  
  // toString...
  public String toString() {
  
    String s;
    
    s = "Word(";

    s += "f=";
    if (fileName != null)
      s += "\""+fileName+"\"";
    else
      s += "null";
    
    s += ",flseqs=(";
    if (fileSeq != null) s += fileSeq;
    s += ",";
    if (lineSeq != null) s += lineSeq;
    s += ")";

    if (lineCol != null)
      s += lineCol.toString();
    else
      s += "null";
      
    s += ",";
    if (span == null)
      s += "null";
    else
      s += "span=" + span.toString();

    s += ",len=";
    if (len != null)
      s += len;
    else
      s += "null";

    s += ",type=";
    if (type == null)
      s += "null";
    else
      s += type;
    
    s += ",val=\""+utils.javaStringLiteral(value)+"\"";

    s += ")";
    
    return s;
  }
 
  public String toString(int maxLen) {
    return utils.ellipsis(toString(),maxLen);
  } 
  
  //
  // Print/Filter options...
  //
  
  public static class Options {
    // Filter options...
    public boolean showCOMMENT;
    public boolean showWHITE;
    public boolean showEOL;
    public boolean showEOF;
    // Column options...
    public boolean showIndex;    
    public boolean showFileName;
    public boolean showLineSeq;
    public boolean showFileSeq;
    public boolean showLineCol;
    public boolean showSpan;
    public boolean showLen;
    public boolean showType;
    public boolean showValue;
    public int     ellipsisLen;
    // Constructors...
    public Options() {
      showCOMMENT   = true;
      showWHITE     = true;
      showEOL       = true;
      showEOF       = true;
      showIndex     = true;
      showFileName  = true;
      showLineSeq   = true;
      showFileSeq   = true;
      showLineCol   = true;
      showSpan      = true;
      showLen       = true;
      showType      = true;
      showValue     = true;
      ellipsisLen   = 0;
    }
    public Options(Options that) {
      this.showCOMMENT  = that.showCOMMENT;
      this.showWHITE    = that.showWHITE;
      this.showEOL      = that.showEOL;
      this.showEOF      = that.showEOF;
      this.showIndex    = that.showIndex;
      this.showFileName = that.showFileName;
      this.showLineSeq  = that.showLineSeq;
      this.showFileSeq  = that.showFileSeq;
      this.showLineCol  = that.showLineCol;
      this.showSpan     = that.showSpan;
      this.showLen      = that.showLen;
      this.showType     = that.showType;
      this.showValue    = that.showValue;
      this.ellipsisLen  = that.ellipsisLen;
    }
  }
  
  public static class OptionsAll extends Options {
    public OptionsAll() {
      super();
      showIndex = false;
    }
  }
  
  public static class OptionsMinimal extends Options {
    public OptionsMinimal() {
      showCOMMENT   = false;
      showWHITE     = false;
      showEOL       = false;
      showEOF       = false;
      showIndex     = false;
      showFileName  = false;
      showLineSeq   = false;
      showFileSeq   = false;
      showLineCol   = false;
      showSpan      = false;
      showLen       = false;
      showType      = false;
      showValue     = false;
      ellipsisLen   = 3;
    }
  }
  
  public static class OptionsNominal extends Options {
    public OptionsNominal() {
      showCOMMENT   = false;
      showWHITE     = false;
      showEOL       = false;
      showEOF       = false;
      showIndex     = false;
      showFileName  = false;
      showLineSeq   = true;
      showFileSeq   = true;
      showLineCol   = true;
      showSpan      = true;
      showLen       = true;
      showType      = true;
      showValue     = true;
      ellipsisLen   = 20;
    }
  }

  public static void printWordsFiltered(Word[] words,String prefix,Options opts) {
    String[] lines = outputWordsFiltered(words,opts);
    for (int i=0; i < lines.length; i++)
      out.print(prefix+lines[i]+"\n");
  }
  
  public static String[] outputWordsFiltered(Word[] words,Options opts) {
  
    String[]           colNames = {"Index","fileName","fileSeq","lineSeq","lineCol","span","len","type","value"};
    Boolean[]          leftCols = {false,  true,      false,    false,    false,    false, false,true,  true};
    ArrayList<String>  colNamesList = new ArrayList<String>();
    ArrayList<Boolean> leftColsList = new ArrayList<Boolean>();
    String[][]         printRows;

    // Default Options if null
    if (opts == null)
      opts = new OptionsNominal();

    // Populate printCols based on the Options...
    for (int col=0; col < colNames.length; col++) {
      boolean doit;
      switch (colNames[col]) {
        case "Index":     doit = opts.showIndex;    break;
        case "fileName":  doit = opts.showFileName; break;
        case "fileSeq":   doit = opts.showFileSeq;  break;
        case "lineSeq":   doit = opts.showLineSeq;  break;
        case "lineCol":   doit = opts.showLineCol;  break;
        case "span":      doit = opts.showSpan;     break;
        case "len":       doit = opts.showLen;      break;
        case "type":      doit = opts.showType;     break;
        case "value":     doit = opts.showValue;    break;
        default:          doit = false;             break;
      }
      if (doit) {
        colNamesList.add(colNames[col]);
        leftColsList.add(leftCols[col]);
      }
    }
    colNames = new String[colNamesList.size()];
    colNames = colNamesList.toArray(colNames);
    leftCols = new Boolean[leftColsList.size()];
    leftCols = leftColsList.toArray(leftCols);
    
    // If no rows or columns, error and finish
    if (colNames.length == 0) {
      String[] result = {"*** No columns to print ***"};
      return result;
    }
    if (words.length == 0) {
      String[] result = {"*** No Words to print ***\n"};
      return result;
    }
    
    // Allocate col/row data
    printRows = new String[words.length+2][colNames.length];

    // Populate the rows
    printRows[0] = colNames;
    printRows[1] = new String[colNames.length];
    for (int row=2; row < words.length+2; row++) {
      printRows[row] = new String[colNames.length];
      for (int col=0; col < colNames.length; col++) {
        Word word = words[row-2];
        switch (colNames[col]) {
          case "Index":     printRows[row][col] = "" + (row-2);                        break;
          case "fileName":  printRows[row][col] = "" + word.fileName;                  break;
          case "fileSeq":   printRows[row][col] = ""+word.fileSeq;                     break;
          case "lineSeq":   printRows[row][col] = ""+word.lineSeq;                     break;
          case "lineCol":   printRows[row][col] = ""+word.lineCol.toString();          break;
          case "col":       printRows[row][col] = ""+word.lineCol.col;                 break;
          case "span":      printRows[row][col] = word.span.toString();                break;
          case "len":       printRows[row][col] = ""+word.len;                         break;
          case "type":      printRows[row][col] = ""+word.type;                        break;
          case "value":     printRows[row][col] = utils.javaStringLiteral(word.value); break;
          default:                                                                     break;
        }
      }
    }
    
    // Calculate maxLens...
    int[] maxLens = new int[colNames.length];
    for (int row=0; row < printRows.length; row++) {
      String[] printRow = printRows[row];
      for (int col=0; col < printRow.length; col++)
        maxLens[col] = Math.max(maxLens[col],(printRow[col] == null) ? 4 : printRow[col].length());
    }
    
    // Populate "---" row...
    String[] hyphens = new String[colNames.length];
    for (int col=0; col < hyphens.length; col++)
      hyphens[col] = utils.replicateChar('-',maxLens[col]);
    printRows[1] = hyphens;
    
    // Pad and justify them all...
    for (int row=0; row < printRows.length; row++) {
      String printRow[] = printRows[row];
      for (int col=0; col < printRow.length; col++) {
        printRow[col] = utils.spacePad(printRow[col],maxLens[col]);
        if (row > 1 && !leftCols[col])
          printRow[col] = utils.rightJustify(printRow[col]);
      }
      printRows[row] = printRow;
    }
    
    // Return rows...
    String[] result = new String[printRows.length];
    for (int row = 0; row < result.length; row++) {
      result[row] = "";
      for (int col = 0; col < printRows[row].length; col++) {
        if (col != 0)
          result[row] += " ";
        result[row] += printRows[row][col];
      }
    }
    return result;
  }
 
  //
  // Slicing
  //

  public static Word[] slice(Word[] src, Span span) {
    
    return new Slice<Word>().slice(src,span);
    
  }
  
  /**
  Removes an exclusive Span from a Word[].
  */
  
  public static Word[] ArrayRemove(Word[] words, Integer index) {
    if (index == null || index < 0 || index >= words.length)
      return words;
    Span span = new Span(index,index+1,1);
    return ArrayRemove(words,span);
  }
  
  public static Word[] ArrayRemove(Word[] words, Span span) {

    Span ourSpan = new Span(span).normalise(words.length);
    ArrayList<Word> list = new ArrayList<Word>();
    for (int i = 0; i < words.length; i++)
      list.add(new Word(words[i]));

    for (int i=ourSpan.start; i < ourSpan.end; i += ourSpan.step)
      list.remove((int)ourSpan.start);
    Word[] newArray = new Word[list.size()];
    newArray = list.toArray(newArray);

    return newArray;
  }

  /** Inserts a copy of a word into a Word[] array */  
  public static Word[] ArrayInsert(Word[] words, Word word, Integer index) {
    if (index == null || index < 0 || index > words.length)
      return words;
    ArrayList<Word> list = new ArrayList<Word>();
    for (int i = 0; i < words.length; i++) {
      if (i == index)
        list.add(word);
      list.add(new Word(words[i]));
    }
    if (index >= words.length)
      list.add(new Word(word));
    Word[] newArray = new Word[list.size()];
    newArray = list.toArray(newArray);
    
    return newArray;
  }
  
  public static Word[] ArrayGetSpan(Word[] words, Span getSpan) {
    if (words == null || getSpan == null)
      return null;
    Span ourSpan = new Span(getSpan).normalise(words.length);
    Word[] result = new Word[ourSpan.size()];
    if (ourSpan.step > 0) {
      for (int i=ourSpan.start;i < ourSpan.end; i += ourSpan.step)
        result[i-ourSpan.start] = new Word(words[i]);
    }
    else if (ourSpan.step < 0) {
      for (int i = ourSpan.start; i > ourSpan.end; i += ourSpan.step)
        result[ourSpan.start-0] = new Word(words[i]);
    }
    return result;
  }
 
  public static String ArrayToString(Word[] words) {
    return ArrayToString(words,false);
  }
  
  public static String ArrayToString(Word[] words,boolean showIndex) {
    String out = "";
    if (words == null || words.length == 0)
      return out;
    for (int i=0; i < words.length; i++) {
      if (i != 0)
        out += " ";
      if (showIndex)
        out += "" + i + ":";
      Word word = words[i];
      if (word.type == Word.Type.DQUOTE)
        out += "\"" + utils.javaStringLiteral(word.value) + "\"";
      else if (word.type == Word.Type.SQUOTE)
        out += "'" + utils.javaStringLiteral(word.value) + "'";
      else {
        if (showIndex)
          out += "`";
        out += word.value;
        if (showIndex)
          out += "`";
      }
    }
    return out;
  }
  
}