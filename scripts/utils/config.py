#!/usr/bin/python3
# coding=utf-8
#===============================================================================
#
# File:    config.py
#
# Purpose: Utility functions to manage Porridge configuration files
#
# Notes:   Configuration files ('porridge.json') are located by looking in the
#          following places and in the following order...
#
#            1) /etc/intel/cloudsecurity
#            2) /home/porridge
#            3) /etc/porridge
#            4) ~
#            5) If radosgw-admin available: Ceph object 'porridge.json' (if time)
#
#          If the file isn't found, an exception is raised.
#
#          These globals are refreshed each time a new config object is
#          instatiated, allowing us to "refresh" on demand.
#
#          Since getting an object from RadosGW takes a bit of time, we defer
#          re-loading (if from RadosGW) to a configured delay.
#
#===============================================================================

import sys
import getopt
try:          # ◄── We do this so we'll work in Python 2 and 3
    import configparser
except:
    import ConfigParser as configparser
import os
import subprocess
import json
import jsonFuncs
from   errors     import *
from   funcs      import *
from   globals    import unicode
from   DotDict    import DotDict
from   debug      import *
import swift
import dbfuncs

#
# config Globals
#

params              = None       # ◄── Contains the Json in porridge.json

globals   = DotDict()
globals.checkRadosGW = bool(which('radosgw-admin')) # ◄── "Check for 'porridge.json' in Rados?

lastLoaded  = 0 # ◄── Last dtNow() of config.load()
configDelay = 1 # ◄── Number of seconds config.load() delays

#-------------------------------------------------------------------------------
#
# Function: reset - Reset all config globals
#

def reset():

    global globals

    keys = list(globals.keys())
    for key in keys:
        if key != 'fileName':
            del globals[key]

    if 'fileName' not in globals:
        globals.fileName = ''

    globals.file      = ''
    globals.configLoaded  = False      # ◄── "Have we already loaded 'porridge.json'?
    globals.radosGWLoaded = False      # ◄── "Have we already asked RadosGW for swift user/key?"
    globals.radosGWDelay  = 30         # ◄── Delay re-opening ceph 'porridge.json'
    globals.radosGWLast   = 0          # ◄── Last date/time we opened ceph 'porridge.json'
    globals.hosts         = {}
    globals.quotetime     = 0
    globals.gateways      = []
    globals.gateway       = ''
    globals.admins        = []
    globals.oats          = []
    globals.pcas          = []
    globals.mysql         = DotDict()
    globals.pools         = []
    globals.hypervisors   = []
    globals.vms           = []
    globals.services      = []
    globals.swiftuser     = ''
    globals.swiftkey      = ''
    globals.swiftcontainer= ''
    globals.maxage        = 0
    globals.adminsaveevery= 0

reset() # ◄── Reset globals when module loads

#------------------------------------------------------------------------------
#
# Function: loadJson - Load 'porridge.json' from wherever you can find it.
#
# Usage:    jsonstr = loadJson()
#
# Where:    jsonstr - str: Contents of 'porridge.json' ('' = Not found)
#
# Notes:    We first search local disk for the 'porridge.json' (see findFile).
#           If not found AND we can find radosgw-admin, we try loading it from
#           RadosGW (ceph) instead.
#

def loadJson(fileName=globals.fileName):

    # Compute file name (or swift object name)...
    if not fileName:

        fn = "porridge.json"
        fileName   = findFile(fn)

    if not fileName:
        return ({},'')

    confFile = open(fileName)
    contents = confFile.read()

    return (contents,fileName)

#-------------------------------------------------------------------------------
#
# Function: load - Load config parameters from configuration files
#

