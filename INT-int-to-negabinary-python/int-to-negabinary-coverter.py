# The MIT License (MIT)
# Copyright (c) 2017 Kinga Makowiecka
#
# Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation file
# (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
# publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
# so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
# FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
# WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

import math
import sys

base = -2.0
result = ""
number = int(sys.argv[1])
print "converting number " + str(number) + " to negabinary value:"
while number != 0:
    ceilOfDivision = int(math.ceil(number / base))
    reminder = int(number % base)
    result = result + str(int(math.fabs(reminder)))
    number = ceilOfDivision
print result[::-1]
