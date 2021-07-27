#===============================================================================
#
# File:  SuedeReport.py
#
# Purpose:  This module provides the SuadeReport class. It is responsible for
#           gathering report information from the database and producing both
#           PDF and XML reports on demand.
#
# Notes:    1) Class SuedeReport implementation is definitely in progress
#           2) Have, so far, only retrieved data, formatting reports remains.
#           3) Only PostgreSQL is currently implemented. Intent is to make
#              it URL dependent so data can be stored in MySQL, Oracle, etc.
#           4) Have implemented the idea of a report "header" and "details"
#
# Concerns: 1) Putting "inventory" column in "reports" table is exceedingly
#              bad SQL design, should be separate table "report_items" with
#              'id' and columns "name" and "price". Concern is based on
#              personal database design principles that don't assume there
#              are a small number of details. So code should iterate, row
#              by row, to process the item details.
#           2) Having said that, item details shouldn't be kept in a python
#              member variable in the "SuadeReport" class either. Instead,
#              it should be redesigned throwing the iteration into the
#              the document generation stage.
#           3) Not sure of the idea of a "database URL" is viable or not.
#              We'll see.
#
#===============================================================================

# External code
import pg

#
# Local Globals
#

prefix = "***SuadeReport."  # Used for debugging/exception purposes

#===============================================================================
#
# Class:   SuadeReport - Implement Suade Test programming example given to
#                        Bruce Ferris by Murat Abur on 06-Apr-2016
#
# Methods: __init__   - Initialise SuadeReport with optional PostgreSql URL
#          setUrl     - Set the current database URL
#          connect    - Connect to current database URL
#          disconnect - Disconnect from current database (if any)
#
# Notes:   1) Doesn't handle non-PostgreSql databses yet
#          2) Doesn't produce XML or PDF reports yet
#          3) DOES gather report data ready to be formatted
#

class SuadeReport():

  def __init__(self, *args):
  
    if len(args) > 1:
      raise Exception(prefix+"__init__ expected only one argument (Database URL)")
    if len(args) == 1:
      self.dbUrl = args[0]
    else:
      self.dbUrl = None
      
    # Initialise other members...
    self.db          = None
    self.reportId    = None
    self.headerInfo  = None
    self.itemDetails = None
    
  def setUrl(self, url):
    self.dbUrl = url
    self.disconnect()
    return
    
  def connect(self):
  
    # Sanity checks...
    if not self.dbUrl:
      raise Exception(prefix+"connect: No URL specified")
 
    # Disconnect if already connected...
    if self.isConnected():
      self.disconnect()
      
    # Connect to database URL
    self.db = pg.connect(self.dbUrl)
    
  def disconnect(self):
  
    if self.db:
      self.db.disconnect()
      self.db = None
      
    return
    
  def isConnected(self):
    return self.db != None
    
  def getReportIds(self):
  
    # Sanity checks...
    if not self.dbUrl:
      raise Exception(prefix+"getReportIds: No URL specified")
    if not self.db:
      raise Exception(prefix+"getReportIds: Not connected to database")

    # Do the query
    query = self.db.query("select id from reports")
    rows = query.getresult()
    
    # Put into list...
    ids = []
    for row in rows:
      ids.append(row[0])
      
    # Got them...
    return ids

  def getReport(self, reportId):
  
    # Sanity checks...
    if not self.dbUrl:
      raise Exception(prefix+"getReport: No URL specified")
    if not self.db:
      raise Exception(prefix+"getReport: Not connected to database")
    if type(reportId) != type(1):
      raise Exception(prefix+"getReport: Expected numberic reportId")
      
    # Do the query
    stmt = "select * from reports where id = "+str(reportId)
    query = self.db.query(stmt)
    rows = query.dictresult()
    
    # Make sure one and only one row...
    if len(rows) < 1:
      raise Exception(prefix+"getReport: ReportId '"+str(reportId)+"' does not exist")
    if len(rows) > 1:
      raise Exception(prefix+"getReport: Multiple rows with Report Id '"+str(reportId)+"'")
      
    # Pull it apart
    row = rows[0]
    self.reportId    = reportId
    self.headerInfo  = eval(row['type'])
    self.itemDetails = self.headerInfo["inventory"]
    del self.headerInfo["inventory"]
        
#===============================================================================
#
# Code:  __main__ - Used for testing purposes
#
# Usage: 1) Import it and use the SuadeReport class or...
#        2) Use it for testing on command line: python SuadeReport.py
#
      
if __name__ == "__main__":

  report = SuadeReport("postgres://interview:LetMeIn@candidate.suade.org/suade")
  report.connect()
  ids = report.getReportIds()
  for id in ids:
    report.getReport(id)
    print "Id: "+str(report.reportId)
    print " items: "+repr(report.itemDetails)
    
    
  