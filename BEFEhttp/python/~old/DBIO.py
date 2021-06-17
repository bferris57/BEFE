#!/usr/bin/python
#coding=utf8
#===============================================================================
#
# File:    DBIO - General Database I/O
#
# Purpose: To wrap database connections/cursors in an opaque wrapper
#
# Notes:   Current implementation is PostgreSQL only. Expect to expand to MySQL
#          and others in the near future.
#
#          This class only connects to a single database at a time. Use multiple
#          instances if you need multiple connections or cross-database access.
#
#===============================================================================

# External dependencies
import psycopg2 as dbapi2
import MySQLdb  as dbmysql
import time

from DBIO_functions import *
from DBIO_metadata  import *
from DBIO_errors    import *
from DBIO_Struct    import *

#===============================================================================
#
# Class: DBIO - General DataBase I/O
#
#===============================================================================

class DB:

  def __init__(self, url=None):
  
    self.url    = url
    self.db     = None
    self.dbtype = None
    self.schema = ''
    
    # Connect if URL given
    if url:
      self.Connect(url)
    
  def IsConnected(self):
  
    if self.db:
      return True
    return False
    
  def Connect(self, url=None):

    # Disconnect first...
    if self.IsConnected():
      self.Disconnect()
      
    # Parse the URL
    dict = ProcessURL(url)
      
    # Pull stuff we need from it...
    args = {}
    for key in dict:
      if key in ['dbname','database','user','password','host','port']:
        args[key] = dict[key]
        
    # Connect
    #try:
    dbtype = dict['dbtype']
    if len(dbtype) >= 2 and dbtype[0] == 'p' and dbtype.find('g') > 1:
      dbtype = 'postgresql'
    if dbtype == 'postgresql':
      db = dbapi2.connect(**args)
    elif dbtype == 'mysql':
      print("DEBUG: ***Connecting to mysql...")
      myargs = {}
      myargs['host'] = args['host']
      myargs['user'] = args['user']
      myargs['passwd'] = args['password']
      myargs['db']     = args['database']
      db = dbmysql.connect(**myargs)
      print("DEBUG: Connected")
    else:
      raise Failed("Unknown database type '"+dbtype+"'")
    self.dbtype = dbtype
    
    #except:
    #  raise Failed('Failed to connect')

    # Save our connection
    self.db = db
    
    # Set schema if specified...
    schema = dict['schema']
    if schema:
      schemas = Schemas(self)
      if not schema in schemas:
        raise Failed("Schema '"+schema+"' does not exist")
      SetCurrentSchema(self,schema)
       
  def Disconnect(self):
  
    if not self.db: return
      
    # Disconnect
    #self.db.Close()
    self.db = None

  def Describe(self,stmt):
  
    if not self.db:
      raise Failed("DB.Rows: Not connected")
    if not StatementType(stmt) == 'select':
      raise Failed("DB.Rows: Expected 'select', got '"+StatementType(stmt)+"' statement")

    csr = self.db.cursor()
    csr.execute(stmt)
    
    # If we're supposed to describe it...
    struct = Struct()
    for d in csr.description:
      name,type_code,display_size,internal_size,precision,scale,null_ok = d
      name = name.lower()
      elm = ElmDef()
      #dict[name] = (type_code,display_size,internal_size,precision,scale,null_ok)
      elm.name = name
      elm.type = type_code
      if display_size:
        elm.dimension = [display_size]
      if precision:
        elm.precision = [precision,scale]
      if null_ok:
        elm.nullable = True
      else:
        elm.nullable = False
      struct.AddElmDef(elm)
      
    return struct

  def Rows(self,stmt,desc=False):
  
    if not self.db:
      raise Failed("DB.Rows: Not connected")
    if not StatementType(stmt) == 'select':
      raise Failed("DB.Rows: Expected 'select', got '"+StatementType(stmt)+"' statement")

    csr = self.db.cursor()
    csr.execute(stmt)
    
    # If we're supposed to describe it...
    if desc:
      dict = {}
      dict['__cols__'] = []
      for d in csr.description:
        name,type_code,display_size,internal_size,precision,scale,null_ok = d
        name = name.lower()
        dict['__cols__'].append(name)
        dict[name] = (type_code,display_size,internal_size,precision,scale,null_ok)

    # Fetch the rows...      
    while True:
      row = csr.fetchone()
      if row is None:
        csr.close()
        raise StopIteration
      
      # If non-described row...
      if not desc:
        yield row
        continue
        
      # Build described row...
      d = {}
      cols = dict['__cols__']
      for i in range(0,len(cols)):
        d[cols[i]] = row[i]
        
      yield d
      
    raise StopIteration
      
  def StructRows(self,stmt):
  
    if not self.db:
      raise Failed("DB.Rows: Not connected")
    if not StatementType(stmt) == 'select':
      raise Failed("DB.Rows: Expected 'select', got '"+StatementType(stmt)+"' statement")

    # Execute first      
    csr = self.db.cursor()
    csr.execute(stmt)
    
    # Build struct
    cols = ''
    for desc in csr.description:
      name = desc[0]
      if cols:
        cols += ','
      cols += name
    struct = Struct(cols)
    
    # Do it...
    while True:
      row = csr.fetchone()
      if row is None:
        csr.close()
        raise StopIteration
      struct.SetValues(row)
      yield struct
      
def trythis():

  db = DB()
  
  url = 'postgresql://localhost/BEFE?user=bruce,password=bruce,schema=bruce'
  db.Connect(url)
  
  cnt = 0
  stmt = """
select *
  from information_schema.tables
 order by
       table_schema,
       table_type,
       table_name
         """

  # Strip of leading \n
  stmt = stmt[1:]
  
  if False:
    for row in db.StructRows(stmt):
      print("table = "+row.table_schema+'.'+row.table_name)
      cnt += 1
      if cnt > 200: break
    return None
    
  if True:
    stmt = """
select * from information_schema.columns
 where table_name = ''
"""
    stmt = stmt[1:]
    struct = db.Describe(stmt)

    lines = stmt.split('\n')
    for lineno in range(0,len(lines)):
      if lineno == 0:
        print("Stmt: ",)
      else:
        print("      ",)
      if lines[lineno].strip() != '':
        print(lines[lineno])
    
    print('')
    print("Description...")
    nelms = len(struct)
    for i in range(0,nelms):
      print("  "+repr(struct.GetElmDef(i)))
    return struct
    
if __name__ == '__main__':

  trythis()
