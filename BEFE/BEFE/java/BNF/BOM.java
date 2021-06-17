// Copyright (©) 2016, Bruce Ferris (UK), all rights reserved
package BNF;

import java.util.*;        // For ArrayList, etc.

public class BOM { // BOM...
 
  //
  // BOM Definitions...
  //
  
  private static String[] BOMdefs = {
    "UTF-8",       "EFBBBF",
    "UTF-16 (BE)", "FEFF",
    "UTF-16 (LE)", "FFFE",
    "UTF-32 (BE)", "0000FEFF",
    "UTF-32 (LE)", "FFFE0000",
    "UTF-7+/v8",   "2B2F7638",
    "UTF-7+/v9",   "2B2F7639",
    "UTF-7+/v+",   "2B2F762B",
    "UTF-7+/v/",   "2B2F762F",
    "UTF-7+/v8-",  "2B2F76382D",
    "UTF-1",       "F7644C",
    "UTF-EBCDIC",  "DD736673",
    "SCSU",        "0EFEFF",
    "BOCU-1",      "FBEE28",
    "GB-18030",    "84319533",
  };
  
  // Driving Map...
  public static HashMap<String,byte[]> BOMs = processDefs();
  
  //
  // Housework...
  //

  private static HashMap<String,byte[]> processDefs() {
    
    HashMap<String,byte[]> that = new HashMap<String,byte[]>();

    // For each def...
    for (int defIdx=0; defIdx < BOMdefs.length; defIdx += 2) {
      
      String name  = BOMdefs[defIdx];
      String value = BOMdefs[defIdx+1];
      byte[] hexBytes;

      // Turn hex nibbles in value into proper bytes...
      hexBytes = new byte[(value.length()&-2)/2];
      for (int i=0; i < value.length(); i+=2) {
        int  c1 = (int)value.charAt(i)   & 0xff;
        int  c2 = (int)value.charAt(i+1) & 0xff;
        if (c1 >= '0' && c1 <= '9')
          c1 -= '0';
        else if (c1 >= 'A' && c1 <= 'F')
          c1 = c1 - 'A' + 10;
        else if (c1 >= 'a' && c1 <= 'f')
          c1 = c1 - 'a' + 10;
        else
          c1 = 0;
        if (c2 >= '0' && c2 <= '9')
          c2 -= '0';
        else if (c2 >= 'A' && c2 <= 'F')
          c2 = c2 - 'A' + 10;
        else if (c2 >= 'a' && c2 <= 'f')
          c2 = c2 - 'a' + 10;
        else
          c2 = 0;
        hexBytes[i>>1] = (byte)((c1 << 4) + c2);
      }
      
      // Note: If hex bytes in "value" aren't proper or valid, we'll sort it
      that.put(name,hexBytes);

    }      
    
    return that;
    
  }
  
  //
  // String BOMname - Calculate name of the BOM (if any) at start of a byte array
  //

  public static String BOMname(byte[] data) {
    
    String BOMname = null;
    
    // For each possible beginning...
    for (String name : BOMs.keySet()) {
      byte[] bom = BOMs.get(name);
      if (data.length >= bom.length && Arrays.equals(bom,Arrays.copyOfRange(data,0,bom.length))) {
        BOMname = name;
        break;
      }
    }

    return BOMname;
    
  }

} // ...BOM