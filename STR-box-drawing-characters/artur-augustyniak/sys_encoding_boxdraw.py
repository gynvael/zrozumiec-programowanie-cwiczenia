#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

sys_enc = sys.stdout.encoding
print sys_enc

lines = [u'\u250c' + u'\u2500' * 3 + u'\u252c' + u'\u2500' * 3 + u'\u252c' + u'\u2500' * 3 + u'\u2510',
         u'\u2502' + " o " + u'\u2502' + (' ' * 3 + u'\u2502') * 2,
         u'\u251c' + (u'\u2500' * 3 + u'\u253c') * 2 + u'\u2500' * 3 + u'\u2524',
         u'\u2502' + " o " + u'\u2502' + " x " + u'\u2502' + \
         (' ' * 3 + u'\u2502') * 1, u'\u251c' + (u'\u2500' * 3 + u'\u253c') * 2 + u'\u2500' * 3 + u'\u2524',
         u'\u2502' + ' o ' + u'\u2502' + (' ' * 3 + u'\u2502') * 2,
         u'\u2514' + (u'\u2500' * 3 + u'\u2534') * 2 + u'\u2500' * 3 + u'\u2518']

# cmd dzieki temu tez zadziala
for l in lines:
    print l.encode(sys_enc)
