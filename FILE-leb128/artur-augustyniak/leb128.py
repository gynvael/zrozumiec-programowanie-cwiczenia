#!/usr/bin/env python
# -*- coding: utf-8 -*-


def leb128encode(x):
    leb = 0
    i = 0
    while x:
        b = x & 0x7f
        x >>= 7
        if (x == -1 and (b & 0x40)) or (x == 0 and not (b & 0x40)):
            x = 0
        else:
            b |= 0x80
        leb |= (b << i * 8)
        i += 1
    return leb


def leb128decode(leb):
    val = 0
    i = 0
    while True:
        b = (leb >> (i * 8)) & 0xFF
        val |= (b & 0x7f) << (i * 7)
        i += 1
        if not (b & 0x80):
            break
    if b & 0x40:
        val |= (~0 << (i * 7))
    return val


def uleb128encode(x):
    leb = 0
    i = 0
    while x:
        b = x & 0x7f
        x >>= 7
        if x:
            b |= 0x80
        leb |= (b << i * 8)
        i += 1
    return leb


def uleb128decode(leb):
    val = 0
    i = 0
    while True:
        b = (leb >> (i * 8)) & 0xFF
        val |= (b & 0x7f) << (i * 7)
        if not b & 0x80:
            break
        i += 1
    return val
