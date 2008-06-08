#!/usr/bin/env python
from pylab import *
loglog([1,2,3,4], [1,4,9,16], 'ro-')
axis([0.5, 6, 0.5, 20])
#savefig('simple-loglog-plot.png')
show()

