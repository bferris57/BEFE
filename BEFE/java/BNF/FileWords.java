// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;

import java.util.*; // For ArrayList, etc.
import java.io.*;   // For PrintStream, etc.
import BEFE.*;
import BEFE.Error;

public class FileWords implements Iterable<Word> {

  static PrintStream out = System.out; // For output and debugging
  
  //
  // FileWords Members
  //
  
  // String constants...
  private static String commentChars = "#%-/";
  private static String whiteSpace   = " \t\f";
  private static String ops          =  "[](){}|;.*";
  private static String nameStarts   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz";
  private static String nameChars    = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz0123456789";
  
  // Private members...
  private String       fileName; // File name
  private byte[]       data;     // File contents
  private Span[]       lines;    // Lines in file
  private List<Error>  errors;   // Errors
  private Context      ctx;      // Processing context
  private List<Word>   words;    // Words in File(s)
  private Word.Options opts;     // Filter/Print options

  // Processing States...
  private enum State {NULL,NAME,INT,HEX,QUOTE,WHITE,COMMENT,MLCOMMENT,ACTION}
    
  // Processing context...
  private class Context {
    State                  state;      // Processing state
    Span                   span;       // Line span
    int                    pos;        // Current position in data (should be in span)
    LineCol                lineCol;    // Line/Column (0../0..)
    int                    fileSeq;    // Word file sequence (0..)
    int                    lineSeq;    // Wine line sequence (0..)
    char                   quoteChar;  // TEMP: Current quote char
    Word                   word;       // Word we're building
    int                    errorCount; // Number of errors generated on this line so far
  };

  //
  // Iterable interface Methods
  //  
  
  public Iterator<Word> iterator() {
      return new FileWordsIterator(words,opts);
  }

  // Iterator
  private class FileWordsIterator implements Iterator<Word> {

    private Word[]  words;
    private int     len;
    private int     pos;
    
    public FileWordsIterator(List<Word> words, Word.Options opts) {
      
      pos = 0;
      if (words == null)
        this.words = null;
      else {
        ArrayList<Word> tempWords = new ArrayList<Word>();
        for (Word word : words) {
          if (word.type == Word.Type.COMMENT && !opts.showCOMMENT) continue;
          if (word.type == Word.Type.WHITE   && !opts.showWHITE)   continue;
          if (word.type == Word.Type.EOL     && !opts.showEOL)     continue;
          if (word.type == Word.Type.EOF     && !opts.showEOF)     continue;
          tempWords.add(word);
        }
        this.words = new Word[tempWords.size()];
        for (pos = 0; pos < tempWords.size(); pos++)
          this.words[pos] = new Word(tempWords.get(pos));
        pos = 0;
      }
       
    }

    public boolean hasNext() {
      return (pos < len)?true:false;
    }

    //@Override
    public Word next() throws NoSuchElementException {
    
      Word word;
      
      if (!hasNext())
        throw new NoSuchElementException();
      
      return words[pos++];
      
    }

    public void remove() throws UnsupportedOperationException {
      throw new UnsupportedOperationException();
    }
    
  }

  //
  // FileWords Methods...
  //
  
  public FileWords(String filename) {
    
    fileName  = filename;
    data      = null;
    errors    = new ArrayList<Error>();
    ctx       = new Context();

    lines     = null;
    
    // Read the file's contents    
    if (filename != null) {

      // Get the contents...
      data = utils.fileReadBytes(filename);

      // Populate the lines...
      if (data != null) {
        
        ArrayList<Span> arrayLines = new ArrayList<Span>();
        
        for (Span line : new FileLines(data))
          arrayLines.add(line);
        lines = arrayLines.toArray(new Span[arrayLines.size()]);
      
      }
      else
        lines = null;
      
      // Process the lines...
      if (lines != null) {
        processContents();
      }
      
    }
    
  }
  
  // Various queriables...
  public String  getFileName() {
    return fileName;
  }
  
  public boolean hasContents() {
    return (data == null) ? false : true;
  }
  
  public int contentLength() {
    if (data == null)
      return 0;
    return data.length;
  }
  
