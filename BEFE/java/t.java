
import java.io.*; // For PrintStream, etc.
import BNF.utils;

public class t {
  
  public static void main(String[] args) {
    
    char a = ' ';
      
    System.out.printf(" a = 0x%04x",a);
    a = -a;
    System.out.printf("-a = 0x%04x\n",a);
    
  }
}
