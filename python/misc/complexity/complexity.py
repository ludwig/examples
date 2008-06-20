#!/usr/bin/env python

"""
Find the most complex functions/methods in your python code.

This program is offered freely into the public domain by
Gary Wilson (gary.wilson@gmail.com).

This code requires the complexity shell scripts found at
http://journyx.com/curt/complexity.html
Download the four files and then specify the path to the
"complexity" script below, as PATH_TO_COMPLEXITY.

Note: To get the scripts to work for me I had to change the line
open (T,"|$dn/tab|sort -n +5") || die "no tab?";
to
open (T,"|$dn/tab|sort -n -k 6") || die "no tab?";
This line appears once in "complexdef" and twice in "complexdefclass".

Example uses:

Search the current directory.
python complexity.py

Search the mypythonpackage package.
python complexity.py ~/mypythonpackage

You can also specify multiple files and/or directories.
python complexity.py ~/mypythonpackage /src/myscript.py

This script will search for all files ending in ".py" within the
paths specified on the command line.  All functions and methods will
be sorted by decreasing complexity and printed along with the
filename and number of lines of code.

By default, this script will print out all functions/methods found.
If you would rather only see functions/methods with a certain
complexity or above, then change the COMPLEXITY_LEVEL setting below.
"""

## User Settings ###########################################
#PATH_TO_COMPLEXITY = '~/complex/complexity'
PATH_TO_COMPLEXITY = './complex/complexity'
# Only show functions/methods with a complexity greater than or
# equal to COMPLEXITY_LEVEL.  Setting to 0 will print all functions/methods.
COMPLEXITY_LEVEL = 0
############################################################

import sys
import re
import os
from subprocess import Popen, PIPE

def complexity_for_file(filename):
    cmd = '%s %s' % (PATH_TO_COMPLEXITY, filename)
    p = Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE, close_fds=True)
    errors = p.stderr.read()
    if errors:
        sys.exit(errors)
    tuples = []
    reached_functions = False
    function_header = re.compile('Funcname|Classless_Function')
    for line in p.stdout.readlines():
        if function_header.match(line):
            reached_functions = True
        if reached_functions:
            fields = line.split()
            # We don't want the header lines.
            try:
                int(fields[5])
            except:
                continue
            # filename, function name, lines of code, and complexity.
            tuples.append((filename, fields[0], fields[4], fields[5]))
    return tuples

def add_to_functions(functions, dir, filenames):
    for filename in filenames:
        if filename.endswith('.py'):
            functions += complexity_for_file(os.path.join(dir, filename))

def get_column_widths(function_tuples):
    widths = []
    if not function_tuples:
        return widths
    for col in range(len(function_tuples[0])):
        widths.append(max([len(ft[col]) for ft in function_tuples]))
    return widths

def get_print_parms(widths, function_tuple):
    parms = []
    for x in range(len(widths)):
        parms.append(widths[x])
        parms.append(function_tuple[x])
    return tuple(parms)

def main():
    function_tuples = []
    paths = sys.argv[1:] or os.path.curdir
    for path in paths:
        path = os.path.expanduser(path)
        if os.path.isdir(path):
            os.path.walk(path, add_to_functions, function_tuples)
        elif os.path.isfile(path):
            function_tuples += complexity_for_file(path)
    # Filter out functions less than desired complexity level.
    function_tuples = [t for t in function_tuples if int(t[-1]) >= COMPLEXITY_LEVEL]
    # Sort by complexity.
    function_tuples.sort(key=lambda t: int(t[-1]))
    function_tuples.reverse()
    print "\nShowing functions/methods with complexity greater than or equal to %s:\n" % COMPLEXITY_LEVEL
    if function_tuples:
        headers = ('Filename', 'Function/Method', 'Lines of Code', 'Complexity')
        widths = get_column_widths(function_tuples + [headers])
        print '%-*s %-*s %*s %*s' % get_print_parms(widths, headers)
        for ft in function_tuples:
            print '%-*s %-*s %*s %*s' % get_print_parms(widths, ft)
    else:
        print "None."

if __name__ == "__main__":
    sys.exit(main())
