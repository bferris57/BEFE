//----------------------------------------------------------------------
// File: Intfuncs.cpp - Implementation of Primitive Int datatype functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Note: Some of these twiddles were taken from the following website
//       and adjusted for our purposes...
//
//          http://graphics.stanford.edu/~seander/bithacks.html
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Integer.h"
#include "Platform.h"
#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Function: ShortReverseBytes - Reverse bytes in a UShort value
//
// Usage:    rev = ShortReverseBytes(l)
//
// Where:    UShort l   - In:  Value with bytes
//
// Returns:  UShort rev - Out: Value with bytes reversed
//
// Notes:    Same as IntReverseBytes (see below) except on a UShort
//

UShort ShortReverseBytes(UShort theShort) {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

  return (theShort >> 8) | (theShort << 8);
  
#else

  return theShort;
  
#endif
  
}
  
//----------------------------------------------------------------------
//
// Function: IntReverseBytes - Reverse bytes in a UInt value
//
// Usage:    rev = IntReverseBytes(i)
//
// Where:    UInt i   - In:  Value with bytes
//
// Returns:  UInt rev - Out: Value with bytes reversed
//
// This function is typically used in place of the winsock.h functions
// ntohl() and htonl().
//
// As such, it should only be called on platforms where the processor
// used LSB byte order.
//
// NOTE: In GNU's G++, this can be checked using the standard GNU
//       preprocessor macros __BYTE_ORDER__ and __ORDER_LITTLE_ENDIAN__
//       as follows...
//
//         ...
//
//         // Swap to Network Byte Order
//         #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//         i = IntReverseBytes(i);
//         #endif
//
//         ...
//
//         // Swap back to Processor Byte Order
//         #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
//         i = IntReverseBytes(i);
//         #endif
//
//         ...
//
// NOTE: As you can see, we've actually done that below, so if you
//       STILL want to reverse the bytes in some odd case where you're
//       not running on a little-endian processor, WRITE IT YOURSELF!
//
// NOTE: We haven't yet decided what to do about PDP byte ordering or
//       even if we're going to be running on such a platform.
//
//       So, DON'T WORRY ABOUT IT!!!
//

UInt IntReverseBytes(UInt i) {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

  union {
    UInt32 theInt;
    Byte   theBytes[4];
  } u;

  u.theInt = i;
  u.theBytes[0] ^= u.theBytes[3];
  u.theBytes[3] ^= u.theBytes[0];
  u.theBytes[0] ^= u.theBytes[3];
  u.theBytes[1] ^= u.theBytes[2];
  u.theBytes[2] ^= u.theBytes[1];
  u.theBytes[1] ^= u.theBytes[2];

  return u.theInt;
  
#else

  return i;
  
#endif
  
}
  
//----------------------------------------------------------------------
//
// Function: LongReverseBytes - Reverse bytes in a ULong value
//
// Usage:    rev = LongReverseBytes(l)
//
// Where:    ULong l   - In:  Value with bytes
//
// Returns:  ULong rev - Out: Value with bytes reversed
//
// Notes:    Same as IntReverseBytes (see above) except on a ULong
//

ULong LongReverseBytes(ULong theLong) {

#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__

  union {
    UInt64 theInt;
    Byte   theBytes[8];
  } u;

  u.theInt = theLong;
  u.theBytes[0] ^= u.theBytes[7];
  u.theBytes[7] ^= u.theBytes[0];
  u.theBytes[0] ^= u.theBytes[7];

  u.theBytes[1] ^= u.theBytes[6];
  u.theBytes[6] ^= u.theBytes[1];
  u.theBytes[1] ^= u.theBytes[6];

  u.theBytes[2] ^= u.theBytes[5];
  u.theBytes[5] ^= u.theBytes[2];
  u.theBytes[2] ^= u.theBytes[5];

  u.theBytes[3] ^= u.theBytes[4];
  u.theBytes[4] ^= u.theBytes[3];
  u.theBytes[3] ^= u.theBytes[4];

  return u.theInt;
  
#else

  return theLong;
  
#endif
  
}
  
//----------------------------------------------------------------------
//
// Function: IntSqrt - Calculate exact or next square root of integer
//
// Usage:    sqrt = IntSqrt(i)
//
// Where:    int i    - Number to return square root of
//
// Returns:  int sqrt - Sqrt(i) (rounded up to next integer)
//
// Notes: If a number < 1 is passed, the number itself is returned.
//
//        The best way to ensure it wasn't called wrong is to square
//        the result and see if you get the original number.
//

UInt IntSqrt(UInt i)
{
  Int nv;
  Int v;
  Int c;

  v = i >> 1;
  c = 0;
  if (v <= 0)
    return i;
  do {
    nv = (v + i/v)>>1;
    if (Abs(v - nv) <= 1)
      return nv;
    v = nv;
  } while (c++ < 25);
  return nv;
}

