#include <unistd.h>
#include <iostream>
#include "Primitives.h"

using namespace std;
using namespace BEFE;

int main(int argc, char **argv) {

  cout << "sizeof(size_t)             = " << sizeof(size_t) << endl;
  cout << "sizeof(int)                = " << sizeof(int) << endl;
  cout << "sizeof(unsigned long)      = " << sizeof(unsigned long) << endl;
  cout << "sizeof(unsigned long long) = " << sizeof(unsigned long long) << endl;
  cout << "getpagesize()              = " << getpagesize() << endl;
  cout << "sizeof(PtrInt)             = " << sizeof(PtrInt) << endl;
  cout << "sizoeof(time_t)            = " << sizeof(time_t) << endl;

  return 0;

}
