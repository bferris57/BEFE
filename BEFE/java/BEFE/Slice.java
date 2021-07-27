package BEFE;

import java.util.*;
import java.lang.reflect.*;

@SuppressWarnings("unchecked")
public class Slice <T> {

  public T[] slice(T[] source,Span span) {
    
    List<T>  dest = new ArrayList<T>();
    Span     tSpan = new Span(span);
    int      srcPos;
    T[]      destArray;

    class container {T[] value;}
    container inst = new container();

    tSpan.normalise(source.length);
    dest = new ArrayList<T>();
    srcPos = tSpan.start;
    if (tSpan.end >= tSpan.start) { // Forwards...
      while (srcPos < tSpan.end) {
        dest.add(source[srcPos]);
        srcPos += tSpan.step;
      }
    }
    else { // Backwards...
      while (srcPos > tSpan.end) {
        dest.add(source[srcPos]);
        srcPos += tSpan.step;
      }
    }
    
    destArray = (T[])Array.newInstance(source.getClass().getComponentType(),dest.size());
    destArray = dest.toArray(destArray);
    
    return destArray;
  }
}