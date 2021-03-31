#!/usr/bin/python3
# coding=utf-8
#===============================================================================
#
# File: OatConnection.py - OatConnection and related classes
#
# Classes: OatConnection - Porridge Oat connection (All JSON API stuff)
#          OatMessage    - Porridge Oat message (Request and Reply)
#
#===============================================================================

from   Struct    import Struct
from   Enum      import Enum
from   funcs     import *
from   jsonFuncs   import *
import uuid
import zmq
import globals

from OatConnection import *

#-------------------------------------------------------------------------------
#
# Class: OatMessage - An OAT Message struct
#

class OatMessage(Struct):

    members = "connection,request,response,errors"

    def __init__(self,connection=None):

        import OatConnection

        super(OatMessage,self).__init__(OatMessage.members)

        if not globals.python2 and connection is not None and type(connection) != OatConnection.OatConnection:

            raise TypeError("OatMessage.__init__: " +
                            "Expected connection to be an OatConnection instance" +
                            ", got type %s"%repr(type(connection))
                           )

        self.connection = connection
        self.request    = {}
        self.response   = {}
        self.errors     = {}

    def __str__(self):

        return self.toString()

    def toString(self):

        data = {'data':{}}
        if self.request:
            data['data']['request'] = self.request
        if self.response:
            data['data']['response'] = self.response
        if self.errors:
            data['errors'] = self.errors
        ret = json.dumps(data,indent=2)

        return ret

    def send(self):

        if not self.connection or not self.connection.socket:
            raise Exception("OatMessage.send: Not connected (socket missing)")

        data = {}
        if self.errors:
            data["errors"] = self.errors
        if self.request or self.response:
            data["data"] = {}
        if self.request:
            data["data"]["request"] = self.request
        if self.response:
            data["data"]["response"] = self.response
        #if not data:
        #    raise Exception("OatMessage.send: Nothing to send")

        # Fill in essentials...
        if self.request:
            self.request["requestFrom"] = getHostName()
            self.request["id"]          = '{' + str(uuid.uuid4()) + '}'

        # Send it...
        strMessage = json.dumps(data,indent=2)
        self.connection.send(strMessage)

        return True

    def recv(self,socket=None):

        if not self.connection or not self.connection.socket:
            raise Exception("OatMessage.recv: Not connected (socket missing)")
        poll   = self.connection.poll
        if not socket:
            socket = self.connection.socket
        socks = dict(poll.poll(4000))
        okay = True
        if socks.get(socket) == zmq.POLLIN:
            msg = socket.recv()
            if type(msg) == bytes:
                msg = msg.decode('UTF-8')
            self.request = self.response = self.errors = None
            if isJsonResponse(msg):
                self.response = jsonFuncs.loads(msg,expectdata=True)['data']['response']
            if isJsonRequest(msg):
                self.request  = jsonFuncs.loads(msg,expectdata=True)['data']['request']
            if isJsonErrors(msg):
                self.errors = jsonFuncs.loads(msg)['errors']
            #if not self.request and not self.response and not self.errors:
            #    print("OatMessage.recv: Malformed message length %d..."%len(msg))
            #    print("OatMessage.recv: msg = %s"%msg)
        else:
            okay = False

        return okay

    def sendrecv(self):

        self.send()
        okay = self.recv()

        return okay

    def getJsonApi(self):

        if self.errors:
            return {'errors':self.errors}

        return {'data':{'request':self.request,'response':self.response}}

    def getErrorsDetails(self):

        if not self.errors:
            return None

        details = []
        for error in self.errors:
            details.append(error['detail'])

        return details

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ ==  "__main__":

    if False:

        import config
        config.load()
        if not config.oats:
            Error("No OATs configured (see porridge.json)")
            sys.exit()
        for oat in config.oats:
            con = OatConnection(config.oats[0])
            okay = con.connect()
            if not okay:
                print("OatConnection() failed")
                continue
            print("Sending heartbeat to %s..."%repr(oat))
            msg.request = {'action':'heartbeat','requestFrom':getHostName()}
            msg = OatMessage(con)
            if not okay:
                print("  Heartbeat failed")
            elif msg.errors:
                print("  Heartbeat errors")
            else:
                print("  Heartbeat succeeded: msg.response['status'] = %s"%msg.response['status'])

    if True:

        msg = OatMessage()
        msg.errors = jsonErrorsObject("Dude!!!")
        details = msg.getErrorsDetails()
        print("getErrorsDetails = %s"%repr(details))
