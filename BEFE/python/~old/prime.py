import math

def isPrime(n):
  if n == 2:
    return True
  if n < 2 or n%2 == 0 or int(n) != n: 
    return False
  if n % 2 == 0: return False
  return all(n % i for i in range(3, int(math.sqrt(n)) + 1, 2))
  
if __name__ == "__main__":
  for i in range(1,51):
    if isPrime(i):
      print "PRIME"
    else:
      print i

      General Equivalency Diploma