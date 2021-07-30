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

class FloatSum {

  private: float n;   // Number of floats so far
  private: float sum; // Running sum

  public: FloatSum() {
    n =   0.0;
    sum = 0.0;
  }

  public: void push(float that) {
    n += 1.0;
    sum += that;
  }

  public: float average() {
    return sum/n;
  }

};

//------------------------------------------------------------------------------
//
// Function: isFloat - Is a string a valid floating point number?
//

bool isFloat(string myString) {
  try {
    float f = stof(myString);
  } catch(exception e) {
    return false;
  }
  return true;
}

int main(int argc, char **argv) {

  string   curarg;
  FloatSum sum;
  int      bad = 0;

  for(int i=1; i < argc; i++) {
    curarg = string(argv[i]);
    if (isFloat(curarg)) {
      sum.push(stof(curarg));
      cout << i << ": '" << curarg << "'\t-- running average = " << sum.average() << endl;
    } else {
      cout << i << ": Ignoring '" << curarg << "'" << endl;
      bad++;
    }
  }

  cout << "Final average: " << sum.average() << endl;

  return (bad)?-1:0;

}
