#!/usr/bin/python3
#coding=utf-8
#===============================================================================
#
# File:    dbfuncs.py - Various SQL Database related common functions
#
# Functions: latestQuoteNames   - Return {<host>:<objName>...} of latest quotes
#            validateQuote      - Validate a TrustAgent quote
#            updatewhitelist    - Update single White List (the work horse function)
#            updateAIK          - Add a host to mw_hosts
#            getMysqlConnection - Connect to MySQL and return a cursor to use
#
#===============================================================================

#
# Globals
#

import globals
import swift
import MySQLdb

from funcs   import *
from jsonApi import JsonApiErrors
from jsonApi import JsonApiWarnings

import              properties
from DotDict import DotDict

swiftcon = None

#-------------------------------------------------------------------------------
#
# Function: latest   = latestQuoteNames() - Return {<host>:<objName>...} of latest quotes
#
# Where:    host     - str: Host name
#           objName  - str: Object name (None = "No latest quote")
#           latest  - dict: <host>:<objName> dictionary
#

def latestQuoteNames():

    # Get the swift quotes
    global swiftcon
    swiftcon = swift.SwiftConnection()
    swiftcon.connect()
    quotes = swiftcon.getHostQuotes()

    # Do it...
    latest = {}
    for host in quotes:
        latest[host] = quotes[host][-1]

    return latest

#-------------------------------------------------------------------------------
#
# Function: validateQuote - Validate a TrustAgent quote
#
# Usage:    (errors, warnings,aikquote,files) = validateQuote(contents)
#
# Where:    contents - str: Contents of the quote
#
#           errors   - list: Any errors encountered
#           warnings - list: Any warnings encountered
#           aikcert  - str:  Quote's AIK Certificate (None if errors)
#           files    - dict: Dictionary of hashes (<file>:[<hash>...])
#

def validateQuote(quote):

    errors   = []
    warnings = []
    aikcert  = None
    files    = {}

    posStart = quote.find('<ima>')
    if posStart < 0:
        warnings.append("Malformed quote: no '<ima>' in body, skipping")
        return (errors,warnings,aikcert,files)

    posStart += 5
    posEnd   = quote.find('</ima>',posStart)
    if posEnd < 0:
        warnings.append("Malformed quote: no '</ima>' in body, skipping")
        return (errors,warnings,aikcert,files)

    fullQuote = quote
    quote = quote[posStart:posEnd]
    lines = quote.split('\n')

    # Get the AIK Certificate...
    begin = '<aikcert>'
    end   = '</aikcert>'
    aikStart = fullQuote.find(begin)
    aikEnd   = fullQuote.find(end)
    if aikStart < 0 or aikEnd <= aikStart:
        errors.append("Malformed quote: No '<aikcert>'")
        return (errors,warnings,aikcert,files)
    aikcert = fullQuote[aikStart+len(begin):aikEnd]

    # Process the IMA lines...
    for i in range(0,len(lines)):
        line = lines[i].strip()
        if not line:
            continue
        parts = line.split(' ')
        if len(parts) < 5:
            warnings.append("Malformed line %d in quote: %s"%(i+1,repr(line)))
            continue
        template = parts[2]
        if template not in ['ima','ima-ng','ima-sig','log1']:
            errors.append("Unknown template in line %d in quote: %s"%(i+1,repr(line)))
            continue

        hash = parts[3]
        if hash[0:5] == 'sha1:':
            hash = hash[5:]
        if hash[0:7] == 'sha256:':
            hash = hash[7:]

        file = parts[4]
        if file not in files:
            files[file] = []
        if hash not in files[file]:
            files[file].append(hash)

    return (errors,warnings,aikcert,files)

#-------------------------------------------------------------------------------
#
# Function: updatewhitelist - Update single White List
#
# Usage:    (errors,warnings) = updatewhitelist(host,clean=False)
#
# Where:    hosts - str: Host name
#

