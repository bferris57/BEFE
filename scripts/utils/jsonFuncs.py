#!/usr/bin/python3
#coding=utf-8
from __future__ import print_function
#==============================================================================
#
# File: jsonFuncs.py - Various utility functions for Porridge Json Protocol
#
# Functions: isJson            - "Is this object JSON?"
#            isJsonable        - "Is this object Jsonable?"
#
#            jsonErrors        - Return JSON API equivalent of a JSON API "/errors" object
#            jsonErrorsObject  - Return JSON API equivalent of an 'error object'
#            jsonData          - Return JSON API equivalent of data as string
#            jsonDataObject    - Return JSON API equivalent of a 'data object'
#
#            isJsonApi         - "Is this object a valid JSON API object?"
#            isJsonErrors      - "Is this object a valid JSON API '/errors' object?"
#            isJsonData        - "Is this object a valid '/data' object?
#            isJsonRequest     - "Is this object a valid JSON API '/data/request' object?"
#            isJsonResponse    - "Is this object a valid JSON API '/data/response' object?"
#
#            validJsonPointer  - "Is this a valid RFC 6901(ish) Pointer?"
#            jsonPointerExists - "Does this Pointer reference something in a '/data' object?"
#            jsonPointerGet    - Get entry given a Pointer
#
#            jsonUnicodeToStr  - De-unicode strings
#            loads             - Load a Json String into Json Objects
#            dumps             - Dump a Jsonable object into a Json String
#
# Notes:     All the isJson... functions are passed the root JSONAPI object
#
#==============================================================================

import json
import globals
from   errors  import *
from   funcs   import *

from   jsonApi import JsonApiErrors
from   jsonApi import JsonApiWarnings

#-----------------------------------------------------------------------------
#
# Function: isJson - "Is this object Json?"
#
# Notes: Basically, if it's a string and json likes it, answer: True
#        Otherwise, we call isJsonable to answer the question.
#

def isJson(that):

    if type(that) is str:
        try:
            loads(that)
            return True
        except:
            return False
    else:
        return isJsonable(that)

#-----------------------------------------------------------------------------
#
# Function: isJsonable - "Is this object Jsonable?"
#
# Usage:    result = isJsonable(that)
#
# Where:    that   - obj: Object to test
#           result - bool: True = "Is Jsonable", False = "Isn't Jsonable"
#
# Notes:    Basically what we're testing here is whether an object is
#           either a str, bool, float, None, dict, or list, or None.
#
#           If a dict or list, it recursively tests and fails if recursive.
#
#           Special case of instances with "toJsonObject()" method are allowed.
#
#           We not only test the types, but we make sure it's not recursive
#           as well
#

def isJsonable(that):

    scalarTypes = (str,bool,float,int)
    structTypes = (list,dict)
    if that == None or type(that) in scalarTypes:
        return True
    if type(that) not in structTypes:
        return False

    passed  = []     # ◄── Lists/Maps we've already processed
    structs = [that] # ◄── Lists/Maps we need to process

    while structs:
        tos = structs[0]
        if type(tos) == dict:
            keys = tuple(tos.keys())
            for key in keys:
                if type(key) != str:
                    return False
        else:
            keys = range(0,len(tos))
        for key in keys:
            item = tos[key]
            if item == None or type(item) in scalarTypes:
                continue
            if type(item) not in structTypes:
                return False
            if id(item) == id(tos) or item in passed:
                return False # ◄── Recursive
            structs.append(item)
        del structs[0]
        passed.append(tos)

    return True

#-----------------------------------------------------------------------------
#
# Functions: jsonErrors       - Return JSON API error message as String
#            jsonErrorsObject - Return JSON API error message as Object
#            jsonData         - Return JSON API data as String
#            jsonDataObject   - Return JSON API data as Object
#

def jsonErrors(msg,full=True,httpstatus=500):

    return json.dumps(jsonErrorsObject(msg,full=full,httpstatus=httpstatus))

