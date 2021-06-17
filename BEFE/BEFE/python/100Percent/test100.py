#!/usr/bin/python
#coding=utf8
#===============================================================================
#===============================================================================
#
# File: test100.py - 100 Percent IT Ltd. technical test
#
# This file contains the main solution to the 100 Percent IT Ltd. technical
# test written by Bruce Ferris on 21-Sep-2017 and 22-Sep-2017.
#
# Note: Have used "stuff I coded earlier" which is in separate files
#       starting with "BEFE_..." to make the coding go faster
#
#       Also, instead of installing MySQL on my laptop along with the Python
#       interface it I just read in the .sql file into Python lists in
#       the same format and coded from there.
#
#       The main solution is in the function 'processProject' which works
#       on using the fips, snats, and v9 lists.
#
#  Code Challenge
#  --------------
#
#  This is a simplified example of a ‘real life’ problem that you might have to solve
#  if working at 100 Percent IT.  We track bandwidth transferred to and from servers.
#  This code challenge considers some of the potential issues with tracking bandwidth
#  usage. 
#   
#  The necessary details are in two separate types of tables. The acct_v9 table
#  contains time-stamped details of how much data was transferred into or out of
#  a particular IP address in 1 minute windows. The audit tables contains the
#  information on which project each IP address is allocated to in 5 minute windows
#  (IP addresses are fairly regularly reassigned between customers on a self-service
#  basis).
#   
#  Your challenge is to write a program to show the total bandwidth usage per project.
#  Some things you may wish to consider:
#    1. How will you work out how much bandwidth a customer has used if the IP address
#       starts to be used between five minute audits? 
#    2. What about if a project drops an IP address between the five minute audit intervals?
#    3. What happens if an IP address is switched to a different project during the day or
#        between 5 minute audit windows?
#
# TODO: Validate ipProjs, what do we do when assignments overlap each other?
#       What do we do when assignments conflict, split stats?
#      
#===============================================================================

import                  string
from BEFE_Struct import Struct
from BEFE_errors import *
from datetime    import datetime
from datetime    import timedelta

#
# Globals - These are our "Fake SQL tables" presented as lists of Audit or Acct_v9 instances...
#

# "Fake Tables"
fips  = []
snats = []
v9s   = []

# "Temporary Tables"...
projStats = {}   # ProjStats by proj
ipProjs   = {}   # ipProjs by ip

# Constants

oneMin  = timedelta(0,60*1)
fiveMin = timedelta(0,60*5)

# Debugging...
debug       = True
verbose     = True
veryVerbose = False

#
# Row Types...
#

# Audit represents a single row in neutron_fip_audit or neutron_snat_audit
class Audit(Struct):
  members = "id,uuid,project,ip,timestamp"
  def __init__(self):
    super(Audit,self).__init__(Audit.members)

# Acct_v9 represents a single row in acct_v9
class Acct_v9(Struct):
  members = "srcIp,dstIp,bytes,inserted,updated"
  def __init__(self):
    super(Acct_v9,self).__init__(Acct_v9.members)

# ProjStat represents a single project's usage statistics
class ProjStat(Struct):
  members = "proj,bytes,seconds"
  def __init__(self,proj):
    super(ProjStat,self).__init__(ProjStat.members)
    self.proj    = proj
    self.bytes   = 0
    self.seconds = 0

# IpProj represents a single IP assignment per project through time    
class IpProj(Struct):
  members = "ip,start,end,seconds,proj"
  def __init__(self,ip,start,proj):
    super(IpProj,self).__init__(IpProj.members)
    self.ip      = ip
    self.start   = start
    self.end     = start+fiveMin
    self.seconds = fiveMin
    self.proj    = proj
  
#-------------------------------------------------------------------------------
#
# Debugging...
#

# DEBUG...
if debug and verbose:

  import sys
  
  def dot():
    sys.stdout.write('.')
    sys.stdout.flush()
# ...DEBUG

#-------------------------------------------------------------------------------
#
# Parsing...
#

#
# str2datetime - Convert SQL timestamp text to a Python datetime object
#

def str2datetime(s):

  if s is None:
    return None
    
  parts = s.split(' ')
  y,mon,d = parts[0].split('-')
  h,min,s = parts[1].split(':')
  y = int(y)
  mon = int(mon)
  d = int(d)
  h = int(h)
  min = int(min)
  s = int(s)
  return datetime(y,mon,d,h,min,s)
  
#
# nextValue - Pull apart '(...)' row from 'INSERT' statement
#

def nextValue(values,start):

  # Find next '(...)'
  s = string.find(values,'(',start)
  if s < 0:
    return (None,None)
  e = string.find(values,')',s)
  if e < 0:
    return (None,None)
    
  return (s,e+1)

