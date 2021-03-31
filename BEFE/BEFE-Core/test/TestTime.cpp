//!befe-publish src
//----------------------------------------------------------------------
// File: testTime.cpp - Implementation of the TestTime Class
//----------------------------------------------------------------------
//
// This Class implements the BEFE Time Class Unit Tests.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Time.h"
#include "Test.h"

namespace BEFE { // Namespace BEFE...

Status TestTime::Run() {

  Time t1;
  Time t2;
  Time et;
  Int  count;
  Int  sum;

  if (!gQuiet)
    out << indent << "***Testing Time..." << Endl;

  t1.Set();
  if (gVeryVerbose) {
    out << "  DaysPerCentury      = " << Time::DaysPerCentury << Endl;
    out << "  DaysPerLeapYear     = " << Time::DaysPerLeapYear << Endl;
    out << "  DaysPerNonLeapYear  = " << Time::DaysPerNonLeapYear << Endl;
    out << "  DaysPerQuadYear     = " << Time::DaysPerQuadYear << Endl;
    out << "  DaysPerCentury      = " << Time::DaysPerCentury << Endl;
    out << "  DaysPerQuadCentury  = " << Time::DaysPerQuadCentury << Endl;
    out << "  EpochDaysSince1601  = " << Time::EpochDaysSince1601 << Endl;
    out << "  EpochDaysSince1970  = " << Time::EpochDaysSince1970 << Endl;
    out << "  EpochDaysSince1996  = " << Time::EpochDaysSince1996 << Endl;
  }

  //t1.Set();
  if (gVeryVerbose) {
    out << "  Started at:  " << t1.GetAs(Time::Hours)        << "h"
                             << t1.GetAs(Time::Minutes)      << "m"
      		                   << t1.GetAs(Time::Seconds)      << "s"
      		                   << t1.GetAs(Time::Milliseconds) << "ms"
                             << Endl;

    out << "  " << Time::NUMUNITS << " total units" << Endl;
  }

  t2.Set();
  et = t1.Delta(t2);
  if (gVeryVerbose) {
    out << "  Elapsed Minutes      = " << et.GetAs(Time::Minutes) << Endl;
    out << "  Elapsed Seconds      = " << et.GetAs(Time::Seconds) << Endl;
    out << "  Elapsed Milliseconds = " << et.GetAs(Time::Milliseconds) << Endl;
    out << "  Day of year          = " << t1.GetDayOfYear() << Endl;
    //out << ind << "Day of week          = " << Time.enLongDaysOfWeek[t1.GetDayOfWeek()] << Endl;
    out << "  Day of week          = " << t1.GetDayOfWeek() << Endl;
    out << Endl;
    out << "  Day of month         = " << t1.GetDayOfMonth() << Endl;
    //out << ind << "Month of year        = " << Time.enLongMonthsOfYear[t1.GetMonthOfYear()];
    out << "  Month of year        = " << t1.GetMonthOfYear() << Endl;
    out << "  Year                 = " << t1.GetYear() << Endl;
    out << Endl;
    out << "  Hour                 = " << t1.GetHour() << Endl;
    out << "  Minute               = " << t1.GetMinute() << Endl;
    out << "  Second               = " << t1.GetSecond() << Endl;
  }
  if (t1.IsLeapYear()) {
    if (gVeryVerbose)
      out << t1.GetYear() << " IS a leap year!" << Endl;
  }
  else {
    if (gVeryVerbose)
      out << "  " << t1.GetYear() << " IS NOT a leap year!" << Endl;
  }

  if (gVeryVerbose) {
    out << "  Finished at: " << t2.GetAs(Time::Hours)        << "h"
                             << t2.GetAs(Time::Minutes)      << "m"
                             << t2.GetAs(Time::Seconds)      << "s"
                             << t2.GetAs(Time::Milliseconds) << "ms"
                             << Endl;
  }

  // For now...
  if (true) goto DONE;

  // Test Spin...
  if (gVerbose)
    out << "  Testing Spin...\n";

  t1 = Time::Now();
  sum = 0;
  t2 = t1;
  for (count=0;count<1000;count++) {
    t2.Set();
    sum += t2.Spin();
  }
  et.Set();

  if (gVerbose) {
    out << "                 Spins = " << count << "\n";
    out << "    Total   Spin Time  = " << et-t1 << "ms\n";
    out << "    Average Spin Time  = ";
    if (et-t1 == 0)
      out << "0ms\n";
    else
      out << (sum/count)/(et-t1) << "ms\n";
    out << "    Total   Spin Count = " << sum << "\n";
    out << "    Average Spin Count = " << sum/count << "\n";
  }

  while (false) {
    DONE: break;
  }

  return Error::None;

}

} // ...Namespace BEFE