def jsonErrorsObject(errmsg=None,warnmsg=None,full=True,httpstatus=200):

    dic = {}
    if errmsg or not warnmsg:
        dic['errors'] = {
                'title' : ('No ' if httpstatus == 200 else '') + 'Porridge JSON API Error',
                'status': str(httpstatus),
                'code'  : '1',
                'detail': errmsg if errmsg else "*** Unknown Internal Server Error ***",
                }
    if warnmsg:
        dic['warnings'] = {
                'title' : ('No ' if httpstatus == 200 else '') + 'Porridge JSON API Error',
                'status': str(httpstatus),
                'code'  : '1',
                'detail': warnmsg,
                }

    return dic

def jsonData(data,pretty=True):

    if pretty:
        result = json.dumps(jsonDataObject(data),indent=2, sort_keys=True)
    else:
        result = json.dumps(jsonDataObject(data))
    return result

def jsonDataObject(data):

    data = {'data':data}

    return data

#-------------------------------------------------------------------------------
#
# Functions: isJsonApi        - "Is this object a valid JSON API object?"
#            isJsonErrors     - "Is this object a valid JSON API '/errors' object?"
#            isJsonData       - "Is this object a valid JSON API '/data' object?
#            isJsonRequest    - "Is this object a valid JSON API '/data/request' object?
#            isJsonResponse   - "Is this object a valid JSON API '/data/response object?
#
# Notes:     All the isJson... functions are passed the root JSONAPI object
#

def isJsonApi(that):

    validKeys = ['data','errors']

    # If passed a string, turn into Json Object...
    if type(that) == str:
        try:
            that = loads(that)
        except:
            return False

    # Validate it...
    if type(that) != dict: return False
    keys = that.keys()
    if 'data' not in keys and 'errors' not in keys:
        return False

    return True

def isJsonErrors(that):

    # If passed a string, turn into Json Object...
    if type(that) == str:
        try:
            that = loads(that)
        except:
            return False

    # Test it...
    if not 'errors' in that or type(that['errors']) not in (dict,JsonApiErrors,JsonApiWarnings):
        return False
    if 'errors' in that:
        errors = that['errors']
    if not errors:
        return False
    for entry in errors:
        if type(entry) != dict:          return False
        if 'title'  not in entry:        return False
        if type(entry['title'])  != str: return False
        if 'status' not in entry:        return False
        if type(entry['status']) != str: return False
        if 'code'   not in entry:        return False
        if type(entry['code'])   != str: return False
        if 'detail' not in entry:        return False
        if type(entry['detail']) != str: return False
    return True

def isJsonData(that):

    # If passed a string, turn into Json Object...
    if type(that) == type(b''):
        that = that.decode('utf-8')
    if type(that) == str:
        try:
            that = loads(that)
        except:
            return False

    # Test it...
    if type(that) != dict:                    return False
    if 'data' not in that.keys():             return False
    if type(that['data']) not in [dict,list]: return False

    return True

def isJsonRequest(that):

    try:
        validateJsonRequest(that);
    except Exception as e:
        return False
    return True

