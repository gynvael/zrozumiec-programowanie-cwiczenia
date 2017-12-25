#!/usr/bin/python
# -*- coding: utf-8 -*-

from types import DictionaryType, IntType, StringType
from cStringIO import StringIO


def prepare_ini(dct, acc=StringIO(), level=0):
    for k, v in dct.iteritems():
        t = type(v)
        if t in [StringType, IntType]:
            acc.write("%s=%s\n" % (str(k), str(v)))
        elif t == DictionaryType:
            if level == 1:
                raise ValueError("max nesting level 2")
            acc.write("[%s]\n" % k)
            prepare_ini(dct[k], acc, level + 1)
            acc.write("\n")
    if level == 0:
        return acc.getvalue()


if __name__ == "__main__":
    conf = {
        'max_enemy_count': 20,
        'custom_str_toplevel': 'toplevel_str',
        'Hero': {
            'start_hp': 100,
            # 'dsf': {},
            'start_mp': "1000"
        },
        'last': 42
    }
    textural = prepare_ini(conf)
    if textural:
        with open("blah.ini", "wb") as f:
            f.write(textural)
