package BEFE;

import java.util.*;     // For ArrayList, List, Map, etc.

/**
Stack<T> implements a generic stack of a given type.  Unlike List<T>, a
Stack<T> is zero indexed with Top Of Stack (TOS) always at index 0 and
never contains null, so null is used to indicate various "unable to perform"
actions.

Since BEFE.Stack<T> use an ArrayList<T> it is NOT thread safe. If you
need a thread safe one we'll consider SafeStack<T>. Just ask.

The reason it uses ArrayList<T> instead of Vector<T> is simply because of
the size dynamics (increases by 50% when needed instead of 200%).
*/

public class Stack<T> {
  private ArrayList<T> list;
  
  public Stack() {
    list = new ArrayList<T>();
  }
  
  public Stack(T entry) {
    list = new ArrayList<T>();
    if (entry != null)
      push(entry);
  }
  
  public T push(T entry) {
    if (entry != null)
      list.add(entry);
    return entry;
  }
  
  public T pop() {
    if (list.size() != 0)
      return list.remove(list.size()-1);
    return null;
  }

  public void clear() {
    list = new ArrayList<T>();
  }  
  
  public T get(int index) {
    if (index < 0 || index >= list.size())
      return null;
    return list.get(list.size()-1-index);
  }
  
  public int size() {
    return list.size();
  }
  
  public boolean isEmpty() {
    return (list.size() == 0);
  }
  
  public T TOS() {
    if (list.size() != 0)
      return list.get(list.size()-1);
    return null;
  }
}