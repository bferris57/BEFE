package bferris.core;

import java.io.*;
import java.util.UUID;
//import java.util.ArrayList;

public class IdStore {

	static PrintStream out = System.out;
	
	private static final String sfname = "d:\\temp\\bferris\\IdStore.befdb";
  private static final long   magic0 = 0xBefDB000BEEFabcdL;
  public static void New() {
  	New(0x10000);
  }
  
	public static void New(long lmaxpersist) {
		
		RandomAccessFile store;
		
		int      slen;
		int      maxpersist;
		int      numpersist;
		File     sfile;
		String   spath;
		UUID     uuid;
		Id       id;
		byte[]   bytes;
		
		if (lmaxpersist < 0)
			throw new RuntimeException("Isn't "+lmaxpersist+" a rather silly maximum number of persistent Ids?");
		if (lmaxpersist > 0x10000)
			throw new RuntimeException("Too many Ids: "+lmaxpersist+"... time to write Id chunks!!!");
    
		// Calculate Id store file size in bytes
		maxpersist = (int)lmaxpersist;
    numpersist = Ids.FixedIds.length;
    slen = maxpersist*16+(maxpersist+7)/8;
    out.println("IdStore.NewStore: slen = "+slen);
    
    // Make sure the path to it exists BUT the file itself doesn't exist
    //spath = Path(sfname);
    spath = new File(sfname).getParent();
    new File(spath).mkdirs();
    sfile = new File(sfname); 
    if (sfile.exists() && sfile.length() != 0)
     	throw new RuntimeException("File \""+sfname+"\" already exists and has something in it");      	

    // Create and open the file
    try {
      store = new RandomAccessFile(sfile,"rw");
    }
    catch (FileNotFoundException e) {
    	throw new RuntimeException("Something went wrong trying to create the file");
    }

    // Write the file contents
    try {
      // Write Magic and maximum/number of persistent Ids
      store.writeLong(magic0);
      store.writeInt(maxpersist);
      store.writeInt(numpersist);
      // Write Id allocation bitlist
      byte[] bits = new byte[((int)lmaxpersist+7)/8];
      store.write(bits);
      // Write preallocated ids...
      for (int i=0;i<maxpersist;i++) {
      	if (i < numpersist) {
      		id = Ids.FixedIds[i];
      		bytes = id.ToBytes();
      		store.write(bytes);
      	}
     		else {
      	  uuid = UUID.randomUUID();
      	  store.writeLong(uuid.getMostSignificantBits());
      	  store.writeLong(uuid.getLeastSignificantBits());
     		}
      }
    }
    catch (IOException e) {
    	throw new RuntimeException("Something went wrong trying to write the file");
    }
    
	}
	

}
