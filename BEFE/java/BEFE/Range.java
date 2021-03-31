package BEFE;

import BNF.utils;

public class Range {

  public Integer start;
  public Integer end;
  
  public Range()                                {start = end = null; }
  public Range(Integer s)                       {start=s; end = null; }
  public Range(Integer s, Integer e)            {start=s; end = e;}

  public Range(String str) {

    String[] parts = str.split("\\.\\.");
 
    start = end = null;
    
    try {    
      if (parts.length > 3)
        throw new SyntaxError("Too many '..' specified, maximum of 2 allowed");

      if (parts.length > 0) {
        String part;
        part = parts[0].trim();
        if (part.length() != 0)
          start = utils.toInteger(part);
        else
          start = null;
        if (parts.length > 1) {
          part = parts[1].trim();
          if (part.length() != 0)
            end = utils.toInteger(part);
          else
            end = null;
        }
      }
    }
    catch (Exception e) {
      start = end = null;
    }

  }

  public Range(Range that) {
  
    if (that != null) {
      start = that.start;
      end   = that.end;
    }
    
  }

  public boolean equals(Range that) {
    int     thisStart;
    int     thisEnd;
    int     thatStart;
    int     thatEnd;
    if (that == null)
      return false;
    if (this.start == null)
      thisStart = 0;
    else
      thisStart = this.start;
    if (this.end == null)
      thisEnd = 0;
    else
      thisEnd = this.end;
    if (that.start == null)
      thatStart = 0;
    else
      thatStart = that.start;
    if (that.end == null)
      thatEnd = 0;
    else
      thatEnd = that.end;
    return (thisStart == thatStart && thisEnd == thatEnd);    
  } 
  
  @Override
  public String toString() { // ToString...

    String result;
  
    result = "";
    
    //normalise();
    if (start != null)
      result += start;
    result += "..";
    if (end != null)
      result += end;
    
    return result;
    
  } // ...ToString
  
  public Integer size() {
    
    normalise();
    if (start == null || end == null)
      return 0;
    return (end-start);

  }

  
  /**
  Turn Range into consistent, predictable form regardless
  of what it's applied to.
  */
  public Range normalise() {
    if (start == null)
      start = 0;
    if (end == null)
      end = 0;
    if (start > end) {
      start ^= end;
      end   ^= start;
      start ^= end;
    }

    return this;
  }
  
}