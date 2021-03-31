#!/usr/bin/python3
#coding=utf-8
from __future__ import print_function
#===============================================================================
#
# File:    host_cmd.py - Host command handler
#
# Purpose: Manage configured hosts
#
# Notes:   This implements CLI 'host' command as follows...
#
#             add <host>          - Add a new host
#             set|upd[ate] <host> - Set/update an existing host
#             del[ete] <host>     - Delete an existing host
#             rem[ove] <host>     -   "    "     "      "
#
#          Where <host> is a host name in the following format...
#
#             <name>[@[<ip>[:<port>]]]
#
#          Where: <ip> is a valid ipv4/ipv6 ip address.
#
#                 <port> is a valid trustagent port number
#
#          Adding a new host sets the host's AIK Certificate to ''.
#          You need to use the 'updatewhitelist' command for it for
#          a successful attestation.
#
#===============================================================================

from   funcs   import *
from   swift   import *
from   MinExp  import MinExp
import dbfuncs

import MySQLdb
import config

#-------------------------------------------------------------------------------
#
# Function: host_cmd - Command: Handle host commands
#

def host_cmd(argv=[],ipChar='@'):

    config.load()

    if not argv:
        return "'host' what?"

    toDo = argv[0]
    del argv[0]

    if toDo not in MinExp("add|del[ete]|rem[ove]"):
        return "Unknown host sub-command %s"%repr(toDo)

    config.load()

    if toDo in MinExp("add"):

        if not argv:
            return "'host add' what?"
        if len(argv) > 1:
            return "'host add' expects single <host>"

        host,ip,port,error = parseIp(argv[0],ipChar=ipChar)
        if error:
            return error

        if host in config.globals.hosts:
            return "Host %s is already configured, use 'host set' instead"%repr(host)

        info = {"name":host}
        if ip:
            info['ip'] = ip
        if port:
            info['port'] = int(port)

        config.globals.hosts[host] = info
        config.save()

        # Send config to OATs...
        src = '/home/porridge/porridge.json'
        dst = src
        cmd = "scp %s %s:%s"%(src,host,dst)
        for oat in config.globals.oats:
            cmd = "scp %s %s:%s"%(src,oat,dst)
            execute(cmd)

        # Update the AIK certificate to get mw_hosts updated...
        dbcon, dbcur = dbfuncs.getMysqlConnection()
        dbfuncs.updateAIK(host,dbcur,aikcert='',quiet=False)
        dbcon.commit()

        # Create attest#<host> swift object...
        sc = SwiftConnection()
        sc.connect()
        sc.putObject("attest#"+host)

        return ''

    if toDo in MinExp("del[ete]|rem[ove]"):

        if not argv:
            return "'host delete' what?"
        if len(argv) > 1:
            return "'host delete' expects only a single <host>"

        host,ip,port,error = parseIp(argv[0])
        if error:
            return error
        if ip or port:
            return "'host delete' does not expect IP/Port in <host>"

        if not host in config.globals.hosts:
            return "Host %s not configured, can't delete"%repr(host)
        if '.' not in host:
            return "Expected '.' in host name, got %s"%repr(host)

        del config.globals.hosts[host]
        config.save()

        # Remove the attest#<host> object if it's there...
        objName = "attest#"+host
        sc = SwiftConnection()
        sc.connect()
        sc.deleteObject(objName)

        # Send config to OATs...
        src = config.globals.fileName
        dst = src
        cmd = "scp %s %s:%s"%(src,host,dst)
        for oat in config.globals.oats:
            cmd = "scp %s %s:%s"%(src,oat,dst)
            execute(cmd)

        # Remove the database rows...
        dbcon, dbcur = dbfuncs.getMysqlConnection()
        sql = "delete from mw_hosts where name = '" + host + "'"
        count = dbcur.execute(sql)

        sql = "delete from mw_ima where hostname = '" + host + "'"
        count = dbcur.execute(sql)

        dbcon.commit()

        return ''

    return "Unknown host command %s"%repr(toDo)