def validateJsonRequest(that):

    import config
    config.load()

    # If passed a string, turn into Json Object...
    if type(that) == str:
        try:
            that = loads(that)
        except:
            raise Exception("'that' is not a Jsonable object")

    # Make sure it's a valid JSON API object...
    if not isJsonApi(that):           raise Exception("Not a valid JSON API object")

    # Test it...
    if not isJsonData(that):          raise Exception("Not a valid JSON APAI '/data' object")
    if type(that['data']) != dict:    raise Exception("'/data' malformed")
    if 'request' not in that['data']: raise Exception("'/data/request' not found")
    request = that['data']['request']
    if type(request) != dict:         raise Exception("'/data/request' malformed")
    if 'action' not in request:       raise Exception("'/data/request/action' missing")

    action = request['action']

    if action == "attest":

        if "hostName" in request:
            hostName = request['hostName']
            if type(hostName) != str:
                return jsonErrors("'/data/request/action/hostName' is malformed")
            if not hostName in config.hosts:
                raise Exception("Host %s in '/data/request/action/hostName' is not configured (see porridge.json)"%
                                  repr(hostName)
                                 )
        if "quoteName" in request:
            quote = request['quoteName']
            if type(quote) != str:
                raise Exception("'/data/request/quote' is malformed")
            parts = quote.split('#')
            if len(parts) != 2 or parts[0] != 'quote':
                raise Exception("Quote %s in '/data/request/quote' is malformed"%repr(quote))
            parts = parts[1].split('@')
            if len(parts) != 2:
                raise Exception("Quote %s in '/data/request/quote' is malformed"%repr(quote))
            hostName = parts[0]
            ts       = parts[1]
            if not hostName in config.hosts:
                raise Exception("Host %s in quote %s is not configured (see porridge.json)"%repr(hostName),repr(quote))
            if not isValidNonce(ts):
                raise Exception("Timestamp %s in quote %s is not configured (see porridge.json)"%repr(ts),repr(quote))
        else:
            raise Exception("'/data/request' missing 'hostName' or 'quote'")

        if "replyTo" not in request:
            raise Exception("'/data/request/replyTo' missing for 'action'='attest'")
        if type(request['replyTo']) != str:
            raise Exception("'/data/request/replyTo' is malformed")

    # NOTE: Need to finish these validations at some point but in a hurry at the moment...
    elif action == "heartbeat":
        pass
    elif action == "attested":
        pass
    elif action == "quote":
        pass
    elif action == "state":
        pass
    elif action == "pstate":
        pass
    elif action == "heap":
        pass
    elif action == "xTrust":
        pass
    elif action == "remTrust":
        pass
    else:
        raise Exception("'/data/request/action' has unknown action %s"%repr(action))

    return

def isJsonResponse(that):

    # If passed a string, turn into Json Object...
    if type(that) == str:
        try:
            that = loads(that)
        except:
            return False

    # Make sure it's a JSON API valid object...
    if not isJsonApi(that):         return False

    # Test it...
    if not isJsonData(that):        return False
    if type(that['data']) != dict:  return False
    data = that['data']
    if 'response' not in data:         return False

    return True

#-------------------------------------------------------------------------------
#
# Functions: validJsonPointer  - "Is this a valid RFC 6901(ish) Pointer?"
#            jsonPointerExists - "Does this Pointer reference something in a '/data' object?"
#            jsonPointerGet    - Get a Map entry given a Pointer
#

def validJsonPointer(ptr):

    if type(ptr) != str: return False

    parts = ptr.split('/')

    if not parts:      return False
    if len(parts) < 2: return False

    for i in range(0,len(parts)):
        part = parts[i]
        if i == 0 and part != '': return False
        if not part: continue
        if isInt(part) and int(part) < 0: return False
        if part == '': return False

    return True

def __find(ptr,data):

    if type(ptr) != str: return None

    if '~' in ptr:
        if '~0' not in ptr and '~1' not in ptr:
            raise Exception("Invalid Pointer %s at '~'"%repr(ptr))
        while '~0' in part or '~1' in part:
            ptr = '~'.join(ptr.split('~0'))
            ptr = '/'.join(ptr.split('~1'))
    if '\\\\' in ptr:
        ptr = '\\'.join(ptr.split('\\\\'))

    parts = ptr.split('/')

    if not parts:      return data
    if len(parts) < 2: raise Exception("Json Pointer need to start with '/data'")
    if parts[0] != '': raise Exception("Json Pointer just start with '/'")

    cur   = data
    soFar = '/'
    del parts[0]

    while parts:

        part = parts[0]
        del parts[0]
        soFar += part

        if not part:
            if parts: raise Exception("Json part %s does not exist"%repr(soFar))
            break

        if type(cur) == dict:
            if isInt(part): raise Exception("Json part %s is a Map, can't index by Integer"%repr(soFar))
            if part not in cur: raise Exception("Json part %s does not exist"%repr(soFar))
            cur = cur[part]
            if parts: soFar += '/'
            continue
        elif type(cur) == list:
            if not isInt(part): raise Exception("Json part %s is a List, can't index by String"%repr(soFar))
            i = int(part)
            if i < 0 or i >= len(cur): raise Exception("Json part %s does not exist"%repr(soFar))
            cur = cur[i]
            if parts: soFar += '/'
            continue
        raise Exception("Json part %s is not a Map or List"%repr(soFar))

    return cur

