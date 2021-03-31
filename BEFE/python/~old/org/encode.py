import random
from error import *

#
#  Bytes
# --------
#
# aaaaaaaa ───► 00aaaaaa 00aa????                        Output 1: in[0]>>2
#                                                               2: (in[0]&0x03)<<4 + rand(0..15)
#
# aaaaaaaa ─┬─► 00aaaaaa 00aabbbb 00bbbb??               Output 1: in[0]>>2
# bbbbbbbb ─┘                                                   2: (in[0]&0x03)<<4 + (in[1]>>4)
#                                                               3: (in[1]&0x0f)<<2 + rand(0..3)
#
# aaaaaaaa ─┬─► 00aaaaaa 00aabbbb 00bbbbcc 00cccccc      Output 1: in[0]>>2
# bbbbbbbb  |                                                   2: (in[0]&0x03)<<4 + (in[1]>>4)
# cccccccc ─┘                                                   3: (in[1]&0x0f)<<2 + in[2]>>6
#                                                               4: (in[2]&0x3f)
#
# The resoluting output values (0..63) are looked up in the mask for a matching character.
#

default_mask = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ+-'
random_mask  = '4g23EXtdpAsi9KDVN8Ly+ZnP6rYe5GQaRjBzb1lUHuMJ-fOk0xS7qFWwhTCmIovc'


#-----------------------------------------------------------------------
#
# Function: IsValidMask - Is given mask valid to use for Encode/Decode?
#

def IsValidMask(mask):

  if type(mask) != str or len(mask) != 64:
    return False
  for char in mask:
    numoccur = mask.count(char)
    if numoccur != 1:
      return False
  return True

#-----------------------------------------------------------------------
#
# Function: Decode - Decode encoded string given optional 6-bit mask
#
# Usage:    output = Decode(input[,mask])
#
# Where:    input  - Input string of encoded bytes
#           mask   - 64 character mask to use to extract 6 bits from
#
#           output - Unencoded ('raw' bytes) string (characters 0..255)
#
# Each input character is looked up in the mask and produces a six bit
# value as described here...
#
#     Input bytes                                Output bytes
#     -----------------------------------        ------------
#
#     00aaaaaa                             ───►  ** Invalid **
#     00aaaaaa 00aa????                    ───►  aaaaaaaa
#     00aaaaaa 00aabbbb 00bb????           ───►  aaaaaaaa bbbbbbbb
#     00aaaaaa 00aabbbb 00bbbbcc 00cccccc  ───►  aaaaaaaa bbbbbbbb cccccccc
#
# This means an input string of length N is invalid if floor(N/4) == 1.
#

def Decode(input,mask=None):

  if not mask:
    mask = default_mask
  else:
    if not IsValidMask(mask):
      raise Error("Malformed encoding mask")

  if type(input) != str:
    raise Error("Can only decode strings, was given",repr(type(input)))

  if len(input) % 4 == 1:
    raise Error("Malformed encoded string")

  i = 0
  output = []
  while True:

    # Get the next fragment
    frag = input[i:(i+4)]
    length = len(frag)
    chars = list(frag)
    values = [0]*4
    for j in range(0,length):
      char = chars[j]
      if char not in mask:
        raise Error("Invalid character '%s' in encrypted string"%char)
      values[j] = mask.find(char)

    # Handle 4 byte fragment
    #
    # aaaaaaaa ---> 00aaaaaa 00aabbbb 00bbbbcc 00cccccc      Output 1: in[0]>>2
    # bbbbbbbb  |                                                   2: (in[0]&0x03)<<4 + (in[1]>>4)
    # cccccccc -+                                                   3: (in[1]&0x0f)<<2 + in[2]>>6

    if length == 4:
      o1 =  (values[0]<<2)         | (values[1]>>4)
      o2 = ((values[1]<<4) & 0xf0) | (values[2]>>2)
      o3 = ((values[2]&0x03)<<6)   | (values[3])
      output.append(chr(o1))
      output.append(chr(o2))
      output.append(chr(o3))
      i = i + 4
      continue

    # Handle 3 byte fragment
    #
    # aaaaaaaa <--> 00aaaaaa 00aabbbb 00bbbb??               Output 1: in[0]>>2
    # bbbbbbbb -+                                                   2: (in[0]&0x03)<<4 + (in[1]>>4)
    #                                                               3: (in[1]&0x0f)<<2 + rand(0..3)
    elif length == 3:
      o1 =  (values[0]<<2)         | (values[1]>>4)
      o2 = ((values[1]<<4) & 0xf0) | (values[2]>>2)
      output.append(chr(o1))
      output.append(chr(o2))
      i = i + 3
      continue

    # Handle 2 byte fragment
    #
    # aaaaaaaa <--> 00aaaaaa 00aa????                        Output 1: in[0]>>2

    elif length == 2:
      o1 = (values[0]<<2)      | (values[1]>>4)
      output.append(chr(o1))
      i = i + 2
      continue
    # Handle 1 byte fragment
    elif length == 1:
      raise Error("Malformed encrypted string, expected more than 1 byte in fragment")
    # Handle 0 byte fragment
    else:
      break

  return ''.join(output)

