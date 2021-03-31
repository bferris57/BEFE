#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
#
# File: iso8601.py - Implementation of the ISO 8601 dates/times
#
# This file implementes dates, times, and deltas as defined in by the
# ISO standard 8601 Data elements and interchange formats —
# Information interchange — Representation of dates and times
#
#
# Functions: Now         - Return elapsed time since Jan 1, 1970
#
# Classes:   ElapsedTime - An elapsed time in microseconds
#
#
# Notes: You can read about it here...
#
#           http://en.wikipedia.org/wiki/ISO_8601
#
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

#    * 3 Dates
#          o 3.1 Years
#          o 3.2 Calendar dates
#          o 3.3 Week dates
#          o 3.4 Ordinal dates
#    * 4 Times
#          o 4.1 Time zone designators
#                + 4.1.1 UTC
#                + 4.1.2 Time offsets from UTC
#    * 5 Combined date and time representations
#    * 6 Durations
#    * 7 Time intervals
#          o 7.1 Repeating intervals
#    * 8 Truncated representations
