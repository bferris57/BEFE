#!/usr/bin/env python3
#  coding=utf-8
from __future__ import print_function
#==============================================================================
#
# File: funcs.py - Various utility functions
#
# Functions: print              - Replacement for Python's print (with counter in place)
#            execute            - Execute a shell command without redirection 
#            Error              - Print an error
#            Warning            - Print a warning
#            Info               - Print info (if verbose set)
#            Internal           - Print an internal error
#            Literal            - Print literal message
#            Debug              - Print debug message (if debug and verbose are set)
#            NotImp             - Print "Not implemented"
#            Notice             - Print a notice message
#            Prefix             - Print string with prefix
#            deepSearch         - Search directory deep for files
#
#            dtNow              - datetime.datetime object for datetime now
#            dtToReadable       - datetime.datetime object to human readable date @ time
#
#            getHostName        - "What's my machine name?"
#            myIps              - Return list of my IP addresses
#            promptInput        - Prompt and get input from <stdin>
#            fixUri             - Substitute proper URI characters (%xx) if needed
#            hostAlive          - "Is named host alive? (ping it once)"
#            getPid             - General "find PID of something somewhere"
#            getPids            - General "Find PID(s) of something somewhere"
#            flushout           - Flush stdout and stderr
#            isInt              - "Is this string a valid integer?"
#            isFloat            - "Is this string a valid floating point numbere?"
#            isHexString        - "Is this a hexidecimal string?"
#            username           - Return current OS user name
#
#            postEllipse        - Turn string into fixed size post-ellipsified string
#            preEllipse         - Turn string into fixed size pre-ellipsified string
#            midEllipse         - Turn string into fixed size mid-ellipsified string
#
#            pathFindUp         - Search for something from directory up
#            reprLen            - "How many characters in repr(x)?"
#            findChars          - Look for many characters in a string
#            flushOut           - Flush sys.stdout
#            isIpv4             - "Is this string a valid ipv4 address?"
#            parseIp            - Parse IPv4/IPv6 address/port
#            splitName          - Split a "...#...@..." object name into 3 parts
#
#            formatHex          - Format bytes into readable text
#            hexToBytes         - Convert hexidecimal string into bytes
#            bytesToHex         - Convert bytes to hexidecimal string
#
#            sha256             - Calculate sha256 checksum of file
#            sha1               - Calculate sha1   checksum of file
#            sha1_bytes         - Compute sha1 checksum of a byte string
#            sha256_bytes       - Compute sha2 checksum of a byte string
#
#            kallsyms           - Return dictionary of /proc/kallsyms 'T' addresses
#            linuxVersion       - Return Linux equivalent of "uname -r"
#
#            osName             - Return 'Ubuntu', 'Centos', or None based on OS version
#            isUbuntu           - "Are we running on Ubuntu?"
#            isCentOS           - "Are we running on CentOS?"
#
#            which              - Search $PATH for executable
#
#            exceptionLines     - Turn exception traceback into printable lines
#
#            sudo               - Restart running process as sudo if not already (!!!Danger!!!)
#
#            ipAddresses        - Return iterable of host's IP addresses
#            ipv4Addresses      - Return iterable of host's IPv4 addresses
#
#            packages           - Return list of installed packages
#
#            red   - Print red error
#            green - Print green message
#            blue  - Print blue message
#            error - Print red error and exit with status
#            color - Use colr.color if stdout is a tty
#
#==============================================================================

import subprocess
import os
import sys
import pwd
import datetime
import time
import json
import shlex
import hashlib
import re
try:
  import colr
except:
  class colr:
    def color(*args,**pargs):
      return args[0]

verbose = False
debug   = False
quiet   = False

#-------------------------------------------------------------------------------
#
# Function: execute - Execute shell command and, optionally, show output
#
# Usage:    rc,out,err = execute(cmd,prefix=None)
#
# Where:    cmd     - str:  Shell command to execute
#           prefix  - str:  If set, prefix to prepend to output lines (if showout=True)
#           showout - bool: False="Quiet", True="Show stdout"
#           showerr - bool: False="Quiet", True="Show stderr"
#           env     - dict: Environment to use
#           debug   - bool: False="Normal", True="Call gdb --args with command"
#
#           rc      - int: Return code
#           out     - str: Output from stdout
#           err     - str: Output from stderr
#
# Notes:    If prefix is None, stdout will not be displayed.
#

def execute(cmd,prefix='',showout=False,showerr=False,env=os.environ,debug=False,shell=False):

    if type(cmd) == list:
      args = cmd
    else:
      args = shlex.split(cmd)

    if args and args[0] == 'gdb':
      stdout = sys.stdout
      stderr = sys.stderr
    else:
      stdout = subprocess.PIPE
      stderr = subprocess.PIPE

    p = subprocess.run(args,stdin=sys.stdin,stdout=stdout,stderr=stderr,env=env,shell=shell)

    rc  = p.returncode
    out = str(p.stdout,'utf-8')
    err = str(p.stderr,'utf-8')

    if showout:
        lines = out.split('\n')
        if lines and not lines[-1]:
            del lines[-1]
        print("execute: stdout...")
        for line in lines:
            print(prefix+line)
        print("...stdout")

    if showerr and err:
        lines = err.split('\n')
        if lines and not lines[-1]:
            del lines[-1]
        print("execute: stderr...")
        for line in lines:
            print(prefix+line)
        print("...stderr")
        if not showerr and env:
            if 'LD_LIBRARY_PATH' not in env:
                print("Error: LD_LIBRARY_PATH missing")
            else:
                print("LD_LIBARY_PATH = %s"%repr(env['LD_LIBRARY_PATH']))

    return (rc,out,err)

