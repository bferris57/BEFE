#include <stdio.h>
#include <stdint.h>

int32_t func1(int32_t x) {
  return ( x < 0 ) + ( ( x >> 31 ) ^ x );
}

#define MACRO(x)      ( sizeof(x) / sizeof( *(x) ) )

int32_t func3( uint32_t x ) {
  int32_t y;
  for ( y=0; x; y = !y )
    x ^= x & -x;
  return y;
}

int32_t func3_1( uint32_t x ) {
  x ^= x >> 16;
  x ^= x >> 8;
  x ^= x >> 4;
  x ^= x >> 2;
  x ^= x >> 1;
  return x & 1;
}

int main(int argc, char **argv) {
  
  struct X {
    int a;
    int b;
  };
  struct X  x;
  struct X *xp;
  char      c;
  char     *cp;
  int32_t   i;
  int32_t  *ip;
  
  printf("Start test...\n\n");  
  
  /* Question 1... abs(int32_t)... */
  printf("Question 1: abs(uint32_t x)...\n");
  for (i = -16; i <= 16; i++) 
    printf("  i = %d, func1(i) = %d\n",i,func1(i));
  
  /* Question 2... WTF?... */
  printf("Question 2: \"How many x's will fit into a pointer to an x?\"...\n");
  xp = &x;
  cp = &c;
  ip = 0;
  printf("  func(xp)  = %d\n",MACRO(xp));
  printf("  func(&xp) = %d\n",MACRO(&xp));
  printf("  func(cp)  = %d\n",MACRO(cp));
  printf("  func(&cp) = %d\n",MACRO(&cp));
  printf("  func(ip)  = %d\n",MACRO(ip));
  printf("  func(&ip) = %d\n",MACRO(&ip));
  
  /* Question 3... "Are odd number of bits set in int32_t value? (i.e., compute 'even parity' bit)"... */
  printf("Question 3: Calculate even parity, or answers \"Is parity odd?\"...\n");
  for (i=-16; i <= 15; i++)
    printf("  func2:   i = %3d (0x%08x), func3(i) = %d, func3_1(i) = %d\n",i,i,func3(i),func3_1(i));
  
  return 0;
}