#!/usr/bin/python
#coding=utf8
#===============================================================================

from DBIO_errors import *
import decimal
import time

# Note: Tuples contain... (pytype, minitype, min_value, max_value, min_len, max_len, dim, scale, pec)
#
# Where: pytype     = Python type (int, float, str, etc.)
#        minitype   = Single character (mainly)...
#                       'N' = Numeric
#                       'M' = Monentary
#                       'X' = Bytes
#                       'x' = Bits
#                       'S' = Character strings
#                       'D' = Date/time
#                       'B' = Boolean
#                       'E' = Enumerated
#                       'G' = Geometric
#                       'U' = UUID
#                       'A' = Internet address types
#                       'm' = XML Types
#                       'j' = Json Types
#                       'C' = Composit (struct) types
#         min_value = Minimum value (None if not applicable)
#         max_value = Maximum value (None if not applicable)
#         min_len   = Minimum length (None of not applicable)
#         max_len   = Maximum length (None of not applicable)
#         dim       = Dimension (list of dimension size max)
#         scale     = Numeric scale (None if not applicable)
#         prec      = Numeric precision (None if not applicable)
sql_datatypes = {

"smallint":         (int             , 'N', None, None, None, None, [], None,None),
"integer":          (int             , 'N', None, None, None, None, [], None,None),
"bigint":           (int             , 'N', None, None, None, None, [], None,None),
"decimal":          (int             , 'N', None, None, None, None, [], None,None),
"numeric":          (float           , 'N', None, None, None, None, [], None,None),
"real":             (float           , 'N', None, None, None, None, [], None,None),
"double"
  " precision":     (float           , 'N', None, None, None, None, [], None,None),
"boolean":          (int             , 'N', None, None, None, None, [], None,None),

"character "
  "varying":        (int             , 'S', None, None, None, None, [], None,None),
"character":        (int             , 'S', None, None, None, None, [], None,None),
"varchar":          (int             , 'S', None, None, None, None, [], None,None),
"char":             (int             , 'S', None, None, None, None, [], None,None),
"text":             (int             , 'S', None, None, None, None, [], None,None),

"timestamp":        (time.struct_time, 'D', None, None, None, None, [], None,None),
"timestamp"         
  " with"
  " time zone":     (time.struct_time, 'D', None, None, None, None, [], None,None),
"timestamp"
  " without"
  " time zone"
"timestamptz":      (time.struct_time, 'D', None, None, None, None, [], None,None),
"time":             (time.struct_time, 'D', None, None, None, None, [], None,None),
"time"
  " with"
  " time zone":     (time.struct_time, 'D', None, None, None, None, [], None,None),
"time"
  " without"
  " time zone":     (time.struct_time, 'D', None, None, None, None, [], None,None),

  
"small serial":     (float           , 'N', None, None, None, None, [], None,None),
"serial":           (float           , 'N', None, None, None, None, [], None,None),
"big serial":       (float           , 'N', None, None, None, None, [], None,None),

"money":            (decimal.Decimal , 'N', None, None, None, None, [], None,None),

"inet":             (float           , 'N', None, None, None, None, [], None,None),
"cidr":             (float           , 'N', None, None, None, None, [], None,None),
"macaddress":       (float           , 'N', None, None, None, None, [], None,None),
}

def Schemas(db):

  stmt = "select schema_name from information_schema.schemata"
  if db.dbtype == 'mysql':
    stmt = "show databases"
  csr = db.db.cursor()
  csr.execute(stmt)
  rows = csr.fetchall()
  schemas = []
  for row in rows:
    schemas.append(row[0])
  return schemas
  
def SearchPath(db):

  if db.dbtype == 'mysql':
    return db.dbtype
    
  stmt = "show search_path"
  csr = db.db.cursor()
  csr.execute(stmt)
  row = csr.fetchone()
  sp = row[0]
  if sp and sp[0] == '"':
    sp = sp[1:-1]
  return sp
  
def SetSearchPath(db,slist):

  if db.dbtype == 'mysql':
    return
    
  schemas = Schemas(db)
  slist = slist.split(',')
  
  final = ''
  for schema in slist:
    schema = schema.strip()
    if not schema in schemas:
      raise AttributeError("Cannot use schema '"+schema+"' - schema doesn't exists")
    if final:
      final += ','
    final += schema
    
  stmt = "set search_path to '"+final+"'"
  csr = db.db.cursor()
  csr.execute(stmt)

