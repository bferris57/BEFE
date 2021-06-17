from library   import *
from libitems  import *
from liberrors import *
from libutils  import *

lib = Library()
typename = "CD"
item = lib.NewLibItemByTypeName(typename)

errors = item.Validate()
if errors != None and len(errors):
  for error in errors:
    print "Error: "+LibError(error)
else:
  print "new LibItem() validates just fine"

print "item.memberNames =", repr(item.memberNames)
print "item.members     =", repr(item.members)
print "item.Type        =", repr(item.Type)

str = "19/12/1957"
result = ValidateDate(str)
if result:
  print "Validate('"+str+"') =", LibError(result)