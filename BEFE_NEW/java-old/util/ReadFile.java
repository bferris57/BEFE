package bferris.util;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;

public class ReadFile {

  //----------------------------------------------------------------------------
  //
  // Method: Read - Read an entire file as a string
  //
	// Notes: Nicked this from the web at:
	//
	//          http://www.javapractices.com/topic/TopicAction.do?Id=42
	//
  //----------------------------------------------------------------------------

	public static String ReadString(java.io.File aFile) {

    StringBuilder contents = new StringBuilder();
    
    try {
      BufferedReader input =  new BufferedReader(new FileReader(aFile));
      try {
        String line = null;
        while (( line = input.readLine()) != null){
          contents.append(line);
          contents.append(System.getProperty("line.separator"));
        }
      }
      finally {
        input.close();
      }
    }
    catch (IOException ex){
      ex.printStackTrace();
    }
    
    return contents.toString();
  }
  
}