#
# parseData - Parse all values in a SQL 'INSERT' line
#
  
def parseData(values):

  start = 0
  results = []
  while start < len(values):
    start,end = nextValue(values,start)
    if start is None or end is None:
      break
    result = values[start+1:end-1].split(',')
    # Fiddle columns based on whether it's 'audit' or 'acct'...
    if len(result) == 5: # audit...
      audit = Audit()
      audit.id, audit.uuid, audit.project, audit.ip, audit.timestamp = result
      audit.id        = audit.id[1:-1]
      audit.uuid      = audit.uuid[1:-1]
      audit.project   = audit.project[1:-1]
      audit.ip        = audit.ip[1:-1]
      audit.timestamp = str2datetime(audit.timestamp[1:-1])
      results.append(audit)
    elif len(result) == 19: # acct...
      v9 = Acct_v9()
      v9.srcIp    = result[7][1:-1]
      v9.dstIp    = result[8][1:-1]
      t = result[15][1:-1]
      v9.bytes    = 0 if not t else int(t)
      v9.inserted = str2datetime(result[17][1:-1])
      v9.updated  = str2datetime(result[18][1:-1])
      results.append(v9)
    elif len(result) == 0: # EMPTY...
      pass
    else:
      raise InternalError("Expected 5 or 19 values, got %d"%len(result))
    start = end
    
  return results

#
# loadTables - Read .sql file and populate global 'fake tables'...
#

def loadTables(fileName):

  global fips, nats, v9s
  
  print("loadTables: Loading table data...")
  f = open(fileName)
  lines = f.readlines()
  data = []
  numRows = 0
  numInserts = 0
  for line in lines:
    if line[:6] == 'INSERT':
      numInserts += 1
      parts = line.split(' ')
      tabname = parts[2][1:-1]
      values = ' '.join(parts[4:])
      results = parseData(values)
      numRows += len(results)
      # Populate fake tables...
      if tabname == 'acct_v9':
        v9s.extend(results)
      elif tabname == 'neutron_fip_audit':
        fips.extend(results)
      elif tabname == 'neutron_snat_audit':
        snats.extend(results)
      else:
        raise InternalError("Unknown table name '%s'"%tabname)
  print("  %d lines, %d INSERTs, numRows = %d..."%(len(lines),numInserts,numRows))
  print("  'v9s'   has %d rows"%len(v9s))
  print("  'snats' has %s rows"%len(snats))
  print("  'fips'  has %d rows"%len(fips))

#
# buildAssignments - Build ipProjs table
#

def buildAssignments():

  global ipProjs
  
  # DEBUG...
  if debug and verbose:
    print("buildAssignments...")
    count = 0
  # ...DEBUG
  
  ipProjs = {}
  
  for row in fips:
    # DEBUG...
    if debug and verbose:
      count += 1
      if count%1000 == 0:
        dot()
    # ...DEBUG
    if row.ip not in ipProjs:
      ipProjs[row.ip] = []
    else:
      assign = IpProj(row.ip,row.timestamp,row.project)
      ipProjs[row.ip].append(assign)
  for row in snats:
    # DEBUG...
    if debug and verbose:
      count += 1
      if count%1000 == 0:
        dot()
    # ...DEBUG
    if row.ip not in ipProjs:
      ipProjs[row.ip] = []
    else:
      assign = IpProj(row.ip,row.timestamp,row.project)
      ipProjs[row.ip].append(assign)
    
  # DEBUG...
  if debug and verbose:
    print("\nbuildAssignments: sorting %d ip addresses"%len(ipProjs))
  # ...DEBUG
  
  # Now, sort them by ip/start    
  for ip in ipProjs:
  
    # DEBUG...
    if debug and verbose:
      count += 1
      dot()
    # ...DEBUG

    # Sort by start
    ipProjs[ip].sort()
    
    # For each assignment, clip previous one if overlap...
    numIdx = len(ipProjs[ip])
    idx = 1
    while idx < numIdx:
      cur  = ipProjs[ip][idx]
      prev = ipProjs[ip][idx-1]
      if cur.start >= prev.start and cur.start < prev.end:
        prev.end = cur.start
        prev.seconds = cur.start-prev.start
      idx += 1        
  
  # DEBUG...
  '''
  if debug and verbose:
    print("\nbuildAssignments: %d ip addresses"%len(ipProjs))
    if veryVerbose:
      ips = list(ipProjs)
      ips.sort()
      for ip in ips:
        print("  %s has %d assignments"%(ip,len(ipProjs[ip])))
  '''
  # ...DEBUG
  
