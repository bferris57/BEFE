#!/usr/bin/python3
#coding=utf-8
#==============================================================================
#
# File:    swift.py - Various utility functions/classes for OpenStack Swift
#
# Classes: SwiftConnection - Swift client Connection
#          SwiftObject     - Struct mirroring a Swift Object
#
#==============================================================================

from Struct        import Struct
from OatConnection import OatConnection
from funcs         import *

# Porridge imports...
import re
import globals
try:
    import swiftclient
except:
    raise Exception("INTERNAL_ERROR: Package 'swiftclient' not installed")

#------------------------------------------------------------------------------
#
# Class: SwiftConnection - Wrapper for swiftclient.client.Connection
#

class SwiftConnection:

    prefix = 'SwiftConnection'
    emptyp = ' '*len(prefix)

    def __init__(self,user=None,key=None,authurl=None,container=None):

        self.container = container

        if not user or not key or not authurl or not container:
            import config
            config.load()
            if not user:
                user = config.globals.swiftuser
            if not key:
                key = config.globals.swiftkey
            if not authurl:
                authurl = config.globals.gateway
            if not container:
                container = config.globals.swiftcontainer

        self.user      = user
        self.key       = key
        self.authurl   = authurl
        self.container = container

        self.setAuth(self.user,self.key,self.authurl)
        self.conn = None

    def setAuth(self,user=None,key=None,authurl=None):

        self.setUser(user)
        self.setKey(key)
        self.setAuthUrl(authurl)

    def setUser(self,userName=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not userName:
            self.user = ''
        elif type(userName) != str and type(userName) != unicode:
            raise TypeError(prefix+".setUser: Expected user to be type str/unicode, got '%s' instead"%typeName(userName))
        else:
            self.user = userName

        return

    def setKey(self,key=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not key:
            self.key = ''
        elif type(key) != str and type(key) != unicode:
            raise TypeError(prefix+".setKey: Expected key to be type str/unicode, got '%s' instead"%typeName(key))
        else:
            self.key = key

        return

    def setAuthUrl(self,authurl=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not authurl:
            self.authurl = ''
        elif type(authurl) != str and type(authurl) != unicode:
            raise TypeError(prefix+".setAuthUrl: Expected authurl to be type str/unicode, got '%s' instead"%typeName(authurl))
        else:
            self.authurl = authurl

        return

    def getAuth(self):

        return {'user':self.user,'key':self.key,'authurl':self.authurl}

    def connect(self):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not self.user or not self.key or not self.authurl:
            raise RuntimeError(prefix+".connect: user, key, or authurl not set")

        # Disconnect if connected
        if self.conn:
            self.conn = None

        # Connect
        #print("DEBUG: swift.connect: user    = %s"%repr(self.user))
        #print("DEBUG: swift.connect: key     = %s"%repr(self.key))
        #print("DEBUG: swift.connect: authurl = %s"%repr(self.authurl))
        self.conn = swiftclient.client.Connection(user    = self.user,
                                                  key     = self.key,
                                                  authurl = self.authurl)

        return self

    def listContainers(self):

        # Connect if not already connected
        if not self.conn: self.connect()

        # Get the account info...
        info = self.conn.get_account()

        # Get the list of containers...
        containers = []
        for entry in info[1]:
            containers.append(entry['name'])

        return containers

    def createContainer(self,container):

        # Connect if not already connected
        if not self.conn: self.connect()

        # Get the account info...
        info = self.conn.get_account()

        # Get current list of containers...
        containers = self.listContainers()
        if container in containers:
            return "Container %s already exists"%repr(container)

        self.conn.put_container(container)

        # Make sure its there
        resp_headers, containers = self.conn.get_account()
        if container not in containers:
            return "Failed to create container %s"%repr(container)

        return None

    def deleteContainer(self,container):

        # Connect if not already connected
        if not self.conn: self.connect()

        # Get the account info...
        info = self.conn.get_account()

        # Get current list of containers...
        containers = self.listContainers()
        if container not in containers:
            return "Container %s doesn't exist"%repr(container)

        self.conn.delete_container(container)

    def listObjects(self,container=None,prefix=None):

        pprefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        # Connect if not already connected
        if not self.conn: self.connect()

        # We expect a container name...
        if not container:
            container = self.container
        if not container:
            raise RuntimeError(pprefix+".listObjects: Expected container name")

        # Get the objects (info is a list of dictionaries)...
        objs = []
        try:
          headers,info = self.conn.get_container(container,prefix=prefix)
          for entry in info:
              objs.append(entry['name'])
        except:
          pass
        return objs

    def getObjectNames(self,regexp=None):

        # Connect if not already connected
        if not self.conn: self.connect()

        names = self.listObjects(self.container)
        names.sort()

        # Handle optional regexp
        if regexp:
            try:
                regexp = re.compile(regexp)
            except Exception as e:
                raise RuntimeError("Bad regular expression %s: %s"%(repr(regexp),repr(str(e))))
                return

        objs = []
        for name in names:
            if regexp and not regexp.search(name):
                continue
            objs.append(name)

        return objs

    def getHostQuotes(self,host=None):

        # Connect if not already connected
        if not self.conn: self.connect()

        quotes = {}
        objs = self.getObjectNames()
        for obj in objs:
            idx = obj.find('#')
            if idx < 0: continue
            head = obj[:idx]
            if not head in ('quote','quote2'): continue
            parts = obj[idx+1:].split('@')
            if len(parts) != 2: continue
            client,ts = parts
            if host and client != host:
                continue
            if client not in quotes:
                quotes[client] = []
            quotes[client].append(obj)

        # Make sure they're sorted so quotes[client][-1] is "Most Recent"...
        for client in quotes:
            names = quotes[client]
            names.sort()
            quotes[client] = names

        if host and host in quotes:
            return quotes[host]

        return quotes

    def getHostAttestations(self,host=None,filtered=False):

        # Connect if not already connected
        if not self.conn: self.connect()

        attests = {}
        objs = self.getObjectNames()
        sw = 'attested#' if not filtered else 'attested.filtered#'
        for obj in objs:
            if not obj.startswith(sw): continue
            parts = obj[len(sw):].split('@')
            if len(parts) != 2: continue
            client,ts = parts
            if host and client != host:
                continue
            if client not in attests:
                attests[client] = []
            attests[client].append(obj)

        # Make sure they're sorted so attests[client][-1] is "Most Recent"...
        for client in attests:
            names = attests[client]
            names.sort()
            attests[client] = names

        if host and host in attests:
            return attests[host]

        return attests

    def statObject(self,name,container=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        # Connect if not already connected
        if not self.conn: self.connect()

        # For now, we expect container name...
        if not container:
            container = self.container
        if not container:
            raise RuntimeError(prefix+".listObjects: Expected container name")

        # Get the objects (info is a list of dictionaries)...
        headers,object = self.conn.get_object(container,name)

        return headers

    def objectExists(self,name,container=None):

        objs = self.listObjects(container)
        return (name in objs)

    def getObject(self,name,container=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not container:
            container = self.container

        if not container or type(container) is not str:
            raise RuntimeError(prefix+".getObject: Expected container to be type str and not empty")
        if not name or (type(name) != str and type(name) != unicode):
            raise RuntimeError(prefix+".getObject: Expected name to be type str/unicode and not empty")

        # Connect if not already connected
        if not self.conn: self.connect()

        # Get contents...
        #
        # Example headers...
        #   {'etag': '26ef469b3820dd42658ac7a50558b6be',
        #    'content-length': '1241812',
        #    'content-type':   'text/plain',
        #    'last-modified':  'Fri, 24 Nov 2017 14:13:27 GMT',
        #    'x-timestamp':    '1511532807.49330',
        #    'date':           'Fri, 24 Nov 2017 14:13:29 GMT',
        #    'accept-ranges':  'bytes',
        #    'x-trans-id':      'tx0000000000000004719cb-005a182909-3784-default'
        #   }

        try:

            headers,contents = self.conn.get_object(container,name)

            # Make the object...
            obj                = SwiftObject(connection=self)
            obj.container      = container
            obj.name           = name
            obj.contents       = contents
            obj.content_type   = headers['content-type']
            obj.last_modified  = headers['last-modified']
            obj.last_retrieved = headers['date']

        except swiftclient.exceptions.ClientException as e:

            obj = None

        return obj

    def putObject(self,name,contents=None,container=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not container:
            container = self.container

        # Adjust contents...
        if type(contents) == SwiftObject:
            contents = contents.contents
        if contents == None:
            contents = b''
        if type(contents) == str:
            contents = contents.encode()

        # First, some argument validation...
        if not container or type(container) is not str:
            raise RuntimeError(prefix+".getObject: Expected container to be type str and not empty")
        if not name or type(name) is not str:
            raise RuntimeError(prefix+".getObject: Expected name to be type str and not empty")
        if type(contents) != bytes:
            raise RuntimeError(prefix+".putObject: Expected contents to be type bytes")

        # Connect if not already connected
        if not self.conn: self.connect()

        # Do it...
        #print("DEBUG: swift.putObject: container = %s"%repr(container))
        #print("DEBUG: swift.putObject: name      = %s"%repr(name))
        #print("DEBUG: swift.putObject: type(contents) = %s"%repr(type(contents)))
        result = self.conn.put_object(container,name,contents)

    def deleteObject(self,name,container=None):

        prefix = SwiftConnection.prefix
        emptyp = SwiftConnection.emptyp

        if not container:
            container = self.container

        if not container or type(container) is not str:
            raise RuntimeError(prefix+".statObject: Expected container to be type str and not empty")
        if not name or type(name) is not str:
            raise RuntimeError(prefix+".statObject: Expected name to be type str and not empty")

        # Connect if not already connected
        if not self.conn: self.connect()

        result = False
        try:
            self.conn.delete_object(container,name)
            result = True
        except:
            pass

        return result

    # Note: collectGarbage should only be called on a host where
    #       /home/porridge/porridge.json exists (i.e., not on a TrustAgent client)
    def collectGarbage(self,argv=[],quiet=False,prefix='',logger=None):

        import config
        import time

        # Connect if not already connected
        if not self.conn: self.connect()

        config.load()

        # Handle optional <client>...
        clients = []
        if argv:
            if not argv:
                Error("Expected client names")
                return
            numErrors = 0
            while argv:
                client = argv[0]
                del argv[0]
                if client not in config.globals.hosts:
                    Error("Host %s not configured (see porridge.json)"%repr(client))
                    numErrors += 1
                elif not numErrors:
                    clients.append(client)
            if numErrors:
                return

        # Determine maximum age (int seconds)...
        try:    maxage = int(config.globals.maxage)
        except: maxage = 0

        if maxage <= 0:
            msg = "porridge.json maxage is set to zero, change it if you really want to remove rubbish"
            if logger:
                logger.error(msg)
            else:
                Error(prefix+msg)
            return

        # Get list of agable objects...
        objs = self.listObjects()
        delObjs = []
        for obj in objs:
            parts = splitName(obj)
            if parts and parts[0] == 'explicitTrust':
                continue
            age = objNameAge(obj)
            client = objNameHost(obj)
            if age == None:
                continue
            if clients and client not in clients:
                continue
            if age > maxage:
                delObjs.append(obj)

        if not delObjs:
            return

        #
        # Do it...
        #

        if not quiet:
            msg = "Deleting %d swift objects older than %d seconds..."%(len(delObjs),maxage)
            if logger:
                logger.info(msg)
            else:
                print(prefix+msg)
                sys.stdout.flush()

        then = time.time()
        for obj in delObjs:
            self.deleteObject(obj)

        now = time.time()
        elapsed = now-then
        if not quiet:
            msg = "took %.3f seconds"%elapsed
            if logger:
                logger.info("  "+msg)
            else:
                print(msg)
                sys.stdout.flush()

        return

#------------------------------------------------------------------------------
#
# Class: SwiftObject - Struct mirroring a Swift Object
#
# Members: container      - str:   Container name
#          name           - str:   Object name
#          connection     - obj:   Connection (SwiftConnection)
#          content_type   - str:   Content type
#          last_modified  - str:   Last modified date
#          last_retrieved - str:   Last retrieved date
#          contents       - bytes: Object's contents
#

class SwiftObject(Struct):

    members = 'container,name,connection,content_type,last_modified,last_retrieved,contents'
    prefix  = 'SwiftObject'
    emptyp  = ' '*len(prefix)

    def __init__(self,connection=None,container=None,name=None):

        super(SwiftObject,self).__init__(SwiftObject.members)
        if not globals.python2 and connection and type(connection) != SwiftConnection:
            raise RuntimeError("SwiftObject.__init__: Expected connection to be None or a SwiftConnection, got %s instead"%repr(type(connection)))
        self.name       = name
        self.container  = container
        self.connection = connection

    def cloneFrom(self,that):

        self.container      = that.container
        self.name           = that.name
        self.connection     = that.connection
        self.content_type   = that.content_type
        self.last_modified  = that.last_modified
        self.last_retrieved = that.last_retrieved
        self.contents       = that.contents

    def getContents(self):

        if not self.contents:
            that = self.connection.getObject(container=self.container,name=self.name)
            self.cloneFrom(that)

        return self.contents

#-------------------------------------------------------------------------------
#
# __main__
#

def main(argv):

    prefix = 'main'
    emptyp = '    '

    print(prefix+": Connecting...")
    sc = SwiftConnection().connect()
    print(prefix+": Connected okay")
    #containers = sc.listContainers()
    #print(prefix+": Containers... %s"%repr(containers))

    objCount = 0
    errCount = 0
    atCount  = 0
    oldCount = 0
    now = dtNow()

    if False:

        objs = sc.listObjects()
        print(prefix+": Objects...")
        for name in objs:

            if '@' in name:
                atCount += 1
                id,nonce = name.split('@')
                ignore,host = id.split('#')
                dt = dtFromNonce(nonce)
                if dt == None:
                    print("ERROR: name = %s: Bad nonce"%repr(name))
                    continue
                delta = now-dt
                if delta.days != 0 or delta.seconds >= 1*60*60:
                    oldCount += 1
                    continue
                print(emptyp+"    Object = %s, seconds = %d"%(repr(name),delta.seconds))
            else:
                print(emptyp+"    Object = %s"%repr(name))
                continue

            # TEMP...
            #continue
            # ...TEMP

            try:

                obj = sc.getObject(name=name)
                objCount += 1

                if globals.debug and globals.verbose:

                    print(emptyp+"    '"+obj.container+'/'+obj.name+"'")

                    print(emptyp+"      type(contents) = "+repr(typeName(obj.contents)))
                    print(emptyp+"      len(contents)  = "+str(len(obj.contents)))
                    print(emptyp+"      last_modified  = "+repr(obj.last_modified))
                    print(emptyp+"      last_retrieved = "+repr(obj.last_retrieved))
                    print(emptyp+"      content_type   = "+repr(obj.content_type))

            except:

                print("ERROR: sc.getObject failed...")
                errCount += 1

        print("*** %6d total objects queried ***"%objCount)
        print("*** %6d total objects errored ***"%errCount)
        print("*** %6d total '@' objects     ***"%atCount)
        print("*** %6d total old objects     ***"%oldCount)

    if True:

        print("Listing containers...")
        sys.stdout.flush()
        cons = sc.listContainers()
        print("cons = %s"%repr(cons))

if __name__ == "__main__":

    import math

    sc = SwiftConnection()
    sc.connect()
    attests = sc.getHostAttestations()
    keys = list(attests.keys())
    keys.sort()

    maxLen = 0
    for key in keys: maxLen = max(maxLen,len(key))

    for key in keys:
        print("%s..."%repr(key))
        objs = attests[key]
        for obj in objs:
            print("  "+obj)