#-------------------------------------------------------------------------------
#
# Functions: Error    - Print an error
#            Warning  - Print a warning
#            Internal - Print an internal error
#            Info     - Print info (if verbose set)
#            Literal  - Print literal message
#            Debug    - Print debug message (if debug and verbose are set)
#            Notice   - Print a notice message
#            NotImp   - Print "Not implemented"
#
#            Prefix   - Print string with prefix
#
#-------------------------------------------------------------------------------

def Error(msg,nl=True,fore='red',style='bright'):

    Prefix("Error: ",msg,nl=nl,fore=fore,style=style)

def Warning(msg,nl=True):

    Prefix("Warning: ",msg,nl=nl)

def Internal(msg,nl=True):

    Prefix("Internal Error: ",msg,nl=nl)

def Info(msg,nl=True):

    if verbose:
        Prefix("Info: ",msg,nl=nl)

def Literal(msg,nl=True):

    Prefix('',msg,nl=nl)

def Debug(msg,nl=True):

    if debug and verbose:
      Prefix("Debug: ",msg,nl=nl)

def Notice(msg,nl=True):

    if not quiet:
      Prefix("Notice: ",msg,nl=nl)

def NotImp(item=None,nl=True):

    if not item:
        Error("***Not Implemented Yet***",nl)
    else:
        Error("*** %s Not Implemented Yet ***"%repr(item)),nl

def Deprecated(item=None,nl=True):

    if not item:
        error("*** Deprecated ***",nl)
    else:
        Error("*** %s Deprecated ***"%repr(item),nl)

def Prefix(msg,prefix='',samePrefix=False,nl=True,fore=None,style=None):

    end = '\n' if nl else ''

    msgs = msg.split('\n')
    thisPrefix = prefix
    if len(msgs) > 1:
        print('')
        for i in range(0,len(msgs)):
            msg = thisPrefix+msgs[i]
            msg = color(msg,fore=fore,style=style)
            print(msg)
            if not samePrefix:
                thisPrefix = ' '*len(prefix)
    else:
        msg = thisPrefix+msg
        msg = color(msg,fore=fore,style=style)
        print(msg)

#-------------------------------------------------------------------------------
#
# Function: typeName - Return the type name as a String
#
#-------------------------------------------------------------------------------

def typeName(that):

    typeStr = repr(type(that))
    typeStr = typeStr[1:-1].split(' ')[1][1:-1]

    return typeStr

#-------------------------------------------------------------------------------
#
# Function: formatKeys - Given a dict, format the keys in a list of output lines
#
#-------------------------------------------------------------------------------

def formatKeys(dict):
    keys = list(dict.keys())
    keys.sort()
    maxLen = 0
    for key in keys: maxLen = max(maxLen,len(key))
    result = []
    for key in keys:
        line = key+' '*(maxLen-len(key))+' = '+repr(dict[key])
        result.append(line)
    return result

#-------------------------------------------------------------------------------
#
# Function: deepSearch - Recursively search for files
#
# Usage:    files = deepFind(dir,match)
#
# Where:    dir   - str:  Source directory
#           match - func: Function to test if file is wanted (passed full path)
#           files - list: List of selected files
#
# Example:  The following is a sample 'match' function looking for "*.war"...
#
#             def matchWar(that):
#               return that[-4:] == '.war'
#
#           Can do it even cooler than that...
#
#             files = deepFind('<some dir>',lambda x: x[-4:] == '.war')
#

def deepSearch(dir,match):

    stack = []
    stack.append(dir)
    files = []

    while stack:
        dir = stack[0]
        del stack[0]
        things = os.listdir(dir)
        for thing in things:
            thing = dir+os.sep+thing
            if os.path.isdir(thing):
                stack.append(thing)
            elif os.path.isfile(thing) and match(thing):
                files.append(thing)

    return files

#-------------------------------------------------------------------------------
#
# Function:  dtNow        - datetime.datetime object for time now
#            dtFromNonce  - datetime.datetime object from Nonce string
#            dtToNonce    - datetime.datetime object to a Nonce string
#            dtToReadable - datetime.datetime object to human readable date @ time
#            deltaSeconds - "How old is a datetime object in seconds?"
#            dtNowNonce   - Return string Timestamp of datetime now
#            isValidNonce - "Is this a valid timestamp nonce?"
#

def dtNow():

    result = datetime.datetime.fromtimestamp(time.time())

    return result

