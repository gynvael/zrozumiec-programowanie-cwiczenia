#!/usr/bin/env python
# -*- coding: utf-8 -*-

import struct
import sys


def encode_utf8(unicodes):
    # utf -  bits : bytes
    UTF_BYTE_FORMS = {
        7: 1,
        11: 2,
        16: 3,
        21: 4,
        26: 5,
        31: 6,
    }
    utf8_code = []
    for code_point in unicodes:
        # kod ASCII
        if 0x0 <= code_point <= 0x7f:
            utf8_code.append(code_point)
        # cos wiecej
        else:
            bit_size = code_point.bit_length()
            # ile bajtow utf potrzebujemy zeby zakodowac codepoint o dlugosci bit_size (najmniejszy 'od gory')
            utf_byte_form = UTF_BYTE_FORMS[min([i for i in UTF_BYTE_FORMS.keys() if i >= bit_size])]
            byte_count = utf_byte_form
            utf8_chr = []
            # wszystkie bajty oprocz pierwszego wypelniamy bitami code pointu po 6
            while utf_byte_form > 1:
                # wg schematu 10xxxxxx, stad 2 << 6 i 0x3f
                utf8_chr.append((2 << 6) | code_point & 0x3f)
                code_point >>= 6
                utf_byte_form -= 1
            # w pierwszym bajcie kodujemy ilosc bajtow uzytych do zakodowania znaku + ew. to co zostalo z bitow code pointu
            utf8_chr.append((0xff << (8 - byte_count)) & 0xFF | code_point)
            utf8_chr.reverse()
            utf8_code += utf8_chr
    return utf8_code


def decode_utf16le(s):
    """
    Kod z ksiazki
    https://github.com/gynvael/zrozumiec-programowanie/blob/master/010-Czesc_II-Rozdzial_6-Znaki_i_lancuchy_znakow/utf16-test.py
    """
    unicodes = []
    code = None
    i = 0
    while i < len(s):
        # Odczytaj 16-bitową liczbę naturalną (LE).
        codepoint = struct.unpack("<H", s[i:i + 2])[0]

        if ((codepoint >= 0x0000 and codepoint <= 0xD7FF) or
                (codepoint >= 0xE000 and codepoint <= 0xFFFF)):
            unicodes.append(codepoint)
            i += 2
            continue

        if code is None:
            # Mamy do czynienia w pierwszą 16-bitową liczbą. Warto się upewnić czy
            # tak na pewno jest.
            if not (codepoint >= 0xD800 and codepoint <= 0xDBFF):
                print>> sys.stderr, "Invalid UTF-16 string (1)."
                return None
            code = (codepoint & 0x3ff) << 10
            i += 2
            continue

        # Mamy do czynienia z drugą 16-bitową liczbą. Podobnie jak powyżej, warto
        # sprawdzić czy tak na pewno jest.
        if not (codepoint >= 0xDC00 and codepoint <= 0xDFFF):
            print>> sys.stderr, "Invalid UTF-16 string (2)."
            return None

        code |= (codepoint & 0x3ff)
        code += 0x10000
        unicodes.append(code)
        code = None
        i += 2

    return unicodes


if __name__ == "__main__":
    sys_enc = sys.stdout.encoding
    print "STDOUT ENC:", sys_enc

    # z tym cmd sobie nie radzi :(
    # utf16_le_s = u'≠\U0001f408 zażółć gęślą jaźń aπœę©ß←↓→óþ'.encode('utf-16-le')

    utf16_le_s = u'zażółć gęślą jaźń'.encode('utf-16-le')
    unicode = decode_utf16le(utf16_le_s)
    utf8 = encode_utf8(unicode)
    # with open('utf8.txt', 'wb') as f:
    #     for b in utf8:
    #         f.write(struct.pack("B", b))
    #     f.write('\n')
    print "CODE POINTS", unicode
    print "UTF8 BYTES", utf8
    unicode_str = utf16_le_s.decode('utf-16-le')
    print "UNICODE STRING\t", unicode_str, type(unicode_str)
    utf8_str = str(bytearray(utf8))
    print "UTF8 STRING\t", utf8_str.decode('utf8'), type(utf8_str)
