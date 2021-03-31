#!/usr/bin/python3
#coding=utf-8
#===============================================================================
#
# File: Heartbeat.py - OAT Heartbeat class
#
# Purpose: Send hearbeat to a ZMQ OAT server
#
#===============================================================================

import uuid

from OatConnection import OatConnection
from OatMessage    import OatMessage
from funcs         import *

#-------------------------------------------------------------------------------
#
# Class: Heartbeat - An OAT Heartbeat
#
# Returns: int - -1 = Dead (timeout)
#                 0 = Invalid response
#                 1 = Alive and working properly
#
#-------------------------------------------------------------------------------

class Heartbeat():

    def __init__(self,connection=None):

        self.connection      = connection
        self.status          = None
        self.queueSize       = None
        self.queue           = []
        self.heapMemoryStart = None
        self.heapMemoryEnd   = None
        self.response        = None

    def beat(self):

        if not self.connection:
            return -1

        msg = OatMessage(self.connection)
        msg.request = {"action":"heartbeat","requestFrom":getHostName()}

        msg.send()
        okay = msg.recv()
        result = 0
        if msg.errors:
            print("Heartbeat Error: %s"%repr(msg.errors[0]['detail']))
        elif not okay:
            result = -1
        else:
            result = 1

        #if not msg.errors:
        #  print("Heartbeat %s: Response = %s"%(repr(self.connection.hostname),repr(msg.response)))
        if msg.response:
            self.response = response = msg.response
            if "msgQueue.size" in response:
                self.queueSize = response["msgQueue.size"]
            if "heapMemoryStart" in response:
                self.heapMemoryStart = response["heapMemoryStart"]
            if "heapMemoryEnd" in response:
                self.heapMemoryEnd = response["heapMemoryEnd"]
            if "msgQueue" in response:
                self.queue = response["msgQueue"]

        return result
