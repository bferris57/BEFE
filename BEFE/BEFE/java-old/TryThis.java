
import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.zip.CRC32;
import java.util.Random;

import bferris.core.Time;

class TryThis {

	//
	// Class members
	//
	
	// Output control
	static PrintStream              out = System.out;
	static boolean                  quiet   = false;
	static boolean                  verbose = true;
	
	// The word grid
	static char[]                   grid;
	static int                      width  = 1000;
	static int                      height = 1000;
	static int[]                    gridcharcounts;
	
  // Word/dictionary info
  static int                      totchars;
  static int                      totwords;
  static int                      minwordlength;
  static int                      maxwordlength;

  // Word/dictionary processing
	static String[]                 thewords;
	static int[]                    startswith = new int[26];
  static int[]                    endswith   = new int[26];
  static int[]                    contains   = new int[26];
  static HashMap<Integer,Integer> crcs;
  static int                      crcduplicates;
  static int[]                    lencounts;

  // Search criteria
  static int                      wordmin = 3;
  //static int                      wordmax = 11;
  static int                      wordmax = 40;
  
  // Search statistics
  static int                      wordsfound;
  static int                      distinctfound;
  static HashMap<String,Integer>  foundcount;
  
	//
	// Utility class
	//
	
	class CrcRing {

		int     size;
	  CRC32[] buffer;
	  int     top;
    int     bot;
    
	  CrcRing(int bufsize) {
	  	this.size   = bufsize+1;
	  	this.buffer = new CRC32[size];
	  	for (int i=0;i<size;i++) {
	  		this.buffer[i] = new CRC32();
	  		this.buffer[i].reset();
	  	}
	  	this.top    = 0;
	  	this.bot    = 0;
	  }
	  
	  void Push(char c) {
	  	int i;

	  	// Bump top
	  	top++;
	  	top %= size;
	  	if (top == bot) {
	  		bot++;
	  		bot %= size;
	  	}
	  	this.buffer[top].reset();
	  	
	  	// Process ones between bot and top...
	  	i = bot;
	  	while (i!= top) {
	  		this.buffer[i++].update(c);
	  		i %= size;
	  	}
	  	
	  }
	  
	  void SearchWords(int x, int y) {
	  
	  	int     bufidx;
	  	int     grididx;
	  	int     crc;
	  	String  theword;
	  	char[]  wordchars;
	  	int     px;
	  	int     i;
	  	int     occurs;
	  	
	  	wordchars = new char[size];
	  	
 			for (bufidx=0;bufidx<size;bufidx++) {
 				crc = (int)this.buffer[bufidx].getValue();
 				if (crc != 0 && crcs.containsKey(crc)) {
 					
 					// Get the suspected word
 					theword = thewords[crcs.get(crc)];
 					px = x - (theword.length()-1);
 					wordchars = theword.toCharArray();
 					
 					// Make sure it's there in the grid
 					if (wordchars.length-1 > x)
 						continue;
          grididx = y*width + px;
          for (i=0; i<wordchars.length && wordchars[i] == grid[grididx++];i++);
          if (i < wordchars.length)
          	continue;
          
          // Update list
          if (!foundcount.containsKey(theword)) {
          	distinctfound++;
          	occurs = 1;
          }
          else
          	occurs = foundcount.get(theword)+1;
          foundcount.put(theword,occurs);
          
 					// Display it
          //if (verbose)
   				//	out.println(String.format("[%d,%d] = '%s'", px,y,theword));
 					wordsfound++;
 				}
 			}
 			
 			return;
	  }
	  
	};

	//
	// Class methods
	//
	
