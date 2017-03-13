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
