from datetime import datetime
from datetime import timedelta
from test100  import str2datetime

start = str2datetime('2017-09-22 06:00:00')
end   = str2datetime('2017-09-22 06:00:30')

diff1 = (end-start).seconds
diff2 = (start-end).seconds

print("diff1 = %d, diff2 = %d"%(diff1,diff2))