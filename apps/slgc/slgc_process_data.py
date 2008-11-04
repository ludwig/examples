#!/usr/bin/env python

from datetime import datetime, date, timedelta
from calendar import Calendar


def getweek(t):
    """
    Given a specific datetime, it picks the corresponding week and
    returns an array containing the 7 dates corresponding to each day.
    """
    #cal = Calendar(6)  # sunday as first day of week
    cal = Calendar(1) # tuesday as first day
    weeks = [w for w in cal.monthdatescalendar(t.year, t.month)]
    for w in weeks:
        for d in w:
            if d.day == t.day:
                return w
    return []

def lastsevendays(t):
    """Given a specific datetime, it returns an array containing
    the last seven days"""
    t0 = datetime(t.year, t.month, t.day)
    dt = timedelta(1) - timedelta(0,1)
    return [ t0-i*dt for i in range(6,-1,-1) ]

def dayspace(d,n=24):
    """
    Given a specific day, it partitions the day into n parts, and
    returns an array containing the datetimes.
    """
    t0 = datetime(d.year, d.month, d.day)
    dt = timedelta(1) / n
    return [t0 + i*dt for i in xrange(0,n+1)]


def main():
    
    import os
    import sqlite3
    import numpy

    flags = sqlite3.PARSE_DECLTYPES | sqlite3.PARSE_COLNAMES
    dbfile = os.path.join(os.environ['HOME'], 'slgc.db')
    conn = sqlite3.connect(dbfile, detect_types=flags)
    cursor = conn.cursor()

    today = datetime.today()

    #week = getweek(datetime(2008,8,25))
    #week = getweek(today)
    week = lastsevendays(today)

    cols = 7
    #rows = 24 # hourly data
    rows = 24 * 60/5    # 5 min data

    A = numpy.zeros((cols, rows), dtype=int)
    for (i,day) in enumerate(week):
        ts = dayspace(day, n=rows)
        for (j,(tmin,tmax)) in enumerate(zip(ts[:-1],ts[1:])):
            cursor.execute('select max(avatars) from slgc_log where (ts > ?) and (ts <= ?)', (tmin, tmax))
            result = cursor.fetchone()
            if result[0]:
                A[i,j] = result[0]
    cursor.close()

    print A.T
    numpy.savetxt('seven.dat', A.T, fmt='%d')

if __name__ == '__main__':
    main()
