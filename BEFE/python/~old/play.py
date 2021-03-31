#coding=utf8

#import pg
import psycopg2 as dbapi2
import time

conInfo = {'database':'BEFE',
           'host'    :'127.0.0.1',
           'user'    :'bruce',
           'password':'bruce'}
           
ddlInfo = (
  {'type':'table',   'name':'nodes',     'cols':( ('id',   'int')       ,('parent', 'int')       ,('name','text') ) },
  {'type':'table',   'name':'snapshots', 'cols':( ('start','timestamp') ,('finish', 'timestamp')                  ) },
  {'type':'sequence','name':'seq_node_id'},
  )

"""  
def use_pg():

  pg.set_defhost   = '127.0.0.1'
  pg.set_defbase   = 'BEFE'
  
  print("Connecting...")
  con = pg.connect(**conInfo)
  print("Connected")

  qry = con.query('select id,line from t;')
  flds = qry.listfields()
  rows = qry.getresult()
  print("flds =",flds)
  print("rows =",rows)
"""

def sqlNow():

  t = time.gmtime()
  ts = dbapi2.Timestamp(t.tm_year, t.tm_mon, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec)
  return str(ts)
  
def iterRows(csr):
  
  while True:
    row = csr.fetchone()
    if row is None: break
    yield row
  
def use_psychopg():

  db = dbapi2.connect(**conInfo)
  cur = db.cursor()
  
  cur.execute("SELECT id,line FROM t");
  #desc = cur.description
  #nRows = cur.rowcount
  #aSize = cur.arraysize
  #print("desc =",desc)
  #print("nRows =",nRows)
  #print("aSize =",aSize)
  
  for row in iterRows(cur):
    id,line = row
    print("Line "+str(id)+": "+line)
  
  cur.close()
  db.commit()
  db.close()

def do_ddl_create(db):

  csr = db.cursor()
  for info in ddlInfo:
    stmt = 'create '
    type = info['type']
    stmt += type
    stmt += ' '
    stmt += info['name']
    if type == 'table':
      stmt += ' ('
      for colDef in info['cols']:
        name,type = colDef
        if stmt[-1] != '(':
          stmt += ', '
        stmt += name + ' ' + type
      stmt += ')'
    csr.execute(stmt)
  csr.close()
  db.commit()

def do_ddl_drop(db):

  csr = db.cursor()
  for info in ddlInfo:
    stmt = 'drop ' + info['type'] + ' ' + info['name']
    csr.execute(stmt)
  csr.close()
  db.commit()

def ddl_verify(db):

  return False
  
def connect():

  db = dbapi2.connect(**conInfo)
  
  return db

def TableExists(db, name):

    cur = db.cursor()
    cur.execute("""
      SELECT COUNT(*)
      FROM information_schema.tables
      WHERE table_name = '{0}'
      """.format(name.replace('\'', '\'\'')))
    answer = False
    if cur.fetchone()[0] == 1:
      answer = True
      cur.close()
      
    cur.close()
    return answer
    
def trythis_uuid():

  db = connect()
  print("Connected...")
  cur = db.cursor()
  cur.execute("SELECT uuid_generate_v4()");
  desc = cur.description
  nRows = cur.rowcount
  aSize = cur.arraysize
  #print("desc =",desc)
  #print("nRows =",nRows)
  #print("aSize =",aSize)
  
  for row in iterRows(cur):
    print("uuid =",row[0])
  
  cur.close()
  db.commit()
  db.close()
  
def trythis(*pargs,**nargs):

  print("pargs =",repr(pargs))
  print("nargs =",repr(nargs))
  
if __name__ == "__main__":

  trythis(1,2,3,x=10,y=11)