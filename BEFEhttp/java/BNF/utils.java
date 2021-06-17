// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;
  
import java.io.*;          // For PrintStream, etc.
import java.nio.file.*;    // For File, Path, etc.
import java.nio.charset.*; // For StandardCharsets, etc.
import java.util.*;        // For ArrayList, List, Map, etc.

import java.io.UnsupportedEncodingException;
import BEFE.Error;
import BEFE.*;
import BNF.*;

public class utils { // utils...

  private static PrintStream out = System.out;
  private static PrintStream err = System.err;
  private static InputStream in  = System.in;
  
  //
  // String fileReadBytes
  //
  
  public static byte[] fileReadBytes(String fileName) {

    Path   path;      // File path
    byte[] contents;  // File contents

    if (fileName == null)
      return null;
    
    try {
    
      // Sanity checks...
      path = Paths.get(fileName);
      if (!Files.exists(path) ||
          !Files.isReadable(path) ||
          !Files.isRegularFile(path))
        return null;

      // Do it...
      contents = Files.readAllBytes(path);
    
    }
    
    catch (IOException e) {
      contents = null;
    }

    return contents;
    
  }

  public static String fileReadString(String fileName) {

    Path   path;      // File path
    byte[] contents;  // File contents
    String result;    // Return value
    
    try {
    
      // Sanity checks...
      path = Paths.get(fileName);
      if (!Files.exists(path) ||
          !Files.isReadable(path) ||
          !Files.isRegularFile(path))
        return null;

      // Do it...
      contents = Files.readAllBytes(path);
    
    }
    
    catch (IOException e) {
      contents = null;
    }

    // We're making a UTF-8 string out of it...
    try {
      result = new String(contents,"UTF-8");
    }
    catch (UnsupportedEncodingException e) {
      result = null;
    }
    
    return result;
    
  }
  
  //
  // String javaStringLiteral
  //
  // Note: This does NOT put the heading/trailing quotes in it, that's up to you
  
  public static String javaStringLiteral(byte[] bytes) {
    String str = new String(bytes,StandardCharsets.UTF_8);
    return javaStringLiteral(str);
  }
  
  public static String javaStringLiteral(String str) {
    StringBuilder sb = new StringBuilder("");
    for (int i=0; str != null && i<str.length(); i++) {
      char c = str.charAt(i);
      if (c == '\t')
        sb.append("\\t");
      else if (c == '\b')
        sb.append("\\b");
      else if (c == '\n')
        sb.append("\\n");
      else if (c == '\r')
        sb.append("\\r");
      else if (c == '\f')
        sb.append("\\f");
      else if (c == '"')
        sb.append("\\\"");
      else if (c == '\\')
        sb.append("\\\\");
      else if (c < 0x20)
        sb.append(String.format("\\x%02x", (int)c));
      else if (c >= 0x80 && c <= 0x7fff)
        sb.append(String.format("\\u%04x", (int)c));
      else
        sb.append(c);
    }
    return sb.toString();
    
  }

  //
  // String utilities...
  //
  
  public static String spacePad(int len) {
    if (len >= 0)
      return new String(new char[len]).replace("\0"," ");
    else
      return new String("");
  }
  
  public static String spacePad(String str, int len) {
    if (str.length() >= len)
      return str;
    if (len >= 0)
      return str+(new String(new char[len-str.length()]).replace("\0"," "));
    else
      return new String("");
  }
  
  public static String replicateChar(char c, int repCount) {
    char[] arr = new char[repCount];
    for (int i=0; i < arr.length; i++) arr[i] = c;
    return new String(arr);
  }

  public static String leftJustify(String str) {
    char[] chars = str.toCharArray();
    int d,s;
    d = 0;
    for (s=0;s<chars.length && chars[s] == ' ';s++);
    if (s != d) {
      while (s < chars.length)
        chars[d++] = chars[s++];
      while (d < chars.length)
        chars[d++] = ' ';
    }
    return new String(chars);
  }
  
  public static String rightJustify(String str) {
    char[] chars = str.toCharArray();
    int d,s;
    s = chars.length-1;
    while (s >= 0 && chars[s] == ' ') s--;
    d = chars.length-1;
    while (s >= 0) chars[d--] = chars[s--];
    while (d >= 0) chars[d--] = ' ';
    return new String(chars);
  }
  
  //
  // String ellipsis
  //
  
