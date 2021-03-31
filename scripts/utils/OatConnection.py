#!/usr/bin/python3
# coding=utf-8
#===============================================================================
#
# File: OatConnection.py - OatConnection class
#
# Class: OatConnection - Porridge Oat connection (ZeroMQ Messages)
#
#===============================================================================

from   Struct     import Struct
from   Enum       import Enum
from   funcs      import *
from   jsonFuncs  import *
from   OatMessage import *
import zmq
import globals

class OatConnection:

    prefix = "OatConnection: "
    Type   = Enum("notset,client,server")

    port   = '9996'                # ◄── Both admin and OAT are listening in this port
                                   #       1) OAT is listening for admin requests
                                   #       2) Admin is listening for...
                                   #          a) OAT success/failure reports
                                   #          b) HTTPs/Admin status requests

    def typeError(msg):

        raise TypeError(Oat.prefix+msg)

    def __init__(self,hostname=None):

        self.hostname   = None
        self.context    = None
        self.poll       = None
        self.socket     = None

        if hostname:
            self.type = OatConnection.Type.client
            self.connect(hostname)
            self.hostname = hostname
        else:
            self.type = OatConnection.Type.notset

    #def __del__(self):

    #  self.reset()

    def reset(self):

        self.hostname   = None
        if self.context:
            self.context.destroy()
            self.context.term()
        self.context    = None
        self.poll       = None
        self.socket     = None

    def connect(self,hostname=None):

        typ = OatConnection.Type
        if self.type == typ.notset:
            self.type = typ.client
        elif self.type != typ.client:
            raise RuntimeError("Can only connect if type OatConnection.client")

        if hostname == 'localhost':
            hostname = getHostName()

        if not hostname:
            if not self.hostname:
                TypeError("No hostname specified")
            hostname = self.hostname
        if type(hostname) is not str:
            typeError("Expected hostname to be a string")
        self.reset()
        self.hostname = hostname

        if not self.context:
            self.context = zmq.Context()
        context = self.context

        socket = context.socket(zmq.REQ)
        socket.connect("tcp://%s:%s" % (hostname,OatConnection.port))

        poll = zmq.Poller()
        poll.register(socket, zmq.POLLIN)

        self.hostname = hostname
        self.context  = context
        self.poll     = poll
        self.socket   = socket

        # Tell OAT who we are...
        msg = OatMessage(connection=self)

        return True

    def disconnect(self):

        hostname = self.hostname
        self.reset()
        self.hostname = hostname

    def reconnect(self):

        self.disconnect()
        self.connect()

    def isConnected(self):

        return self.socket is not None

    # Send a String message...
    def send(self,msg):

        if not self.socket:
            Error("OatMessage.send: Not connected (socket missing)")
            return False

        if type(msg) is str or type(msg) is not bytes:

            if type(msg) == OatMessage:
              msg = msg.toString()

            if (type(msg)) is not str:
                if not isJsonable(msg):
                    raise Exception("OatConnection.send: Message isn't Jsonable")
                msg = json.dumps(msg,indent=2)
                prettyMsg = msg
            else:
                prettyMsg = json.dumps(jsonFuncs.loads(msg,expectdata=True),indent=2)

            if globals.python2:
                msg = unicode(msg)
            else:
                msg = bytes(msg,'utf-8')

        if globals.veryverbose:
            print("OatConnection.send: Sending... %s"%prettyMsg)
            flushOut()

        try:

            if globals.python2:
                self.socket.send_string(msg)
            else:
                self.socket.send(msg)

            #print("OatConnection.send: Sent %d bytes"%len(msg))

        except zmq.ZMQError as z:

            print("Trying to recover from ZMQError %s..."%repr(str(z)))
            self.reconnect()
            self.socket.send(msg)
            print("...Recovered!!!")

        return True

    # Recieve a String message...
    def recv(self):

        if not self.poll or not self.socket:
            raise Exception("OatConnection.recv: Not connected (socket missing)")
        poll   = self.poll
        socket = self.socket
        socks = dict(poll.poll(10000))
        msg = None
        if socks.get(socket) == zmq.POLLIN:
            msg = socket.recv()
            if type(msg) is bytes:
                msg = msg.decode('utf-8')

        return msg

    # Send a String message and return String response...
    def sendrecv(self,msg):

        okay = self.send(msg)
        if not okay: return None
        response = self.recv()

        return response

    # Listen for OAT messages...
    def listen(self,handler,wait=10,verbose=False,timer=None):

        import socket

        if not wait:
            wait = 10

        if timer and not hasattr(timer, '__call__'):
            raise TypeError("Expected timer to be callable")

        if wait and type(wait) not in (float,int):
            raise TypeError("Expected wait to be float or int in seconds")

        self.reset()
        self.type = OatConnection.Type.server

        # Open context, socket, and poll...
        if not self.context:
            self.context = zmq.Context()
        ourName = socket.gethostname()

        print('')
        listenOn = '*:'+str(OatConnection.port)
        print("Listening on %s..."%repr(listenOn))
        now = dtNow()

        self.socket = self.context.socket(zmq.REP)
        self.socket.bind("tcp://" + listenOn)
        self.poll = zmq.Poller()
        self.poll.register(self.socket, zmq.POLLIN)

        print("Admin Listening on port %s..."%repr(OatConnection.port))

        iteration = 0

        while True:

            if timer and deltaSeconds(dtNow()-now) >= wait:
                now = dtNow()
                timer()

            iteration += 1

            socks = dict(self.poll.poll(1000))

            if socks.get(self.socket) != zmq.POLLIN:
                if timer: timer()
                continue

            msg = OatMessage(connection=self)

            okay = msg.recv()
            if not okay:
                raise RuntimeError("OatConnection.listen: recv failed")

            if globals.veryverbose:
                if msg.request:
                    print("OatConnection.listen: Received request:  %s"%json.dumps(msg.request,indent=2))
                else:
                    print("Oatconnection.listen: No request")
                if msg.response:
                    print("OatConnection.listen: Recieved response: %s"%json.dumps(msg.response,indent=2))
                else:
                    print("OatConnection.listen: No response")
                if msg.errors:
                    print("OatConnection.listen: Recieved errors:    %s"%json.dumps(msg.errors,indent=2))
                else:
                    print("OatConnection.listen: No errors")

            response = handler(msg)
            if response != None and type(response) != OatMessage:
                print("  Ignoring response (type=%s)"%repr(type(response)))
                continue
            if response:
                response.send()

            continue

        self.reset()