#-----------------------------------------------------------------------
#
# Function: Encode - Encode unencoded ('raw' bytes) string given
#                    optional 6-bit mask
#
# Usage:    output = Encode(input[,mask])
#
# Where:    input  - Input string of unencoded ('raw') bytes (0..255)
#           mask   - 64 character mask to use to extract 6 bits from
#
#           output - Encoded  string (characters in mask)
#
# Each input character is looked up in the mask and produces a six bit
# value as described here...
#
#   Input bytes                     Output bytes
#   --------------------------      ------------
#
#   aaaaaaaa                   ───► 00aaaaaa 00aa????
#   aaaaaaaa bbbbbbbb          ───► 00aaaaaa 00aabbbb 00bb????
#   aaaaaaaa bbbbbbbb cccccccc ───► 00aaaaaa 00aabbbb 00bbbbcc 00cccccc
#
# This means an input string of length N > 0 'raw' bytes produces a string
# of N + N%3 + 1 bytes and and input string of length 0 produces zero bytes...
#
#   Len
#   ---
#
#   0   ─►  0
#   1   ─►  2
#   2   ─►  3
#   3   ─►  4
#   4   ─►  6
#   5   ─►  7
#   6   ─►  8
#   7   ─►  10
#     ...
#
# This also means encoded strings of length N, where N > 0 and N%3 == 1,
# are invalid (e.g., 1, 5, 9).
#

def Encode(input,mask=None):

  if not mask:
    mask = default_mask
  else:
    if not IsValidMask(mask):
      raise Error("Malformed encoding mask")

  if type(input) != str:
    raise Error("Can only encode strings, was given",repr(type(input)))


  # Turn input into single character strings
  l = list(input)
  i = 0
  output = []

  # Handle fragments of 3, 2, and 1 bytes
  while True:
    rembytes = len(l) - i
    fragbytes = min(3,rembytes)
    if   fragbytes == 0:
      break
    elif fragbytes == 1:
      i0 = ord(input[i])
      i = i + 1
      b0 = i0>>2
      output.append(mask[b0])
      b1 = ((i0&0x03)<<4) + random.randint(0,15)
      output.append(mask[b1])
      continue
    elif fragbytes == 2:
      i0 = ord(input[i])
      i1 = ord(input[i+1])
      i = i + 2
      b0 = i0>>2
      b1 = ((i0&0x03)<<4) + (i1>>4)
      b2 = ((i1&0x0f)<<2) + random.randint(0,3)
      output.append(mask[b0])
      output.append(mask[b1])
      output.append(mask[b2])
      continue
    elif fragbytes == 3:
      i0 = ord(input[i])
      i1 = ord(input[i+1])
      i2 = ord(input[i+2])
      i = i + 3
      b0 = i0>>2
      b1 = ((i0&0x03)<<4) + (i1>>4)
      b2 = ((i1&0x0f)<<2) + (i2>>6)
      b3 =  (i2&0x3f)
      output.append(mask[b0])
      output.append(mask[b1])
      output.append(mask[b2])
      output.append(mask[b3])
      continue
    else:
      raise Error("***Duh! Not sure how we got here!!!!!")
  output = ''.join(output)
  return output

if __name__ == '__main__':

  print("")
  print("-----------------------------------------")
  print("encode.py - Test string encoding/decoding")
  print("-----------------------------------------")
  print("")
  print("Press CTRL-C to exit")

  while True:
    print("")
    i = input("Enter string: ")
    print ("")
    print ("        Input  = %s"%repr(i))
    e = Encode(i)
    print ("        Encode = ",e)
    d = Decode(e)
    print ("        Decode = %s"%repr(d))
