//----------------------------------------------------------------------
// File: Time.cpp - Implementaion of the Time Class
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Time.h"
#include "OperatingSystem.h"

namespace BEFE { // Namespace BEFE...

const Long BaseUnitsPerMillisecond = 1;
const Long BaseUnitsPerSecond      = 1000*BaseUnitsPerMillisecond;
const Long BaseUnitsPerMinute      = 60*BaseUnitsPerSecond;
const Long BaseUnitsPerHour        = 60*BaseUnitsPerMinute;
const Long BaseUnitsPerDay         = 24*BaseUnitsPerHour;
const Long BaseUnitsPerWeek        = 7*BaseUnitsPerDay;

const Long Units[Time::NUMUNITS+1] =
  {BaseUnitsPerMillisecond,
   BaseUnitsPerSecond,
   BaseUnitsPerMinute,
   BaseUnitsPerHour,
   BaseUnitsPerDay,
   BaseUnitsPerWeek,
  0};

const Int DaysInMonth[12] = {31,29,31,30,31,30,31,31,30,31,30,31};

//--------------------------------------------------------------------
//
// Class Methods
//
//--------------------------------------------------------------------

//
// Time Now() - Calculate time now
//

Time Time::Now() {

  Time now;
  now.Set();
  return now;

}

//--------------------------------------------------------------------
//
// Instance Methods
//
//--------------------------------------------------------------------

Long Time::Set() {
  if (!IsNull(TheBefe->TheOS))
    value = TheBefe->TheOS->GetCurrentTime();
  else
    value = LongNaN;
  return value;
}

void Time::Set(Long thetime) {
  value = thetime;
}

void Time::Reset() {
  value = LongNaN;
}

Int Time::Spin() {
  Int count;
  count = 1;
  while (value == Now())
    count++;
  return count;
}

Long Time::Get() {
  return value;
}

// For plural units (Milliseconds, seconds, etc.)
Long Time::Get(Unit unit) {
  if (unit >= 0 && unit < NUMUNITS)
    return value/Units[unit];
  return value/Seconds;
}

// For singular unit (Millisecond, Second, etc.)
Long Time::GetAs(Unit unit) {

  if (unit < 0 || unit >= NUMUNITS)
    return value%Minutes/Seconds;

  if (Units[unit+1] == 0)
    return value/Units[unit];

  return (value%Units[unit+1])/Units[unit];
}

//
// Method:  RoundUpTo(unit) - Round up to next integer multiple of a
//                            specified unit
//
// Usage:   Time time = new Time();
//          ...
//          value = time.RoundUpTo(Time::<unit>);
//
// Where:   <unit> - Time unit to round to (invalid assumes Time::Seconds)
//
//          Should be Milliseconds, Seconds, Minutes, Hours, Days or Weeks.
//
// Returns: (time+<unit>/2)/<unit>*<unit>, e.g. rounded to the unit.
//          For example, if Time.Seconds is passed and it's 1/2 a second past
//          midnight on 01 January, 2000 (e.g., time.Get() returned 500ms),
//          this method returns (Timet+500)/1000*1000, which is 1000.
//
// Note:    If an invalid unit is specified, the time is rounded to seconds
//          without notification.
//

Long Time::RoundUpTo(Unit unit) {

  Long uvalue;
  Long t;

  if (unit < 0 || unit >= NUMUNITS)
    uvalue = Seconds;
  else
    uvalue = Units[unit];

  t = value;
  if (t < 0)
    t = t - (uvalue >> 1);
  else
    t = t + (uvalue >> 1);

  return t/uvalue*uvalue;
}

//
// Method:  TruncateDownTo(unit) - Truncate to next lower integer multiple
//                                 of a specified unit
//
// Usage:   Time time;
//          ...
//          value = time.TruncateDownTo(Time::<unit>);
//
// Where:   <unit> - Time unit to truncate to (invalid assumes Time::Seconds)
//
//          Should be Milliseconds, Seconds, Minutes, Hours, Days or Weeks.
//
// Returns: time/<unit>*<unit>, e.g. truncate to the next lower unit.
//          For example, if Time.Seconds is passed and it's 1/2 a second past
//          midnight on 01 January, 2000 (e.g., time.Get() returned 500ms),
//          this method returns time/1000*1000, which is 0.
//
// Note:    If an invalid unit is specified, the time is rounded to seconds
//          without notification.
//
//          If the time is negative it goes the other way...  -1ms becomes
//          -1000ms.  This is so that this function always truncates in the
//          same "down" direction (meaning earlier in time) instead of doing
//          a modulus which truncates "towards zero".  ;-)
//

Long Time::TruncateDownTo(Unit unit) {

  Long uvalue;

  if (unit < 0 || unit >= NUMUNITS)
    uvalue = Seconds;
  else
    uvalue = Units[unit];

  return value/uvalue*uvalue;
}

//
// Method:  Delta() - Time difference
//
// Usage:   Time t1;
//          Time t2;
//          ...
//          t2.Refresh();
//          diff = t1.Delta(t2);
//
// Where:   ** Nothing to say **
//
// Returns: Time/unit, truncated.  For example, if Time.Seconds is
//          passed this function returns time/1000.  It'll be huge.
//
// If an invalid unit is specified, the time in milliseconds since
// 01 January, 2000 is returned.
//

Long Time::Delta(Time other) {

  if (other.value < value)
    return value - other.value;
  else
    return other.value - value;
}

//----------------------------------------------------------------------------
//
// GREGORIAN Date instance methods
//
//----------------------------------------------------------------------------
//
// Method:  GetMillisecond() - Return millisecond of second (0..999)
//          GetSecond()      - Return second of minute (0..59)
//          GetMinute()      - Return minutes of hour (0..59)
//          GetHour()        - Return hour of day (0..23)
//          GetDayOfWeek()   - Return day of week (0..6, Monday=0)
//          GetDayOfMonth()  - Return day of month (0..30)
//          GetDayOfYear()   - Return day of calendar year (0..365)
//          GetMonthOfYear() - Return Gregorian month of year (0..11)
//          GetYear()        - Return Gregorian year (-nnnnnnnnn..nnnnnnnnn)
//          IsLeapYear()     - Is this year a leap year?
//
// Usage:   ** Sort of obvious **
//
// Returns: Time/unit, truncated.  For example, if Time.Seconds is
//          passed this function returns time/1000.  It'll be huge.
//
// Note:    GetDayOfWeek()   returns day in range 1..7 (Monday = 1)
//          GetMonthOfYear() returns month in range 1..12
//          GetDayOfMonth()  returns day in range 1..31
//          GetDayOfYear()   returns day in range 1..366
//
//          GetYear() returns year -N through +N but NEVER a year zero
//            because...
//
//          There is NO year 0 AD or 0 BC in the Gregorian calendar.  So,
//          if GetAsYear() returned -1001, GetYear() will return -1000.
//
//          If you're one of those sadly frustrated astonomers out there,
//          you'll most likely to be using GetAsYear() instead of
//          GetYear().  This is because the Gregorian dates are not likely
//          your style.  From my experience, astronomers are more likely to
//          be interested in date arithmetic and should feel free to use
//          Time.Delta() for calculations.  So, a word to the wise...
//          stay away from Gregorian dates if you don't care for Gregorian
//          dates.  ;-)
//

Int Time::GetMillisecond() {
	return (Int)GetAs(Milliseconds);
}

Int Time::GetSecond() {
	return (Int)GetAs(Seconds);
}

Int Time::GetMinute() {
	return (Int)GetAs(Minutes);
}

Int Time::GetHour() {
	return (Int)GetAs(Hours);
}

Int Time::GetDayOfWeek() {

  Int days;

  days = Get(Days);
  
  if (days < 0)
    return days%-7 + 1;
  else
    return days%7 + 1;

}

Int Time::GetDayOfMonth() {

  Int dayofyear;
  Int dayofmonth;
  Int month;

  dayofyear = GetDayOfYear();
  if (!IsLeapYear() && dayofyear >= 59)
    dayofyear++;
  dayofmonth = dayofyear - 1;
  for (month=0; month<12 && dayofmonth >= DaysInMonth[month]; month++)
    dayofmonth -= DaysInMonth[month];

  return dayofmonth+1;
}

Int Time::GetDayOfYear() {

  Int days;
  Int year;

  days = Get(Days);
  year = GetYear() - 2000;
  if (year >= 400) {
    days -= year/400 * Time::DaysPerQuadCentury;
    year = year % 400;
  }
  if (year >= 100) {
    days -= year/100 * Time::DaysPerCentury;
    year = year % 100;
  }
  if (year >= 4) {
    days -= year/4 * Time::DaysPerQuadYear;
    year = year % 4;
  }
  if (year < 0)
    days -= year * Time::DaysPerNonLeapYear;
  else if (year)
    days -= Time::DaysPerLeapYear + (year-1)*Time::DaysPerNonLeapYear;
  
  return days+1;
  
}

Int Time::GetMonthOfYear() {

  Int dayofyear;
  Int dayofmonth;
  Int month;

  dayofyear = GetDayOfYear();
  if (!IsLeapYear() && dayofyear >= 59)
    dayofyear++;
  dayofmonth = dayofyear;
  for (month=0; month<12 && dayofmonth > DaysInMonth[month];month++)
    dayofmonth -= DaysInMonth[month];

  return month+1;
}

Int Time::GetYear() {

  Int days;
  Int absDays;
  Int quadCenturies;
  Int centuries;
  Int quadYears;
  Int year;

  absDays = days = Get(Days);
  if (absDays < 0) absDays = -absDays;

  quadCenturies = absDays / Time::DaysPerQuadCentury;
  if (quadCenturies)
    absDays -= quadCenturies * Time::DaysPerQuadCentury;  
  centuries = absDays / Time::DaysPerCentury;
  if (centuries)
    absDays -= centuries * Time::DaysPerCentury;
  quadYears = absDays / Time::DaysPerQuadYear;
  if (quadYears)
    absDays -= quadYears * Time::DaysPerQuadYear;
    
  year = 0;
  if (days >= 0) {
    year = 0;
    if (absDays >= Time::DaysPerLeapYear) {
      year++;
      absDays -= Time::DaysPerLeapYear;
    }
  }
  year += absDays/Time::DaysPerNonLeapYear;
  
  year += quadYears*4;
  year += centuries*100;
  year += quadCenturies*400;
  
  if (days < 0)
    year = 2000 - year;
  else
    year = 2000 + year;
    
  return year;
}

Boolean Time::IsLeapYear() {

  Int year;

  year = GetYear();
  if (year%400 == 0)
    return true;
  else if (year%100 == 0)
    return false;
  else if (year%4 == 0)
    return true;
  else
     return false;
}

} // ...Namespace BEFE