def dtFromNonce(nonce):

    try:
        parts = nonce.split('T')
        #print("dtFromNonce: a...")
        year,mon,day = parts[0].split('-')
        #print("dtFromNonce: b...")
        hour,min,sec = parts[1].split(':')
        #print("dtFromNonce: c...")
        sec,ms = sec.split('.')
        #print("dtFromNonce: d...")
        #print("dtFromNonce: year=%s, mon=%s, day=%s"%(repr(year),repr(mon),repr(day)))
        #print("dtFromNonce: hour=%s, min=%s, sec=%s, ms=%s"%(repr(hour),repr(min),repr(sec),repr(ms)))
        year = int(year)
        mon  = int(mon)
        day  = int(day)
        hour = int(hour)
        min  = int(min)
        sec  = int(sec)
        ms   = int(ms)%1000
        #print("dtFromNonce: y,m,d,h,m,s = %d,%d,%d,%d,%d,%d,%d"%(year,mon,day,hour,min,sec,ms))
    except:
        return None

    result = datetime.datetime(year,mon,day,hour=hour,minute=min,second=sec,microsecond=ms*1000)
    #print("dtFromNonce: result = %s"%repr(result))

    return result

def dtToNonce(dt):

    year  = dt.year
    mon   = dt.month
    day   = dt.day
    hour  = dt.hour
    min   = dt.minute
    sec   = dt.second
    ms    = (dt.microsecond+500)/1000
    nonce = '%04d-%02d-%02dT%02d:%02d:%02d.%03d'%(year,mon,day,hour,min,sec,ms%1000)

    return nonce

def dtToReadable(dt,ampm=True):

    nonce = dtToNonce(dt)

    parts = nonce.split('T')
    
    if ampm:
      dat = parts[1][:-4]
      hour = int(dat[-8:-6])
      dat  = dat[:-8] + '%02d' + dat[-6:] + '%s'
      if hour > 12:
        ampm = 'pm'
        hour -= 12
      else:
        ampm = 'am'
      parts[1] = dat%(hour,ampm)

    return ' @ '.join(parts)

def deltaSeconds(delta): # ◄── delta=datetime.timedelta, returns float seconds

    diff = float(delta.days*(60*60*24)+delta.seconds) + delta.microseconds/1000000.0

    return diff

def dtNowNonce():

    return dtToNonce(dtNow())

def isValidNonce(ts):

    if type(ts) is not str:
        return False
    if len(ts) != 23:
        return False
    if ts[10] != 'T':
        return False

    # Example: 2018-06-22T11:39:50.898

    date = ts[:10]
    time = ts[11:]

    if date[4] != '-' or date[7] != '-':
        return False
    if not isInt(date[:4]) or not isInt(date[5:7]) or not isInt(date[8:]):
        return False
    if time[2] != ':' or time[5] != ':' or time[8] != '.':
        return False
    if not isInt(time[:2]) or not isInt(time[3:5]) or not isInt(time[6:8]) or not isInt(time[9:]):
        return False

    return True

#------------------------------------------------------------------------------
#
# Function: myIps - Return list of my IP addresses (removes '127.0.0.1')
#
# Usage:    ips = myIps()
#
# Where:    ips - str/list: List of non 127.0.0.1 ips or string if only one
#

def myIps():

    rc,output,err = execute("sudo ifconfig",showout=False)
    lines = output.split('\n')

    ips = []
    for i in range(0,len(lines)):
        line = lines[i]
        if 'inet ' not in line:
            continue
        parts = line.split(' ')
        for j in range(0,len(parts)-1):
          ip = parts[j]
          if not isIpv4(ip):
            continue
          if ip != '127.0.0.1':
            ips.append(ip)
            break
    if not ips: 
      return ips

    return ips if len(ips) > 1 else ips[0]

#-------------------------------------------------------------------------------
#
# Function: getHostName - "What's my host name?"
#
# Usage:    iam = getHostName()
#
# Where:    iam - str: Current host's name (if any)
#
def getHostName():

    import socket
    return socket.gethostname()

#-------------------------------------------------------------------------------
#
# Function: promptInput - Prompt and get input from <stdin>
#
# Usage:    line = promptInput(prompt,history=None)
#
# Where     line    - str:  Input line
#           prompt  - str:  Prompt string
#           history - list: History list (or None)
#

def promptInput(prompt,history=None):

    import getch

    line = ''

    print(prompt,end='')

    while True:
        c = getch()

#------------------------------------------------------------------------------
#
# Function: fixUri - Replace reserved characters with %xx equivalents
#
# Notes: From https://en.wikipedia.org/wiki/Percent-encoding#Percent-encoding_reserved_characters
#
#        Reserved characters after percent-encoding ()
#        !   #   $   &   '   (   )   *   +   ,   /   :   ;   =   ?   @   [   ]
#        %21 %23 %24 %26 %27 %28 %29 %2A %2B %2C %2F %3A %3B %3D %3F %40 %5B %5D
#

fixes = {}
fixes['!'] = '%21'
fixes['#'] = '%23'
fixes['$'] = '%24'
fixes['&'] = '%26'
fixes["'"] = '%27'
fixes['('] = '%28'
fixes[')'] = '%29'
fixes['*'] = '%2a'
fixes['+'] = '%2b'
fixes[','] = '%2c'
fixes['/'] = '%2f'
fixes[':'] = '%3a'
fixes[';'] = '%3b'
fixes['='] = '%3d'
fixes['?'] = '%3f'
fixes['@'] = '%40'
fixes['['] = '%5b'
fixes[']'] = '%5d'