//----------------------------------------------------------------------
//
// Function: IntLog2 - Calculate Base 2 Logarithm
//
// Usage:    log = IntLog2(i)
//
// Where:    UInt i   - Number to calculate Log2 from
//
// Returns:  UInt low - Log10(i) (rounded up to next integer)
//
//

static const char LogTable256[256] = {
#define LT(n) n, n, n, n, n, n, n, n, n, n, n, n, n, n, n, n
    -1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
    LT(4), LT(5), LT(5), LT(6), LT(6), LT(6), LT(6),
    LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7), LT(7)
};


UInt IntLog2(UInt v) {
  
  UInt               r;  // r will be lg(v)
  BEFE_Register UInt t;  // Temporary
  BEFE_Register UInt tt; // Temporary
  
  if ((tt = v >> 16) != 0) {
    r = (t = tt >> 8) ? 24 + LogTable256[t] : 16 + LogTable256[tt];
  }
  else {
    r = (t = v >> 8) ? 8 + LogTable256[t] : LogTable256[v];
  }

  return r;

}

//----------------------------------------------------------------------
//
// Function: IntLog10 - Calculate Base 10 Logarithm
//
// Usage:    log = IntLog10(i)
//
// Where:    UInt i   - Number to calculate Log10 from
//
// Returns:  UInt low - Log10(i) (rounded up to next integer)
//
//

UInt IntLog10(UInt i) {
  
  Int log10;
  
  log10=0;
  while (i >= 10) {
    log10++;
    i /= 10;
  }
  
  return (UInt)log10;
  
}

//----------------------------------------------------------------------
//
// Function: IntIsPrime - Is prime number?
//
// Usage:    if (IntIsPrime(n))...
//
// Where:    int n    - Number to test
//
// Returns:  bool result - true  = "IS prime"
//                         false = "Is NOT prime"
//

Boolean IntIsPrime(UInt n)
{

  UInt iMax;
	UInt i;

	if (n < 2) return false;
	if (n < 4) return true;
	if (n % 2 == 0) return false;

	iMax = (Int)IntSqrt(n) + 1;
	for (i = 3; i <= iMax; i += 2)
		if (n % i == 0)
			return false;

	return true;
}

//----------------------------------------------------------------------
//
// Function: IntNextPrime - Return next prime number after given number
//
// Usage:    next = IntNextPrime(n)
//
// Where:    UInt n    - Number to start at
//
// Returns:  UInt next - If n is prime, n
//                       If n is not prime, next prime after n
//
// Notes: This code was cribbed from the following web page...
//
//          http://yves.gallot.pagesperso-orange.fr/src/aks.html
//
//        It is based on the AKS algorithm (please look it up).
//

UInt IntNextPrime(UInt n)
{
  if (n < 2)
    return 2;
  if (n%2 == 0)
    n = n+1;
  while (!IntIsPrime(n))
    n += 2;
  return n;
}

//----------------------------------------------------------------------
//
// Function: IntGCD - Return Greatest Common Divisor of two Integers
//
// Usage:    gcd = IntGCD(num1, num2)
//
// Where:    UInt num1 - An integer
//           UInt num2 - Another integer
//
// Returns:  UInt gcd  - Greatest common divisor
//

UInt IntGCD(UInt num1, UInt num2) {

  UInt sqrt1;
  //UInt sqrt2;
  UInt i;

  if (num1 == 0 || num2 == 0)       return 1;
  if (IsNull(num1) || IsNull(num2)) return UNaN;
  if (num1 == num2)                 return num1;
  if (num1%num2 == 0)               return num2;
  if (num2%num1 == 0)               return num1;

  sqrt1 = IntSqrt(num1);
  //sqrt2 = IntSqrt(num2);

  if (num1 < num2) {
    i = num1;
    num1 = num2;
    num2 = i;
  }

  // num1 <= num2
  for (i=1; i <= sqrt1; i++)
    if (num1%i == 0) return i;

  return 1;
}

//----------------------------------------------------------------------
//
// Function: IntLCM - Return Least Common Multiple of two Integers
//
// Usage:    lcm = IntLCM(num1, num2)
//
// Where:    UInt num1 - An integer
//           UInt num2 - Another integer
//
// Returns:  UInt lcm  - Least common multiple
//

UInt IntLCM(UInt num1, UInt num2) {

  if ( IsNull(num1) || IsNull(num2) )
    return UNaN;
  return num1*num2/IntGCD(num1,num2);

}

