#!/usr/bin/env python3
#coding=utf-8
#===============================================================================
#
# File: jsonApi.py - Json API classes and related functions
#
# Classes: ApiException    - exception: Api Exception
#          JsonApiMessage  - class:     Request/response/errors/warnings
#          JsonApiRequest  - class:     Api Request
#          JsonApiResponse - class:     Api Response
#          JsonApiErrors   - class:     Api Errors
#          JsonApiWarnings - class:     Api Warnings
#
#===============================================================================
# Copyright (C) 2019 - 100percentit.com
#===============================================================================

from   errors    import *
import json

#-------------------------------------------------------------------------------
#
# Class: ApiException - API Exception
#

class ApiException(Exception):

    def __init__(self,msg):

        self.msg = msg

    def __str__(self):

        return str(self.msg)

#-------------------------------------------------------------------------------
#
# Class: JsonApiBase - Base class (knows about its own dynamic members)
#

class JsonApiBase:

    def __init__(self,**args):

        for arg in args:
            setattr(self,arg,args[arg])

    def __str__(self):

        jsonable = self.toJsonable()
        s = json.dumps(jsonable,indent=2)

        return s

    def __repr__(self):

        members = self.members()
        members.sort()
        maxLen = 0
        for member in members: maxLen = max(maxLen,len(member))

        s = ''
        first = self.__class__.__name__ + '('
        pad   = ' '*len(first) if len(members) > 1 else ''
        for i in range(0,len(members)):
            member = members[i]
            s += first if not s else pad
            s += member + ' '*(maxLen-len(member)) + ' = ' + repr(getattr(self,member))
            if len(members) > 1:
                s += ')' if i == len(members)-1 else ',\n'

        return s

    def __contains__(self,that):

        return that in self.members()

    def __bool__(self):

        members = self.members()
        if not members: return False
        result = False
        for member in members:
            if getattr(self,member):
                return True
        return False

    def __getitem__(self,member):

        if member not in self.members():
            raise ApiException("JsonApiBase: %s is not a member of instance"%repr(member))
        return getattr(self,member)

    def members(self):

        keys = dir(self)
        i = -1
        while i < len(keys)-1:
            i += 1
            key = keys[i]
            attr = getattr(self,key)
            if key.startswith('__') or callable(attr):
                del keys[i]
                i -= 1

        return keys

    def reset(self):

        for member in self.members():
            setattr(self,member,None)

    def toJsonable(self):

        out = {}
        members = self.members()
        members.sort()

        for member in members:
            val = getattr(self,member)
            out[member] = getattr(self,member)
            if type(self.member) in (JsonApiRequest,JsonApiResponse,JsonApiErrors,JsonApiMessages):
                out[member] = out[member].toJsonable()
        return out

    def fromJsonable(self,jsonable):

        self.reset()
        if type(jsonable) != dict:
            raise ApiException("JsonApiBase.fromJsonable: Expected jsonable to be a dict")
        members = self.members()
        for key in jsonable.keys():
            if not members or key in members:
                setattr(self,key,jsonable[key])

#-------------------------------------------------------------------------------
#
# Class: JsonApiMessage - Request/response/errors/warnings message
#