#
# spreadIpUsage - Spread IP usage over applicable projects
#
# Note: This is called twice for each IP in an acct_v9 row (both
#       ip_src and ip_dst) and spreads the bytes transfered over
#       the applicable projects.
#
#       This is because, if BOTH are a registered server/service,
#       the bytes transferred are considered part of the bandwidth
#       for BOTH servers.
#
#       If a given IP isn't assigned to a server/service, it doesn't
#       count in the server's/service's bandwidth.
#

def spreadIpUsage(ip, bytes, start, end):

  # Ignore if ip not assigned at all
  if ip not in ipProjs:
    return
    
  # DEBUG...
  if debug and veryVerbose:
    print("spreadIpUsage: ip = %s, len(ipProjs) = %d"%(ip,len(ipProjs)))
  # ...DEBUG
  
  # Binary chop IP's time assignments, finding time period in range...
  projs = ipProjs[ip]
  ll = 0
  ul = len(projs)-1
  idx = -1
  while ll < ul:
    mid = (ll+ul)/2
    cur = projs[mid]
    if cur.start <= start:
      if cur.end > start: # In range...
        idx = mid
        break
      ll = mid+1
      continue
    ul = mid-1
  
  # If none found, start at first one 
  if idx == -1:
    idx = 0
  
  # If off the end, nothing to do...
  if idx >= len(projs):
    return
 
  # Found one (idx) in time range, walk backwards to first one...
  while idx:
    prevIdx = idx-1
    prev = projs[prevIdx]
    if prev.start <= start and prev.end > start:
      idx -= 1
      continue
    break
      
  # Positioned in IP's time frame, break up the usage into appropriate bits
  # discarding any unused bytes...
  
  totSeconds = (end-start).seconds
  totBytes   = bytes
  
  # DEBUG...
  if debug and veryVerbose:
    print("               ip = %s, idx = %d totSeconds = %d, totBytes = %d"%(ip,idx, totSeconds, totBytes))
  # ...DEBUG
  
  while idx < len(projs):
    
    # Finished if out of time assignments...
    row = projs[idx]
    if row.start >= end:
      break
  
    # Got one, spread it...
    idx += 1 # For next iteration
    proj = row.proj
    fragStart   = row.start if row.start >= start else start
    fragEnd     = row.end   if row.end < end else end      
    fragSeconds = (fragEnd-fragStart).seconds
    if fragEnd <= fragStart:
      break
    fragBytes   = 0
    if totSeconds:
      fragBytes   = totBytes*fragSeconds/totSeconds
    # Sum them
    if fragBytes and fragSeconds:
      stats = projStats[proj]
      stats.bytes   += fragBytes
      stats.seconds += fragSeconds
    
  return
  
#
# processV9 - Process acct_v9 rows
#
# Note: This is where the main work is performed AND it's performed using
#       a single pass over the largest table - acct_v9
#

def processV9():

  # DEBUG...
  if debug and verbose:
    print("\nprocessV9...")
  # ...DEBUG

  # For each acct_v9 row, adjust IP statistics...
  #
  # Note: This should really be a SQL query row by row but for now...
  #
  # Note: neutron_fip_audit and neutron_snat_audit will be appropriately
  #       cached into fips and nats because they are needed to build
  #       ipProjs. Makes sense since they're the lesser rows and
  #       faster to access in Python than in SQL.
  #
  
  count = 0
  
  for row in v9s:

    # DEBUG...
    if debug and verbose:
      count += 1
      if count%100 == 0:
        dot()
    # ...DEBUG
    
    ip1 = row.srcIp
    ip2 = row.dstIp
    start = row.inserted
    if row.updated:
      start = row.updated
    end = start+oneMin
    bytes = row.bytes
    
    spreadIpUsage(ip1,bytes,start,end)
    spreadIpUsage(ip2,bytes,start,end)
    
  # DEBUG...
  if debug and verbose:
    print('')
  # ...DEBUG
    
#-------------------------------------------------------------------------------
# 
# Main
#

if __name__ == '__main__':

  # Load tables from .SQL file instead of using MySQL
  #loadTables('test_dataset_billing_task.sql')
  loadTables('small.sql')
  
  # Build 'temporary tables'
  for row in snats:
    if row.project not in projStats:
      projStats[row.project] = ProjStat(row.project)

  for row in fips:
    if row.project not in projStats:
      projStats[row.project] = ProjStat(row.project)

  # Do it...
  buildAssignments()
  processV9()

  # Display results...
  projects = list(projStats)
  projects.sort()
  for proj in projects:
    stat = projStats[proj]
    bytes = stat.bytes
    seconds = stat.seconds
    if seconds == 0:
      bandWidth = 0
    else:
      bandWidth = bytes/seconds
    print("project %s: bytes = %d, seconds = %d, bandwidth = %d"%(repr(proj),bytes,seconds,bandWidth))  
