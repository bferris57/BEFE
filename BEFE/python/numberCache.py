#
# numberCache.py - Calculate range of Python's "int cache"
#
# Note: This code presumes there's a minimum and maximum int
#       that's cached. Assuming so, seedMin and seedMax should
#       be set to numbers outside this range, if it exists...
#

seedMin = -1100
seedMax = 1000

def produceInt(val):

  if val < seedMin-1:
    ret = val-1
    ret += 1
    return ret
  ret = seedMin
  while ret < val: ret += 1
  return ret
  
def displayRange(range):

  if range[0] is not None or range[1] is not None:
    print("Inclusive range: %s..%s"%(repr(range[0]),repr(range[1])))
  
if __name__ == "__main__":

  cached = []
  val = seedMin
  while val <= seedMax:
    newVal = produceInt(val)
    if val is newVal and newVal not in (seedMin,seedMax):
      cached.append(newVal)
    val += 1
  
  # Have got cached ones in 'cached' and they're in sorted order.
  # Now, walk through them displaying list of contiguous cached
  # number ranges (suspect there's only a single range, e.g. 'x..y')
  idx = -1
  range = [None,None]
  while idx < len(cached)-1:
    idx += 1
    val = cached[idx]
    if range[0] == None:
      range[0] = val
    elif range[1] == None or val == range[1] + 1:
      range[1] = val
    else:
      displayRange(range)
      range = [val,None]
  displayRange(range)