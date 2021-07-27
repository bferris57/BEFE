// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;

import java.util.*; // For ArrayList, etc.
import BEFE.*;

public class FileLines implements Iterable<Span> {

  //
  // FileWords Members
  //

  public Span   span;
  public byte[] data;
  
  //
  // FileLines Methods...
  //
  
  public FileLines(byte[] data) {
    
    this.data = data;
    if (data != null)
      this.span = new Span(0,data.length);
    else
      this.span = new Span(0,null);
    
  }

  //
  // Iterable interface Methods
  //  
  
  public Iterator<Span> iterator() {
      return new FileLinesIterator(data);
  }

  // Inner class example
  private class FileLinesIterator implements Iterator<Span> {
    
    private Span   itSpan;
    private byte[] data;

    public FileLinesIterator(byte[] data) {
       this.itSpan = new Span(span);
       this.data   = data;
    }

    public boolean hasNext() {
        if (itSpan == null || itSpan.start == null || itSpan.end == null)
          return false;
        return itSpan.start < itSpan.end;
    }

    //@Override
    public Span next() throws NoSuchElementException {
    
      Span retSpan = new Span(itSpan);
      int  nextNL = -1;
      
      // Look for next '\n'...
      if (retSpan.start < retSpan.end) {
        for (nextNL=retSpan.start; nextNL < retSpan.end; nextNL++)
          if (data[nextNL] == (byte)'\n') break;
        if (nextNL >= retSpan.end)
          nextNL = -1;
      }
      
      // If found it, return it...
      if (nextNL < 0 || (nextNL >= 0 && nextNL > retSpan.start)) {
        if (nextNL >= 0)
          retSpan.end = nextNL+1;
        itSpan.start = retSpan.end;
        return retSpan;
      }
      
      // Didn't find it, error
      throw new NoSuchElementException();

    }

    public void remove() throws UnsupportedOperationException {
      throw new UnsupportedOperationException();
    }
    
  }

}