  public int numLines() {
    if (lines == null)
      return 0;
    return lines.length;    
  }
  
  public byte[] getLine(int lineNo) {
    
    Span   lineSpan;
    byte[] line;
    
    if (lines == null || lineNo < 0 || lineNo >= lines.length)
      return null;
    
    lineSpan = new Span(lines[lineNo]);

    // Removing trailing \n and \r...
    while (lineSpan.end > lineSpan.start) {
      if (data[lineSpan.end-1] == '\n' || data[lineSpan.end-1] == '\r') {
        lineSpan.end--;
      }
      else
        break;
    }

    // Get it
    line = Arrays.copyOfRange(data,lineSpan.start,lineSpan.end);
    
    return line;
  }
  
  public boolean hasErrors() {
    return (errors != null && errors.size() != 0) ? true : false;
  }
  
  public Error[] getErrors() {

    if (errors == null)
      return null;

    Error[] result = new Error[errors.size()];

    result = errors.toArray(result);

    return result;
    
  }

  // Get Span of this.data as a String...
  private String spanString(Span span) {
    
    if (span.start < 0 || span.end < 0 || span.end <= span.start)
      return "";
    
    byte[] value      = new byte[span.end-span.start];
    String string;
    
    System.arraycopy(data,span.start,value,0,span.end-span.start);
    try                 {string = new String(value, "UTF-8");}
    catch (Exception e) {string = "";}
    
    return string;
    
  }
  
  // Post a SyntaxError...
  private void postError(String message) {
    Error err = new Error(fileName,message);
    errors.add(err);
    ctx.errorCount++;
  }
  private void postError(int line, int col, String message) {
    SyntaxError err = new SyntaxError(fileName,line,col,message);
    errors.add(err);
    ctx.errorCount++;
  }
  private void postError(LineCol lineCol, String message) {
    SyntaxError err = new SyntaxError(fileName,lineCol,message);
    errors.add(err);
    ctx.errorCount++;
  }

  // Add a word to list
  private void addWord(Word word) {
    if (word.type != Word.Type.WHITE &&
        word.type != Word.Type.EOL   &&
        word.type != Word.Type.EOF) {
      if (word.lineSeq == null) word.lineSeq = ctx.lineSeq++;
      if (word.fileSeq == null) word.fileSeq = ctx.fileSeq++;
    }
    word.fileName = fileName;
    words.add(word);
  }

  // Print errors
  public void printErrors() {
    Error[] errors = getErrors();
    utils.printErrors("",errors);
  }
  public void printErrors(String prefix) {
    Error[] errors = getErrors();
    utils.printErrors(prefix,errors);
  }

  public Word.Options getOptions()         {return new Word.Options(opts);}
  public Word.Options getOptionsAll()      {return new Word.OptionsAll();}
  public Word.Options getOptionsMinimal()  {return new Word.OptionsMinimal();}
  public Word.Options getOptionsNominal()  {return new Word.OptionsNominal();}

  public void setOptions(Word.Options opts) {
    this.opts = (opts != null) ? new Word.Options(opts) : new Word.OptionsAll();
  }
  
  //  
  // Print Words...
  //
  
  public void printWords() {
    opts = new Word.OptionsNominal();
    Word.printWordsFiltered(getWords(),"",opts);    
  }
  
  public void printWords(String prefix) {
    opts = new Word.OptionsAll();
    Word.printWordsFiltered(getWords(),prefix,opts);
  }

  public void printWordsFiltered(String prefix) {
    Word.printWordsFiltered(getWords(opts),prefix,opts);    
  } 
  
  //
  // getWords - Get the Words as an array
  //

  public Word[] getWords() {

    if (opts == null)
      opts = new Word.OptionsNominal();
    
    return getWords(opts);
    
  }