def SchemaExists(db,schema):

  if db.dbtype == 'mysql':
    stmt = "show databases"
  else:
    stmt = "select count(*) from information_schema.schemata where schema_name = '"+schema+"'"
  csr = db.db.cursor()
  csr.execute(stmt)
  row = csr.fetchone()
  result = row[0] != 0
  return result
  
def CurrentSchema(db):

  stmt = "show search_path"
  csr = db.db.cursor()
  csr.execute(stmt)
  row = csr.fetchone()
  sp = row[0]
  if sp and sp[0] == '"':
    sp = sp[1:-1]
  sp = sp.split(',')
  return sp[0]
  
def SetCurrentSchema(db,schema):

  if not SchemaExists(db,schema):
    raise Failed("Schema '"+schema+"' does not exist")

  sp = SearchPath(db).split(',')
  try:
    idx = sp.index(schema)
  except ValueError:
    idx = -1
  if idx >= 0:
    del sp[idx]
    
  sp.insert(0,schema)
  sp = ','.join(sp)
  SetSearchPath(db, sp)
  db.schema = schema
  
def Tables(db,schema=None):

  if not schema:
    schema = db.schema
  if db.dbtype == 'mysql':
    stmt = "selec "
  stmt =  "select table_name"
  stmt += "  from information_schema.tables"
  stmt += " where table_schema = '"+schema+"'"
  csr = db.db.cursor()
  rows = db.Rows(stmt)
  result = []
  for row in rows:
    result.append(row[0])
  
  return result

def Columns(db,table,schema=None):

  if not schema:
    schema = db.schema
  stmt =  "select column_name"
  stmt += "  from information_schema.columns"
  stmt += " where table_schema= '"+schema+"' and table_name = '"+table+"'"
  stmt += " order by ordinal_position"
  csr = db.db.cursor()
  rows = db.Rows(stmt)
  result = []
  for row in rows:
    result.append(row[0])
    
  return result
  
def ColumnDefs(db,table,schema=None):

  if not schema:
    schema = db.schema
  stmt =  "select *"
  stmt += "  from information_schema.columns"
  stmt += " where table_schema= '"+schema+"' and table_name = '"+table+"'"
  stmt += " order by ordinal_position"
  
  rows = db.Rows(stmt,True)
  
  result = []
  
  nrows = 0
  
  for row in rows:
  
    nrows += 1
    
    # Do name, type, default...
    cn = row['column_name']
    dt = row['data_type'].strip()
    cd = row['column_default']
    
    varying = False
    if dt[-8:] == ' varying':
      dt = dt[0:-8]
      varying = True
    if dt == 'timestamp with time zone':
      dt = 'timestamptz'
    if dt == 'timestamp without time zone':
      dt = 'timestamp'
      
    default = ''
    if cd:
      default = cd
    
    t = [cn,dt,default]
    
    # Build [], scale[,precision]...
    if varying:
      var = '[]'
    else:
      var = ''
      
    prec = ''
    
    if row.has_key('numeric_precision_radix'):
      npr  = row['numeric_precision_radix']
    else:
      npr = None
    np   = row['numeric_precision']
    ns   = row['numeric_scale']    
    cmxl = row['character_maximum_length']
    dtp  = row['datetime_precision']
    
    if npr and npr != 2 and (np or ns):
      prec = '('
      if np:
        prec += str(np)
      if ns:
        prec += ','+str(ns)
      prec += ')'
      
    if cmxl:
      prec = '('+str(cmxl)+')'
    
    if dtp:
      prec = '('+str(dtp)+')'
      
    t.append(prec)
    t.append(var)
    
    # Build modifiers
    mods = ''
    if row.has_key('is_updatable') and row['is_updatable'] == 'NO':
      if mods: mods += ' '
      mods += 'immutable'
    if row['is_nullable'] == 'NO':
      if mods: mods += ' '
      mods += 'not null'
    
    t.append(mods)
    
    # Return the row
    result.append(tuple(t))
  
  return result
