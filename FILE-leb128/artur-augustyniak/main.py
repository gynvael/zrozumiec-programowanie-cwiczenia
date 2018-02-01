#!/usr/bin/python -B
# -*- coding: utf-8 -*-

from leb128 import *

import struct
import binascii


def dissect_int(i):
    raw = struct.pack("<i", i)
    print "\tDissecting 4 byte int %s count from LSB:" % i
    hexif = binascii.hexlify(raw)
    for i in xrange(0, len(hexif), 2):
        b = hexif[i:i + 2]
        print "\tbyte: %s" % (i / 2), bin(int(b, 16)), b


def uleb_test():
    print "\nULEB TEST\n" \
          "---------------------------------"
    print "assert check loop in progress..."
    for x in range(0, 624485):
        assert x == uleb128decode(uleb128encode(x))

    val = 1234
    print "VALUE", val, bin(val)
    dissect_int(val)
    uleb_val = uleb128encode(val)
    print "ULEB ENC", uleb_val, bin(uleb_val)
    dissect_int(uleb_val)
    decoded_val = uleb128decode(uleb_val)
    print "ULEB DEC", decoded_val, bin(decoded_val)
    dissect_int(decoded_val)


def leb_test():
    print "\nLEB TEST\n" \
          "---------------------------------"
    print "LEB assert check loop in progress..."
    for x in range(-362341, 362341):
        assert x == leb128decode(leb128encode(x))

    val = 1234
    print "VALUE", val, bin(val)
    dissect_int(val)
    leb_val = leb128encode(val)
    print "LEB ENC", leb_val, bin(leb_val)
    dissect_int(leb_val)
    decoded_val = leb128decode(leb_val)
    print "LEB DEC", decoded_val, bin(decoded_val)
    dissect_int(decoded_val)
    print "NEGATIVE ------------------"
    val = -1234
    print "VALUE", val, bin(val)
    dissect_int(val)
    leb_val = leb128encode(val)
    print "LEB ENC", leb_val, bin(leb_val)
    dissect_int(leb_val)
    decoded_val = leb128decode(leb_val)
    print "LEB DEC", decoded_val, bin(decoded_val)
    dissect_int(decoded_val)


if __name__ == "__main__":
    uleb_test()
    leb_test()
