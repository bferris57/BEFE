package BEFE;

public class Error extends Exception {

  public String fileName;
  public String message;

  public Error(String message) {
    super();
    this.message = message;
    this.fileName = null;
  }

  public Error(String fileName, String message) {
    super();
    this.fileName = fileName;
    this.message  = message;
  }
  
  @Override
  public String getMessage() {
    String msg;
    if (fileName == null) {
      msg = "Error";
    }
    else
      msg = "Error in '"+fileName+"'";
    msg += ": "+message;
    return msg;
  }
  
}
