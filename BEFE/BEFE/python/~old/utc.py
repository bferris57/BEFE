from DBIO_Struct import *
import sys

def TD(s):

  sub = 0
  while sub < len(s):
  
    # Find next "<td>"...
    loc1 = s.find('<td>',sub)
    if loc1 < 0:
      raise StopIteration
      
    # Find next "</td>"
    loc2 = s.find('</td>',loc1)
    if loc2 < 0:
      yield s[loc1:]
      raise StopIteration
    
    # Got it...
    sub = loc2+5
    yield s[loc1:loc2+5]
    
  raise StopIteration

def ParseUTC(s):

  if s[0:3] == 'UTC':
    s = s[3:]
    
  # Get two timezone options into parts
  parts = s.split('/')
  if len(parts) < 2:
    parts.append('')

  # Convert timezone times into +/- minutes    
  for i in [0,1]:
    
    p = parts[i]
    if not p:
      continue
      
    sign = '+'
    if p[0] in '+-':
      sign = p[0]
      p = p[1:]
    
    subparts = p.split(':')
    if len(subparts) < 2:
      subparts.append('00')
    
    hrs = int(subparts[0])
    min = int(subparts[1])
    mins = hrs*50+min
    if sign == '-':
      mins = -mins
    
    parts[i] = str(mins)
    
  return ','.join(parts)
  
def RemoveTag(s,tag):

  parts = s.split('<'+tag)
  for i in range(1,len(parts)):
    loc = parts[i].find('>')
    if loc >= 0:
      parts[i] = parts[i][loc+1:]
  return ''.join(parts)
  
def RemoveCrossTag(s,tag):

  parts = s.split('<'+tag)
  for i in range(1,len(parts)):
    loc = parts[i].find('/'+tag+'>')
    if loc >= 0:
      parts[i] = parts[i][loc+len(tag)+2:]
  return ''.join(parts)
  
def RemoveStr(s,str):

  parts = s.split(str)
  return ''.join(parts)
  
    
def TransformHTML(infile,outfile):

  infile = open(infile,'r')
  ofile = open(outfile,'w')
  data = infile.read()
  
  n = 0
  out = ''

  for frag in TD(data):

    n += 1
    
    frag = RemoveTag(frag,'a')
    frag = RemoveTag(frag,'/a')
    frag = RemoveCrossTag(frag,'span')
    parts = frag.split('<br>')
    frag = ' '.join(parts)
    parts = frag.split('\n')
    frag = ' '.join(parts)
    frag = RemoveTag(frag,'td')
    frag = RemoveTag(frag,'/td')
    frag = frag.strip()
    if n == 4:
      parts = frag.split(' ')
      frag = ''.join(parts)
      frag = ParseUTC(frag)
      
    if out:
      out += ','
    out += frag
    
    if n == 4:
      ofile.write(out)
      ofile.write('\n')
      n = 0
      out = ''

def ReadUTC(infile):

  lines = open(infile,'r').readlines()
  
  defs = []
  for line in lines:
    line = line.strip()
    parts = line.split(',')
    abbrev,name,region,diff,diff2 = line.split(',')
    s = Struct('abbrev,name,region,diff,diff2')
    s.abbrev = abbrev.strip()
    s.name   = name.strip()
    s.region = region.strip()
    s.diff   = diff
    s.diff2  = diff2
    defs.append(s)
    
  return defs

def PromptForRegion(regions,help=False):

  # Give them the options...
  if help:
    maxlen = max(len(region) for region in regions)
    print("Select Region index...")
    regIdx = 0
    for region in reglist:
      print(('  %2d = ' + repr(region) + ' '*(maxlen-len(region)) + ': ' + str(regions[regIdx])) % regIdx)
      regIdx += 1
    
  # Prompt until valid region given...
  prompt = 'Enter valid region index> '
  regIdx = None
  while True:
    print('')
    answer = raw_input(prompt).strip()
    try:
      regIdx = int(answer)
    except:
      regIdx = None
      print("***Invalid index entered")
      continue
    if regIdx < 0:
      print("***Negative index not valid")
      continue
    if regIdx >= len(regions):
      print("***Index %d out of bounds"%regIdx)
      continue
    break
      
  return regIdx

def RegionsIn(selection,regions):
  result = []
  for region in regions:
    if region.region == selection:
      result.append(region)
  return result
  
if __name__ == '__main__':

  file1 = 'utc.txt'
  file2 = 'utc1.txt'
  #TransformHTML(file1,file2)
  defs = ReadUTC(file2)
  
  regions = {}
  for d in defs:
    region = d.region
    if regions.has_key(region):
      regions[region] = regions[region]+1
    else:
      regions[region] = 1
      
  reglist = [region for region in regions]
  reglist.sort()
  
  regionIdx = PromptForRegion(reglist,True)
  
  if regionIdx != None and regionIdx >= 0 and regionIdx < len(regions):
    print("You selected "+repr(reglist[regionIdx]))
  else:
    print("***No Region Selected***")
    
  chosen = RegionsIn(reglist[regionIdx],defs)
  for item in chosen:
    print(repr(item))