def fixUri(uri):

    i = -1
    while True:
        i += 1
        if i >= len(uri): break
        c = uri[i]
        if c in fixes:
            uri = uri[:i] + fixes[c] + uri[i+1:]
            i += 2

    return uri

#-------------------------------------------------------------------------------
#
# Function: hostAlive -  "Is named host alive? (ping it once)"
#

def hostAlive(host):

    if type(host) != str:
        raise ValueError("Expected host parameter to be a string")

    cmd = "ping -c 1 "+host
    rc,out,err = execute(cmd,showout=False)
    if host in out and ", 1 received," in out:
        return True

    return False

#------------------------------------------------------------------------------
#
# Function: kill - Kill a given pid or pids on host
#

def kill(host,pids):

    if not pids:
        return

    if type(pids) in (list,tuple):
        pids = ' '.join(pids)
    cmd = "sudo ssh %s kill -9 %s"%(host,pids)
    rc,output,err = execute(cmd,showout=False)
    if output:
        lines = output.split('\n')
        for i in range(0,len(lines)):
            lines[i] = '... '+lines[i]
        output = '\n'.join(lines)
        print("funcs.kill: Failed to kill pids %s, output follows...\n%s"%(pids,output))

#-------------------------------------------------------------------------------
#
# Functions: getPid - General "find PID of something somewhere"
#
# Usage: pid = getPid(lookFor=<lookFor>,host=<host>)
#        pids = getPids(lookFor,host=<host>)
#
#        Where: <lookFor> - str:  String to look for
#               <host>    - str:  Host to look on ('' or None means "This machine")
#               pid       - str:  Pid found ('' means "Not Found")
#               pids      - list: List of PID(s)
#
# Note: We GUARANTEE return value with be either '' or a valid integer string
#
#       If neither lookFor or host are specified this returns our Pid.
#

def getPid(lookFor='',host=''):

    if not lookFor and not host:
        return str(os.getpid())

    pids = getPids(lookFor,host)
    if not pids:
        return ''

    return pids[0]

def getPids(lookFor='',host=''):

    if not lookFor and not host:
        return str(os.getpid())

    if not lookFor and host:
        raise ValueError("lookFor == '' and host %s specified, doesn't mean anything to me"%repr(host))

    if type(lookFor) != str:
        raise ValueError("Expected 'lookFor' to be type str")
    if type(host) != str:
        raise ValueError("Expected 'host' to be type str")

    if not host:
        cmd = "ps -aux"
    else:
        cmd = "ssh %s 'ps -aux'"%host

    rc,out,err = execute(cmd,showout=False)
    if not out:
        raise ValueError("Expected non-empty output")
        return []

    # Find a line with lookFor in it...
    lines = out.split('\n')
    results = []
    for line in lines:
        if lookFor in line:
            parts = line.split(' ')
            i = 0
            while i < len(parts):
                if not parts[i]:
                    del parts[i]
                else:
                    i += 1
            if len(parts) < 2:
                continue
            if isInt(parts[1]):
                results.append(parts[1])

    return results

#-------------------------------------------------------------------------------
#
# Function: flushOut - Flush stdout and stderr
#

def flushOut():

    sys.stdout.flush()
    sys.stderr.flush()

#-------------------------------------------------------------------------------
#
# Functions: isInt        - "Is this string a valid integer?"
#            isFloat      - "Is this string a valid floating point number?"
#            isHexString  - "Is this string a valid hexidecimal string?"
#

def isInt(string):

    try:
        i = int(string)
    except:
        return False

    return True

def isFloat(string):

    try:
        f = float(string)
    except:
        return False

    return True

def isHexString(string):

    if type(string) != str or not string or len(string)%2 != 0:
        return False

    string = string.strip().lower()
    chars = '0123456789abcdefg'
    for char in string:
        if char not in chars:
            return False

    return True

#-------------------------------------------------------------------------------
#
# Function: username - Return current OS user name
#

def username():

    return pwd.getpwuid(os.getuid()).pw_name

#------------------------------------------------------------------------------
#
# Function: postEllipse - Turn string into fixed size post-ellipsified string
#                         (e.g. "abcdef...")
#
# Usage:    result = postEllipse(inStr,tlen,fill=' '))
#
# Where:    inStr  - str: Input string
#           tlen   - int: Resulting string length (see notes below)
#           fill   - str: Fill character
#           result - str: Resulting string of length len
#
# Notes:    If len < 3, as many '.' as possible will form the string
#
#           If len <= 0, an empty string is returned
#
#           if len(inStr) < len, it will be padded with ' '
#

def postEllipse(inStr,tlen,fill=' '):

    if type(inStr) is not str:
        raise TypeError("Expected inStr to be type str")
    if type(tlen) is not int:
        raise TypeError("Expected tlen to be type int")

    if tlen <= 0:
        return ''
    if tlen <= 3:
        return '...'[0:tlen]

    if len(inStr) <= tlen-3:
        return inStr + fill[0]*(tlen-len(inStr))

    inStr = inStr[:tlen-3] + fill[0]*3

    return inStr