//----------------------------------------------------------------------
//
// Function: IntCrc32 - Calculate 32-bit CRC of given bytes
//
// Usage:    crc32 = IntCrc32(data,len)
//
// Where:    UByte *data - Some Bytes of data
//           UInt   len  - Number of data bytes
//
// Returns:  UInt32 crc32 - 32 bit Integer CRC
//
// Notes: If a number < 1 is passed, the number itself is returned.
//
//        The best way to ensure it wasn't called wrong is to square
//        the result and see if you get the original number.
//
// The following code was nicked from
//
//   http://www.codeproject.com/KB/recipes/crc32.aspx
//
// I think it's entirely copyright free.
//

// "Real" pointer declared below code
extern UInt32 *pcrc32_tab;

UInt32 IntCrc32(UInt8 *s, UInt len) {

  UInt   i;
  UInt32 crc32val;

  crc32val = 0;

  for (i = 0;  i < len;  i ++)
    //crc32val = pcrc32_tab[(crc32val ^ s[i]) & 0xff] ^ (crc32val >> 8);
	  crc32val = (crc32val >> 8) ^ pcrc32_tab[*s++ ^ ((crc32val) & 0xFF) ];

  return crc32val;

}

UInt32 IntCrc32(UInt32 seed, UInt32 c) {
  
  seed = (seed >> 8) ^ pcrc32_tab[((c >> 24) & 0xFF) ^ (seed & 0xFF)];
  seed = (seed >> 8) ^ pcrc32_tab[((c >> 16) & 0xFF) ^ (seed & 0xFF)];
  seed = (seed >> 8) ^ pcrc32_tab[((c >>  8) & 0xFF) ^ (seed & 0xFF)];
  seed = (seed >> 8) ^ pcrc32_tab[((c      ) & 0xFF) ^ (seed & 0xFF)];
  
  return seed;
  
}

static UInt32 crc32_tab[256] = {
	0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA,
	0x076DC419, 0x706AF48F, 0xE963A535, 0x9E6495A3,
	0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
	0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91,
	0x1DB71064, 0x6AB020F2, 0xF3B97148, 0x84BE41DE,
	0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
	0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC,
	0x14015C4F, 0x63066CD9, 0xFA0F3D63, 0x8D080DF5,
	0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
	0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B,
	0x35B5A8FA, 0x42B2986C, 0xDBBBC9D6, 0xACBCF940,
	0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
	0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116,
	0x21B4F4B5, 0x56B3C423, 0xCFBA9599, 0xB8BDA50F,
	0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
	0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D,

	0x76DC4190, 0x01DB7106, 0x98D220BC, 0xEFD5102A,
	0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
	0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818,
	0x7F6A0DBB, 0x086D3D2D, 0x91646C97, 0xE6635C01,
	0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
	0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457,
	0x65B0D9C6, 0x12B7E950, 0x8BBEB8EA, 0xFCB9887C,
	0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
	0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2,
	0x4ADFA541, 0x3DD895D7, 0xA4D1C46D, 0xD3D6F4FB,
	0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
	0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9,
	0x5005713C, 0x270241AA, 0xBE0B1010, 0xC90C2086,
	0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
	0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4,
	0x59B33D17, 0x2EB40D81, 0xB7BD5C3B, 0xC0BA6CAD,

	0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
	0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683,
	0xE3630B12, 0x94643B84, 0x0D6D6A3E, 0x7A6A5AA8,
	0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
	0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE,
	0xF762575D, 0x806567CB, 0x196C3671, 0x6E6B06E7,
	0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
	0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5,
	0xD6D6A3E8, 0xA1D1937E, 0x38D8C2C4, 0x4FDFF252,
	0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
	0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60,
	0xDF60EFC3, 0xA867DF55, 0x316E8EEF, 0x4669BE79,
	0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
	0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F,
	0xC5BA3BBE, 0xB2BD0B28, 0x2BB45A92, 0x5CB36A04,
	0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,

	0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A,
	0x9C0906A9, 0xEB0E363F, 0x72076785, 0x05005713,
	0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
	0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21,
	0x86D3D2D4, 0xF1D4E242, 0x68DDB3F8, 0x1FDA836E,
	0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
	0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C,
	0x8F659EFF, 0xF862AE69, 0x616BFFD3, 0x166CCF45,
	0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
	0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB,
	0xAED16A4A, 0xD9D65ADC, 0x40DF0B66, 0x37D83BF0,
	0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
	0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6,
	0xBAD03605, 0xCDD70693, 0x54DE5729, 0x23D967BF,
	0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
	0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D,
   };
UInt32 *pcrc32_tab = crc32_tab;