def load():

    global globals
    global lastLoaded

    # Don't reload if too soon
    now = dtNow()
    if lastLoaded and deltaSeconds(now-lastLoaded) < configDelay:
        return
    lastLoaded = now

    # Reset first...
    reset()

    # Load json contents...
    contents,fileName = loadJson()
    if not contents:
        raise InternalError("No configuration file found ANYWHERE!!!")

    #
    # Load it...
    #

    try:
        globals = DotDict(jsonFuncs.loads(contents))
    except Exception as e:
        # TEMP...
        print("Internal error: exception = %s"%str(e))
        # ...TEMP
        raise RuntimeError(str(e))
    globals.fileName = fileName

    # Make sure expected sections are in config file...
    expect = ['hosts','admins','pcas','oats','gateways']
    missing = 0
    for label in expect:
        if label not in globals:
            missing += 1
            print("Error: Mandatory configuration %s missing"%repr(label))
    if missing:
        raise RuntimeError("Mandatory section%s missing from %s" %
                             ('' if missing == 1 else 's',repr(globals.fileName)))

    # Load 'hosts' first...
    if type(globals.hosts) != dict:
        raise RuntimeError("Expected 'hosts' to be a dict in %s"%repr(globals.fileName))
    globals.hosts = DotDict(globals.hosts)
    for hostName in globals.hosts.keys():
        host = globals.hosts[hostName]
        if type(host) != dict:
            raise RuntimeError("Host %s is not a Json dictionary"%repr(host))
        globals.hosts[hostName] = DotDict(host)

    # Load unstructured stuff...
    if 'swiftcontainer' not in globals:
        raise RuntimeError("globals.swiftcontainer missing in %s"%repr(globals.fileName))

    if 'maxage' in globals and not isInt(globals.maxage):
        raise RuntimeError("Expected globals.maxage to be an integer in %s"%repr(globals.fileName))
    if 'maxage' not in globals:
        globals.maxage = 300

    if 'quotetime' not in globals and not isInt(globals.quotetime):
        raise RuntimeError("Expected globals.quotetime to be an integer in %s"%repr(globals.fileName))

    if 'user' not in globals:
        raise RuntimeError("Missing globals.user in %s"%repr(globals.fileName))
    if type(globals.user) not in (str,unicode):
        raise RuntimeError("Expected globals.user to be a string in %s"%repr(globals.fileName))


    if 'quotetime' not in globals:
        globals.quotetime = 300

    if 'adminsaveevery' in globals:
        if not isInt(globals.adminsaveevery):
            raise RuntimeError("Expected globals.adminsaveevery. to be an integer in %s"%repr(globals.fileName))
    else:
        globals.adminsaveevery = 300

    if 'attestdelay' in globals:
        if not isInt(globals.attestdelay):
            raise RuntimeError("Expected globals.attestdelay to be an integer in %s"%repr(globals.fileName))
    else:
        globals.attestdelay = 0

    if globals.attestdelay < 0:
        globals.attestdelay = 0

    # Load 'gateways'...
    if 'gateways' not in globals:
        raise RuntimeError("'gateways' missing in %s"%repr(globals.fileName))
    if type(globals.gateways) != list:
        raise RuntimeError("Expected 'gateways' to be a list in %s"%repr(globals.fileName))
    if len(globals.gateways) < 1:
        raise RuntimeError("Expected at least 1 'gateways' entry in %s"%repr(globals.fileName))
    for gateway in globals.gateways:
        if gateway not in globals.hosts:
            raise RuntimeError("Gateway %s not configured in 'hosts' in %s"%(repr(globals.gateway),repr(globals.fileName)))

    # Load lists of hosts...
    numBad = 0
    things = ['admins','oats','pcas']
    for thing in things:
        if not thing in globals:
            raise RuntimeError("%s missing in %s"%(repr(thing),repr(globals.fileName)))
        if type(globals[thing]) is not list:
            raise RuntimeError("Expected %s to be a list of host names in %s"%(repr(thing),repr(globals.fileName)))
        for host in globals[thing]:
            if host not in globals.hosts:
                Error("%s host %s not configured in 'hosts' in %s"%(repr(thing),repr(host),repr(globals.fileName)))
                numBad += 1
    if numBad:
        raise RuntimeError("Misconfigured hosts in %s"%repr(globals.fileName))

    # Load 'mysql'...
    if 'mysql' not in globals:
        raise RuntimeError("'mysql' missing in %s"%repr(globals.fileName))
    globals.mysql = DotDict(globals.mysql)
    if 'server' not in globals.mysql:
        raise RuntimeError("mysql.server missing in %s"%repr(globals.fileName))
    if 'password' not in globals.mysql:
        raise RuntimeError("mysql.password missing in %s"%repr(globals.fileName))
    if 'user' not in globals.mysql:
        raise RuntimeError("mysql.user missing in %s"%repr(globals.fileName))
    if type(globals.mysql.server) not in (str,unicode):
        raise RuntimeError("Expected mysql.server to be a string in %s"%repr(globals.fileName))
    if globals.mysql.server not in globals.hosts:
        raise RuntimeError("Expected mysql.server %s is not a host in %s"%(repr(server),repr(globals.fileName)))
    if type(globals.mysql.password) not in (str,unicode) :
        raise RuntimeError("Expected mysql.password to be a string in %s"%repr(globals.fileName))
    if type(globals.mysql.user) not in (str,unicode):
        raise RuntimeError("Expected mysql.user to be a string in %s"%repr(globals.fileName))

    # Load 'pools'...
    if 'pools' not in globals:
        raise RuntimeError("'pools' missing in %s"%repr(globals.fileName))
    if type(globals.pools) != list:
        raise RuntimeError("Expected 'pools' to be a Map in %s"%repr(globals.fileName))
    numBad = 0
    numGood = 0
    for pool in globals.pools:
        if type(pool) != dict:
            Error("Expected pool '%s' to be a Map in %s"%(json.dumps(pool),repr(globals.fileName)))
            numBad += 1
            continue
        numGood += 1
    if numBad:
        raise RuntimeError("Misconfigured 'pools' in %s"%repr(globals.fileName))
    if numGood < 1:
        raise RuntimeError("Expected at least one 'pools' entry in %s"%repr(globals.fileName))

    # Load 'hypervisors'...
    if 'hypervisors' in globals:
        if type(globals.hypervisors) != dict:
            raise RuntimeError("Expected 'hypervisors' to be a Map in %s"%repr(globals.fileName))
        numBad = 0
        for hypervisor in globals.hypervisors.keys():
            if hypervisor not in hosts:
                Error("Hypervisor %s not configured in 'hosts'"%repr(hypervisor))
                numBad += 1
            tvms = globals.hypervisors[hypervisor]
            if type(tvms) != list:
                Error("Expected 'hypervisor'[%s] to be a List"%repr(hypervisor))
                numBad += 1
            for vm in globals.tvms:
                if type(vm) not in (str,unicode):
                    Error("Expected 'hypervisor'[%s] to be a List of Strings"%repr(hypervisor))
                    numBad += 1
                elif vm not in globals.hosts:
                    Error("'hypervisor'[%s] vm %s not configured in 'hosts'"%(repr(hypervisor),repr(vm)))
                    numBad += 1
                elif vm in globals.vms:
                    Error("Vm %s configured multiple times"%repr(vm))
                    numBad +1
                vms[vm] = hypervisor

        if numBad:
            raise RuntimeError("Misconfigured 'hypervisors' in %s"%repr(globals.fileName))

    # Load 'services'...
    if 'services' not in globals:
        globals.services = {}

    # Load RadosGW info...
    if 'checkRadosGW' not in globals:
        globals.checkRadosGW  = True
        globals.radosGWLoaded = False
    loadRadosGWInfo()

    # Get hosts from chas...
    try:
        dbcon,dbcur = dbfuncs.getMysqlConnection(server=globals.mysql.server,
                                                 user=globals.mysql.user,
                                                 password=globals.mysql.password,
                                                 db='chas')
        sql = "select name from core_machine order by name"
        count = dbcur.execute(sql)
        results = dbcur.fetchall()
        for row in results:
            host = row[0]
            if host not in globals.hosts:
                globals.hosts[host] = {'name':host}
    except Exception as e:
        lines = exceptionLines(e)
        print("***Failed to load 'chas' hosts***")
        for line in lines:
            print("..."+line)

    # While we're at it, default OAT's 'notify'...
    for oat in globals.oats:
        if oat in globals.hosts and 'notify' not in globals.hosts[oat]:
            globals.hosts[oat]['notify'] = 'localhost:9997'