#------------------------------------------------------------------------------
#
# Function: preEllipse - Turn string into fixed size pre-ellipsified string
#                         (e.g. "...abcdef")
#
# Usage:    result = preEllipse(inStr,tlen,fill=' '))
#
# Where:    inStr  - str: Input string
#           tlen   - int: Resulting string length (see notes below)
#           fill   - str: Single character string to fill with
#           result - str: Resulting string of length len
#
# Notes:    If len < 3, as many '.' as possible will form the string
#
#           If len <= 0, an empty string is returned
#
#           if len(inStr) < len, it will be pre/post-padded with ' '
#

def preEllipse(inStr,tlen,fill=' '):

    if type(inStr) is not str:
        raise TypeError("Expected inStr to be type str")
    if type(tlen) is not int:
        raise TypeError("Expected tlen to be type int")

    if tlen < 3:
        return ''

    if len(inStr) >= tlen-3:
        return fill[0]*3 + inStr[-(tlen-3):]
    else:
        return fill[0]*(tlen-len(inStr)) + inStr[tlen-3:] 

#------------------------------------------------------------------------------
#
# Function: midEllipse - Turn string into fixed size mid-ellipsified string
#                         (e.g. "abcdef...wxyz")
#
# Usage:    result = midEllipse(inStr,tlen)
#
# Where:    inStr  - str: Input string
#           tlen   - int: Length of resulting String
#           result - str: Resulting string of length len
#
# Notes:    If len < 3, as many '.' as possible will form the string
#
#           If len <= 0, an empty string it returned
#
#           if len(inStr) < len, it will be padded with ' '
#

def midEllipse(inStr,tlen):

    if type(inStr) is not str:
        raise TypeError("Expected inStr to be type str")
    if type(tlen) is not int:
        raise TypeError("Expected tlen to be type int")

    if tlen <= 0:
        return ''
    if tlen <= 3:
        return '...'[0:tlen]

    if len(inStr) <= tlen:
        return inStr + ' '*(tlen-len(inStr))

    len1 = int((tlen-3)/2)
    len2 = (tlen-3)-len1

    return inStr[0:(len1)] + '...' + inStr[-len2:]

#------------------------------------------------------------------------------
#
# Function: pathFindUp - Search for something from directory up
#
# Usage:    loc = pathFindUp(startDir,searchFor)
#
# Where:    startDir  - str: Directory to start from ('' means current dir)
#           searchFor - str: What to search for (musn't have os.sep in it)
#           loc       - str: Where found ('' = "Not Found")
#
# Notes:    If searchFor has os.sep ('/' in it), '' will be returned in loc.
#

def pathFindUp(startDir,searchFor):

    sep = os.sep

    if sep in searchFor:
        return ''

    if not startDir:
        startDir = os.getcwd()

    startDir = os.path.abspath(startDir)
    parts = startDir.split(sep)
    if parts[0] == '':
        del parts[0]

    while parts:
        curDir = sep + sep.join(parts)
        if os.path.isdir(curDir):
            lookFor = sep + sep.join(parts) + sep + searchFor
            if os.path.exists(lookFor):
                return lookFor
        del parts[-1]

    return ''

#-------------------------------------------------------------------------------
#
# Function: reprLen - "How many characters in repr(x)?"
#
# Usage:    numchr = reprLen(that)
#
# Where:    that   - object: Thing to repr()
#           numchr - int:    Number of characters
#

def reprLen(that):

    return len(repr(that))

#-------------------------------------------------------------------------------
#
# Function: findChars  - Look for many characters in a string
#
# Usage:    pos = findChars(string,chars,startPos)
#
# Where:    string   - str: String to search
#           chars    - str: Characters to search for
#           startPos - int: Starting position
#
#           pos      - int: Where found (-1 = "Not found")
#

def findChars(string,chars,startPos):

    for pos in range(startPos,len(string)):
        c = string[pos]
        if c in chars:
            return pos
    return -1

#------------------------------------------------------------------------------
#
# Function: flushout - Flush sys.stdout
#

def flushout():

    sys.stdout.flush()

#-------------------------------------------------------------------------------
#
# Function: isIpv4 - "Is this string a valid ipv4 address?"
#

def isIpv4(that):

    if type(that) != str:
        return False

    parts = that.split('.')
    if len(parts) != 4:
        return False

    for part in parts:
        if not isInt(part):
            return False
        p = int(part)
        if p < 0 or p > 255:
            return False

    return True

#--------------------------------------------------------------------------
#
# Function: parseIp - Parse IPv4/IPv6 address/port
#
# Usage:    host, ip, port, error = parseIp(that,ipChar='@')
#
# Where:    that   - str: IPv4/IPv6 address/port string
#           ipChar - str: Character separating <host> and <ip> (if any)
#           host   - str: Host name ('' = None)
#           ip     - str: IP Address ('' = None)
#           port   - str: Port number ('' = None)
#           error  - str: Error message ('' = No error)
#
# Note:     We don't currently handle IPv6 address ([...]:port) because
#           can't be arsed at the moment.
#
#           We handle the following formats of IP...
#
#             <host>[@[<ip>[:<port>]]]
#