def jsonPointerExists(ptr,data):

    try:

        that = __find(ptr,data)

        return True

    except:

        return False

def jsonPointerGet(ptr,data):

    try:
        that = __find(ptr,data)
    except:
        return None

    return that

#-------------------------------------------------------------------------------
#
# Function: jsonUnicodeToStr(obj) - De-unicode strings
#
# Usage:    obj - object: Json loaded object
#
# Notes:    This function takes a Json loaded object and, if Python 2, turns
#           all u'...' strings into '...' strings, DEEP.
#

def jsonUnicodeToStr(obj):

    if not globals.python2:
        return obj

    if type(obj) == unicode:
        return obj.encode()

    process   = [obj]
    processed = []

    while process:

        obj0 = process[0]
        if type(obj0) == list:
            for i in range(0,len(obj)):
                this = obj0[i]
                if type(this) in (list,dict):
                    if this not in processed and this not in process:
                        process.append(this)
                elif type(this) == unicode:
                    obj0[i] = this.encode()
            processed.append(obj)
        elif type(obj0) == dict:
            keys = list(obj0.keys())
            for key in keys:
                this = obj0[key]
                if type(key) == unicode:
                    key1 = key.encode()
                    del obj0[key]
                    obj0[key1] = this
                    key = key1
                if type(this) in (list,dict):
                    if this not in processed and this not in process:
                        process.append(this)
                elif type(this) == unicode:
                    obj0[key] = this.encode()
            processed.append(obj0)
        elif type(obj) == str:
            obj0 = obj0.encode()

        del process[0]

    return obj

#-------------------------------------------------------------------------------
#
# Functions: loads - Load a Json String into Json 'data' Objects
#            dumps - Dump a Jsonable object into a Json String
#
# Notes:     We only provide these as convenience functions that proxy to their
#            json module equivalents.
#
#            Actually, we ensure the input/output is a properly formed Json
#            'error' or 'data' object.
#

def loads(string,expectdata=False):

    if type(string) != str:
        return jsonErrorsObject("loads() expects String argument")
    try:
        obj = jsonUnicodeToStr(json.loads(string))
    except Exception as e:
        raise e
        return jsonErrorsObject("Malformed json string")
    if expectdata and not isJsonData(obj):
        return jsonErrorsObject("Json object not a 'data' object")

    return obj