//----------------------------------------------------------------------
//
// Functions: BitRange     - Calculate bit ranges in integer
//            IsPowerOfTwo - "Is integer a power of two?"
//            RoundToPowerOfTwo - Round integer up to nearest power of 2
//
// Usage:    BitRange(someint,&hibit,&lobit);
//
//           if (IsPowerOfTwo(someint))...
//
//           nicepower = RoundToPowerOfTwo(someint)
//
// Where:    Int someint - Some arbitrairy integer
//           Int hibit   - Highest bit set (0..31)
//           Int lobit   - Lowest bit set (0..31)
//

void BitRange(UInt num, UInt &bit0, UInt &bit1) {

  Int tnum;
  Int tbit0;
  Int tbit1;

  if (num != 0) {
    // Figure out first bit...
    tbit0 = 0;
    for (tnum=num; tnum > 0; tnum<<=1)
      tbit0++;
    // Figure out last bit...
    tbit1 = 32;
    for (tnum=num; (tnum&1) == 0; tnum>>=1)
      tbit1--;
  }
  else {
    tbit0 = 0;
    tbit1 = 0;
  }
  bit0 = tbit0;
  bit1 = tbit1;
  return;
}

Boolean IsPowerOfTwo(UInt n) {

  UInt bit0;
  UInt bit1;

  BitRange(n,bit0,bit1);

  return (bit1-bit0) == 1;

}

UInt RoundToPowerOfTwo(UInt n) {

  UInt bit0;
  UInt bit1;

  BitRange(n,bit0,bit1);
  if (bit0 != bit1 && bit1-bit0 != 1 && bit0 != 0) {
    BitRange(n,bit0,bit1);
    n = 0x80000000 >> (bit0-1);
  }

  return n;

}

//--------------------------------------------------------------------
//
// Function: Hash - Compute hash of various data types and values
//

UInt  Hash(UInt i) {
  UInt hash;
  UInt j;

  hash = 0;
  for (j=0;j<sizeof(i);j++) {
    hash = IntCrc32(hash,i&0x0f);
    i >>= 8;
  }
  return hash;
}

/* !!!DEPRECATED???!!!
UInt Hash(UInt8 *addr) {

  UInt32 hash;
  UInt32 temp[2];

#if IS64BIT
  temp[0] = (UInt32)addr;
#else
   *(UInt64 *)&temp[0] = (UInt64)addr; 
   temp[0] ^= temp[1];
#endif

  hash = temp[0];
  for (j=0;j<sizeof(addr);j++) {
    hash = IntCrc32(hash,((UInt)addr)&0x0f);
    addr = (UInt8 *)(((UInt)addr)>>8);
  }
  return hash;
}
*/

UInt Hash(const char *s) {

  UInt        len;
  const char *ts;

  len = 0;
  if (!IsNull(s))
    for (ts=s; *ts++ != 0; len++);
  return IntCrc32((UInt8 *)s,len);
}

UInt Hash(const char *s, UInt len) {
  return IntCrc32((UInt8 *)s,len);
}

//--------------------------------------------------------------------
//
// Some static data...
//
// NOTE: _ValidateIntBits varlidates the table's contents and should
//      be called from time to time as a "Sanity check".
//
// NOTE: Same goes for _ValidateIntbitMax.
//

// Given a Byte, how many bits are in it?...
static char intBits[] = {  //  00 01 02 03 04 05 06 07 08 09 0a 0b 0c 0d 0e 0f
                                0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, // 00
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 10
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 20
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 30
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 40
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 50
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 60
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // 70
                                1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, // 80
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // 90
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // a0
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // b0
                                2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, // c0
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // d0
                                3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, // e0
                                4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8, // f0
                                -1
};

Boolean _ValidateIntBits() {

  UInt theUInt;
  UInt tempUInt;
  UInt ourAnswer;
  UInt otherAnswer;
  UInt numBad;

  numBad = 0;
  for (theUInt=0; theUInt <= 0xff; theUInt++) {
    ourAnswer = 0;
    tempUInt = theUInt;
    while (tempUInt) {
      if (tempUInt&1) ourAnswer++;
      tempUInt >>= 1;
    }
    otherAnswer = IntBitCount(theUInt);
    if (otherAnswer != ourAnswer) {
      Cout << "_ValidateIntBits: 0x" << ToHexString(theUInt,2)
           << "... expected " << (Int)ourAnswer << " got " << (Int)otherAnswer << '\n';
      numBad++;
    }
  }

  return numBad == 0;

}

