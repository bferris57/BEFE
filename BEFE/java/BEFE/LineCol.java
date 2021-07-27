package BEFE;

public class LineCol {

  public Integer line;
  public Integer col;
  
  public LineCol() {
    line = 0;
    col  = 0;
  }
  
  public LineCol(int line, int col) {
    this.line = line;
    this.col  = col;
  }
  
  public LineCol(LineCol that) {
    line = null;
    col  = null;
    if (that != null) {
      this.line = that.line;
      this.col  = that.col;
    }
  }
  
  public LineCol(String lineCol) {
    
    String[] parts = lineCol.split("/");
    
    line = col = 0;
    if (parts.length >= 1) try {
      line = Integer.valueOf(parts[0]);
    }
    catch (NumberFormatException e) {}
    if (parts.length >= 2) try {
      col = Integer.valueOf(parts[1]);
    }
    catch (NumberFormatException e) {}
    
  }
  
  public String toString() {
    return "" + line + "/" + col;
  }
  
  public String to1String() {
    return "" + (line+1) + "/" + (col+1);
  }
  
}