#!/usr/bin/env python

import os
import urllib2
import sqlite3

from datetime import datetime
from BeautifulSoup import BeautifulSoup

# Get exact date
t = datetime.now()

# Download data
fp = urllib2.urlopen('http://www.notabene-sl.com/slgc/in-world.php')
in_world = fp.read()
fp.close()

# Parse number of avatars at go center
soup = BeautifulSoup(in_world)
rows = soup.html.body.table.findAll('tr')
a = int(rows[0].contents[1].contents[0].contents[0])
m = int(rows[1].contents[1].contents[0].contents[0])

# Record only nonzero entries
if (a > 0) or (m > 0):

    # Initialize persistence store (creating the table if it does not exist).
    # The flags allow us to use datetime objects directly by letting sqlite3
    # take care of the conversions.
    conn = None
    flags = sqlite3.PARSE_DECLTYPES | sqlite3.PARSE_COLNAMES
    dbfile = os.path.join(os.environ['HOME'], 'slgc.db')
    if not os.path.exists(dbfile):
        conn = sqlite3.connect(dbfile, detect_types=flags)
        cursor = conn.cursor()
        cursor.execute("CREATE TABLE slgc_log (avatars integer, members integer, ts timestamp)")
    if not conn:
        conn = sqlite3.connect(dbfile, detect_types=flags)
        cursor = conn.cursor()

    # Now store the answer in slgc.db
    cursor.execute("INSERT INTO slgc_log (avatars,members,ts) VALUES (?,?,?)", (a,m,t))
    conn.commit()
    cursor.close()