// Given a number of Bits, what's the maximum number that can fit in it?...
static UInt intBitMax[] = {
   0x00000000     // 0 bits
  ,0x00000001     // 1 bit
  ,0x00000003     // 2 bits
  ,0x00000007     // 3 bits
  ,0x0000000f     // 4 bits
  ,0x0000001f     // 5 bits
  ,0x0000003f     // 6 bits
  ,0x0000007f     // 7 bits
  ,0x000000ff     // 8 bits
  ,0x000001ff     // 9 bits
  ,0x000003ff     // 10 bits
  ,0x000007ff     // 11 bits
  ,0x00000fff     // 12 bits
  ,0x00001fff     // 13 bits
  ,0x00003fff     // 14 bits
  ,0x00007fff     // 15 bits
  ,0x0000ffff     // 16 bits
  ,0x0001ffff     // 17 bits
  ,0x0003ffff     // 18 bits
  ,0x0007ffff     // 19 bits
  ,0x000fffff     // 20 bits
  ,0x001fffff     // 21 bits
  ,0x003fffff     // 22 bits
  ,0x007fffff     // 23 bits
  ,0x00ffffff     // 24 bits
  ,0x01ffffff     // 25 bits
  ,0x03ffffff     // 26 bits
  ,0x07ffffff     // 27 bits
  ,0x0fffffff     // 28 bits
  ,0x1fffffff     // 29 bits
  ,0x3fffffff     // 30 bits
  ,0x7fffffff     // 31 bits
  ,0xffffffff     // 32 bits
};

Boolean _ValidateIntBitMax() {

  UInt theUInt;
  UInt ourAnswer;
  UInt otherAnswer;
  UInt numBad;
  Int  i;

  numBad = 0;
  for (theUInt=0; theUInt <= 32; theUInt++) {
    ourAnswer = 0;
    for (i=theUInt; i > 0; i--) {
      ourAnswer = (ourAnswer<<1) | 1;
    }
    otherAnswer = IntBitMax(theUInt);
    if (otherAnswer != ourAnswer) {
      Cout << "_ValidateIntBitBax: " << theUInt
           << "... expected " << (Int)ourAnswer << " got " << (Int)otherAnswer << '\n';
      numBad++;
    }
  }

  return numBad == 0;

}

//--------------------------------------------------------------------
//
// Functions: IntBitCount   - Count number of bits set
//            IntBitMax     - Maximum unsigned number that will fit in N bits
//            IntBitScatter - Scatter an Integer across a mask
//            IntBitCollect - Gather a Scattered Integer
//
// The idea here is that it comes in handy in certain situations to
// store an Integer value spread across non-contiguous bits.
//
// This is better described with an example. Suppose you have a 5 bit
// number which we'll represent as follows...
//
//   âˆ™âˆ™âˆ™ â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”�
//       â”‚4â”‚3â”‚2â”‚1â”‚0â”‚
//   âˆ™âˆ™âˆ™ â”´â”€â”´â”€â”´â”€â”´â”€â”´â”€â”˜
//
//       Where: 0, 1, 2, 3, 4, are some aritrairy bit number assignments
//
// We will call that a <Collected Int>, meaning that all the bits are
// contiguous and together, like a "normal" integer we're all used to
// manipulating.
//
// Now, let's say you want it spread in a 32-bit word like this...
//
//         3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//         1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//        â”œâ”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¼â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¼â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¼â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¤
//        â”‚âˆ™ âˆ™ âˆ™ âˆ™ âˆ™â”‚4â”‚3â”‚âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™â”‚2â”‚1â”‚âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™â”‚0â”‚âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™ âˆ™â”‚
//        â”œâ”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¼â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¼â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¼â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¬â”€â”¤
//
//   Mask:â”‚0 0 0 0â”‚0 1 1 0â”‚0 0 0 0â”‚0 0 0 1â”‚1 0 0 0â”‚0 0 0 0â”‚1 0 0 0â”‚0 0 0 0â”‚
//
//    Hex:â”‚      0â”‚      6â”‚      0â”‚      1â”‚      8â”‚      0â”‚      8â”‚      0â”‚
//
//
// We'll call that a <Scattered Int>, meaning the bits in the Int are
// scattered to various bits throughout the 32-bit word.
//
// This is where these functions come in.  Let's say you have the mask
// MYMASK set to (or defined as) 0x06018080 (see above) and then you call
// these functions...
//
//   IntBitCount(MYMASK)              will return the value 5 (there's 5 bits set in the mask)
//   IntBitMax(MYMASK)                will return the value 31 (0x1F, all 5 bits set)
//
//   IntBitScatter(0x1F,MYMASK)       will return the value 0x06018080
//   IntBitScatter(0x0E,MYMASK)       will return the value 0x02018000
//
//   IntBitCollect(0x06018080,MYMASK) will return the value 0x1F (31)
//   IntBitCollect(0x02018000,MYMASK) will return the value 0x0E (14)
//
// It's that simple!!!
//

