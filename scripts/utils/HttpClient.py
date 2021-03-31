#!/usr/bin/python3
#coding=utf-8
#===============================================================================
#
# File:    HttpClient.py - Porridge HttpClient class
#
# Purpose: For connecting to and operating through the Porridge HTTP service
#          interface.
#
# Notes:   Example curl command to http-admin service...
#
#            curl -X GET -k https://<authkey>@87.254.4.138:9000/status/<host>
#
#            Where: <authkey> - One of the swift keys
#                   <host>    - Host name
#
#===============================================================================

import http.client
import ssl
from   errors import *
from   funcs  import *
from   base64 import b64encode

class HttpClient:

    def __init__(self,url=None):

        if url:

            if type(url) != str:
                raise InternalError("Expected url to be str, got %s instead"%repr(type(url)))
            url = url.strip()
            if url.startswith('https://'):
                url = url[8:]
                self.https = True
            elif url.startswith('http://'):
                url = url[7:]
                self.https = False
            else:
                raise InternalError("Malformed url: %s"%repr(url))
            if '/' in url:
                InternalError("Unexpected '/' in url %s, removing it..."%repr(url))
            url = url.split('/')[0]
            parts = url.split('@')
            if len(parts) != 2:
                raise InternalError("Expected single '@' in url %s"%repr(url))
            subparts = parts[1].split(':')
            if len(subparts) != 2:
                raise InternalError("Malformed host:port in url %s"%repr(url))
            
            self.key  = parts[0]
            self.ip   = subparts[0]
            self.port = subparts[1]

        else:

            import config
            config.load()
          
            self.key = swiftkey if swiftkey else config.globals.swiftkey

            # Get service info...
            if not config.globals.services:
                raise InternalError("config.services missing")
            service = 'http-admin'
            if service not in config.globals.services:
                raise InternalError("Service %s missing in config"%repr(service))
            servInfo = config.globals.services[service]

            if 'ip' not in servInfo:
                raise InternalError("'ip' missing in config.services[%s]"%repr(service))
            if 'port' not in servInfo:
                raise InternalError("'port' missing in config.services[%s]"%repr(service))

            self.ip     = ip   if ip   else servInfo['ip']
            self.port   = port if port else servInfo['port']
            self.https  = False

        if self.https:
            self.url = "https://"
        else:
            self.url = "http://"
        self.url    = self.url  + self.key + "@" + self.ip + ":" + str(self.port)
        self.user   = b64encode((self.key).encode('utf-8')).decode("ascii")
        self.client = None

        self.connect()

    def connect(self):

        if self.https:
            self.client = http.client.HTTPSConnection(
                self.ip+':'+str(self.port),context=ssl.SSLContext(ssl.PROTOCOL_TLSv1)
            )
        else:
            self.client = http.client.HTTPConnection(self.ip+':'+str(self.port))

    def disconnect(self):

        self.client = None

    def status(self,host=None):

        if not self.client:
            raise InternalError("HttpClient not connected")
        client = self.client

        uri = "/status"
        if host:
            uri += '/' + str(host)
        headers = { 'Authorization' : 'Basic ' + self.user}
        client.request("GET",uri,headers=headers)
        response = client.getresponse()
        body = response.read()
        body = json.loads(body.decode('utf-8'))

        return body

    def pstatus(self,host=None):

        if not self.client:
            raise InternalError("HttpClient not connected")
        client = self.client

        uri = "/pstatus"
        if host:
            uri += '/' + str(host)
        headers = { 'Authorization' : 'Basic ' + self.user}
        client.request("GET",uri,headers=headers)
        response = client.getresponse()
        body = response.read()
        body = json.loads(body.decode('utf-8'))

        return body

    def hosts(self):

        if not self.client:
            raise InternalError("HttpClient not connected")
        client = self.client

        uri = "/hosts"
        headers = { 'Authorization' : 'Basic ' + self.user}
        client.request("GET",uri,headers=headers)
        response = client.getresponse()
        body = response.read()
        body = json.loads(body.decode('utf-8'))

        return body

    def updatewhitelist(self,host=None):

        if not self.client:
            raise InternalError("HttpClient not connected")
        client = self.client

        if not host:
            body = jsonError("/upatewhitelist expected host",httpstatus=443)
            return body

        uri = "/updatewhitelist/"+str(host)
        headers = { 'Authorization' : 'Basic ' + self.user}
        client.request("PUT",uri,headers=headers)
        response = client.getresponse()
        body = response.read()
        body = json.loads(body.decode('utf-8'))

        return body

    def hosts(self):

        if not self.client:
            raise InternalError("HttpClient not connected")
        client = self.client

        uri = "/hosts/show"
        headers = { 'Authorization' : 'Basic ' + self.user}
        client.request("GET",uri,headers=headers)
        response = client.getresponse()
        body = response.read()
        body = json.loads(body.decode('utf-8'))

        return body

    def tests(self):

        if not self.client:
            raise InternalError("HttpClient not connected")
        client = self.client

        uri = "/tests?a=b%20&c=d"
        headers = { 'Authorization' : 'Basic ' + self.user}
        client.request("GET",uri,headers=headers)
        response = client.getresponse()
        body = response.read()
        body = json.loads(body.decode('utf-8'))

        return body

    def quote(self,json):

      if not self.client:
          raise InternalError("HttpClient not connected")
      client = self.client
      uri = "/quote/" + getHostName()
      headers = {}
      headers['Authorization'] = 'Basic ' + self.user
      headers['Content-Type']  = 'application/json'
      client.request("PUT",uri,body=json,headers=headers)
      response = client.getresponse()
      body = response.read()
      body = json.loads(body.decode('utf-8'))

      return body

#-------------------------------------------------------------------------------
#
# __main__
#

if __name__ == '__main__':

    import json
    import jsonFuncs

    client = HttpClient(url="http://H9Mg3ASvnYe6YEIb9Pa7CCTqYiDX71gAdiLKnlHZ@127.0.0.1:8079")

    if True:

        data = client.status()
        if not jsonFuncs.isJsonData(data):
            raise InternalError("Response isn't a proper Json 'data' object")
        data = data['data']
        state = data['state']
        ts    = data['timestamp']
        print("Status as of %s..."%repr(ts))

        hosts = list(state.keys())
        hosts.sort()
        maxHost = 0
        for host in hosts: maxHost = max(maxHost,len(host))
        for host in hosts:
            hostData = state[host]
            if not hostData: continue
            entry = hostData[-1]
            tsQuote   = entry['tsQuote']
            tsAttest  = entry['tsAttest']
            status    = entry['status']
            numExcept = len(entry['exceptions'])
            print("  %s%s - %s: %s%s (%d exceptions)"%(host,' '*(maxHost-len(host)),tsAttest,status,' '*(len('NOT trusted')-len(status)),numExcept))

    if False:

        data = client.updatewhitelist('xxx')

    if False:

        data = client.hosts()

    if False:

        data = client.tests()
