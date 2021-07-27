#!/usr/bin/python
#coding=utf8

if __name__ == "__main__":

  lines = open('t.txt').readlines()
  print("%d lines to start with..."%len(lines))      
  lineNo = -1
  while lineNo < len(lines)-1:
    lineNo += 1
    line = lines[lineNo]
    if line[0:2] != '0x':
      del lines[lineNo]
      lineNo -= 1
  print("%d lines remaining"%len(lines))
  
  print("Processing...")
  allocs = {}
  frees  = []
  for line in lines:
  
    counter,name,address,size = line.split(' ')
    if name == 'MemoryAllocRaw:':
      allocs[counter] = line
    elif name == 'MemoryFreeRaw:':
      if counter not in allocs:
        frees.append(line)
      else:
        del allocs[counter]
  
  numOut = 0
  for key in allocs:
    numOut += 1
    print allocs[key]
  for line in frees:
    numOut += 1
    print line
        
  if numOut == 0:
    print "Everything allocated was freed!!!"  
    
  