class JsonApiMessage(JsonApiBase):

    def __init__(self,jsonable=None,errors=None,warnings=None):

        from jsonFuncs import isJsonErrors

        self.reset()
        if jsonable:
            self.fromJsonable(jsonable)
        if errors:
            self.errors = JsonApiErrors(errors)
        if warnings:
            self.warnings = JsonApiWarnings(warnings)

    def __bool__(self):

        return (
                bool(self.request)  or
                bool(self.response) or
                bool(self.errors)   or
                bool(self.warnings)
               )

    def __str__(self):

        return json.dumps(self.toJsonable(),indent=2)

    def reset(self):

        self.request  = JsonApiRequest()
        self.response = JsonApiResponse()
        self.errors   = {}
        self.warnings = {}

    def validate(self):

        return False

    def toJsonable(self):

        from jsonFuncs import isJsonErrors,jsonErrorsObject

        msg = {}
        if self.request or self.response:
            msg['data'] = {}
        if self.request:
            msg['data']['request'] = self.request.toJsonable()
        if self.response:
            msg['data']['response'] = self.response.toJsonable()
        if self.errors:
            if type(self.errors) in (JsonApiErrors,JsonApiWarnings):
                msg['errors'] = self.errors.toJsonable()
            else:
                msg['errors'] = self.errors
        if self.warnings:
            if type(self.warnings) in (JsonApiErrors,JsonApiWarnings):
                msg['warnings'] = self.warnings.toJsonable()
            else:
                msg['warnings'] = self.warnings

        return msg

    def fromJsonable(self,obj):

        self.reset()

        if type(obj) != dict:
            raise ApiException("JsonApiMessage.fromJsonable: Expected obj to be a dict")

        if 'data' in obj:
            data = obj['data']
            if type(data) != dict:
                raise ApiException("JsonApiMessage.fromJsonable: Expected obj['data'] to be a dict")

            if 'request' in data:
                request = data['request']
                if type(request) != dict:
                    raise ApiException("JsonApiMessage.fromJsonable: Expected obj['data']['request'] to be a dict")
                self.request = JsonApiRequest(jsonable=request)
            else:
                self.request = {}

            if 'response' in data:
                response = data['response']
                if type(response) != dict:
                    raise ApiException("JsonApiMessage.fromJsonable: Expected obj['data']['response'] to be a dict")
                self.response = JsonApiResponse(jsonable=response)
            else:
                self.response = {}

        if 'errors' in obj:
            errors = data['errors']
            if type(errors) != dict:
                raise ApiException("JsonApiMessage.fromJsonable: Expected obj['errors'] to be a dict")
            self.errors = JsonApiErrors(errors)

        if 'warnings' in obj:
            warnings = data['warnings']
            if type(warnings) != dict:
                raise ApiException("JsonApiMessage.fromJsonable: Expected obj['warnings'] to be a dict")
            self.warnings = JsonApiWarnings(warnings)

#-------------------------------------------------------------------------------
#
# Class: JsonApiRequest  - Request  portion of Json API Message
#        JsonApiResponse - Response portion of Json API Message
#        JsonApiWarnings - Warnings portion of Json API Message
#        JsonApiErrors   - Errors   portion of Json API Message
#

class JsonApiRequest(JsonApiBase):

    def __init__(self,jsonable=None):

        if jsonable:
            self.fromJsonable(jsonable)

class JsonApiResponse(JsonApiBase):

    def __init__(self,jsonable=None):

        if jsonable:
            self.fromJsonable(jsonable)

class JsonApiWarnings(JsonApiBase):

    def __init__(self,obj=None):

        from jsonFuncs import isJsonErrors

        self.detail = None
        typ = type(obj)
        if type(obj) == str:
            self.detail = obj
        elif type(obj) in (list,tuple):
            self.detail = list(obj)
        elif type(obj) == dict:
            if 'errors' in obj:
                obj = obj['errors']
            if type(obj) != dict and not isJsonErrors(obj) and ('detail' not in obj or 'status' not in obj):
                print("DEBUG: JsonApiWarnings.__init__: obj = %s"%repr(obj))
                raise ApiException("Expected dict to be isJsonErrors()")
            if 'errors' in obj:
                obj = obj['errors']
            self.detail = obj['detail']
            self.status = obj['status']
        elif obj == None:
            self.reset()
        else:
            raise ApiException("Expected str, list, or tuple. Got %s"%repr(type(obj)))

    def __getitem__(self,member):

        if type(member) == int:
            return self.detail[member]
        return super().__getitem__(self,member)

    def __len__(self):

        return 0 if not self.detail else len(self.detail)

    def extend(self,that):

        # Make sure that is iterable...
        try:
            temp = iter(that)
        except:
            raise TypeError("'that' is not iterable")

        if self.detail is not None:
            self.detail.extend(that)
        else:
            self.detail = list(that)

        return self.detail

    def reset(self):

        self.detail = []
        self.status = None

    def toJsonable(self):

        members = self.members()
        detail = None if not 'detail' in members else self.detail
        status = 200 if not 'status' in members else self.status

        d = {}
        d['detail'] = detail
        d['title']  = 'Porridge JSON API Warning' if detail else 'NO Porridge JSON API Warning'
        if type(detail) == list and len(detail) != 1:
            d['title'] = d['title'] + 's'
        d['code']   = 1
        d['status'] = str(status)

        return d

    def fromJsonable(self,jsonable):

        self.detail = None
        self.status = None

        if type(jsonable) != dict:
            raise ApiException("JsonApiWarnings.fromJsonable: Expected a dict")
        if 'detail' in jsonable:
            self.detail = jsonable['detail']
        if 'status' in jsonable:
            self.status = jsonable['status']

    def append(self,msgs):

        if type(msgs) not in (str,list):
            raise ApiException("JsonApiWarnings.append: Expected msgs to be str or list, got %s instead"%repr(type(msgs)))

        if self.detail == None:
            self.detail = []
        elif type(self.detail) == str:
            self.detail = [self.detail]

        if type(msgs) == str:
            self.detail.append(msgs)
        else:
            self.detail.extend(msgs)

        return

