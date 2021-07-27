package BEFE;

public class Failed extends Error {

  public Failed(String message) {
    super(message);
    this.message = message;
  }

}