UInt IntBitCount(UInt theUInt) {

  union {
    UInt32 theUInt;
    UInt8  theBytes[4];
  } intBytes;
  UInt8 *someUInt8;
  UInt   ourAnswer;

  intBytes.theUInt = theUInt;
  someUInt8 = &intBytes.theBytes[0];

  ourAnswer  = intBits[*someUInt8++];
  ourAnswer += intBits[*someUInt8++];
  ourAnswer += intBits[*someUInt8++];
  ourAnswer += intBits[*someUInt8++];

  return ourAnswer;

}

UInt IntBitMax(UInt numBits) {

  if (numBits > 32)
    return UNaN;
  return intBitMax[numBits];

}

UInt IntBitScatter(UInt theCollectedInt, UInt theMask) {

  UInt oneBit;
  UInt theResult;

  theResult = 0;
  oneBit    = 1;

  while (theMask) {
    if (theMask&1) {
      if (theCollectedInt & 1)
        theResult |= oneBit;
      theCollectedInt >>= 1;
    }
    theMask >>= 1;
    oneBit  <<= 1;
  }

  return theResult;

}

UInt IntBitCollect(UInt theScatteredInt, UInt theMask) {

  UInt theResult;

  theResult = 0;

  while (theMask) {
    if (theMask&1) {
      theResult <<= 1;
      theResult |= (theScatteredInt & 1);
    }
    theMask         >>= 1;
    theScatteredInt >>= 1;
  }

  return theResult;

}

//--------------------------------------------------------------------
//
// Function: UInt32FromString - Workhorse for String -> UInt32
//
// Usage:    status = UInt32Fromstring(str,slice,base,theUInt)
//
// Where:    uint   - UInt   Out: Result (NaN = "Invalid character sequence or base")
//           str    - String In:  Input string containing characters to convert
//           base   - Int    In:  Conversion base (2..16)
//           theInt - UInt  &Out: The resulting UInt
//           status - Status Out: Error (Error::None if no error)
//
// This function is repsonsible for converting a non-zero length
// sequence of characters (as defined by the str and base parameters)
// from character to 32 bit internal binary representation.
//
// The valid input characters are '0'..'9' and 'a'..'f' (or 'A'..'F').
//
// Characters not in this range will cause NaN to be returned.  This includes
// any sign ('-' or '+') and space.
//
// These characters map, unambiguously to the representational number 0..15.
//
// These representational numbers are combined, in the order defined by the
// sequence of the slice, with the most significant digit appearing first
// in the sequence.
//
// This means the caller can use a negative stepping in the slice if the
// characters are represented in the string with least significan digit
// appearing first.
//
// If the representational number is not less than the base, NaN will be
// returned (eg, 'A' will return NaN when bases 0..10 are used).
//
// Note: Empty strings and invalid bases also cause NaN to be returned.
//
// Note: If the sequence of digits represents a number two big to fit in
//       32 bits, NaN will be returned.
//

Status UInt32FromString(Byte *buf, UInt bufl, Slice slice, UInt base, UInt32 &theUInt) { // UInt32FromString...

  Status  status;
  UInt32  work1;
  UInt32  work2;
  Slice   wslice;
  Int     wlen;
  Int     wpos;
  Char    c;
  UInt32  i;

  // Initialise work variables
  work1  = 0;

  // Validate parameters
  if (IsNull(buf) || bufl <= 0) goto INVALID;

  // Calculate working slice
  wlen = bufl;
  wslice = slice.Resolve(wlen);
  wlen   = wslice.Length();
  wpos   = wslice.idx1;
  if (wlen <= 0) goto INVALID;

  // Weed out bad cases
  if (base < 2 || base > 16 || wlen <= 0) goto INVALID;

  //
  // Process the digits...
  //

  do {

    // Check for terminating condition...
    if (wslice.step < 0) {
      if (wpos <= wslice.idx2) break;
    }
    else {
      if (wpos >= wslice.idx2) break;
    }

    // Handle the character
    c = (Char)buf[wpos];
    if      (c >= '0' && c <= '9')
      i = (Int)c - '0';
    else if (c >= 'a' && c <= 'f')
      i = (Int)c  - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
      i = (Int)c - 'A' + 10;
    else
      goto INVALID;
    if (i >= base) goto INVALID;
    work2 = work1 * base;
    if (work1 != 0 && work2 < base) goto INVALID;
    work1 = work2 + i;
    if (work1 < work2) goto INVALID;

    // Position to next character
    wpos += wslice.step;

  } while (true);

  theUInt = work1;

  // Handle errors
  status = Error::None;
  while (false) {
    INVALID: status = Error::InvalidInteger; break;
  }
  if (status)
    theUInt = UNaN;
    
  return status;

} // ...UInt32FromString

UInt32 UInt32FromString(Byte *buf, UInt bufl, Slice slice, UInt base) {
  UInt32 result;
  UInt32FromString(buf, bufl, slice, base, result);
  return result;
}

