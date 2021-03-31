#!/usr/bin/python3
#coding=utf-8
#==============================================================================
#
# File: repos.py - Data declaring all Cyberhive repos
#

import os
import sys
from   MinExps import MinExps

# Dictionary of name:<dir>
dirs = [
  "com[mon]",
  "http[d[aemon]]",
  "[p[ython]]oat[d[aemon]]",
  "kern[el]",
  "c[md]l[ine]i[nterface]",
  "j[ava]t[rust]agent",
  "p[ython]t[rust]agent",
  ]

dirs = MinExps(dirs)
dirs["common"]           = "../cyberhive-common-python"
dirs["http"]             = "../cyberhive-http-python"
dirs["pythonoatdaemon"]  = "../cyberhive-oat-python"
dirs["kernel"]           = "../cyberhive-kernel-module"
dirs["cmdlineinterface"] = "../cyberhive-admin-cli"
dirs["javatrustagent"]   = "../cyberhive-trust-agent-java"
dirs["pythontrustagent"] = "../cyberhive-trust-agent-python"
dirs["ibmtpm"]           = "../cyberhive-ibmtpm"
dirs["ibmtss"]           = "../cyberhive-ibmtss"
dirs["ibmacs"]           = "../cyberhive-ibmacs"
dirs["attestation"]      = "../cyberhive-attestation"


# Resolve these to full paths...
for key in dirs.realkeys():
    relDir = dirs[key]
    dirs[key] = os.path.abspath(os.path.dirname(__file__)+os.sep+relDir)

#-------------------------------------------------------------------------------
#
# Function: installed - "Is this repo installed?"
#
# Usage:    answer = installed(repo)
#
# Where:    answer - bool: False = "Is not installed", True = "Is installed"
#           repo   - str:  Repository short name
#

def installed(repo):

    if repo not in dirs:
        return False

    dir = dirs[repo]
    if os.path.exists(dir) and os.path.isdir(dir):
        return True

    return False

#-------------------------------------------------------------------------------
#
# __main__
#

def exit(code=None):

    if type(code) == str:
        print("Error: "+code)
        code = 1
    elif not code:
        code = 0
    elif type(code) != int:
        print("Unknown exit code %s"%repr(code))
        code = 1

    sys.exit(code)

if __name__ == "__main__":

    args = sys.argv[1:]

    if not args:

        maxLen1 = 0
        maxLen2 = 0
        for key in dirs.realkeys():
            maxLen1 = max(maxLen1,len(key))
            maxLen2 = max(maxLen2,len(dirs[key]))

        keys = list(dirs.realkeys())
        keys.sort()

        for key in keys:
            inst = "Not Installed" if not installed(key) else "Installed"
            print("%s%s = %s%s (%s)"%(key,' '*(maxLen1-len(key)),repr(dirs[key]),' '*(maxLen2-len(dirs[key])),inst))

        exit()

    exit("%s: Arguments not implemented yet"%repr(__file__))