#-------------------------------------------------------------------------------
#
# Function: save - Save modified config.params
#

def save(fileName = None):

    if 'params' in globals:
        del globals['params']

    params = eval(repr(globals))

    s = json.dumps(params,indent=2,sort_keys=True)

    if type(params) != dict:
        Error("config.save: params is not a dict")
        return

    if 'fileName' not in params:
        Error("config.save: 'fileName' missing from config.params")
        return

    try:
        out = open(params['fileName'],'w').write(s)
    except Exception as e:
        lines = exceptionLines(e,["***Runtime Exception***"])
        Error("Failed to open %s for writing...\n\n%s"%(repr(params['fileName']),'\n'.join(lines)))
        return

#-------------------------------------------------------------------------------
#
# Function: loadRadosGWInfo - Ask radosgw-admin for swift user info
#
# Note:     This can take a few seconds so we only do it ONCE!!!
#

def loadRadosGWInfo():

    if globals.radosGWLoaded:
        return

    result = "***execute('radosgw-admin...') failed***"
    try:
        result,out,err = execute("radosgw-admin user info --uid "+globals.user,showout=False)
        userinfo = jsonFuncs.loads(result)
        globals.swiftuser = userinfo['swift_keys'][0]["user"]
        globals.swiftkey  = userinfo['swift_keys'][0]["secret_key"]
    except:
        import traceback
        Internal("radosgw-admin user info returned bad Json")
        traceback.print_exc()
        #lines = result.split('\n')
        #for line in lines:
        #  print("..."+line)
        globals.radosGWLoaded = False
        return

    globals.radosGWLoaded = True

