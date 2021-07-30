//==============================================================================
//
// File: fotech.cpp - Bruce's answer to Fotech's "Technical Quiz"...
//
// Notes: This code can be tested by compiling it to an executable and
//        invoke it on the command line with multiple floating point arguments.
//        it will display the running sum
//

#include <iostream>

using namespace std;

//------------------------------------------------------------------------------
//
// Class: FloatSum - Sum a set of floats
//

class average {

  private: int   n;   // Number of floats so far
  private: float sum; // Running sum

  public: average() {
    n =   0;
    sum = 0.0;
  }

  public: void add_value(float that) {
    n++;
    sum += that;
  }

  public: float get() {
    return sum/n;
  }

};

//------------------------------------------------------------------------------
//
// FOR TESTING...
//
// Function: isFloat - Is a string a valid floating point number?
//           main    - Executable tester (each arg should be a float string)
//

static bool isFloat(string myString) {
  try {
    float f = stof(myString);
  } catch(exception e) {
    return false;
  }
  return true;
}

int main(int argc, char **argv) {

  string   curarg;
  average  avg;
  int      bad = 0;

  for(int i=1; i < argc; i++) {
    curarg = string(argv[i]);
    if (isFloat(curarg)) {
      avg.add_value(stof(curarg));
      cout << i << ": '" << curarg << "'\t-- running average = " << avg.get() << endl;
    } else {
      cout << i << ": Ignoring '" << curarg << "'" << endl;
      bad++;
    }
  }

  cout << "Final average: " << avg.get() << endl;

  return (bad)?-1:0;

}
