package BEFE;

import java.io.*;
import java.util.Locale;

public final class out {

  public static PrintStream 	printf(Locale l, String format, Object... args) {
    System.out.printf(l,format,args);
    return System.out;
  }
  public static PrintStream 	printf(String format, Object... args) {
    System.out.printf(format,args);
    return System.out;
  }
  public static void          println() {
    System.out.println();
  }
  public static void          println(String msg) {
    System.out.println(msg);
  }
  
  public static void          print(String msg) {
    System.out.print(msg);
  }
  
}