def parseIp(that,ipChar='@'):

    if type(that) != str:
        return '','','','*** cmd_help.parseIp: Expected String ***'

    if '[' in that and ']' in that:
        return '','','','IPv6 addresses not implemented yet'

    error = "Invalid host %s"%repr(that)
    host  = ''
    ip    = ''
    port  = ''

    parts = that.split(ipChar)
    if not parts or len(parts) > 2:
        return '','','',error

    host = parts[0]
    del parts[0]
    if parts:
        ipAndPort = parts[0]
    else:
        ipAndPort = ''

    if ipAndPort:
        parts = ipAndPort.split(':')
        if len(parts) != 1 and len(parts) != 2:
            return '','','',error + " (too many ':')"
        ip = parts[0]
        if len(parts) == 2:
            port = parts[1]

    if ip:
        parts = ip.split('.')
        for part in parts:
            if not isInt(part):
                return '','','',"Invalid ip octet %s"%repr(part)
            octet = int(part)
            if octet < 0 or octet > 255:
                return '','','',"Invalid ip octet %s"%repr(part)

    if port:
        if not isInt(port):
            return '','','',"Invalid port %s"%repr(part)
        p = int(port)
        if p <= 0 or p > 65535:
            return '','','',"Invalid port %s"%repr(port)

    return host,ip,port,''

#--------------------------------------------------------------------------
#
# Function: splitName - Split a "...#...@..." object name into parts
#
# Usage:    result = splitName(that)
#
# Where:    result - List or None
#           that   - The object name
#

def splitName(objName):

    parts = objName.split('#')
    if len(parts) != 2:
        return (parts[0],'','')
    parts2 = parts[1].split('@')
    if len(parts2) != 2:
        return (parts[0],parts2[0],'')
    parts[1] = parts2[0]
    parts.append(parts2[1])

    return parts

#-------------------------------------------------------------------------------
#
# Function: formatHex - Format bytes into readable hex/ascii text
#
# Usage:    out = formatHex(bin)
#
# Where:    bin - bytes: Bytes to format
#           out - str:   Formatted hex/ascii text (with embeddded newlines)
#

def formatHex(bin,startOffset=0):

    if type(bin) != bytes:
        raise ValueError("formatHex: Expected bytes argument")

    digits = ['0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f']

    hex   = ''
    chars = ''
    for val in bin:
        nibs = [val>>4,val&0xf];
        for nib in nibs:
            hex += digits[nib]
        if val < 32 or val > 127:
            chars += '.'
        else:
            chars += chr(val)

    # Now, break both into 16 byte chunks...
    out = ''
    pos = 0
    chrOut = ''
    while pos < len(chars):
        if pos:
            out += '\n'
        out += "%08x"%(pos+startOffset) + ' '
        hexPart = hex[pos*2:pos*2+32]
        for i in range(0,len(hexPart)>>1):
            out += ' '+hexPart[i*2:i*2+2]
        if pos + 16 >= len(chars):
            out += '   '*(16-(len(chars)-pos))
        out += '  '+chars[pos:pos+16]
        pos += 16

    return out

#-------------------------------------------------------------------------------
#
# Functions: hexToBytes - Convert hexidecimal string into bytes
#            bytesToHex - Convert bytes to hexidecimal string
#

def hexToBytes(that):

    if len(that) % 2 != 0:
        raise ValueError("Expected 'that' to be an even number of bytes")
    if type(that) != str:
        raise ValueError("Expected 'that' to be a str")

    b = []
    byteVal = 0
    for i in range(0,len(that)):
        nib = that[i]
        if nib >= '0' and nib <= '9':
            nib = ord(nib) - ord('0')
        elif nib >= 'a' and nib <= 'f':
            nib = ord(nib) - ord('a') + 10
        elif nib >= 'A' and nib <= 'F':
            nib = ord(nib) - ord('A') + 10
        else:
            raise ValueError("Invalid hex digit %s"%repr(nib))
        byteVal = (byteVal*16) + nib
        if i and i%2 == 1:
            b.append(byteVal)
            byteVal = 0

    b = bytes(b)

    return b

def bytesToHex(that):

    s = ''
    if type(that) != bytes:
        raise ValueError("Expected 'that' to be bytes")
    for i in range(0,len(that)):
        byteVal = int(that[i])
        nib1 = int(byteVal/16)
        nib2 = int(byteVal)-(nib1*16)
        if nib1 <= 9:
            s += chr(nib1+ord('0'))
        else:
            s += chr(nib1-10+ord('a'))
        if nib2 <= 9:
            s += chr(nib2+ord('0'))
        else:
            s += chr(nib2-10+ord('a'))

    return s

#-------------------------------------------------------------------------------
#
# Function: sha256 - Calculate sha256 checksum of file
#           sha1   - Calculate sha1   checksum of file
#

