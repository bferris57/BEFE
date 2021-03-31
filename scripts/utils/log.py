#!/usr/bin/python3
#coding=utf-8
#===============================================================================
#
# File: log.py - Daemon log functions
#
#===============================================================================

import logging
from   logging import handlers
import os
import sys

def exit(code):

    if type(code) == str:
        print(code)
        code = 1

    sys.exit(code)

def getLogger(name,stdout=False):

    # create named logger...
    if type(name) != str:
        name = '***Unknown***'
        logfile = 'unknown.log'

    if '.' in name:
        pos = name.find('.')
        logfile = name
        name = name[:pos]

    if os.sep in name:
        parts = name.split(os.sep)
        name = parts[-1]
        logfile = os.sep.join(parts)
    else:
        logfile = '/var/log/' + name + '.log'

    log = logging.getLogger(name)
    log.setLevel(logging.DEBUG)

    # create file/stdout handler which logs even debug messages
    if stdout:
        fh = logging.StreamHandler(sys.stdout)
    else:
        fh = logging.handlers.TimedRotatingFileHandler(logfile,when='midnight',backupCount=3)
    fh.setLevel(logging.DEBUG)

    # create formatter and add it to the handlers
    formatter = logging.Formatter('%(asctime)s.%(msecs)03d - %(name)s - %(levelname)7s - %(message)s','%Y-%m-%d@%H:%M:%S')
    fh.setFormatter(formatter)

    # add the handlers to the logger
    log.addHandler(fh)

    return log

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

    log = getLogger('porridge-oat')

    log.warning("WARNING!!!")
    log.info("INFO!!!")
    log.debug("DEBUG!!")
    log.error("ERROR!!!")
