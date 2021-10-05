#!/usr/bin/env python3
#  coding=utf-8

DaysPerLeapYear     = 366
DaysPerNonLeapYear  = DaysPerLeapYear-1
DaysPerQuadYear     = DaysPerLeapYear+3*DaysPerNonLeapYear
DaysPerCentury      = 25*DaysPerQuadYear-1
DaysPerQuadCentury  = 4*DaysPerCentury+1

# For Gregorian epoch adjustment
EpochDaysSince1600  = DaysPerQuadCentury
EpochDaysSince1601  = EpochDaysSince1600-DaysPerLeapYear
EpochDaysSince1970  = 7*DaysPerQuadYear+2*DaysPerNonLeapYear
EpochDaysSince1996  = DaysPerQuadYear

print('EpochDaysSince 1970 = %d'%EpochDaysSince1970)