  public Word[] getWords(Word.Options opts) {

    ArrayList<Word> filtered = new ArrayList<Word>();
    int             fileSeq = 0;
    int             lineSeq = 0;
    int             prevLine = -1;
    
    if (opts == null)
      opts = new Word.Options();
    if (words == null || words.size() == 0)
      return new Word[0];
    
    if (words == null)
      return new Word[0];
    
    for (Word word : words) {
      if (!opts.showCOMMENT && word.type == Word.Type.COMMENT) continue;
      if (!opts.showWHITE   && word.type == Word.Type.WHITE)   continue;
      if (!opts.showEOL     && word.type == Word.Type.EOL)     continue;
      if (!opts.showEOF     && word.type == Word.Type.EOF)     continue;
      word = new Word(word); // Make sure we don't clobber original sequences etc.
      if (word.lineCol.line != prevLine)
        lineSeq = 0;
      prevLine = word.lineCol.line;
      word.lineSeq = lineSeq++;
      word.fileSeq = fileSeq++;
      filtered.add(word);
    }
    
    Word[] result = new Word[filtered.size()];
    result = filtered.toArray(result);
    
    return result;
  }
  
  //
  // Real stuff...
  //
  
  private void processContents() {

    // Locals...
    Word retWord;
    char c;

    // Initialise context...
    ctx.state      = State.NULL;
    ctx.span       = null;
    ctx.pos        = 0;
    ctx.lineCol    = new LineCol();
    ctx.fileSeq    = 0;
    ctx.lineSeq    = 0;
    ctx.quoteChar  = 0xFF;
    ctx.word       = new Word();
    errors         = new ArrayList<Error>();
    words          = new ArrayList<Word>();

    // Only start processing if we have file contents...
    if (fileName == null || data == null || data.length == 0)
      return;
    
    // Make sure it's UTF-8...
    String BOMname = BOM.BOMname(data);
    if (BOMname != null && BOMname != "UTF-8") {
      postError("File encoding \""+BOMname+"\", expected UTF-8");
      return;
    }
    
    // For each line...
    for (ctx.lineCol.line = 0; ctx.lineCol.line < lines.length; ctx.lineCol.line++) { // Each Line...

      Span span     = new Span(lines[ctx.lineCol.line]);
      Span fullSpan = new Span(span);
      
      // Ignore trailing \n and \r...
      while (span.end > span.start) {
        if (data[span.end-1] == '\n' || data[span.end-1] == '\r')
          span.end--;
        else
          break;
      }

      ctx.span        = span;
      ctx.errorCount  = 0;
      ctx.lineCol.col = 0;

      ctx.pos = ctx.span.start;
      
      while (ctx.pos < ctx.span.end) { // Character...

        c = (char)data[ctx.pos];

        switch (ctx.state) {
          case NULL:      retWord = StateNULL(c);      break;
          case NAME:      retWord = StateNAME(c);      break;
          case INT:       retWord = StateINT(c);       break;
          case HEX:       retWord = StateHEX(c);       break;
          case QUOTE:     retWord = StateQUOTE(c);     break;
          case WHITE:     retWord = StateWHITE(c);     break;
          case COMMENT:   retWord = StateCOMMENT(c);   break;
          case MLCOMMENT: retWord = StateMLCOMMENT(c); break;
          case ACTION:    retWord = StateACTION(c);    break;
          default:        retWord = null;              break;
        }

        // Trash rest of line if error...
        if (ctx.errorCount != 0)
          break;
        
        // Stash the result, if any...
        if (retWord != null)
          addWord(retWord);

        // Get ready for next one
        ctx.pos++;
        ctx.lineCol.col++;
        
      } // ...Character
      
      // Handle end of line...
      if (ctx.state != State.MLCOMMENT) { // End of Line...
        
        if (ctx.word != null && ctx.word.type != null) { // Terminate Word...

          // Handle unterminated quotes and quoted name...
          if (ctx.word.type == Word.Type.DQUOTE || ctx.word.type == Word.Type.SQUOTE) {
            postError(ctx.word.lineCol,"Unterminated string literal");
            ctx.word = null;
          }
          else if (ctx.word.type == Word.Type.QNAME) {
            postError(ctx.word.lineCol,"Unterminated '<...>' name");
            ctx.word = null;
          }

          // Terminate the word...
          if (ctx.word != null && ctx.word.type != null) {
            Word word;
            word = ctx.word;
            word.span.end = ctx.pos;
            word.len      = word.span.end - word.span.start;
            word.value    = spanString(word.span);
            addWord(word);

          }

          ctx.word = new Word();
          ctx.state = State.NULL;
          
          // Post EOL Word
          ctx.word.type     = Word.Type.EOL;
          ctx.word.lineCol  = new LineCol(ctx.lineCol);
          ctx.word.span     = new Span(ctx.pos,fullSpan.end);
          ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
          if (ctx.word.len != 0)
            addWord(ctx.word);
          ctx.word = new Word();

      } // ...Terminate Word
        
      } // ...End of Line

      
      // Restart line sequence counter...
      ctx.lineSeq = 0;
      
    } // ...Each Line
    
    // Handle end of file...
    if (ctx.state == State.MLCOMMENT)
      postError(ctx.word.lineCol,"Unterminated /*... comment");

    // Post EOL Word
    Word lastWord = words.get(words.size()-1);
    ctx.word = new Word();
    ctx.word.type     = Word.Type.EOF;
    ctx.word.lineCol  = new LineCol((lastWord.type == Word.Type.EOL)?ctx.lineCol.line:ctx.lineCol.line-1, ctx.lineCol.col);
    ctx.word.span     = new Span(data.length,data.length);
    ctx.word.len      = 0;
    addWord(ctx.word);      
    
  }
  
