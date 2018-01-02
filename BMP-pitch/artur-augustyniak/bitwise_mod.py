#!/usr/bin/python
# -*- coding: utf-8 -*-


def align_to_power_of2(num, al):
    return (num + (al - 1)) & ~(al - 1)


def align_to(num, al):
    return num + ((al - (num % al)) % al)


if __name__ == "__main__":
    '''
    Problem align/paddingu np w strukturach:
    https://en.wikipedia.org/wiki/Data_structure_alignment#Computing_padding
    w przypadku poteg dwojki mozemy mod zastapic &~   
    '''

    for n in range(0, 1234, 7):
        for align in [2 ** x for x in xrange(1, 16)]:
            aligned = align_to_power_of2(n, align)
            mod_align = align_to(n, align)
            print "n: %s - align to: %s - aligned: %s - mod aligned: %s" % (n, align, aligned, mod_align)
            assert aligned % align == 0
            assert aligned == mod_align

        for a in xrange(1, 1234, 77):
            aligned = align_to(n, a)
            print "n: %s - align to: %s - aligned: %s" % (n, a, aligned)
            assert aligned % a == 0
