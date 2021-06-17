#include <cstdlib>
#include <iostream>
using namespace std;
int main(int argc, char **argv) {
  int a = 0;
  a = a++ + ++a;
  cout << "a = " << a << '\n';
}

