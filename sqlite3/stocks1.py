#!/usr/bin/env python2.5

"""Example taken from http://docs.python.org/lib/module-sqlite3.html"""

import sqlite3

conn = sqlite3.connect('./stocks.db')

c = conn.cursor()

# Create table
c.execute("create table stocks (date text, trans text, symbol text, qty real, price real)")

# Insert a row of data
c.execute("insert into stocks values ('2006-01-05', 'BUY', 'RHAT', 100, 35.14)")

# Larger example
for t in (('2006-03-28', 'BUY', 'IBM', 1000, 45.00),
          ('2006-04-05', 'BUY', 'MSFT', 1000, 72.00),
          ('2006-04-06', 'SELL', 'IBM', 500, 53.00)):
    c.execute('insert into stocks values (?,?,?,?,?)', t)

conn.commit()
