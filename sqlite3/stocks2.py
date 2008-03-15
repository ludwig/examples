#!/usr/bin/env python2.5

"""Example taken from http://docs.python.org/lib/module-sqlite3.html"""

import sqlite3

conn = sqlite3.connect('./stocks.db')

c = conn.cursor()
c.execute('select * from stocks order by price')
for row in c:
    print row
