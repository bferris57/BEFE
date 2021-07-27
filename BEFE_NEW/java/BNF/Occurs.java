package BNF;

import BEFE.Range;

public class Occurs {
  Integer min;
  Integer max;
  public Occurs() {
    min = max = null;
  }
  public Occurs(Integer min, Integer max) {
    if (min != null)
      this.min = new Integer(min);
    if (max != null)
      this.max = new Integer(max);
  }
  public Occurs(Occurs that) {
    this.min = that.min;
    this.max = that.max;
  }
  public Occurs(String that) {
    if (that != null) {
      if (that.trim().equals("*")) {
        this.min = 0;
        this.max = null;
      }
      else {
        Range r = new Range(that);
        this.min = r.start;
        this.max = r.end;
      }
    }
  }
  public Occurs(Range range) {
    this.min = range.start;
    this.max = range.end;
  }
  
  @Override
  public String toString() {
    String str = "";
    if (min != null)
      str += min;
    str += ":";
    if (max != null)
      str += max;
    return str;
    
  }
}