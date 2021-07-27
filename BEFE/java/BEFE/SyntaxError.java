package BEFE;

public class SyntaxError extends Error {

  public LineCol lineCol;

  public SyntaxError(String message) {
    super(message);
    lineCol  = null;
  }

  public SyntaxError(String fileName, String message) {
    super(fileName,message);
    lineCol  = null;
  }
  
  public SyntaxError(int line, int col, String message) {
    super(message);
    lineCol  = new LineCol(line,col);
  }
  
  public SyntaxError(LineCol lineCol, String message) {
    super(message);
    this.lineCol = new LineCol(lineCol);
  }
  
  public SyntaxError(String fileName, LineCol lineCol, String message) {
    super(fileName,message);
    this.lineCol  = new LineCol(lineCol);
  }
  
  public SyntaxError(String fileName, int line, int col, String message) {
    super(fileName,message);
    this.lineCol  = new LineCol(line,col);
  }
  
  @Override
  public String getMessage() {
    String msg;
    msg = "Syntax Error";
    if (super.fileName != null)
      msg += " in file \"" + super.fileName + "\"";
    if (lineCol != null)
      msg += " at " + lineCol.to1String();
    msg += ": "+super.message;
    return msg;
  }
  
}