  public static String ellipsis(String in, Integer maxLen) {
    if (maxLen < 3 || in.length() <= maxLen)
      return in;
    return in.substring(0,maxLen-3)+"...";
  }

  //
  // void hexLines - Convert byte[] to String[] containing printable hex lines
  //
  // Note: Lines DO NOT contain '\n' at the end of them
  //
  
  public static String[] hexLines(byte[] data) {
    
    int           offset;
    StringBuilder line;
    StringBuilder ascii;
    int           theByte;
    int           theNibble;
    String[]      result = new String[(data.length+15)/16];
    
    for (offset = 0; offset < data.length%16; offset += 16) {
      
      line  = new StringBuilder();
      ascii = new StringBuilder();
      
      for (int i = offset; i < offset + 16 && i < data.length; i++) {

        theByte = data[i] & 0xff;

        if (line.length() != 0) line.append(' ');

        theNibble = (theByte >> 4) & 0x0f;
        if (theNibble < 10)
          line = line.append((char)('0'+theNibble));
        else
          line.append((char)('a'+theNibble-10));
        theNibble = theByte & 0x0f;
        if (theNibble < 10)
          line.append((char)('0'+theNibble));
        else
          line.append((char)('a'+theNibble-10));
        if (theByte < 0x20 || theByte >= 0x7f)
          ascii.append('.');
        else
          ascii.append(theByte);
      }
      result[offset] = line.toString();
      if (result[offset].length() < 47 && data.length > 16)
        result[offset] += spacePad(47-line.length());
      result[offset] += "  "+ascii.toString();
    }
    
    return result;
  }
  
  //
  // lengthUTF_8 - Determine length of UTF-8 character at given byte position
  //               in byte array
  //
  // Note: Returns -1 if invalid or truncated
  //
  
  public static int lengthUTF_8(byte[] bytes,int pos) {
    int expectedLen;
    if      (bytes.length == 0) return 0;
    else if ((bytes[pos] & 0b10000000) == 0b00000000) expectedLen = 1;
    else if ((bytes[pos] & 0b11100000) == 0b11000000) expectedLen = 2;
    else if ((bytes[pos] & 0b11110000) == 0b11100000) expectedLen = 3;
    else if ((bytes[pos] & 0b11111000) == 0b11110000) expectedLen = 4;
    else if ((bytes[pos] & 0b11111100) == 0b11111000) expectedLen = 5;
    else if ((bytes[pos] & 0b11111110) == 0b11111100) expectedLen = 6;
    else return 0;

    if (pos + expectedLen-1 >= bytes.length) return -1;

    int remLen = expectedLen;
    for (int i = pos+1; --remLen > 0 && i < bytes.length; i++) {
        if ((bytes[i] & 0b11000000) != 0b10000000) {
            return -1;
        }
    }

    return expectedLen;
  }
  
  //
  // firstNonPrintableIndex
  //
  
  public static int firstNonPrintableIndex(String str) {
    char c;
    int i;
    if (str == null)
      return -1;
    for (i=0; i < str.length(); i++) {
      c = str.charAt(i);
      if (c < 0x20 || c >= 0x7f)
        return i;
    }
    return -1;
  }
  
  //
  // Varions Copy() methods
  //
  