  public static void InitialiseGrid() {

  	Random rand;
  	int    i;
  	char   c;
  	int    ci;
    
    grid = new char[width*height];
    gridcharcounts = new int[26];
    rand = new Random(-1);
    for (i=0;i<width*height;i++) {
    	ci = rand.nextInt() % 26;
    	if (ci < 0)
    		ci = -ci;
    	c = (char)(ci + 'a');
    	grid[i] = c;
    	gridcharcounts[ci]++;
    }
    
    // TEMP: Put test word at start
    //char[] b = {'a','r','e','a','r','e'};
    //int    offset = 712 + 865*width;
    //int    offset = 712 + 12*width;
    //for (i=0;i<b.length;i++)
    //	grid[i+offset] = b[i];
  }
  
	static void SearchGrid() {

    int     x;
    int     y;
    int     curpos;
    TryThis tryit;
    CrcRing ring;
        
    tryit = new TryThis();
    wordsfound = 0;
    distinctfound = 0;
  	foundcount = new HashMap<String,Integer>();
    // TEMP: Just for now
    //for (y=0; y < 1; y++) {
    for (y=0; y < height; y++) {
    	curpos = y*width;
      ring = tryit.new CrcRing(maxwordlength);
      for (x=0; x < width; x++) {
        ring.Push((char)grid[curpos++]);	
        ring.SearchWords(x,y);
      }
    }
    
	}

  public static void ReadWords(String filename) {
    
  	int                     lineno;
    ArrayList<String>       linelist;
    HashMap<String,Integer> dict;
    int                     duplicates;
    
    linelist = new ArrayList<String>();
    dict = new HashMap<String,Integer>(250000);

    duplicates = 0;

    try {
      FileInputStream fstream = new FileInputStream(filename);
      DataInputStream in      = new DataInputStream(fstream);
      BufferedReader  br      = new BufferedReader(new InputStreamReader(in));
      String          line;
    
      lineno = 0;
      
      //Read File Line By Line
      while ((line = br.readLine()) != null)   {
      	lineno++;
      	if (line.length() >= wordmin && line.length() <= wordmax) {
      		line = line.toLowerCase();
      		if (!dict.containsKey(line)) {
          	linelist.add(line.toLowerCase());
          	dict.put(line,lineno);
      		}
      		else
      			duplicates++;
      	//else if (line.length() > 18 && !quiet)
      	//	out.println("  Skipped: "+line);
      	}
      }
      //Close the input stream
      in.close();
    }
    catch (Exception e) {
      System.err.println("Error: " + e.getMessage());
    }
    
    thewords = linelist.toArray(new String[linelist.size()]);

    if (!quiet && duplicates != 0)
    	out.println(duplicates+" duplicate words (ignored)");
    
    return;
  }
  
	static void ProcessWords() {
		
    int     i;
    char    c;
    int     cint;
    int     wordlength;
    CRC32   crc32;
    Integer thecrc;
    
    startswith    = new int[26];
    endswith      = new int[26];
    contains      = new int[26];
    crcs          = new HashMap<Integer,Integer>();
    crcduplicates = 0;
    totchars      = 0;
    totwords      = 0;
    minwordlength = -1;
    maxwordlength = 0;
    
    crc32         = new CRC32();
    lencounts     = new int[wordmax+1];
    
		for (String word : thewords) {

			crc32.reset();
			wordlength = 0;
			
			// TEMP: Add length into CRC
			//crc32.update((byte)word.length());
			// Do character counts...
		 	for (i=0;i<word.length();i++) {
				c = word.charAt(i);
				cint = c-'a';				
				if (cint < 0 || cint >= 26)
					continue;
				if (i==0)
					startswith[cint]++;
				else if (i == word.length()-1) {
					endswith[cint]++;
          if (!quiet && c == 'q')
          	out.println("Ends with 'q': '"+word+"'");
				}
				contains[cint]++;
				totchars++;
				wordlength++;
				crc32.update((int)c);
				
			}
			
			// Add to word length count
			lencounts[wordlength]++;
			
			// TEMP: Validate some special cases...
			//if (word.equals("magnet") || word.equals("magnetism") || word.equals("electromagnetism")) {
			//	out.println(String.format("TEMP: %s = 0x%08x",word,crc32.getValue()));
			//}

			// TEMP: Add length into CRC
			//crc32.update((byte)wordlength);
			// Process CRC...
			thecrc = new Integer((int)crc32.getValue());
			
			if (crcs.containsKey(thecrc)) {
				//crcs.put(thecrc, crcs.get(thecrc)+1);
				crcduplicates++;
				if (verbose)
					out.println("Dup: "+word+" (same as '"+thewords[crcs.get(thecrc)]+")");
			}
			else if (thecrc == 0)
				out.println("!!!!! 0 CRC !!!!!");
			else
				crcs.put(thecrc, totwords);
    
			// Process counts and word lengths
			totwords++;
			if (minwordlength < 0) 
				if (wordlength != 0)
					minwordlength = wordlength;
      if (wordlength != 0 && wordlength < minwordlength)
      	minwordlength = wordlength;
      if (wordlength > maxwordlength)
      	maxwordlength = wordlength;
		}
		
		if (minwordlength < 0)
			minwordlength = 0;

		return;
	}
	
