package bferris.core;

import java.util.Date;

public class Time extends java.lang.Object {

  // Handy unit sizes
  public final static long   BaseUnitsPerMillisecond = 1;
  public final static long   BaseUnitsPerSecond      = 1000*BaseUnitsPerMillisecond;
  public final static long   BaseUnitsPerMinute      = 60*BaseUnitsPerSecond;
  public final static long   BaseUnitsPerHour        = 60*BaseUnitsPerMinute;
  public final static long   BaseUnitsPerDay         = 24*BaseUnitsPerHour;
  public final static long   BaseUnitsPerWeek        = 7*BaseUnitsPerDay;

  // Time units (handy aliases for intuitive semantics)
  public final static long   Milliseconds            = BaseUnitsPerMillisecond;
  public final static long   Seconds                 = BaseUnitsPerSecond;
  public final static long   Minutes                 = BaseUnitsPerMinute;
  public final static long   Hours                   = BaseUnitsPerHour;
  public final static long   Days                    = BaseUnitsPerDay;
  public final static long   Weeks                   = BaseUnitsPerWeek;

  // Unit lookup table
  public final static long[] Units = {Milliseconds,Seconds,Minutes,Hours,Days,Weeks,0};

  // Gregorian units
  public final static int    DaysPerLeapYear         = 366;
  public final static int    DaysPerNonLeapYear      = DaysPerLeapYear-1;
  public final static int    DaysPerQuadYear         = DaysPerLeapYear+3*DaysPerNonLeapYear;
  public final static int    DaysPerCentury          = 25*DaysPerQuadYear+1;
  public final static int    DaysPerQuadCentury      = 4*DaysPerCentury-1;

  // For Gregorian epoch adjustment
  public final static int    EpochDaysSince1970      = 6*DaysPerQuadYear+2*DaysPerNonLeapYear;
  public final static int    EpochDaysSince1996      = DaysPerQuadYear;

  // Month lengths for leap years
  public final int DaysInMonth[] = {31,29,31,30,31,30,31,31,30,31,30,31};

  // Gregorian strings: Long days of week...
  public final static String[] enLongDaysOfWeek =
    {"Monday"
    ,"Tuesday"
    ,"Wednesday"
    ,"Thursday"
    ,"Friday"
    ,"Saturday"
    ,"Sunday"
    };

  // Gregorian strings: Long months of year...
  public final static String[] enLongMonthsOfYear =
    {"January"
    ,"February"
    ,"March"
    ,"April"
    ,"May"
    ,"June"
    ,"July"
    ,"August"
    ,"September"
    ,"October"
    ,"November"
    ,"December"
    };

  // AD, BC, CE, BCE
  public final static String[] enEra =
    {"AD"
    ,"BC"
    ,"CE"
    ,"BCE"
    };

  // Hidden member
  private long time;

  //
  // Class methods
  //

  public static long Now() {
    return (new Date()).getTime()-EpochDaysSince1970;
  }

  //
  // Instance methods
  //


  public Time()          { super(); time = new Date().getTime()-EpochDaysSince1970; };
  public Time(long time) { super();
                           this.time = new Date().getTime()-EpochDaysSince1970;
                           if (time < 0)
                             this.time = 0;
                           else
                             this.time = time;
                         };

  private int FindUnit(long unit) {
    int i = 0;
    while (Units[i]!=unit && Units[i]!= 0) i++;
    if (Units[i] == 0)
      i = 1;
    return i;
  }

  public long Get() {
    return time;
  }

  //----------------------------------------------------------------------------
  //
  // Method: Get(unit) - Return time/unit
  //
  // Usage:   Time time = new Time();
  //          ...
  //          value = time.Get(Time.<unit>);
  //
  // Where:   <unit> - Time unit (invalid assumes Time.Seconds)
  //
  //          Should be Milliseconds, Seconds, Minutes, Hours, Days or Weeks.
  //
  // Returns: Time/unit, not truncated.  For example, if Time.Seconds is
  //          passed, this function returns time/1000.  It'll be huge.  ;-)
  //
  // Note:    If an invalid unit is specified, the time in seconds since
  //          01 January, 2000 is returned.
  //
  //----------------------------------------------------------------------------

  public long Get(long unit) {
    int  uidx;
    long t;
    uidx = FindUnit(unit);
    if (Units[uidx+1] != 0)
      t = time/unit;
    else
      t = time/Seconds;
    return t;
  }

  //
  // Method:  GetAs(unit) - Return time/unit % (next unit)
  //
  // Usage:   Time time = new Time();
  //          ...
  //          value = time.GetAs(Time.<unit>);
  //
  // Where:   <unit> - Time unit (invalid assumes Time.Seconds)
  //
  //          Should be Milliseconds, Seconds, Minutes, Hours, Days or Weeks.
  //
  // Returns: Time/<unit>, truncated to the unit.  For example, if Time.Seconds
  //          is passed this method returns returns time/1000.
  //
  // Note:    If an invalid unit is specified, the time in seconds is
  //          returned without notification.
  //

  public long GetAs(long unit) {

    int  uidx;
    long t;
    uidx = FindUnit(unit);
    if (Units[uidx] == 0)
      return time/Seconds;
    if (Units[uidx+1] == 0)
      t = time;
    else
      t = time % Units[uidx+1];
    return t/unit;
  }

  //
  // Method:  RoundUpTo(unit) - Round up to next integer multiple of a
  //                            specified unit
  //
  // Usage:   Time time = new Time();
  //          ...
  //          value = time.RoundUpTo(Time.<unit>);
  //
  // Where:   <unit> - Time unit to round to (invalid assumes Time.Seconds)
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

