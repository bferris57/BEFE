#!/usr/bin/env python3
#  coding=utf-8

import time
import datetime

then = datetime.datetime.fromtimestamp(time.time())

def deltaSeconds(delta): # ◄── delta=datetime.timedelta, returns float seconds

    diff = float(delta.days*(60*60*24)+delta.seconds) + delta.microseconds/1000000.0

    return diff

now = datetime.datetime.fromtimestamp(time.time())

diff = float(int(deltaSeconds(now-then)*100000))/1000.
print('%5.3f'%diff)