	//
	// Main method
	//
	
	static void Doit() {

		int      i;
    Time     t1;
    Time     t2;
    Time     et;
    
		out.println("Reading file...");
		//ReadWords("d:\\cloud\\dropbox\\lists\\corncob_lowercase.txt");
		ReadWords("d:\\cloud\\dropbox\\lists\\websters_2nd.txt");
    out.println("  Total Words: "+thewords.length);

    if (!quiet)
      out.println("");
    out.println("Processing words...");
    
    ProcessWords();

    if (!quiet)
    	out.println("");
    out.println("Initialising grid...");
    
    InitialiseGrid();

    if (!quiet) {
      out.println("");
      out.println("Word/dictinoary processing statistics...");
      
      out.println("       Word Starts With Word Ends with   Word Contains...  Grid Contains...");
      out.println("       ---------------- ---------------- ----------------  ----------------");
      for (i=0;i<26;i++) {
      	out.println(String.format("'%c' -> %8d = %4.1f%% %8d = %4.1f%% %8d = %4.1f%% %8d = %4.1f%%",'a'+i,
      			startswith[i],    ((float)startswith[i]*100.0+50.0)/(float)thewords.length,
      			endswith[i],      ((float)endswith[i]*100.0+50.0)/(float)thewords.length,
      			contains[i],      ((float)contains[i]*100.0+50.0)/(float)totchars,
      			gridcharcounts[i],((float)gridcharcounts[i]*100.0+50.0)/(float)(width*height)
    			                     ));
      }
      out.println("");
      out.println("Duplicate CRCs      = "+crcduplicates);
      out.println("Minimum word length = "+minwordlength);
      if (thewords.length != 0)
        out.println(String.format("Average word length = %.1f",(float)totchars/(float)thewords.length));
      else
      	out.println("Average word length = 0");
      out.println("Maximum word length = "+maxwordlength);
      
      out.println("");
      out.println("Len Count");
      out.println("--- -----");
      for (i=0;i<lencounts.length;i++) {
      	if (lencounts[i] != 0)
        	out.println(String.format("%3d %5d",i,lencounts[i]));
      }
      out.println("");
    }
    
    t1 = new Time();
    
    out.println("Searching grid...");
    
    //SearchGrid();
    t2 = new Time();
    et = t1.Delta(t2);

    out.println(String.format("  %7d word occurrences found",wordsfound));
    out.println(String.format("  %7d distinct words found",distinctfound));
    out.println("  Elapsed time: "+et.GetAs(Time.Minutes)+"m "
                                  +et.GetAs(Time.Seconds)+"s "
                                  +et.GetAs(Time.Milliseconds)+"ms");
    
    
    if (!quiet)
      out.println("");
    out.println("***Finished***");

	}
}

