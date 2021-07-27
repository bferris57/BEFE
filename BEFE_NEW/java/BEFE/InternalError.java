package BEFE;

/**
Used for internal Assertions and other runtime errors.
*/
public class InternalError extends Error {

  public InternalError(String message) {
    super(message);
  }

  public InternalError(String fileName, String message) {
    super(fileName,message);
  }
  
  @Override
  public String getMessage() {
    String msg;
    msg = "Internal Error";
    if (super.fileName != null)
      msg += " in file \"" + super.fileName + "\"";
    msg += ": "+super.message;
    return msg;
  }
  
}
