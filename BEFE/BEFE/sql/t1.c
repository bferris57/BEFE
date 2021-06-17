#include "stdio.h"

typedef unsigned int uint;
typedef          int bool;
typedef unsigned int ubool;

ubool f1(int x) {
	return !(x & 7);
}

void f2() {
  uint v[10] = {0,1,2,3,4,5,6,7,8,9};
  uint i = 0;
  while (i < 9){
    v[i] = i++;
    printf ("i++ = %d\n",i);
  }
  for (i=0;i<10;i++)
    printf ("v[%d] = %d\n",i,v[i]);
}

bool f3(uint n )
{
  return (n & (n-1)) == 0;
}

bool f4( uint n )
{
  return -n & 7;
}

int main(int argc, char **argv) {

  int   test = 4;
  ubool ub;
  
  switch (test) {
  case 1:
    for (int i=0;i < 16; i++) {
      ub = f1(i);
      printf("1: ub(%d) = 0x%08x\n",i,ub);
    }
    break;
  case 2:
    f2();
    break;
  case 3:
    for (int i=0; i < 17; i++) {
      uint j;
      j = f3(i);
      printf("%d: 0x%08x\n",i,j);
    }
    break;
  case 4:
    for (int i=-16; i < 17; i++) {
      uint j;
      j = f3(i);
      printf("%d: 0x%08x\n",i,j);
    }
    break;
  }
  
  return 1;
  
}