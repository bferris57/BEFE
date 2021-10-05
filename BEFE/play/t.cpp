#include <unistd.h>
#include <iostream>
#include "Primitives.h"
#include <sys/time.h>

using namespace std;
using namespace BEFE;

int main(int argc, char **argv) {

  //time_t now = time(NULL);
  struct timeval now;
  long           l;

  gettimeofday(&now,NULL);
  l = now.tv_sec*1000 + now.tv_usec/1000;
  cout << "START                      = " << l/1000 << '.' << l%1000  << endl;

  cout << "sizeof(size_t)             = " << sizeof(size_t) << endl;
  cout << "sizeof(int)                = " << sizeof(int) << endl;
  cout << "sizeof(unsigned long)      = " << sizeof(unsigned long) << endl;
  cout << "sizeof(unsigned long long) = " << sizeof(unsigned long long) << endl;
  cout << "getpagesize()              = " << getpagesize() << endl;
  cout << "sizeof(PtrInt)             = " << sizeof(PtrInt) << endl;
  cout << "sizoeof(time_t)            = " << sizeof(time_t) << endl;

  usleep(100000);
  gettimeofday(&now,NULL);
  l = now.tv_sec*1000 + now.tv_usec/1000;
  cout << "END                        = " << l/1000 << '.' << l%1000 << endl;

  return 0;

}