def updatewhitelist(host,clean=False,logger=None):

    import swift

    errors = JsonApiErrors()
    warnings = JsonApiWarnings()

    if not host:
        errors.append("No whitelist(s)")
        return (errors,warnings)

    # Get the swift quotes
    quoteNames = latestQuoteNames()

    #
    # Do the dirty work...
    #

    # Make sure swift is connected...
    global swiftcon
    if not swiftcon:
        swiftcon = swift.SwiftConnection()
        swiftcon.connect()

    # Make sure there's a latest quote...
    if host not in quoteNames:
        warnings.append("Host %s has no latest Rados quote, skipping"%repr(host))
        return (errors,warnings)

    lastQuote = quoteNames[host]
    secs      = objNameAge(lastQuote)

    # Get the quote text...
    quote = swiftcon.getObject(lastQuote)

    if not quote: # In case it's one of those weird empty ceph objects...
      warnings.append("Host %s has no latest Rados quote, skipping"%repr(host))
      return (errors,warnings)

    quote = quote.contents.decode('utf-8')
    posStart = quote.find('<ima>')
    if posStart < 0:
        warnings.append("Malformed quote: no '<ima>' in body, skipping")
        return (errors,warnings)

    # Validate it...
    errs,warns,aikcert,files = validateQuote(quote)
    if errs or warns:
        errors.extend(errs)
        warnings.extend(warns)
        if errs:
          return (errors,warnings)

    #
    # Update it...
    #

    dbcon, dbcur = getMysqlConnection(verbose=True)

    # Update the AIK Certificate in mw_hosts...
    updateAIK(host,dbcur=dbcur,aikcert=aikcert,quiet=False)
    dbcon.commit()

    # Get current exclusions (if any)...
    exclusions = {}
    sql = "select filename from mw_ima where hostname = '"+host+"' and filehash = ''"
    count = dbcur.execute(sql)
    if count > 0:
        result = dbcur.fetchall()
        for row in result:
            exclusions[row[0]] = ''

    # Count current rows and get rid of them if [clean] specified, except exclusions...
    sql = "select count(*) from chas.machines where hostName = '"+host+"'"
    count = dbcur.execute(sql)
    if count != 1:
        Error("Invalid count from MySQL = %s"%repr(count))
        return False
    result = dbcur.fetchall()
    count = result[0][0]
    if count and clean:
        msg = "Deleting %s existing whitelist rows for %s"%(count, repr(host))
        if logger:
          logger.info(msg)
        else:
          print(msg)
        sql = "delete from mw_ima where hostName = '"+host+"' and fileHash != ''"
        delCount = dbcur.execute(sql)
        if delCount != count:
            errors.append("Interal Error: delCount %s not same as count %s!!!"%(repr(delCount),repr(count)))
            return (errors,warnings)
    dbcon.commit()

    # See which file hashes are already there...
    existing = {}
    sql = "select fileName, fileHash from mw_ima where hostName = '%s' order by fileName"%host
    count = dbcur.execute(sql)
    if count:
        result = dbcur.fetchall()
        for row in result:
            file, hash = row
            if file not in existing:
                existing[file] = []
            if hash not in existing[file]:
                existing[file].append(hash)

    # Determine which ones are already there so we don't re-insert them...
    # Note: existing - dict: Hashes by file from database
    #       files    - dict: Hashes by file from quote
    keys = list(files.keys())
    keys.sort()
    count = 0
    duplicates = 0
    for file in keys:
        if file not in existing or file not in files:
            continue
        i = 0
        while file in files and i < len(files[file]):
            if files[file][i] in existing[file]:
                del files[file][i]
                duplicates += 1
                if not files[file]:
                    del files[file]
            else:
                i += 1
                count += 1

    keys = list(files.keys())
    keys.sort()
    rows = []
    for file in keys:
        if file in exclusions:
            continue
        # Add each of the file's hash(s)...
        for hash in files[file]:
            rows.append((file,hash,host))

    # Insert them...
    sql = "insert into mw_ima (fileName, fileHash, hostName) values (%s,%s,%s)"
    count = dbcur.executemany(sql,rows)
    if count and rows and count != len(rows):
        errors.append("Recieved count %s from MySql execute, expected %s"%(repr(len(rows)),repr(count)))
        return (errors,warnings)
    dbcon.commit()

    return (errors,warnings)

#-------------------------------------------------------------------------------
#
# Function: updateAIK - Add a host to mw_hosts
#
# Usage:    updateAIK(host,dbcur,aikcert)
#
# Where:    host    - str: Host name
#           dbcur   - Cursor: MySQL DB Cursor (None = "Make new connection")
#           aikcert - str: Host's Aik Certificate
#
# Note:     If the host is already there it will be updated.
#
#           This function DOES NOT commit the changes, it's up to the caller
#           to commit it through the MySQL connection.
#

