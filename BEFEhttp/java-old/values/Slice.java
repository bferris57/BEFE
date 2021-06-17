package bferris.values;

public class Slice {

	private Integer begin;
	private Integer end;
	private Integer step;

	public static Slice New() {
	
		Slice that = new Slice();
		that.begin = null;
		that.end   = null;
		that.step  = null;
		return that;
		
	}
	
	public static Slice New(Integer begin) {

		Slice that = new Slice();
		that.begin = begin;
		that.end   = null;
		that.step  = null;
		return that;
	}

	public static Slice New(Integer begin, Integer end) {

		Slice that = new Slice();
		that.begin = begin;
		that.end   = end;
		that.step  = null;
		return that;

	}
	
	public static Slice New(Integer begin, Integer end, Integer step) {
		
		Slice that = new Slice();
		that.begin = begin;
		that.end   = end;
		that.step  = step;
		return that;		

	}

	public static Slice Copy(Slice that) {
	  return new Slice(that);	
	}
	
	public Slice() {
	  this.begin = null;
	  this.end   = null;
	  this.step  = null;
	}
	
	public Slice(Slice that) {
	  this.begin = that.begin;
	  this.end   = that.end;
	  this.step  = that.step;
	}
	
	public Slice Normalise(Integer size) {

		if (size == null)
			return this;
		
		if (begin == null) begin = 0;
		if (end   == null) end   = size;
		if (step  == null) step  = 1;
		
		if (begin <  0)    begin = begin + size;
		if (begin <  0)    begin = 0;
		if (end   <  0)    end   = end + size;
		if (end   <  0)    end   = 0;

		if (begin > size)  begin = size;
		if (end   > size)  end   = size;

		if (step < 0)
			step = -step;
		
		if (end < begin) {
			step  = -step;
		}
		
		return this;
	}
	
	public String ToString() {

		String s;
		
		s = "";
		if (begin != null)
  		s = begin.toString();
		s = s + ":";
    if (end != null)
    	s = s + end.toString();
    if (step != null && step != 1) {
    	s = s + ":" + step.toString();
    }
		return s;

		
	}
	
	@Override
	public String toString() {
    return ToString();
	}

}
