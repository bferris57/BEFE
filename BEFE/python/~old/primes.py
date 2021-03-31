import math

def is_prime(number):
  if type(number) not in (int,long) or number < 2: return False
  if number == 2:
    return True
  if number % 2 == 0:
    return False
  for current in range(3, int(math.sqrt(number) + 1), 2):
    if number % current == 0: 
      return False
  return True

def is_prime2(n):

  if n == 2:
    return True
  if n < 2 or n%2 == 0 or int(n) != n: 
    return False
  return all(n % i for i in range(3, int(math.sqrt(n)) + 1, 2))
  
def primes(start=2,func=is_prime2):
  if type(start) not in (int, long): raise StopIteration
  if start < 2:
    start = 2
  if start == 2:
    yield 2
    yield 3
    start = 5
  elif start%2 == 0:
    start += 1
  while True:
    if func(start): 
      yield start
    start += 2
    
if __name__ == '__main__':
  max = 10
  cnt = 0
  for i in primes(65535,is_prime2):
    print(i)
    cnt += 1
    if cnt >= max: break
