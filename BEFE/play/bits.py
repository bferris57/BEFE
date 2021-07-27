def bits(val):
  if not type(val) in [int,long]:
    return 0
  val = long(val)
  bits = 0;
  while val and val != -1:
    bits += 1
    val = val/2
  return bits
  
if __name__ == "__main__":

  nums = [0,1,2,3,15,16,1114111]
  for i in range(0,len(nums)):
    num = nums[i]
    print("%d: bits(0x%x) = %d"%(i,num,bits(num)))
 