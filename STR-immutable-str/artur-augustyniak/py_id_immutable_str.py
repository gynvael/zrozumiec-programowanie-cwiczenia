#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

sys_enc = sys.stdout.encoding
print sys_enc


# Python zdaje sie bym mocno COW
def str_info(s, op="NOP"):
    print "CPython passed s addr:", hex(id(s)), s, op


s = 'qwertyuiop_asdfghjkl_zxcvbnm'
print "CPython s addr: ", hex(id(s))
str_info(s)

sc = s
str_info(sc, "assign")
str_info(sc.lower(), "lower()")
str_info(sc.upper(), "upper()")
str_info(sc)
sc = s.upper()
str_info(sc, "reassign")
str_info(sc + "a", "concat")
sc += 'a'
str_info(s + "a", "concat inplace")
str_info(sc.lstrip(), "nop lstrip")
str_info(sc.lstrip('Q'), "lstrip(Q)")

t = s.split('_')
print t
str_info(s)
for w in t:
    str_info(w, "str part")

a = 'aa'
str_info(a)
a += 'bbbbb'
str_info(a, 'inplace concat')
