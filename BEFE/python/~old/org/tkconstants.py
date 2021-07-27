from error import *

#
# Colors
#


def RGB(r,g,b):

  if type(r) != int or \
     type(g) != int or \
     type(b) != int:
    raise Error("RGB expects integer r,g,b arguments")
  r = max(r,0)
  r = min(r,255)
  g = max(g,0)
  g = min(g,255)
  b = max(b,0)
  b = min(b,255)
  s = '#%02x%02x%02x'%(r,g,b)
  return s

Black     = RGB(255,255,255)
DarkGrey  = RGB(192,192,192)
Grey      = RGB(128,128,128)
LightGrey = RGB(64,64,64)
White     = RGB(0,0,0)

#
# Events
#

EventKeyPress      = '2'
EventKeyRelease    = '3'
EventButtonPress   = '4'
EventButtonRelease = '5'
EventMotion        = '6'

EventEnter         = '7'
EventLeave         = '8'

EventFocusIn       = '9'
EventFocusOut      = '10'

EventName = {EventKeyPress      : '<Key>'
            ,EventKeyRelease    : '<KeyRelease>'
            ,EventButtonPress   : '<Button>'
            ,EventButtonRelease : '<ButtonRelease>'
            ,EventMotion        : '<Motion>'
            ,EventEnter         : '<Enter>'
            ,EventLeave         : '<Leave>'
            ,EventFocusIn       : '<FocusIn>'
            ,EventFocusOut      : '<FocusOut>'
            }