def sha256(filename, block_size=65536):
    sha = hashlib.sha256()
    try:
        with open(filename, 'rb') as f:
            for block in iter(lambda: f.read(block_size), b''):
                sha.update(block)
        return sha.hexdigest()
    except:
        return None

def sha1(filename, block_size=65536):
    sha = hashlib.sha1()
    with open(filename,'rb') as f:
        for block in iter(lambda: f.read(block_size),b''):
            sha.update(block)
    return sha.hexdigest()

def sha1_bytes(byteVal):

    sha = hashlib.sha1()
    if type(byteVal) == str:
        byteVal = byteVal.encode('utf-8')
    elif type(byteVal) != bytes:
        raise ValueError("Expected 'bytes' to type bytes")
    sha.update(byteVal)
    return sha.hexdigest()

def sha256_bytes(byteVal):

    sha = hashlib.sha256()
    if type(byteVal) == str:
        byteVal = byteVal.decode('utf-8')
    elif type(byteVal) != bytes:
        raise ValueError("Expected 'bytes' to type bytes")
    sha.update(byteVal)
    return sha.hexdigest()

#-------------------------------------------------------------------------------
#
# Function: kallsyms - Return dictionary of function:address from /proc/kallsyms
#

def kallsyms():

    cmd = 'sudo cat /proc/kallsyms | grep " T "'
    rc,output,err = execute(cmd,showout=False)

    lines = output.split('\n')

    syms = {}

    wanted = [' T ',' B ',' b ']

    numbad = 0
    for line in lines:
        tag = line[16:19]
        if tag not in wanted:
            continue
        parts = line.split(tag)
        if len(parts) != 2:
            numbad += 1
            continue
        syms[parts[1]] = parts[0]

    if numbad:
        print("kallsyms: Skipped %d malformed lines"%numbad)

    return syms

#------------------------------------------------------------------------------
#
# Function: linuxVersion - Return Linux equivalent of "uname -r"
#
# Usage:    version = linuxVersion()
#
# Where:    version - str: ***see "uname -r"
#

_version = None

def linuxVersion():

    global _version

    if not _version:
        _version = execute('uname -r',showout=False)[1]

    return _version

#-------------------------------------------------------------------------------
#
# Functions: sha256_checksum - Compute sha256 checksum of file
#            sha1_checksum   - Compute sha1 checksum of file
#

def sha256_checksum(filename, block_size=65536):
    sha256 = hashlib.sha256()
    with open(filename, 'rb') as f:
        for block in iter(lambda: f.read(block_size), b''):
            sha256.update(block)
    return sha256.hexdigest()

def sha1_checksum(filename, block_size=655536):
    sha1 = hashlib.sha1()
    with open(filename,'rb') as f:
        for block in iter(lambda: f.read(block_size),b''):
            sha1.update(block)
    return sha1.hexdigest()

#-------------------------------------------------------------------------------
#
# Function: osName   - Return 'Ubuntu', 'CentOS', or None based on OS version
#           isUbuntu - "Are we running on Ubuntu?"
#           isCentOS - "Are we running on CentOS?"
#

def osName():

    import platform
    osname,version,name = platform.linux_distribution()
    if 'Ubuntu' in osname:
        return 'Ubuntu'
    elif 'CentOS' in osname:
        return 'CentOS'
    return None

def isUbuntu():

    return osName() == 'Ubuntu'

def isCentOS():

    return osName() == 'CentOS'

#-------------------------------------------------------------------------------
#
# Function: which - Search $PATH for executable
#
# Usage:    where = which(name)
#
# Where:    name  - str: Executable name
#
#           where - str: Where name is founc ('' if not found)
#
# Notes:    If found, we don't check it's executable or not so this may need to
#           change later. All for performance/lazy reasons.  ;-)
#

def which(name):

    if 'PATH' not in os.environ:
        return ''

    paths = os.environ['PATH'].split(':')
    for path in paths:
        fname = path+os.sep+name
        if os.path.exists(fname) and os.path.isfile(fname):
            return fname

    return ''

#-------------------------------------------------------------------------------
#
# Function: exceptionLines - Turn exception traceback into printable lines
#
# Usage:    lines = exceptionLines(exc,lines=[])
#
# Where:    ext   - Exception: Exception to format
#           lines - list:      Optional prefix lines (we append to this)
#
#           lines - list:      Line by line formatted traceback
#

def exceptionLines(exc,lines=[]):

    import traceback

    if type(lines) == str:
      lines = lines.split('\n')
    elif lines == None:
      lines = []

    if not '__traceback__' in dir(exc):
        lines = ['***exceptionLines(): Not called with Exception***']
    else:
        tb = exc.__traceback__
        bogusLines = list(traceback.format_exception(Exception,exc,tb))
        for line in bogusLines:
            if line and line[-1] == '\n':
                line = line[:-1]
            these = line.split('\n')
            for that in these:
                lines.append(that)
    return lines

#-------------------------------------------------------------------------------
#
# Function: sudo - Restart running process as sudo if not already
#                  (!!!Danger Danger!!!)
#
# Usage:    didit = sudo()
#
# Where:    didit - bool: True = "We weren't root", False = "We were already root"
#
# Notes:    Typically this is called from some python __main__ that knows it
#           needs to be running as root.
#
#           DO NOT USE THIS UNLESS YOU KNOW WHAT YOU'RE DOING!!!
#

