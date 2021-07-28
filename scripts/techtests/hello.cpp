#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

  vector <char *> args;
  int             i;

  cout << "Hello World!\n";
  cout << argc-1 << " argument" << ((argc != 1)?"s" : "" ) << "\n";

  for (int i=1; i < argc; i++)
    args.push_back(argv[i]);

  i = 0;
  for (char *x:args) {
    cout << i << ": '" << x << "'" << endl;
    i++;
  }

  return 0;

}
