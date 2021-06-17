// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
import java.util.*;     // For ArrayList<T>, etc.

import BNF.*;
import BEFE.*;
import BEFE.Error;

public class TestMain { // class TESTMAIN...

  private static String      prefix;           // Output prefix

  // Test utils.fileRead    
  public static void testFileRead(String[] args) {
    
    int maxLen = 0;
    
    if (args.length == 0)
      out.println("***Please pass file name arguments***");
    else for (String fileName : args)
      maxLen = Math.max(maxLen,fileName.length());
      
    for (String fileName : args) {
      String contents;
      String pad = new String(new char[maxLen-fileName.length()]).replace("\0", " ");
      contents = utils.fileReadString(fileName);
      if (contents == null)
        out.printf("File \"%s\"%s length = null\n",fileName,pad);
      else
        out.printf("File \"%s\"%s length = %d\n",fileName,pad,contents.length());
    }
    
  }
    
  // Test BOM...
  public static void testBOM(String[] args) {

    String[] fileNames = {"../testfiles/BOM-UTF8.txt",
                          "../testfiles/BOM-UTF16BE.txt",
                            
                         };

    int maxLen = 0;
    for (String fileName : fileNames) {
      maxLen = Math.max(maxLen,fileName.length());
    }
    
    for (String fileName : fileNames) {
      
      String pad     = utils.spacePad(maxLen-fileName.length());
      byte[] data    = utils.fileReadBytes(fileName);
      String BOMname = BOM.BOMname(data);
      
      
      if (BOMname == null)
        out.printf("File \"%s\":%s No BOM\n",fileName,pad);
      else
        out.printf("File \"%s\":%s BOM \"%s\":%d\n",fileName,pad,BOMname,BOM.BOMs.get(BOMname).length);
    }
    
  }

  // Test Span
  private static void testSpan(String[] args) {
    try {
      String[] spans = {":","::","1:",":1","::3","1::3"};
      for (int i=0; i < spans.length; i++) {
        Span span = new Span(spans[i]);
        out.printf("Span %d (\"%s\") = \"%s\"\n",i,spans[i],span.toString());
      }
      //Span s = new Span("abc:2");
      
    }
    catch (Exception e) {
      e.printStackTrace();
    }

  }

  // Test Slice<T>
  public static void testSlice(String[] args) {
    
    String[]  spans = {":","0:","1:","-1:"};
    int       maxLen = 0;
    Integer[] src = {0,1,2,3,4,5,6,7,8,9};
    Integer[] dst;
    String before = "[";
    String after  = "[";
    
    for (int i = 0; i < src.length; i++) {
      if (i != 0)
        before += ",";
      before += src[i];
    }
    before += "]";
    out.println("Source: "+before);

    for (int idx = 0; idx < spans.length; idx++)
      maxLen = Math.max(maxLen,spans[idx].length());
    
    for (int idx = 0; idx < spans.length; idx++) {

      Span   span = new Span(spans[idx]);
      String pad  = utils.replicateChar(' ',maxLen-spans[idx].length());
  
      dst = new Slice<Integer>().slice(src,span);
      
      after = "[";
      for (int i = 0; i < dst.length; i++) {
        if (i != 0)
          after += ",";
        after += dst[i];
      }
      after += "]";
      
      out.printf("Span(\"%s\"):%s %s\n",spans[idx],pad,after);      
    }

  }

  // Test Range
  public static void testRange(String[] args) {
    
    //                       Input      Output
    //                       ---------- ---------
    String[] rangeStrings = {"..",      "..",
                             "0..",     "0..",
                             "1..10",   "1..10",
                             "0x00..",  "0..",
                             "..0xff",  "..255",
                             "'x'..",   "120..",
                             "\"x\"..", "120..",
                             "..'x'",   "..120",
                             "..\"x\"", "..120",
                             "x..y",    "..",
                             "x..",     "..",
                             "1..y",    "1..",
                            };
    int maxLen1 = 0;
    int maxLen2 = 0;
    for (int i = 0; i < rangeStrings.length; i += 2) {
      maxLen1 = Math.max(rangeStrings[i].length(),maxLen1);
      maxLen2 = Math.max(rangeStrings[i+1].length(),maxLen2);
    }
    
    for (int i = 0; i < rangeStrings.length; i += 2) {

      String input  = rangeStrings[i];
      String expect = rangeStrings[i+1];
      String output;
      Range  range;
      
      range  = new Range(input);
      output = range.toString();

      String msg = prefix+"in: "+utils.spacePad(input,maxLen1)+" out: "+utils.spacePad(output,maxLen2);
      if (!output.equals(expect))
        msg += "<-- Mismatch, expected \""+expect+"\"";
      out.println(msg);
    }                            
  }
  
