#! /usr/bin/python3
"""This module provides two functions either to create mask based on provided bit offset and bits per color number or
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
        TypeError: if n is not a number.
        ValueError: if n is negative.
    """
    if not (isinstance(bit_offset, int) and isinstance(bits_per_color, int)):
        raise TypeError
    if bit_offset < 0 or bits_per_color <= 0:
        raise ValueError
    val = (1 << bits_per_color) - 1
    return val << (4 * bit_offset)


def get_bits(bit_mask: int, max_bits: int = 32) -> tuple[int, int]:
    """Calculate the bit offset and color bits for given mask.
    Args:
        bit_mask: the number of bit offset for specific color value.
        max_bits: the number of maximum bits per pixel.
    Returns:
        bit_offset: the number of bit offset for specific color value.
        bits_per_color: the number of bits to encode color value.
    """

    bit_offset, nibbles = 0, 0
    for n in range(0, max_bits, 4):
        temp = (bit_mask >> n) & 0xF
        if temp != 0:
            nibbles += 1
        if temp == 0 and not nibbles:
            bit_offset += 1

    bits_per_color = 4 * nibbles  # Translate hex nibbles to bits amount
    return bit_offset, bits_per_color


def main():
    """Set of tests for the module"""
    masks = [
        0x00FF0000,
        0x0000FF00,
        0x000000FF,
    ]
    for m in masks:
        assert m == get_mask(*get_bits(m))


if __name__ == "__main__":
    main()