#
# Function: loadParameters - Load parameters from Json configuration file
#

def loadParameters():

    globals.fileName = 'porridge.json'
    paramFileName = findFile(globals.fileName)
    if not paramFileName:
        raise RuntimeError("Missing configuration file: %s"%repr(paramFileName))

    try:
        parameters = jsonFuncs.loads(open(paramFileName).read())
    except:
        raise RuntimeError("Configuration file %s has malformed Json in it"%repr(paramFileName))

    return parameters

#-------------------------------------------------------------------------------
#
# Function: findFile(filename)
#
# Purpose:  Finds filename following search path described in header
#
# Usage:    found = findFile(filename='porridge.json')
#
#           Where: filename - str: Config file name
#                  found    - str: Full path of found file ('' = "Not Found")
#

def findFile(filename):

    paths = ['/etc/intel/cloudsecurity','/home/porridge','/etc/porridge','~']

    for path in paths:
        fullpath = os.path.expanduser(os.path.join(path,filename))
        if not os.path.exists(fullpath) or \
           not os.path.exists(fullpath) or \
           not os.path.isfile(fullpath):
            continue
        return fullpath

    return ''

#------------------------------------------------------------------------------
#
# Function: isAdmin         - Check if we are an admin
#           isOat           - Check if we are an OAT
#           isClient        - Check if we are a  Client
#           isHypervisor    - Check if we are a  Hypervisor
#           isVm            - Check if we are a  VM
#
#           checkAdmin      - Check we're on an    'admin'      host and error if not
#           checkOat        - Check we're on an    'oat'        host and error if not
#           checkClient     - Check we're on a     'client'     host and error if not
#           checkHypervisor - Check if we're on a  'hypervisor' host and error if not
#           checkVm         - Check if we're on a  'client'     host and error if not
#

def isAdmin():
    load()
    return getHostName() in globals.admins

def isOat():
    load()
    return getHostName() in globals.oats

def isClient():
    load()
    return getHostName() in globals.hosts

def isHypervisor():
    load()
    return getHostName() in globals.hypervisors

def isVm():
    load()
    return getHostName() in globals.vms

def checkAdmin(cmd=''):

    if not isAdmin():
        if not cmd:
            Error("This command can only be used from an 'admin' host")
        else:
            Error("Command %s can only be used from an 'admin' host"%repr(cmd))
        return False
    return True

def checkOat(cmd=''):

    if not isOat():
        if not cmd:
            Error("This command can only be used from an 'OAT' host")
        else:
            Error("Command %s can only be used from an 'OAT' host"%repr(cmd))
        return False
    return True

def checkClient(cmd=''):

    if not isClient():
        if not cmd:
            Error("This command can only be used from a 'client' host")
        else:
            Error("Command %s can only be used from a 'client' host"%repr(cmd))
        return False
    return True

def checkHypervisor(cmd=''):

    if not isHypervisor():
        if not cmd:
            Error("This command can only be used from a 'hypervisor' host")
        else:
            Error("Command %s can only be used from a 'hypervisor' host"%repr(cmd))
        return False
    return True

def checkVm(cmd=''):

    if not isVm():
        if not cmd:
            Error("This command can only be used from a 'vm' host")
        else:
            Error("Command %s can only be used from a 'vm' host"%repr(cmd))
        return False
    return True

#-------------------------------------------------------------------------------
#
# Block:   __main__
#
# Purpose: Test when run from ccommand line
#

if __name__ == '__main__':

    y = yes
    n = no

    if y:

        load()

        print("Configuration...")
        print("  oats     = "+repr(globals.oats))
        hosts = list(globals.hosts.keys())
        hosts.sort()
        print("  hosts    = "+postEllipse(repr(hosts),200))
        print("  mysql    = "+repr(globals.mysql))
        print("  gateway  = "+repr(globals.gateway))
        print("  pools    = "+repr(globals.pools))

    if n:

        print("Saving...")
        print("  globals.fileName     = %s"%repr(globals.fileName))
        #save()