def updateAIK(hostName,dbcur=None,aikcert=None,quiet=False,logger=None):

    import config
    config.load()
    if not hostName in config.globals.hosts:
        msg = "updateAIK: Host %s not configured (see porridge.json)"%repr(hostName)
        if logger:
            logger.error(msg)
        elif not quiet:
            Error(msg)
        return

    # Connect if no cursor passed...
    if not dbcur:
        dbcon, dbcur = getMysqlConnection()

    # Get host's name and IP...
    host = config.globals.hosts[hostName]['name']
    if 'ip' in config.globals.hosts[hostName]:
        ip = config.globals.hosts[hostName]['ip']
    else:
        ip = ''

    # Fiddle with aikcert turning it into MySQL 'insert' string literal...
    if aikcert:
        start = "-----BEGIN CERTIFICATE-----"
        end   = "-----END CERTIFICATE-----"
        if aikcert[-len(end):] == end:
            aikcert = aikcert[:-len(end)]
        if aikcert[:len(start)] == start:
            aikcert = aikcert[len(start):]
        lines = aikcert.split('\r\n')
        lines.insert(0, "-----BEGIN CERTIFICATE-----")
        if not lines[-1]:
            del lines[-1]
        lines.append("-----END CERTIFICATE-----")
        lines.append('')
        aikcert = '\\r\\n'.join(lines)
    else:
        aikcert = ''

    # Get VMM and BIOS MLE Ids...
    sql = "select ID from mw_mle where MLE_Type = 'VMM'"
    count = dbcur.execute(sql)
    if count != 1:
        msg = "updateAIK: 'VMM' MLE is missing"
        if logger:
            logger.error(msg)
        else:
            Error("updateAIK: 'VMM' MLE is missing")
        return
    result = dbcur.fetchall()
    id_vmm = result[0][0]
    sql = "select ID from mw_mle where MLE_Type = 'BIOS'"
    count = dbcur.execute(sql)
    if count != 1:
        msg = "updateAIK: 'BIOS' MLE is missing"
        if logger:
            logger.error(msg)
        else:
            Error("updateAIK: 'BIOS' MLE is missing")
    result = dbcur.fetchall()
    id_bios = result[0][0]

    # Insert a new row if it doesn't exist yet...
    sql = "select count(*) from mw_hosts where Name = '"+host+"'"
    count = dbcur.execute(sql)
    result = dbcur.fetchall()
    count = result[0][0]
    if count == 0:
        sql =  "insert into mw_hosts (BIOS_MLE_ID, VMM_MLE_ID, Name, IPAddress, Port) "
        sql += "values ("+str(id_bios)+","+str(id_vmm)+",'"+host+"','"+ip+"',9999)"
        count = dbcur.execute(sql)
        if count > 1:
            msg = "updateAIK: Count from dbcur.execute() = %s"%repr(count)
            if logger:
                logger.critical(msg)
            else:
                Internal(msg)
    sql = "update mw_hosts set AIK_Certificate = '"+aikcert+"' where Name = '"+host+"'"
    count = dbcur.execute(sql)
    if count > 1:
        msg = "updateAIK: Expected 1 row update, got %s"%repr(count)
        if logger:
            logger.critical(msg)
        else:
            Internal(msg)

    return

#------------------------------------------------------------------------------
#
# Function: getMysqlConnection - Connect to MySQL and return a cursor to use
#
# Usage:    dbcon, dbcur = getMysqlConnection()
#
# Where:    dbcon - MySQL Connection
#           dbcur - MySQL Cursor
#

def getMysqlConnection(verbose=globals.verbose,logger=None,db="mw_as",server='doom',user='root',password='EnDrAcKa'):

    if not server or not user or not password:

        raise RuntimeError("Expected db, server, user, and password")

    # Connect to database...
    dbcon = MySQLdb.connect(host=server, user=user, passwd=password, db=db, port=3306,use_unicode=True, charset="utf8mb4")
    dbcur = dbcon.cursor()

    return (dbcon,dbcur)

#-------------------------------------------------------------------------------
#
#  __main__
#

if __name__ == "__main__":

    if True:

        dbcon, dbcur = getMysqlConnection()
        sql = "select name from machines order by name"
        count = dbcur.execute(sql)
        results = dbcur.fetchall()
        print("DEBUG: results = %s"%repr(results))
        hosts = []
        for row in results:
            host = row[0]
            if host not in hosts:
                hosts.append(host)
        print("Connected to MySql!!!")
        print("hosts = %s"%repr(hosts))

    if False:

        if len(sys.argv) < 2:
            fn = "empty_filename.quote"
        else:
            fn = sys.argv[1]

        contents = open(fn).read()

        (errors, warnings,aikquote,files) = validateQuote(contents)

        print("%d errors, %d warnings, %d files"%(len(errors),len(warnings),len(files)))

        for file in files:
          hashs = files[file]
          for hash in hashs:
              if hash == 'e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855':
                  print("File = %s"%repr(file))
                  break