class JsonApiErrors(JsonApiWarnings):

    def toJsonable(self):

        d = super().toJsonable()
        if 'title' in d:
            d['title'] = 'Error'.join(d['title'].split('Warning'))

        return d

#-------------------------------------------------------------------------------
#
# __main__
#

jsonableFull = '''
{
  "data": {
    "request": {
      "action": "attested",
      "attestWrite": true,
      "hostName": "doom",
      "id": "{6f7f45d7-8b3f-446b-9272-0fac841a327d}",
      "quoteName": "quote#doom@2019-02-21T15:31:26.763",
      "replyTo": "admin2:9996",
      "requestFrom": "thorny4"
    },
    "response": {
      "attestEnd": "2019-02-21T15:31:35.260",
      "attestStart": "2019-02-21T15:31:33.845",
      "bios": true,
      "heapMemoryEnd": 97900888,
      "heapMemoryStart": 98947064,
      "ima": false,
      "imaExceptions": [
        "bc8bb8e433bf65214540115414c821c904b2a30d60a3ac0424bf9b77a00024b7 /root/t.c",
        "8cc6e128d8f7c9421b734a5c8fa28dad68451d8cd590172251457f8c541b177b /root/hosts"
      ],
      "location": false,
      "msgQueue.size": 1,
      "responseFrom": "thorny4",
      "status": "OK",
      "vmm": true
    }
  }
}
'''
jsonable = '''
{
  "data": {
    "request": {
      "action": "attested",
      "attestWrite": true,
      "hostName": "doom",
      "id": "{6f7f45d7-8b3f-446b-9272-0fac841a327d}",
      "quoteName": "quote#doom@2019-02-21T15:31:26.763",
      "replyTo": "admin2:9996",
      "requestFrom": "thorny4"
    }
  }
}
'''
if __name__ == '__main__':

    tests = (3,)

    for test in tests:

        print("*** Test %s ***"%repr(test))

        if test == 0:

            a = JsonApiMessage()
            print("a.members = %s"%repr(a.members()))

            m = JsonApiMessage(json.loads(jsonableFull))
            s = str(m)
            print("s = %s"%s)

            print("msg.request.action   = %s",repr(m.request.action))
            print("msg.request.hostName = %s",repr(m.request.hostName))
            print("msg.response.imaExceptions...")
            lines = m.response.imaExceptions
            for line in lines:
                print("  "+line)
            print("%d total exceptions"%len(lines))

        if test == 1:

            errors = jsonErrorsObject(['error 1','error 2'],full=False)
            print("errors = %s"%repr(errors))
            print("type(errors) = %s"%repr(type(errors)))
            w = JsonApiWarnings(errors['errors'])
            print("w...")
            s = str(w)
            lines = s.split('\n')
            for line in lines:
                print("  "+line)

            e = JsonApiErrors(w.toJsonable())
            print("e...")
            s = str(e)
            lines = s.split('\n')
            for line in lines:
                print("  "+line)

        if test == 2:

            request  = JsonApiRequest()
            request.action = 'abc'
            response = JsonApiResponse()
            print("bool(JsonApiRequest())  = %s"%repr(bool(request)))
            print("bool(JsonApiResponse()) = %s"%repr(bool(JsonApiResponse())))
            msg1 = JsonApiMessage()
            msg1.request = request
            msg1.response = response
            print("bool(msg1)              = %s"%repr(bool(msg1)))
            msg2 = JsonApiMessage()
            print("bool(msg2)              = %s"%repr(bool(msg2)))

        if test == 3:

            msg = JsonApiMessage(errors=['Line 1','Line 2'])
            print("msg...")
            j = msg.toJsonable()
            print("DEBUG: j = %s"%repr(j))
            lines = json.dumps(msg.toJsonable(),indent=2).split('\n')
            for line in lines:
                print("..."+line)