Status UInt32FromString(String const &str, Slice slice, UInt base, UInt32 &theInt) {

  Status  status;
  Byte   *buf;
  UInt    bufl;

  str._BufAndSize(buf, bufl);
  status = UInt32FromString(buf, bufl, slice, base, theInt);  
  
  return status;
  
}

//--------------------------------------------------------------------
//
// Function: Int32FromString - Workhorse for String -> Int conversion
//
// Usage:    status = Int32Fromstring(str,slice,theInt)
//
// Where:    str    - String In:  Input string containing characters to convert
//           slice  - Slice  In:  Slice declaring characters to get from the string
//           theInt - Int &  Out: Result (NaN = "Invalid, or can't")
//
//           status - Status Out: Error code (Error::None if no error)
//
//
// This function is repsonsible for converting a string to a native
// Int data type.
//
// Unlike UInt32FromString, it's a bit more generous in its syntax.
//
// It handled numbers in the following basic format...
//
//   [<sign>][<base>][<digit>...]
//
// If <sign> ('-') is present, one bit is removed from the number
// of bits in an Int, the top bit is set, and the remaining bits will
// be set to the two's complement of the number represented by the
// sequence of characters.
//
//   Note: We intentionally don't allow '+' because that gets in the
//         way.  So, if that's what you want to allow you'll have to
//         check it for yourself.
//
// If <base> is present it must be either '0x' or '0b' representing
// hexidecimal and binary respectively.
//
// If <base> is not present, base ten is used.
//
// If <sign> is not present, the resulting Int value of the characters
// in the result is only limited by the number of bis in the
// Int data type (typically 32).
//
// If <sign> is present, there's one less bit (gotta store it somewhere!)
//
// Leading, trailing, or embedded whitespace (' ', '\n', '\r', '\t')
// will return NaN.  This is because this function is here to have a
// single place for this syntax of integer literals, so it's not up to
// it to sort out, or live by, the context the string appears in.
// So, we take the approach that whatever the spaces may, or may not,
// mean is up to the caller, we don't want them, and it's not our
// bloody business.
//
// Any overflow, bad character (based on the base), or any other
// problem encountered during the processing, NaN will be returned.
//
// Note: With that definition above the following case arises...
//
//         '0x80000000' == NaN
//
//       That's what commonly represents NaN and is obviously allowed.
//       That's the whole reason we chose that number for NaN, because we're
//       sick bloody tired of having one more negative number than positive
//       ones while, at the same time, C/C++ let's floating point numbers
//       be NaN and we don't think it's fair on integers!
//
//       So, if you're not doing signs (like addresses, etc.) use the
//       UInt32FromString function instead.  And, if you're dead set on using
//       signed numbers AND this function, LEARN TO LIVE WITH IT!
//
// Note: This function DOES NOT, by the definition above, cater for
//       any Unicode characters.  So, it works blindly on a single ASCII
//       octet.
//

Status Int32FromString(Byte *buf, UInt bufl, Slice slice, Int32 &theInt) { // Int32FromString...

  Status  status;
  UInt    base;
  UInt32  work1;
  UInt32  work2;
  Slice   wslice;
  UInt    wlen;
  UInt    wpos;
  Char    c;
  UInt    i;
  Boolean sign;

  // Validate parameters
  if (IsNull(buf) || bufl <= 0) goto INVALID;

  // Initialise work variables
  work1  = 0;
  sign   = false;
  base   = 10;

  // Calculate working slice
  wlen = bufl;
  wslice = slice.Resolve(wlen);
  wlen   = wslice.Length();
  wpos   = wslice.idx1;
  if (wlen <= 0) goto INVALID;

  // Weed out bad cases
  if (wlen <= 0) goto INVALID;

  // If first one '-'...
  c = (Char)buf[wpos];
  if ( IsNull(c) ) goto INVALID;
  if (c == '-') {
    sign = true;
    wpos += wslice.step;
    // Can't be here on its own
    if (--wlen <= 0) goto INVALID;
    c = (Char)buf[wpos];
  }

  // If first one '0', check for '0x' and '0b'...
  if (c == '0' && wlen > 2) {
    c = (Char)buf[wpos+wslice.step];
    if ( IsNull(c) ) goto INVALID;
    if (c == 'x' || c == 'b') {
      base = (c == 'x')?16:2;
      wpos += wslice.step*2;
      wlen -= 2;
    }
  }

  //
  // Process the digits...
  //

  do {

    // Check for terminating condition...
    if (wslice.step < 0) {
      if (wpos <= (UInt)wslice.idx2) break;
    }
    else {
      if (wpos >= (UInt)wslice.idx2) break;
    }

    // Handle the character
    c = (Char)buf[wpos];
    if      (c >= '0' && c <= '9')
      i = (Int)c - '0';
    else if (c >= 'a' && c <= 'f')
      i = (Int)c  - 'a' + 10;
    else if (c >= 'A' && c <= 'F')
      i = (Int)c - 'A' + 10;
    else
      goto INVALID;
    if (i >= base) goto INVALID;
    work2 = work1 * base;
    if (work1 != 0 && work2 < base) goto INVALID;
    work1 = work2 + i;
    if (work1 < work2) goto INVALID;

    // Position to next character
    wpos += wslice.step;

  } while (true);

  // Need to do sign checking and validation code here
  if (sign) {
    work1 = -work1;
    if ((Int)work1 > 0) goto INVALID;
  }

  theInt = work1;

  // Handle errors
  status = Error::None;
  while (false) {
    INVALID: status = Error::InvalidInteger; break;
  }
  if (status)
    theInt = NaN;
    
  return status;

} // ...Int32FromString

