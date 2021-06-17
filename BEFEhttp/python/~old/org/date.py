#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: date.py - General purpose dates, down to the microsecond
#
# This file implementes dates, times, and deltas equialent to the
# python datetime package but with extensions that make more sense
# as far as we're concerned.
#
# Functions: Now         - Return elapsed time since Jan 1, 1970 UTC
#
# Classes:   ElapsedTime - An elapsed time in milliseconds
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

from   error    import *
import datetime

#
# Time units
#

Microseconds = 1
Milliseconds = 1000
Seconds      = Milliseconds * 1000
Minutes      = Seconds*60
Hours        = Minutes*60
Days         = Hours*24
Years        = Days*365

TimeUnits = (Microseconds, Milliseconds, Seconds, Minutes, Hours, Days, Years)


#
# Calendars
#

CalHebrew    = 1
CalRoman     = 2
CalAUC       = 3
CalJulian    = 4
CalIslamic   = 5
CalGregorian = 6

CalendarNames = {"Hebrew"          : CalHebrew
                ,"Roman"           : CalRoman
                ,"Ab urbe condita" : CalAUC
                ,"AUC"             : CalAUC
                ,"Julian"          : CalJulian
                ,"Augustinian"     : CalJulian
                ,"Islamic"         : CalIslamic
                ,"Gregorian"       : CalGregorian
                }

#
# Year numbering
#


#
# Week numbering
#

#-----------------------------------------------------------------------
#
# Function: Now - Calculate elapsed seconds between now and 01 Jan, 1970
#
# This function returns an ElapsedTime instance describing the exact
# time since 01 Jan, 1970 (GMT).  This is a handy value for keeping
# timestamps of events taking place on computers.
#

def Now():
  basedate = datetime.datetime(1970,1,1)
  now = datetime.datetime.now()
  dtime = now.time()
  days = (now - basedate).days * Days
  hours = dtime.hour * Hours
  minutes = dtime.minute * Minutes
  seconds = dtime.second * Seconds
  microseconds = dtime.microsecond
  et = ElapsedTime(days+hours+minutes+seconds+microseconds,Microseconds)
  return et

#-----------------------------------------------------------------------
#
# Class: ElapsedTime - General elapsed time in microseconds
#
# This class implements a positive or zero time in elapsed microseconds
# and other easily related units.
#
# An ElapsedTime is simply a single value.  You set this value in
# whatever units you want.  Regardless of how you set this value, it
# keeps it as microseconds.  You may then get it as any other time unit.
#
# Note: Getting as another unit will truncate NOT round.  If you wish
#       rounding to occur, use the rounding functions.
#
#       Only positive values may be set as this is an elapsed time.
#       This may seem odd but it works out properly in the end.  Think
#       of elapsed time as a "distance between two points", just like
#       inches, feet, etc.  You wouldn't say "that is -5 miles from
#       here" would you?
#
#       Also, take Years as a rough unit since it's Julian years and
#       we, in the western world, use the Gregorian calendar NOT
#       the Julian one.  If you want Gregorian years you'll have
#       to slip into calendar mode NOT elapsed time.
#
# Members: **None**
#
# Methods: Object.Construct
#          Object.Die
#          Object.Validate     - Self check
#          ElapsedTime.Clear   - Clear elapsed time value
#          ElapsedTime.Set     - Set elapsed time (default Seconds)
#          ElapsedTime Get     - Get elapsed time (default Seconds)
#          ElapsedTime.GetOnly - Get elapsed time in truncated unit
#          ElapsedTime.Round   - Round to the nearest given unit
#
# Python:  __init__
#          __str__
#          __repr__
#          __getattr__
#          __setattr__
#
#-----------------------------------------------------------------------

class ElapsedTime:

  @classmethod
  def New(cls):

    obj = cls.__new__(cls)
    obj.Construct()
    return obj

  #
  # Object methods
  #

  def Construct(self,value=0,unit=Microseconds,typeid = None):

    if typeid == None:
      typeid = Id_TypeNode

    #Object.Construct(self,typeid)
    #Members(self)['microseconds'] = 0
    self.Set(value,unit)

  def Die(self):

    self.Validate()
    Object.Die(self)

  def Validate(self):

    #Object.Validate(self)
    return

  #
  # ElapsedTime methods
  #

  def Clear(self):

    self.microseconds = 0

  def Set(self,value,unit=Microseconds):

    if not unit in TimeUnits:
      raise Error("Unknown unit %s passed"%repr(unit))
    if value < 0:
      raise Error("Only zero or positive times allowed")
    self.microseconds = value*unit

  def Get(self,unit=Microseconds):

    if not unit in TimeUnits:
      raise Error("Unknown unit %s passed"%repr(unit))
    return int(self.microseconds/unit)

  def GetOnly(self,unit=Microseconds):

    # Find unit index
    for index in range(0,len(TimeUnits)):
      if TimeUnits[index] == unit: break
    if index >= len(TimeUnits):
      raise Error("Unknown unit %s passed"%repr(unit))

    # Get and truncate the value
    val = self.Get(unit)
    if index < len(TimeUnits)-1:
      val = val % (TimeUnits[index+1]/TimeUnits[index])
    return int(val)

  #
  # Python wrappers
  #

  def __init__(self,value=0,unit=Microseconds):

    self.Construct(value=value, unit=unit,typeid=Id_TypeElapsedTime)

  def __str__(self):

    return str(self.microseconds)

  def __repr__(self):

    return str(self.microseconds)

  def __int__(self):

    return int(self.microseconds)

  def  __lt__(self, other): return int(self) <  other
  def  __le__(self, other): return int(self) <= other
  def  __eq__(self, other): return int(self) == other
  def  __ne__(self, other): return int(self) != other
  def  __gt__(self, other): return int(self) >= other
  def  __ge__(self, other): return int(self) >  other
  def __add__(self, other): return ElapsedTime(int(self) +  int(other))
  def __sub__(self, other): return ElapsedTime(abs(int(self) - abs(int(other))))
  def __abs__(self):        return ElapsedTime(abs(int(self)))
