#
# File: fixprint.py - Troll through all the .py files in current directory and change
#                     all 'print(' to 'print(' and append a ')')
#

import os

if __name__ == '__main__':

  files = os.listdir('.')
  for file in files:
    #if file != 'BNF.py': continue
    parts = os.path.splitext(file)
    if parts[1] != '.py': continue
    lines = open(file).readlines()
    prints = 0
    for lineno in range(0,len(lines)):
      line = lines[lineno]
      pos = line.find('print('))
      if pos < 0:
        continue
      prints += 1
      if line[-1] == "\n":
        line = line[:len(line)-1]
      lines[lineno] = line[0:pos]+"print("+line[pos+6:]+")\n"
    print(file+': %d lines, %d prints'%(len(lines),prints))
    if prints:
      open(file,'w+').writelines(lines)