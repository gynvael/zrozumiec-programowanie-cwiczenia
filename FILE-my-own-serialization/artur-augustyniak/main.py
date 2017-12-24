#!/usr/bin/python -B
# -*- coding: utf-8 -*-

from listserializer import ListSerializer


def serialisation_test(ls, fp, arr):
    ls.serialize(arr, fp)
    deserialized = ls.deserialize(fp)
    print arr, hex(id(arr))
    print deserialized, hex(id(deserialized))
    assert arr == deserialized


def main():
    ls = ListSerializer()
    fp = "globals.pyl"
    arr = list(globals())
    ls.serialize(arr, fp)
    deserialized = ls.deserialize(fp)
    print arr, hex(id(arr))
    print deserialized, hex(id(deserialized))
    assert arr == deserialized


if __name__ == "__main__":
    main()