def sudo():

  # Switch to root if we're not already...
  import os
  if os.getuid():
    import subprocess
    import sys # ◄── No need to do this if already imported ;-)
    sys.argv.insert(0,sys.executable)
    sys.argv.insert(0,"sudo")
    execute(sys.argv,showout=True,prefix='')

    return True
 
  return False

#-------------------------------------------------------------------------------
#
# Function: ipAddresses   - Return iterable of host's IP addresses
#           ipv4Addresses - Return iterable of host's IPv4 addresses
#
# Usage:    addrs = ip[v4]Addresses()
#
# Where:    addrs - iterable: Known IP[v4] addresses (elements of type str)
#
# Note:     ipv4Addresses filters out '127.0.0.1' because we KNOW it's there
#           don't we?
#

def ipAddresses():

    try:
        import netifaces
    except:
        return ()

    ips = []

    for interface in netifaces.interfaces():

        ifInfo = netifaces.ifaddresses(interface)
        for info in ifInfo:
            elms = ifInfo[info]
            for elm in elms:
                if 'addr' in elm: ips.append(elm['addr'])

    return tuple(ips)

#-------------------------------------------------------------------------------
#
# Function: ipv4Addresses - Return iterable of host's IPv4 addresses
#
# Usage:    addrs = ipv4Addresses()
#
# Where:    addrs - iterable: Known IPv4 addresses (elements of type str)
#

def ipv4Addresses():

    addrs = ipAddresses()
    ipv4 = []
    for addr in addrs:
        parts = addr.split('.')
        if len(parts) != 4 or addr == '127.0.0.1': 
            continue
        ipv4.append(addr)

    return tuple(ipv4)

#--------------------------------------------------------------------------------
#
# Function: packages - Return list of installed packages
#
# Usage:    packs = packages()
#
# Where:    packs - list: List of installed package names
#

def packages():

  packs = []

  if isUbuntu():

    # Ask apt...
    rc,out,err = execute("apt list --installed",showout=False)
    lines = out.split('\n')

    # For each line...
    for line in lines:

      if "Listing..." in line:
        continue

      pos = line.find("/")
      if pos < 0 or "Listing..." in line:
        continue

      pack = line[:pos]
      if pack not in packs:
        packs.append(pack)

  elif isCentOS():

    # Ask yum...
    rc,out,err = execute("yum list installed",showout=False)
    lines = out.split('\n')

    # For each line...
    for line in lines:
   
     if "Installed Packages" in line:
       continue

     parts = line.split(" ")
     if not parts:
       continue
     parts = parts[0].split('.')
     if not parts:
       continue
  
     pack = parts[0]
     if pack not in packs:
       packs.append(pack)

  packs.sort()

  return packs

#------------------------------------------------------------------------------ 
#
# Functions: red   - Print red error
#            error - Print red error and exit with status
#            green - Print green message
#            color - Use colr.color if stdout is a tty
#

def red(msg):

  print(color("ERROR: "+msg,fore='red',style='bright'))

def blue(msg):

  print(color(msg,fore='blue',style='bright'))

def green(msg):

  print(color(msg,fore='green',style='bright'))

def error(msg,exit=True):

  red(msg)
  if exit:
    sys.exit(1)

def color(*args,**kwargs):

  if not sys.stdout.isatty():
    return args[0]
  return colr.color(*args,**kwargs)

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == "__main__":

    if 0:
        args = list(sys.argv[1:])
        print("args = %s"%repr(args))
        hosts = parseHosts(args)
        print("hosts = %s"%repr(hosts))

    if 0:
        Error("Dude!!!\nDude1",nl=False)
        Error("Dude2!!!")

    if 0:
        b = os.urandom(20)
        s = bytesToHex(b)
        print("b = %s"%repr(b))
        print("s = %s"%repr(s))
   
    if 0: 
        s = 'abcdef0102030405'
        b = hexToBytes(s)
        print("s = %s"%repr(s))
        print("b = %s"%repr(b))

    if 0:
        print("%s IP addresses..."%repr(getHostName()))
        addresses = ipv4Addresses()
        for addr in addresses:
            print("  "+addr)
        if not addresses:
            print("  ***None***")

    if 0:

      rc,out,err = execute("ls -al",showout=True,prefix='')
      print("DEBUG: len(out) = %d"%len(out))

    if 0:

      s = "30"
      b = hexToBytes(s)
      print("b = %s"%repr(b))

    if 0:

      packs = packages()
      print("%d packages..."%len(packs))

      for i in range(0,len(packs)):
        if i == 10:
          print('...')
        elif i < 10 or i >= len(packs)-10:
          print("  "+packs[i])

    if 0:
    
      s = '0123456789112345678921234567893123456789412345678951234567896'
      pre  = preEllipse(s,50,fill='.')
      post = postEllipse(s,50,fill='.')
      print('preEllipse:  %s'%repr(pre))
      print('postEllipse: %s'%repr(post))

    if 1:

      s = dtToReadable(dtNow())
      print('Now: '+s) 