  //
  // Processing State handlers
  //

  private Word StateNULL(char c) {
          
    Word    retWord = null;
    boolean isComment;
    int     remLen  = ctx.span.end - ctx.pos;

    // If comment...
    isComment = false;
    if (commentChars.indexOf(c) >= 0) {
      isComment = true;
      if (c == '-' && (ctx.pos >= ctx.span.end-1 || data[ctx.pos+1] != '-'))
        isComment = false;
      if (c == '/' && (ctx.pos >= ctx.span.end-1 || data[ctx.pos+1] != '/'))
        isComment = false;
    }
    if (isComment) {
      ctx.word.type     = Word.Type.COMMENT;
      ctx.word.lineCol  = new LineCol(ctx.lineCol);
      ctx.word.span     = new Span(ctx.pos,ctx.span.end);
      ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
      
      ctx.word.value    = spanString(ctx.word.span);
      retWord           = ctx.word;
      ctx.word          = new Word();
      ctx.pos           = ctx.span.end;
      
      return retWord;
    }
    
    // Check for '/*'...
    if (c == '/' && ctx.pos < ctx.span.end-1 && data[ctx.pos+1] == '*') {
      ctx.state         = State.MLCOMMENT;
      ctx.word.lineCol  = new LineCol(ctx.lineCol);
      ctx.word.span     = new Span(ctx.pos,ctx.pos+1);
      ctx.word.len      = 2;
      ctx.word.type     = Word.Type.COMMENT;
      ctx.pos          += 2;
      ctx.lineCol.col  += 2;
      return retWord;
    }

    // If whitespace...
    if (whiteSpace.indexOf(c) >= 0) {
      ctx.state         = State.WHITE;
      ctx.word.lineCol  = new LineCol(ctx.lineCol);
      ctx.word.span     = new Span(ctx.pos,ctx.pos+1);
      ctx.word.len      = 1;
      ctx.word.type     = Word.Type.WHITE;
      return retWord;
    }
    
    // If operator...
    if (ops.indexOf(c) >= 0) {
      ctx.word.type     = Word.Type.OPERATOR;
      ctx.word.lineCol  = new LineCol(ctx.lineCol);
      ctx.word.span     = new Span(ctx.pos,ctx.pos+1);
      ctx.word.len      = 1;
      ctx.word.value    = spanString(ctx.word.span);
      //if (c == '{') { // Handle actions...
      //  ctx.word.type = Word.Type.ACTION;
      //  ctx.state     = State.ACTION;
      //  return null;
      //}
      retWord = ctx.word;
      ctx.word = new Word();
      if (c == '.' && remLen > 1 && data[ctx.pos+1] == '.') { // Handle special case of '..'
        retWord.span.end++;
        retWord.len++;
        retWord.value += '.';
        ctx.pos++;
        ctx.lineCol.col++;
      }
      return retWord;
    }
    
    // Weird permutations of ':=', '::=', and '='...
    if (c == ':' || c == '=') {
      
      int     pos;
      int     len;
      
      pos = ctx.pos;
      len = 1;
      if (remLen >= 3 && data[pos] == ':' && data[pos+1] == ':' && data[pos+2] == '=')
        len = 3;
      else if (remLen >= 2 && data[pos] == ':' && data[pos+1] == '=')
        len = 2;

      retWord  = ctx.word;
      ctx.word = new Word();     
      retWord.lineCol = new LineCol(ctx.lineCol);
      retWord.span    = new Span(pos,pos+len);
      retWord.len     = len;
      retWord.type    = Word.Type.OPERATOR;
      retWord.value   = "::=";
      ctx.pos         += len-1;
      ctx.lineCol.col += len-1;
      return retWord;
    
    }

    // Names...
    if (nameStarts.indexOf(c) >= 0) {
      ctx.state        = State.NAME;
      ctx.word.lineCol = new LineCol(ctx.lineCol);
      ctx.word.span    = new Span(ctx.pos,ctx.pos+1);
      ctx.word.len     = 1;
      ctx.word.type    = Word.Type.NAME;
      return null;
    }
    
    // Integer and Hexidecimal constants...
    if (c >= '0' && c <= '9') {
      if (c == '0' && remLen > 1 && (data[ctx.pos+1] == 'x' || data[ctx.pos+1] == 'X')) {
        ctx.state        = State.HEX;
        ctx.word.lineCol = new LineCol(ctx.lineCol);
        ctx.word.span    = new Span(ctx.pos,ctx.pos+2);
        ctx.word.len     = 2;
        ctx.word.type    = Word.Type.HEX;
        ctx.pos++;
        ctx.lineCol.col++;
      } 
      else {
        ctx.state        = State.INT;
        ctx.word.lineCol = new LineCol(ctx.lineCol);
        ctx.word.span    = new Span(ctx.pos,ctx.pos+1);
        ctx.word.len     = 1;
        ctx.word.type    = Word.Type.INT; 
      }
      ctx.word.value   = null;
      return null;      
    }
    
    // Handle quotes ('<...>' is handled like a quote but changed to NAME at end)
    if (c == '"' || c == '\'' || c == '<') {
      ctx.quoteChar = (c=='<')?'>':c;
      ctx.state = State.QUOTE;
      ctx.word.lineCol = new LineCol(ctx.lineCol);
      ctx.word.span    = new Span(ctx.pos,ctx.pos+1);
      ctx.word.len     = 1;
      if (c == '"')
        ctx.word.type = Word.Type.DQUOTE;
      else if (c == '\'')
        ctx.word.type = Word.Type.SQUOTE;
      else
        ctx.word.type = Word.Type.QNAME;
      // Special case for ''' and """...
      if (remLen >= 3 && ( (c == '"'  && data[ctx.pos+1] == '"'  && data[ctx.pos+2] == '"') || 
                           (c == '\'' && data[ctx.pos+1] == '\'' && data[ctx.pos+2] == '\'') ) ) {
        ctx.word.span.end = ctx.pos+3;
        ctx.word.len = 1;
        ctx.word.value = ""+(char)data[ctx.pos];
        retWord  = ctx.word;
        ctx.word = new Word();
        ctx.pos         += 2;
        ctx.lineCol.col += 2;
        ctx.state = State.NULL;
        return retWord;
      }
      return null;      
    }
    
    postError(ctx.lineCol,"Invalid character");

    return null;
    
  }

