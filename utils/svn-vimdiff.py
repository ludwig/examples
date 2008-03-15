#!/usr/bin/env python
# use this with: svn diff --diff-cmd svn-vimdiff.py
#
# for example, as a shell alias we may define:
#
#   alias svn-vimdiff='svn diff --diff-cmd svn-vimdiff.py'
#

import sys, os

diff_pgm = '/usr/bin/vimdiff'

if len(sys.argv) != 8:
    print "Usage: svn diff --diff-cmd svn-vimdiff.py [ files ]"
    sys.exit(0)

fn_old = sys.argv[6]
fn_working_copy = sys.argv[7]

# arrange the args in the order diff_pgm expects them
args = ['svn-vimdiff.py', '-d', fn_old, fn_working_copy]
os.execv(diff_pgm, args)
