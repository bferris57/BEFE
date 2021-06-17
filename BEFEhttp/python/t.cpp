#include <limits>
#include <cstddef>
#include <iostream>
using namespace std;
#define eol '\n'

void myfunc(int const * const & a) {
  cout << "myfunc: a = " << (int)a << '\n';
}
  
int main(int argc, char *argv[]) {
  int & a = (int &)(int *)0;
  int *b = (int *)&a;
  
  myfunc(b);
  
  return 0;
}