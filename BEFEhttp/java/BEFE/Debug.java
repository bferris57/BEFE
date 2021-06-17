package BEFE;

import java.io.*;
import java.util.Locale;

public final class Debug {

  public static boolean debug   = true;
  public static boolean verbose = false;
  
  public static PrintStream printf(Locale l, String format, Object... args) {
    if (debug)
      System.out.printf(l,format,args);
    return System.out;
  }
  public static PrintStream printf(String format, Object... args) {
    if (debug)
      System.out.printf(format,args);
    return System.out;
  }
  public static void println() {
    if (debug)
      System.out.println();
  }
  public static void println(String msg) {
    if (debug)
      System.out.println(msg);
  }
  public static void print(String msg) {
    if (debug)
      System.out.print(msg);
  }
  
}