#!/usr/bin/python
# -*- coding: utf-8 -*-

from types import ListType
from types import IntType
from types import StringType
from types import NoneType


class ListSerializer(object):

    def __init__(self):
        self.memo = ""
        self.VAL_SEP = "\a"
        self.TYPE_PREFIX_SEP = "\e"
        self.TYPE_DISPATCH = {
            ListType: ['[', self.serialize_list, ']'],
            IntType: ['i', self.serialize_int],
            StringType: ['s', self.serialize_string],
            NoneType: ['n', self.serialize_none]

        }

    def serialize_list(self, l):
        self.memo += self.TYPE_DISPATCH[ListType][0]
        self.memo += self.VAL_SEP
        for e in l:
            self.TYPE_DISPATCH[type(e)][1](e)
        self.memo += self.TYPE_DISPATCH[ListType][2]
        self.memo += self.VAL_SEP

    def serialize_int(self, i):
        self.memo += self.TYPE_DISPATCH[IntType][0]
        self.memo += self.TYPE_PREFIX_SEP
        self.memo += str(i)
        self.memo += self.VAL_SEP

    def serialize_string(self, s):
        self.memo += self.TYPE_DISPATCH[StringType][0]
        self.memo += self.TYPE_PREFIX_SEP
        self.memo += s
        self.memo += self.VAL_SEP

    def serialize_none(self, n):
        self.memo += self.TYPE_DISPATCH[NoneType][0]
        self.memo += self.TYPE_PREFIX_SEP
        self.memo += "None"
        self.memo += self.VAL_SEP

    def serialize(self, item, to_file):
        self.memo = ""
        self.TYPE_DISPATCH[type(item)][1](item)
        with open(to_file, "wb") as f:
            f.write(self.memo)

    def deserialize(self, from_file):
        with open(from_file, 'rb') as f:
            s = f.read()
        return self._deserialize(s)

    def _parse_type(self, token):
        cast = {
            self.TYPE_DISPATCH[IntType][0]: int,
            self.TYPE_DISPATCH[StringType][0]: str,
            self.TYPE_DISPATCH[NoneType][0]: lambda x: None,
        }
        s_token = token.split(self.TYPE_PREFIX_SEP)
        return cast[s_token[0]](s_token[1])

    def _deserialize(self, s):
        stack = [[]]
        for token in s.split(self.VAL_SEP):
            if token == self.TYPE_DISPATCH[ListType][0]:
                stack[-1].append([])
                stack.append(stack[-1][-1])
            elif token == self.TYPE_DISPATCH[ListType][2]:
                stack.pop()
                if not stack:
                    raise ValueError('left bracket missing')
            elif not token:
                continue
            else:
                stack[-1].append(self._parse_type(token))
        if len(stack) > 1:
            raise ValueError('right bracket missing')
        return stack.pop()[0]


def serialisation_test(ls, fp, arr):
    ls.serialize(arr, fp)
    deserialized = ls.deserialize(fp)
    print arr, hex(id(arr))
    print deserialized, hex(id(deserialized))
    assert arr == deserialized


def main():
    ls = ListSerializer()
    fp = "serialisation_test.pyl"
    tests = [
        "",
        "[asd[]",
        -234,
        [],
        [[]],
        [1, 3, 4, [3, 'krowi [ro[wek', '4'], 808, None],
        None
    ]
    for t in tests:
        serialisation_test(ls, fp, t)


if __name__ == "__main__":
    main()
