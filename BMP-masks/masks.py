#! /usr/bin/python3
# -*- coding: utf-8 -*-
"""This module provides two functions, either to create mask based on provided bit offset and bits per color number or
extract these values from provided mask.
"""


def get_mask(bit_offset: int, bits_per_color: int = 8) -> int:
    """Calculate the mask for a specific color and bit offset.
    Args:
        bit_offset: the number of bit offset for specific color value.
        bits_per_color: the number of bits to encode color value.
    Returns:
        The string which represents bit mask for specific color.
    Raises:
        TypeError: if bit_offset or bits_per_color are not a number.
        ValueError: if bit_offset or bits_per_color are negative.
    """
    if not (isinstance(bit_offset, int) and isinstance(bits_per_color, int)):
        raise TypeError
    if bit_offset < 0 or bits_per_color <= 0:
        raise ValueError
    val = (1 << bits_per_color) - 1
    return val << bit_offset


def get_bits(bit_mask: int, max_bits: int = 32) -> tuple[int, int]:
    """Calculate the bit offset and color bits for given mask.
    Args:
        bit_mask: the number of bit offset for specific color value.
        max_bits: the number of maximum bits per pixel.
    Returns:
        bit_offset: the number of bit offset for specific color value.
        bits_per_color: the number of bits to encode color value.
    Raises:
        TypeError: if bit_mask or max_bits are not a number.
        ValueError: if bit_mask or max_bits are negative.
    """
    if not (isinstance(bit_mask, int) and isinstance(max_bits, int)):
        raise TypeError
    if bit_mask < 0 or max_bits <= 0:
        raise ValueError
    bit_offset, bits_per_color = 0, 0
    for b in range(0, max_bits):
        temp = (bit_mask >> b) & 0x1
        if temp != 0:
            bits_per_color += 1
        if temp == 0 and not bits_per_color:
            bit_offset += 1

    return bit_offset, bits_per_color
