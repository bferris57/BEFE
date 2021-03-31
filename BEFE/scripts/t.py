from enum import Enum

if __name__ == '__main__':

  a = Enum(',,,,ee = 100,ff,ggg')
  membs = ''
  for memb in a.Members():
    if membs: membs += ','
    membs += memb+'='+str(a[memb])
  print 'a.Members()   =',membs
  print 'a.Min()       =',a.Min()
  print 'a.Max()       =',a.Max()
  print 'a.Count()     =',a.Count()
  print ''
  a.ggg = 'Hi there!'
  print 'a.ggg         =',repr(a.ggg)
  print 'a.__members__ =',a.__members__
  print 'str(a)        =',str(a)
  print 'repr(a)       =',repr(a)