def dumps(obj):

    if type(obj) == str:
        try:
            obj = jsonUnicodeToStr(json.loads(obj))
        except:
            return jsonErrors("'obj' isn't 'Jsonable'")

    if not isJsonData(obj) and not isJsonErrors(obj):
        return jsonErrors("'obj' parameter doesn't conform to JSON API standards")

    result = json.dumps(obj)
    if result and result[0] == '"' and result[-1] == '"':
        result = result[1:-1]
    parts = result.split('\\"')
    if len(parts):
        parts = '"'.join(parts)

    return json.dumps(obj)

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == "__main__":

    # isJsonable tests...

    if False:

        class x:
            pass
        class y:
            def toJsonObject(self):
                pass

        if not isJsonable(None):
            raise Exception("isJsonable fail 1")
        if isJsonable(x()):
            raise Exception("isJsonable fail 2")
        if not isJsonable({}):
            raise Exception("isJsonable fail 3")
        #if isJsonable([{},"hi there",x()]):
        #  raise Exception("isJsonable fail 4")
        if isJsonable({x():1}):
            raise Exception("isJsonable fail 5")
        if isJsonable(x):
            raise Exception("isJsonable fail 6")
        if isJsonable(y):
            raise Exception("isJsonable fail 7")
        if isJsonable(x()):
            raise Exception("isJsonable fail 8")
        a = []
        b = []
        a.append(b)
        b.append(a)
        if isJsonable(a):
            raise Exception("isJsonable fail 9")

        print("isJsonable     test Passed")

    # isJson... tests...

    if False:

        if isJsonErrors("xxx"):
            raise Exception("isJsonErrors fail 1")
        if isJsonErrors({'errors':None}):
            raise Exception("isJsonErrors fail 2")
        if isJsonErrors({'errors':[]}):
            raise Exception("isJsonErrors fail 3")
        if isJsonErrors({'errors':[{}]}):
            raise Exception("isJsonErrors fail 4")
        myError = {'errors':[{"title":"xxx","status":"200","code":"abc","detail":"some error"}]}
        if not isJsonErrors(myError):
            raise Exception("isJsonErrors fail 5")
        answer = isJsonErrors(jsonErrors("dude!!!"))
        if not answer:
            raise Exception("isJsonErrors fail 6")

        print("isJsonErrors   test passed")

        if isJsonData("xxx"):
            raise Exception("isJsonData fail 1")
        if isJsonData(jsonErrors("dude")):
            raise Exception("isJsonData fail 2")
        if isJsonData({'data':'hi there'}):
            raise Exception("isJsonData fail 3")
        myData = {'data':{}}
        if not isJsonData(myData):
            raise Exception("isJsonData fail 4")
        myData = {'data':[]}
        if not isJsonData(myData):
            raise Exception("isJsonData fail 5")
        myData = jsonData({})
        if not isJsonData(myData):
            raise Exception("isJdonData fail 6")

        print("isJsonData     test Passed")

        result = loads("{}")
        if not isJsonData(result) and not isJsonErrors(result):
            raise Exception("loads fail 1")
        result = loads('{"data":{"request":{"action":"heartbeat"}}}')
        if not isJsonData(result):
            raise Exception("loads fail 2")

        print("loads          test Passed")

        result = dumps('')
        if not isJsonErrors(result):
            raise Exception("dumps fail 1")
        result = dumps({1:2})
        if not isJsonErrors(result):
            raise Exception("dumps fail 2")
        s = '{"data": {}}'
        result = dumps(s)
        if isJsonErrors(result) or result != s:
            raise Exception("dumps fail 3")

        print("dumps          test Passed")

        data = loads('{"data":[1,2,{"three":"Dude!"}]}')
        ptrs = ['/','/data','/data/0','/data/1','/data/2','/data/2/three']
        numFail = 0
        for i in range(0,len(ptrs)):
            ptr = ptrs[i]
            if not jsonPointerExists(ptr,data):
                print("jsonPointerExists fail %d"%i)
                numFail += 1
        if numFail:
            #raise Exception("jsonPointerExists Failed")
            print("jsonPointerExists Failed")

        numFail = 0
        for i in range(0,len(ptrs)):
            ptr = ptrs[i]
            try:
                that = jsonPointerGet(ptr,data)
                #print("jsonPointerGet[%d] found %s"%(i,repr(that)))
            except:
                raise Exception("jsonPointerFind fail %d"%i)
        print("jsonPointerGet test Passed")

    # TEMP...
    if False:

        msg = '{"data":{"request":{"action":"heartbeat"}}}'
        print("msg = %s...\n"%repr(msg))
        print("isJsonApi      = %s"%repr(isJsonApi(msg)))
        print("isJsonRequest  = %s"%repr(isJsonRequest(msg)))
        print("isJsonResponse = %s"%repr(isJsonResponse(msg)))

    if False:

        obj = u'Hi there'
        obj1 = jsonUnicodeToStr(obj)
        print("%s ──► %s"%(repr(obj),repr(obj1)))

        obj = [u'Hi there',{u'Hello':1,u'hello1':[u'abc',u'def']}]
        print("%s ──► "%repr(obj),end='')
        obj1 = jsonUnicodeToStr(obj)
        print("%s"%(obj1))

    if True:

        import os

        contents = open(os.path.expanduser('~/porridge.json')).read()
        print("DEBUG: type(contents) = %s"%repr(type(contents)))
        obj = loads(contents)
        if type(obj) != dict:
            print("obj = %s"%repr(obj))
        else:
            print("DEBUG: type(obj) = %s"%repr(type(obj)))
