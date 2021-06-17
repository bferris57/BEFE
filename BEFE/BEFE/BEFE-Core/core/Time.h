//!befe-publish inc
//----------------------------------------------------------------------
// File: Time.h - Time class header
//----------------------------------------------------------------------
//
// The Time class is intended to be a portable timestamp.  On creation
// of a Time object, the current date/time (in UTC) is set in the object.
//
// This class uses 01 January, 2000 as a base date in Milliseconds.
// Negative timestamp values indicate times before that, positive values
// indicate times after that.
//
// No time zone localisation mechanism is provided as of writing.
//
// Various time units are declared for the class.
//
// Various Gregorian date methods are also provided.  These intend to
// provide ordinal (1's based and Monday 1st day of week) representation
// of the timestamp on the Gregorian calendar.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef TIME_H  // TIME_H...
#define TIME_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

class Time {

  //
  // Time units
  //

  public: enum Unit {Millisecond, Second, Minute, Hour, Day, Week, NUMUNITS,
                     // For intuitive semantic readability...
                     Milliseconds = Millisecond,
                     Seconds      = Second,
                     Minutes      = Minute,
                     Hours        = Hour,
                     Days         = Day,
                     Weeks        = Week};

  public: static const Int DaysPerLeapYear     = 366;
  public: static const Int DaysPerNonLeapYear  = DaysPerLeapYear-1;
  public: static const Int DaysPerQuadYear     = DaysPerLeapYear+3*DaysPerNonLeapYear;
  public: static const Int DaysPerCentury      = 25*DaysPerQuadYear-1;
  public: static const Int DaysPerQuadCentury  = 4*DaysPerCentury+1;

  // For Gregorian epoch adjustment
  public: static const Int EpochDaysSince1600  = DaysPerQuadCentury;
  public: static const Int EpochDaysSince1601  = EpochDaysSince1600-DaysPerLeapYear;
  public: static const Int EpochDaysSince1970  = 7*DaysPerQuadYear+2*DaysPerNonLeapYear;
  public: static const Int EpochDaysSince1996  = DaysPerQuadYear;

  //
  // Private members
  //

  private: Long value;

  //
  // Public static methods
  //

  public: static Time Now();        // Get "Time Now" in UTC milliseconds since Jan 01, 2000 00:00

  //
  // Public instance methods
  //

  // C++ Lifecycle
  public: Time()                            {value=LongNaN;}
  public: Time(Long time)                   {value=time;}
  public: ~Time()                           {}
  public: Time &operator=(Time const that)  {value=that.value; return *this;}
  public: Time &operator=(Long initval)     {value=initval; return *this;}
  public: operator Long()                   {return value;}

  // Setting value
  public: Long    Set();               // <-- Set to UTC "Now"
  public: void    Set(Long time);      // <-- Set to given UTC Time
  public: void    Reset();             // <-- Set to LongNaN
  public: Int     Spin();              // <-- Spin until Time.value != "Now"
                                       //     (returns Spin count)
  // Get as numeric units
  public: Long    Get();
  public: Long    Get(Unit unit);
  public: Long    GetAs(Unit unit);

  // Manipulating value
  public: Long    RoundUpTo(Unit unit);
  public: Long    TruncateDownTo(Unit unit);
  public: Long    Delta(Time that);

  // Get as Gregorian units
  public: Int     GetMillisecond();
  public: Int     GetSecond();
  public: Int     GetMinute();
  public: Int     GetHour();
  public: Int     GetDay();
  public: Int     GetDayOfWeek();
  public: Int     GetDayOfMonth();
  public: Int     GetDayOfYear();
  public: Int     GetMonthOfYear();
  public: Int     GetYear();
  public: Boolean IsLeapYear();

};

//----------------------------------------------------------------------
//
// Time Functions
//

BEFE_INLINE Boolean IsNull(Time const &t) {return ((Time *)&t)->Get() == LongNaN;}
BEFE_INLINE Status  SetNull(Time &t) {t.Set(LongNaN); return Error::None;}

} // ...Namespace BEFE

#endif  // ...TIME_H
