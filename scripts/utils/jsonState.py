#!/usr/bin/python3
#coding=utf-8
from __future__ import print_function
#==============================================================================
#
# File:      jsonState - Return a Jsonable Swift state object
#
# Functions: jsonState - Return a Jsonable Swift state object
#
#==============================================================================

from funcs      import *
from jsonFuncs    import *

import swift
import config
import json
import sys

#-------------------------------------------------------------------------------
#
# Function: jsonState - Return a Jsonable Swift state object
#
# Usage:    obj = jsonState(host=None,ts=None,filtered=False)
#
# Where:    obj      - dict: Described below
#           host     - str:  Host (None = "All Hosts", not None = "Specific host")
#           ts       - str:  Valid Timestamp (yyyy-mm-ddThh:mm:ss.mmm)
#           filtered - bool: True="Filtered", False="Attested"
#
# Notes:    If an error occurs obj will contain a jsonFuncs 'errors' object.
#

def jsonState(host=None,ts=None,filtered=False):

    from HostStatus import HostStatus

    config.load()

    # Sanity checks...
    if host:
        if host not in config.globals.hosts:
            return jsonErrorsObject("Host %s is not configured (see porridge.json)"%repr(host))
    if type(ts) is int:
        ts = str(ts)
    if ts and not isValidNonce(ts) and not isInt(ts):
        return jsonErrorsObject("%s is not a valid timestamp"%repr(ts))
    if ts and not host:
        return jsonErrorsObject("Timestamp %s without host cannot be processed"%repr(ts))

    # Connect to Swift...
    sc = swift.SwiftConnection()
    sc.connect()

    # Get list of 'attested#...' objects...
    prefix = 'attested#' if not filtered else 'attested.filtered#'
    objNames = sc.listObjects()
    process = []
    slot = []
    lookFor = prefix
    lookFor2 = 'explicitTrust#'

    for objName in objNames:

        if not objName.startswith(lookFor) and not objName.startswith(lookFor2):
            continue

        parts = splitName(objName)
        if len(parts) != 3:
            continue

        if host and parts[1] != host:
            continue

        if ts and not isInt(ts) and parts[2] != ts:
            continue

        if objName.startswith(lookFor2):
            slot.append(objName)
        else:
            process.append(objName)

    process.sort()

    # For each 'slot' ones (explicit trust), slot them into after last 'attested.filtered#' for same host...
    # Note: This will fail horribly if process isn't sorted. ;-)
    for explicit in slot:
        parts = splitName(explicit)
        thatHost = parts[1]
        start = None
        end   = None
        for i in range(0,len(process)):
            parts = splitName(process[i])
            thisHost = parts[1]
            if thisHost == thatHost:
                if start == None:
                    start = i
                end = i+1
            elif thisHost > thatHost:
                end = i
                break
        if end == None:
            end = len(process)
        process.insert(end,explicit)

    # If host not specified we only want to process the last attestation...
    if not host:
        out = {}
        for objName in process:
            parts = splitName(objName)
            out[parts[1]] = objName
        process = []
        for key in out:
            process.append(out[key])

    # Build the output dictionary from attested in 'process'...
    out = {}
    for objName in process:
        parts = splitName(objName)
        if not parts:
            continue
        if parts[1] not in out:
            out[parts[1]] = []

        if not objName.startswith('explicitTrust#') and objNameAge(objName) > config.globals.maxage:
            continue

        inStatus = HostStatus()
        okay = inStatus.fromSwift(objName)
        if not okay:
            error = "jsonState: Swift object %s invalid"%repr(objName)
            return jsonErrorsObject(error)

        outStatus = {}
        outStatus['name']       = inStatus.name
        outStatus['ip']         = inStatus.ip
        outStatus['status']     = inStatus.status
        outStatus['why']        = inStatus.why
        outStatus['tsQuote']    = inStatus.tsQuote
        outStatus['tsAttest']   = inStatus.tsAttest
        outStatus['by']         = inStatus.by
        outStatus['exceptions'] = inStatus.exceptions
        outStatus['error']      = inStatus.error
        outStatus['bios']       = inStatus.bios
        outStatus['vmm']        = inStatus.vmm
        outStatus['location']   = inStatus.location
        outStatus['ima']        = inStatus.ima

        out[parts[1]].append(outStatus)

    # If host/ts set, filter...
    keys = list(out.keys())
    if host:
        for key in keys:
            if key != host:
                del out[key]
        if not out:
            return jsonErrorsObject("Host %s has no current attestation"%repr(host))
        if ts:
            attests = out[host]
            if isInt(ts):
                whichOne = int(ts)
                if whichOne < 0:
                    return jsonErrorsObject("Timestamp %d must be between 0 and %d"%(whichOne,len(attests)-1))
                thisOne = attests[len(attests)-1-whichOne]['tsAttest']
            else:
                thisOne = ts
            while attests and attests[0]['tsAttest'] != thisOne:
                del attests[0]
            if not attests:
                return jsonErrorsObject("Host %s has no attestation at %s"%(repr(host),repr(ts)))
            del attests[1:]

    # Finish up the jsonFuncs object...
    out = {'state':out}
    out['timestamp'] = dtToNonce(dtNow())
    out = {'data':out}

    return out