Int32 Int32FromString(Byte *buf, UInt bufl, Slice slice) {
  Int32 result;
  Int32FromString(buf, bufl, slice, result);
  return result;
}

Status Int32FromString(String const &str, Slice slice, Int32 &theInt) {

  Status  status;
  Byte   *buf;
  UInt    bufl;

  str._BufAndSize(buf, bufl);
  status = Int32FromString(buf, bufl, slice, theInt);  
  
  return status;
  
}

Status Int32FromString(String const &str, Int32 &theInt) {
  
  Status  status;
  Byte   *buf;
  UInt    bufl;

  str._BufAndSize(buf, bufl);
  status = Int32FromString(buf, bufl, Slice(), theInt);  
  
  return status;
  
}

//--------------------------------------------------------------------
//
// Function: UInt32FromString - Convenience String -> UInt32 conversion
//
// Usage:    *** See Integer.h ***
//
// These functions are just tiny, but handy, wrappers around their
// "bigger brother" UInt32FromString above.
//

//UInt32  UInt32FromString(Byte *buf,Int bufl) {}
//UInt32  UInt32FromString(String &str) {}
//UInt32  UInt32fromString(String &str, Int base) {}
//UInt32  UInt32FromString(String &str, Slice slice) {}

UInt32  UInt32FromString(String const &str, Slice slice, UInt base) {

  UInt32 result;
  Byte   buf[10];
  UInt   bufl;

  str.ToBytes(buf,sizeof(buf),bufl);
  result = UInt32FromString(buf,bufl,slice,base);

  return result;

}

//UInt32  UInt32FromString(const char *buf) {}
//UInt32  UInt32FromString(const char *buf, UInt base) {}
//UInt32  UInt32FromString(const char *buf, Slice slice) {}
//UInt32  UInt32FromString(const char *buf, Slice slice, UInt base) {}
//UInt32  UInt32FromString(char *buf) {}
//UInt32  UInt32FromString(char *buf, UInt base) {}
//UInt32  UInt32FromString(char *buf, Slice slice) {}
//UInt32  UInt32FromString(char *buf, Slice slice, UInt base) {}


//--------------------------------------------------------------------
//
// Function: UInt32FromString - Convenience String -> UInt32 conversion
//
// Usage:    *** See Integer.h ***
//
// These functions are just tiny, but handy, wrappers around their
// "bigger brother" UInt32FromString above.
//

Int Int32FromString(Byte *buf, UInt bufl) {

  Int   result;
  Slice slice;

  result = Int32FromString(buf,bufl,slice);

  return result;

}

Int Int32FromString(String const &str) {

  Int   result;
  Byte  buf[10];
  UInt  bufl;
  Slice slice;

  str.ToBytes(buf,sizeof(buf),bufl);
  result = Int32FromString(buf,bufl,slice);

  return result;

}

//Int IntfromString(String &str, Int base) {}

Int Int32FromString(String const &str, Slice slice) {

  Int  result;
  Byte buf[10];
  UInt bufl;

  str.ToBytes(buf,sizeof(buf),bufl);
  result = Int32FromString(buf,bufl,slice);

  return result;

}

//Int Int32FromString(String &str, Slice slice, UInt base) {}
//Int Int32FromString(const char *buf) {}
//Int Int32FromString(const char *buf, UInt base) {}
//Int Int32FromString(const char *buf, Slice slice) {}
//Int Int32FromString(const char *buf, Slice slice, UInt base) {}
//Int Int32FromString(char *buf) {}
//Int Int32FromString(char *buf, UInt base) {}
//Int Int32FromString(char *buf, Slice slice) {}
//Int Int32FromString(char *buf, Slice slice, UInt base) {}


} // ...Namespace BEFE
