package BEFE;

public class Span {

  public Integer start;
  public Integer end;
  public Integer step;
  
  public Span()                                 {start = end = step = null; }
  public Span(Integer s)                        {start=s; end = step = null; }
  public Span(Integer s, Integer e)             {start=s; end = e; step = null;}
  public Span(Integer s, Integer e, Integer st) {start=s; end = e; step = st;}

  public Span(String str) {

    String[] parts = str.split(":");

    try {    
      if (parts.length > 3)
        throw new SyntaxError("Too many ':' specified, maximum of 2 allowed");

      if (parts.length == 0) {
        start = end = step = null;
      }
      else {
        String part;
        part = parts[0].trim();
        if (part.length() != 0)
          start = new Integer(part);
        else
          start = null;
        if (parts.length > 1) {
          part = parts[1].trim();
          if (part.length() != 0)
            end = new Integer(part);
          else
            end = null;
        }
        if (parts.length > 2) {
          part = parts[2].trim();
          if (part.length() != 0)
            step = new Integer(part);
          else
            step = null;
        }
      }
    }
    catch (Exception e) {
      start = end = step = null;
    }
    
  }

  public Span(Span that) {
  
    if (that != null) {
      start = that.start;
      end   = that.end;
      step  = that.step;
    }
    
  }
  
  @Override
  public String toString() { // ToString...

    String result;
  
    result = "";
    
    normalise();
    if (start != null)
      result += start;
    result += ":";
    if (end != null)
      result += end;
    if (step != null && step != 1)
      result += ":"+step.toString();
    
    return result;
    
  } // ...ToString
  
  public String to1String() { // ToString...

    String result;

    normalise();    
    result = "";
    result += start+1;
    result += ":";
    result += end+1;
    if (step != 1)
      result += ":"+step.toString();
    
    return result;
    
  } // ...ToString
  
  public Integer size() {
    
    normalise();
    if (start == null || end == null)
      return 0;
    if (step == null || step == 1)
      return (end-start);
    return (end-start)/step;

  }

  
  /**
  Turn Span into consistent, predictable form regardless
  of what it's applied to.
  */
  public Span normalise() {
    normalise(null);
    return this;
  }
  
  /**
  Turn Span into consistent, predictable form based on
  a given sized array to be applied to.
  */
  public Span normalise(Integer len) {
    
    if (len == null) {
      if (start == null)
        start = 0;
      if (step == null)
        step = 1;
      return this;
    }
    
    // Special case for Span("-N:")...
    if (start != null && start < 0 && end == null) {
      start += len;
      if (start < 0)
        start = Math.max(0,len+start);
      end = -1;
      step = -1;
      return this;
    }
    
    // "Normal" cases...
    if (start == null)
      start = 0;
    if (start < 0)
      start = start+len;
    if (end == null)
      end = len;
    if (end < 0)
      end = end + len;
    if (start < 0)
      start = 0;
    if (start > len)
      start = len;
    if (end < 0)
      end = -1;
    if (end > len)
      end = len;
    if (step == null) {
      if (start > end)
        step = -1;
      else
        step = 1;
    }
    return this;

  } 
  
}