  public static int[]  ArrayCopy(int[] that) {
    if (that == null)
      return null;
    int[] copy = new int[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;    
  }
  
  public static Integer[] ArrayCopy(Integer[] that) {
    if (that == null)
      return null;
    Integer[] copy = new Integer[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;    
  }
  
  public static String[] ArrayCopy(String[] that) {
    if (that == null)
      return null;
    String[] copy = new String[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;    
  }

  public static Span[] ArrayCopy(Span[] that) {
    if (that == null)
      return null;
    Span[] copy = new Span[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;
  }

  public static Word[] ArrayCopy(Word[] that) {
    if (that == null)
      return null;
    Word[] copy = new Word[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;
  }

  public static Error[] ArrayCopy(Error[] that) {
    if (that == null)
      return null;
    Error[] copy = new Error[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;
  }
  
  public static Rule[] ArrayCopy(Rule[] that) {
    if (that == null)
      return null;
    Rule[] copy = new Rule[that.length];
    System.arraycopy(that,0,copy,0,that.length);
    return copy;
  }

  // getFileLine/clearLineCache
  private static String savedFileName = null;
  private static byte[] savedFileData;
  private static Span[] savedFileLines;
    
  public static String getFileLine(String fileName,int line) {
    
    String fileLine = null;
    Span   fileSpan = null;
    byte[] lineBytes = null;
    
    if (fileName != savedFileName) {
      savedFileName  = fileName;
      savedFileData  = null;
      savedFileLines = null;
      savedFileData = fileReadBytes(fileName);
      if (savedFileData != null) {
        ArrayList<Span> list = new ArrayList<Span>();
        for (Span lineSpan : new FileLines(savedFileData))
          list.add(lineSpan);
        savedFileLines = list.toArray(new Span[list.size()]);
      }
    }
    if (savedFileData == null || savedFileLines == null)
      return null;
    if (line < 0 || line >= savedFileLines.length)
      return null;
    fileSpan = savedFileLines[line];
    lineBytes = new byte[fileSpan.size()];
    System.arraycopy(savedFileData,fileSpan.start,lineBytes,0,fileSpan.size());
    try   {fileLine = new String(lineBytes,"UTF-8");}
    catch (Exception e) {fileLine = null;}
    return fileLine;
  }
  public static void clearLineCache() {
    savedFileName = null;
    savedFileData = null;
    savedFileLines = null;    
  }
  
  // printErrors
  public static void printErrors(String prefix, Error[] errors,boolean showFileName,boolean showLineCol) {
    
    if (errors == null || errors.length == 0)
      return;
    
    for (int i=0; i < errors.length;i++) {      
    
      Error   err       = errors[i];
      String  fileName  = null;
      LineCol lineCol   = null;
      String  msg;
      String  lineStr   = null;
      String  lineNoStr = "";

      msg = "";
      if (err instanceof SyntaxError) {
        lineCol = new LineCol(((SyntaxError)err).lineCol);
        lineStr = getFileLine(err.fileName,lineCol.line);
        msg += "Syntax Error";
      }
      else if (err instanceof Failed)
        msg += "Failed";
      else if (err instanceof Error)
        msg += "Error";
      
      if (showFileName && err.fileName != null) {
        msg += ": File '"+err.fileName+"'";
      }
      
      if (showLineCol && lineStr != null && lineCol != null) {
        if (lineStr.indexOf('\n') < 0)
          lineStr += '\n';
        lineNoStr = new Integer(lineCol.line+1).toString();
        out.printf(prefix+"%s: %s",lineNoStr,lineStr);
        out.printf(prefix+"%s^\n",utils.replicateChar(' ',lineCol.col+lineNoStr.length()+2));
      }
      if (showLineCol && lineCol != null) {
        msg += " at ";
        msg += lineCol.to1String();
      }
      msg += ": " + err.message;
      out.printf(prefix+msg+"\n");
    }
    clearLineCache();
    return;
  }

  public static void printErrors(String prefix, Error[] errors) {
    printErrors(prefix, errors, true, true);
  }
  public static void printErrors(Error[] errors) {
    printErrors("", errors, true, true);
  }
  public static void printErrorsNoFile(String prefix, Error[] errors) {
    printErrors(prefix,errors, false,true);
  }
  public static void printErrorsNoFile(Error[] errors) {
    printErrors("", errors, false, true);
  }
  
  /**
  Convert String, in various acceptable formats to an Integer instance.
  
  returns Integer - null indicates invalid input
  */
  public static Integer toInteger(String input) {
    Integer output = null;
    input = input.trim();
    
    // Try proper Integers first...
    try {
      output = new Integer(input);
      return output;
    }
    catch (Exception e) {}
    
    // Nope, how about "0x..."
    if (input.indexOf("0x") == 0 || input.indexOf("0X") == 0) try {
      input = input.substring(2,input.length());
      int     digit;
      char    c;
      output = 0;
      for (int i = 0; i < input.length(); i++) {
        c = input.charAt(i);
        if (c >= '0' && c <= '9')
          digit = c - '0';
        else if (c >= 'a' && c <= 'f')
          digit = (c - 'a') + 10;
        else if (c >= 'A' && c <= 'F')
          digit = (c = 'A') + 10;
        else
          throw new NumberFormatException();
        output = (output << 4) + digit;
      }
      return output;
    }
    catch (Exception e) {}

    // Try '<char>' and "<char>"...    
    if (input.length() == 3                                &&
       (input.charAt(0) == '\'' || input.charAt(0) == '"') &&
       input.charAt(0) == input.charAt(2)) {
      
      output = new Integer(input.charAt(1));
      
      return output;
    }

    return null;
 }
  
} // ...utils