  private Word StateNAME(char c) {
    
    Word    retWord = null;
    boolean useIt   = true;
  
    if (nameChars.indexOf(c) < 0) {
      if (c == '.' && ctx.pos < data.length && nameChars.indexOf(data[ctx.pos+1]) >= 0)
        useIt = true;
      else
        useIt = false;
    }
    
    // If not a name character, finished with name...
    if (!useIt) {
      ctx.word.span.end = ctx.pos;
      ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
      ctx.word.value    = spanString(ctx.word.span);
      retWord = ctx.word;
      ctx.word = new Word();
      ctx.pos--;
      ctx.lineCol.col--;
      ctx.state = State.NULL;
      return retWord;  
    }
    
    return retWord;
    
  }
  
  private Word StateINT(char c) {
    
    Word retWord = null;
    
    if (c < '0' || c > '9') {
      ctx.word.span.end = ctx.pos;
      ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
      ctx.word.value    = spanString(ctx.word.span);
      retWord = ctx.word;
      ctx.word = new Word();
      ctx.pos--;
      ctx.lineCol.col--;
      ctx.state = State.NULL;
    }
    
    return retWord;
    
  }
  
  private Word StateHEX(char c) {
    
    Word retWord = null;
    
    if ( (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F') )
      return null;
    else {
      ctx.word.span.end = ctx.pos;
      ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
      ctx.word.value    = spanString(ctx.word.span);
      retWord = ctx.word;
      ctx.word = new Word();
      ctx.pos--;
      ctx.lineCol.col--;
      ctx.state = State.NULL;
      return retWord;
    }
    
  }
  
  private Word StateQUOTE(char c) {

    Word retWord = null;
    int  utf8Len;
    
    if (c == '\\') {
      ctx.pos++;
      ctx.lineCol.col++;
      return null;
    }
    
    utf8Len = utils.lengthUTF_8(data,ctx.pos);
    if (utf8Len < 0) {
      postError(ctx.lineCol,"Unvalid/malformed UTF-8 character");
      return null;
    }
    if (c == ctx.quoteChar) {
      ctx.word.span.end = ctx.pos+1;
      if (ctx.quoteChar != '>') { // Get rid of starting and trailing quotes...
        ctx.word.span.start++;
        ctx.word.span.end--;
        ctx.word.len -= 2;
      }
      ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
      ctx.word.value    = spanString(ctx.word.span);
      retWord = ctx.word;
      ctx.word = new Word();
      ctx.state = State.NULL;
    }
    if (utf8Len > 0)
      ctx.pos += utf8Len-1;
    
    return retWord;
    
  }
  
  private Word StateWHITE(char c) {
    
    Word retWord = null;
    
    if (whiteSpace.indexOf(c) < 0) {
      ctx.word.span.end   = ctx.pos;
      ctx.word.len        = ctx.word.span.end - ctx.word.span.start;
      ctx.word.value      = spanString(ctx.word.span);
      retWord = ctx.word;
      ctx.word = new Word();
      ctx.pos--;
      ctx.lineCol.col--;
      ctx.state = State.NULL;
    }
    
    return retWord;
  
  }
  
  private Word StateCOMMENT(char c) {

    postError(ctx.lineCol,"Comments not implemented");
    
    return null;
    
  }
  private Word StateMLCOMMENT(char c) {
    
    Word retWord = null;

    if (c == '/' && ctx.pos > 0 && data[ctx.pos-1] == '*') {
      ctx.word.span.end = ctx.pos + 1;
      ctx.word.lineSeq  = ctx.lineSeq++;
      ctx.word.fileSeq  = ctx.fileSeq++;
      ctx.word.len      = ctx.word.span.end - ctx.word.span.start;
      ctx.word.value    = spanString(ctx.word.span);
      retWord           = ctx.word;
      ctx.word          = new Word();
      ctx.state         = State.NULL;
    }
    
    return retWord;

  }
  
  private Word StateACTION(char c) {
    
    Word retWord = null;
    
    if (c == '{' && ctx.word.len != null && ctx.word.len != 0) {
      postError(ctx.lineCol,"Nested '{' in Action not allowed");
      return null;
    }
    
    ctx.word.span.end++;
    ctx.word.len++;
    ctx.word.value += c;

    if (c == '}') {
      retWord           = ctx.word;
      ctx.word          = new Word();
      ctx.state         = State.NULL;
    }
    
    return retWord;
    
  }
    
}