  public long RoundUpTo(long unit) {

    int  uidx;
    long t;

    uidx = FindUnit(unit);
    if (Units[uidx] == 0)
      unit = Seconds;
    t = time;
    if (t < 0)
      t = t - unit>>1;
    else
      t = t + unit>>1;
    return t/unit*unit;
  }

  //
  // Method:  TruncateDownTo(unit) - Truncate to next lower integer multiple
  //                                 of a specified unit
  //
  // Usage:   Time time = new Time();
  //          ...
  //          value = time.TruncateDownTo(Time.<unit>);
  //
  // Where:   <unit> - Time unit to truncate to (invalid assumes Time.Seconds)
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

  public long TruncateDownTo(long unit) {

    int  uidx;
    long t;

    t = time;
    if (t < 0)
      t = t-unit+1;
    uidx = FindUnit(unit);
    if (Units[uidx] == 0)
      t = t/Seconds*Seconds;
    else
      t = t/unit*unit;
    return t;
  }

  //
  // Method:  Time.Delta() - Time difference
  //
  // Usage:   Time t1 = new Time();
  //          Time t2;
  //          ...
  //          t2 = new Time();
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

  public Time Delta(Time other) {

    long ourtime;
    
    ourtime = StrictMath.abs(this.time - other.time);

    return new Time(ourtime);

  }

  //----------------------------------------------------------------------------
  //
  // GREGORIAN Date instance methods
  //
  //----------------------------------------------------------------------------
  //
  // Method:  Time.GetMillisecond() - Return millisecond of second (0..999)
  //          Time.GetSecond()      - Return second of minute (0..59)
  //          Time.GetMinute()      - Return minutes of hour (0..59)
  //          Time.GetHour()        - Return hour of day (0..23)
  //          Time.GetDayOfWeek()   - Return day of week (0..6, Monday=0)
  //          Time.GetDayOfMonth()  - Return day of month (0..30)
  //          Time.GetDayOfYear()   - Return day of calendar year (0..365)
  //          Time.GetMonthOfYear() - Return Gregorian month of year (0..11)
  //          Time.GetYear()        - Return Gregorian year (-nnnnnnnnn..nnnnnnnnn)
  //          Time.IsLeapYear()     - Is this year a leap year?
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
  //          GetYear()        returns year -N through +N but NEVER a
  //                             year zero
  //            because...
  //
  //          There is NO year 0 AD or 0 BC in the Gregorian calendar.  So,
  //          if GetAsYear() returned -1001, GetYear() will return -1000.
  //
  //          If you're one of those sadly frustrated astonomers out there
  //          you'll most likely wish to use GetAsYear() instead of
  //          GetYear().  This is because the Gregorian dates are not likely
  //          your style.  From my experience, astronomers are more likely to
  //          be interested in date arithmetic and should feel free to use
  //          Time.Delta() for calculations.  So, a word to the wise...
  //          stay away from Gregorian dates if you don't care for Gregorian
  //          dates.  ;-)
  //

  public int GetMillisecond() {
  	return (int)GetAs(Milliseconds);
  }
  
  public int GetSecond() {
  	return (int)GetAs(Seconds);
  }
  
  public int GetMinute() {
  	return (int)GetAs(Minutes);
  }
  
  public int GetHour() {
  	return (int)GetAs(Hours);
  }
  
  public int GetDayOfWeek() {

    int days;

    days = (int)TruncateDownTo(Days);
    return (days-6)%7 + 1;

  }

  public int GetDayOfMonth() {
    int     dayofyear;
    int     dayofmonth;
    int     month;

    dayofyear = GetDayOfYear()-1;
    if (!IsLeapYear() && dayofyear >= 59)
      dayofyear++;
    dayofmonth = dayofyear;
    for (month=0; month<12 && dayofmonth >= DaysInMonth[month];month++)
      dayofmonth -= DaysInMonth[month];

    return dayofmonth+1;
  }

  public int GetDayOfYear() {
    long    days;
    long    quadcenturyremainder;
    long    quadyearremainder;
    long    yearremainder;

    days = Get(Days);
    quadcenturyremainder = days%DaysPerQuadCentury;
    quadyearremainder    = quadcenturyremainder%DaysPerQuadYear;
    if (quadyearremainder >= DaysPerLeapYear)
      yearremainder = (quadyearremainder-DaysPerLeapYear)%DaysPerNonLeapYear;
    else
      yearremainder = quadyearremainder;
    return (int)yearremainder+1;
  }

  public int GetMonthOfYear() {
    int     dayofyear;
    int     dayofmonth;
    int     month;

    dayofyear = GetDayOfYear()-1;
    if (!IsLeapYear() && dayofyear >= 59)
      dayofyear++;
    dayofmonth = dayofyear;
    for (month=0; month<12 && dayofmonth >= DaysInMonth[month];month++)
      dayofmonth -= DaysInMonth[month];

    return month+1;
  }

  public long GetYear() {

    long    days;
    long    quadcenturyremainder;
    long    quadyearremainder;
    long    year;

    days = Get(Days);
    quadcenturyremainder = days%DaysPerQuadCentury;

    year = days/DaysPerQuadCentury;
    quadyearremainder    = quadcenturyremainder%DaysPerQuadYear;
    year += quadcenturyremainder/DaysPerQuadYear*4;
    year += quadyearremainder/DaysPerLeapYear + 2000;

    return year;
  }

  public boolean IsLeapYear() {

    long year;
    
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

}
