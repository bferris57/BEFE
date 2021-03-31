#!/usr/bin/python3
#coding=utf-8

# Imports...
from   Struct  import Struct
from   funcs   import *
import json
import swift
import config
import jsonFuncs

#-------------------------------------------------------------------------------
#
# Class: HostStatus - A host's status
#
#-------------------------------------------------------------------------------

class HostStatus(Struct):

    members = "name,ip,status,why,tsQuote,tsAttest,by,exceptions,error,bios,vmm,location,ima"

    def __init__(self):

        super(HostStatus,self).__init__(HostStatus.members)
        self.reset()

    def reset(self):

        self.name       = ''
        self.status     = '?'
        self.why        = 'Status not checked yet'
        self.tsQuote    = ''
        self.by         = ''
        self.tsAttest   = ''
        self.exceptions = []
        self.error      = ''
        self.bios       = None
        self.vmm        = None
        self.location   = None
        self.ima        = None

    def toJsonable(self):

        obj               = {}
        obj['name']       = self.name
        obj['status']     = self.status
        obj['why']        = self.why
        obj['tsQuote']    = self.tsQuote
        obj['by']         = self.by
        obj['tsAttest']   = self.tsAttest
        obj['exceptions'] = self.exceptions
        obj['error']      = self.error
        obj['bios']       = self.bios
        obj['vmm']        = self.vmm
        obj['location']   = self.location
        obj['ima']        = self.ima

        return obj

    def fromJsonable(self,obj):

        self.name       = obj['name']
        self.status     = obj['status']
        self.why        = obj['why']
        self.tsQuote    = obj['tsQuote']
        self.by         = obj['by']
        self.tsAttest   = obj['tsAttest']
        self.exceptions = obj['exceptions']
        self.error      = obj['tsAttest']
        self.bios       = obj['bios']
        self.vmm        = obj['vmm']
        self.location   = obj['location']
        self.ima        = obj['ima']

    def toJson(self):

        return json.dumps(self.toJsonable(),indent=2)

    def fromJson(self,input):

        obj = jsonFuncs.loads(input)
        self.fromJsonable(obj)

    def fromSwift(self,objName):

        if not '#' in objName or not '@' in objName:
            return False
        parts = objName.split('#')
        if len(parts) != 2:
            return False
        parts2 = parts[1].split('@')
        if len(parts2) != 2:
            return False
        parts[1] = parts2[0]
        parts.append(parts2[1])

        # Get the object's contents (should be JSON API request/response)...
        sc = swift.SwiftConnection()
        obj = sc.getObject(objName)
        if not obj:
            return False
        #print("SwiftConnection.fromSwift(%s): type(obj) = %s"%(repr(objName),repr(type(obj))))
        self.reset()
        sJson = obj.contents.decode('UTF-8')
        try:
            data = jsonFuncs.loads(sJson)
        except Exception as e:
            data = {}
            #print("DEBUG: Invalid Json in %s"%repr(objName))
            #print("       sJson = %s"%repr(postEllipse(sJson,80)))
        #print("DEBUG: data = %s"%repr(data))
        if 'data' not in data and 'errors' not in data and 'tsAttest' not in data:
            return False

        # Extract 'errors', 'data'['request'], and 'data'['response']...
        errors = []
        if 'errors' in data:
            errors = data['errors']
        if 'data' in data:
            data = data['data']
            #print("SwiftConnection.fromSwift(%s): data.keys() = %s"%(repr(objName),repr(list(data.keys()))))
        request = {}
        if 'request' in data:
            request = data['request']
        response = {}
        respKeys = []
        if 'response' in data:
            response = data['response']
        else:
            response = data # ◄── This happens if the object's "explicitTrust#"
        respKeys = list(response.keys())
        respKeys.sort()

        # Populate ourselves...
        config.load()
        self.name    = parts[1]
        if 'quoteName' in request:
            parts2 = request['quoteName'].split('@')
            if len(parts2) == 2:
                self.tsQuote = parts2[1]

        #print("SwiftConnection.fromSwift(%s): request  = %s"%(repr(objName),repr(postEllipse(repr(request),80))))
        #print("SwiftConnection.fromSwift(%s): response = %s"%(repr(objName),repr(postEllipse(repr(response),80))))
        maxLen = 0
        for key in respKeys: maxLen = max(maxLen,len(key))
        #for key in respKeys:
        #  value = response[key]
        #  print("  " + key + ':' + ' '*(maxLen-len(key)) + repr(value))

        if 'explicit' in objName: # Slightly different JSON to 'attested.filtered#...'

            if 'tsAttest' in respKeys:
                self.tsAttest = response['tsAttest']
            if 'vmm' in respKeys:
                self.vmm = response['vmm']
            if 'bios' in respKeys:
                self.bios = response['bios']
            if 'location' in respKeys:
                self.location = response['location']
            if 'ima' in respKeys:
                self.ima = response['ima']
            if 'by' in respKeys:
                self.by = response['by']
            if 'status' in respKeys:
                self.status = response['status']
            if 'why' in respKeys:
                self.why = response['why']

        else: # Must be an 'attested.filtered#...'

            if 'attestStart' in respKeys:
                self.tsAttest = response['attestStart']
            if 'vmm' in respKeys:
                self.vmm = response['vmm']
            if 'bios' in respKeys:
                self.bios = response['bios']
            if 'location' in respKeys:
                self.location = response['location']
            if 'ima' in respKeys:
                self.ima = response['ima']
            if 'imaExceptions' in response:
                self.exceptions = response['imaExceptions']
            if 'responseFrom' in respKeys:
                self.by     = response['responseFrom']
                #self.status = "Trusted" if self.vmm and self.bios and not self.exceptions else "NOT Trusted"
                self.status = "Trusted" if self.vmm and self.bios and self.ima else "Not Trusted"
                self.why    = "Attested by " + repr(self.by)
                if not self.ima and not self.exceptions:
                    self.why += " (?)"
            if errors:
                self.error = errors[0]['detail']

        return True

if __name__ == "__main__":

    sc = swift.SwiftConnection()
    sc.connect()
    objs = sc.listObjects()
    process = []
    for objName in objs:
        if 'explicitTrust#dewey@' not in objName:
            continue
        process.append(objName)
    process.sort()
    hs = HostStatus()
    objName = process[-1]
    okay = hs.fromSwift(objName)
    if okay:
        lines = hs.toJson().split('\n')
        for line in lines:
            print("       " + line)
    else:
        Error("hs.fromSwift failed")