  // Test File.Lines()
  public static void testFileLines(String[] args) {
    
     
    int maxLen = 0;
    
    if (args.length == 0)
      out.println("***Please pass file name arguments***");
    else for (String fileName : args)
      maxLen = Math.max(maxLen,fileName.length());
      
    for (String fileName : args) {
      byte[] contents;
      String pad = new String(new char[maxLen-fileName.length()]).replace("\0", " ");
      contents = utils.fileReadBytes(fileName);
      if (contents == null)
        out.printf("File \"%s\"%s length = null\n",fileName,pad);
      else {
        int lineNo = 1;
        for (Span line : new FileLines(contents)) {
          out.printf(fileName+": "+line.toString()+": "+utils.javaStringLiteral(Arrays.copyOfRange(contents,line.start,line.end))+'\n');
        }
      }
    }
       
  }
  
  // Test FileWords
  public static void testFileWords(String[] args) {
   
    int maxLen = 0;
    
    // Temp...
    if (args.length == 0)
      args = new String[]{"g:/SVN/testfiles/BEFE-bnf.bnf"};
    //args = new String[]{"../testfiles/t.bnf"};
    // ...Temp

    if (args.length == 0)
      out.println("***Please pass file name arguments***");
    else for (String fileName : args)
      maxLen = Math.max(maxLen,fileName.length());
    for (String fileName : args) {
      FileWords fw = new FileWords(fileName);
      if (!fw.hasContents())
        out.printf("File \"%s\"%s length = null\n",fileName,utils.spacePad(maxLen-fileName.length()));
      //else
      //  out.printf("File \"%s\"%s length = %d...\n",fileName,utils.spacePad(maxLen-fileName.length()),fw.contentLength());
      
      // Print the errors...
      if (fw.hasErrors()) { // Errors...
        fw.printErrors(prefix);
        continue;
      } // ...Errors
      
      // Print the words...
      out.printf("Words...\n");
      Word.Options opts = fw.getOptionsNominal();
      fw.setOptions(opts);
      fw.printWordsFiltered(prefix+"  ");
    }

  }

  // Test Rules...
  public static void testRules(String[] args) {
    
    Rules rules = new Rules();
    Error[] errors;
    
    if (args == null) {
      out.println("***testRules() expected args, was given null***");
      return;
    }
    // Temp...
    if (args.length == 0)
      args = new String[]{"g:/SVN/testfiles/BEFE-bnf.bnf"};
    //args = new String[]{"../testfiles/t.bnf"};
    // ...Temp
    for (String arg : args) {
      out.printf("File \"%s\"...\n",arg);
      errors = rules.processFile(arg);
    }
    
    errors = rules.finishProcessing();
    if (errors != null && errors.length != 0)
      utils.printErrorsNoFile(prefix+"  ",errors);

    rules.printRules(prefix);
    
  }
  
  //  
  // Test Main...
  //
  
  public static void main(String[] args) {

    String[] cases = {/*"FileRead","BOM","FileWords","Slice",*/"Rules",/*"Range",*/};
    int      prefixLen;

    // Compute prefix...
    prefixLen = 0;
    for (int i = 0; i < cases.length; i++)
      prefixLen = Math.max(prefixLen,cases[i].length());
    
    
    // Do test cases...
    for (String which : cases) {
      prefix  = which + ": ";
      prefix += utils.spacePad(prefixLen-which.length());
      switch (which) {
        case "FileRead":  testFileRead(args);  break;
        case "Span":      testSpan(args);      break;
        case "BOM":       testBOM(args);       break;
        case "FileLines": testFileLines(args); break;
        case "FileWords": testFileWords(args); break;
        case "Slice":     testSlice(args);     break;
        case "Range":     testRange(args);     break;
        case "Rules":     testRules(args);     break;
      }
    }
  }
  
} // ...class TESTMAIN