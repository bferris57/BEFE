package bferris.core;

public class Exception extends java.lang.Exception {

	static final long serialVersionUID = -1; // Still don't know what this is about
	
	private String message;
	
	public Exception(String message) {
		this.message = message;
		// !!!!Need to examine runtime stack and put line number in message if necessary
    throw new RuntimeException(message);	  	
	}
	
	@Override
	public String toString() {
		return message;
	}
	
}
