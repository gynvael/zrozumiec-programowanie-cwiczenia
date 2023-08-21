#! /usr/bin/python3
# -*- coding: utf-8 -*-
"""Set of tests for the module.
Usage:
>>> pytest tests.py
"""

import pytest

from masks import get_mask, get_bits


def test_bits_from_masks():
    masks = [0x00FF0000, 0x0000FF00, 0x000000FF, 0x6]
    for m in masks:
        assert m == get_mask(*get_bits(m))


def test_masks_from_bits():
    bits = [
        (0, 8),
        (8, 8),
        (16, 8),
    ]
    for b in bits:
        assert b == get_bits(get_mask(*b))


def test_not_int_bit_offset_raises():
    with pytest.raises(TypeError):
        get_mask("asdf", 8)  # noqa


def test_not_int_bpi_raises():
    with pytest.raises(TypeError):
        get_mask(16, "8")  # noqa


def test_not_int_mask_raises():
    with pytest.raises(TypeError):
        get_bits("0xff00")  # noqa


def test_negative_bit_offset_raises():
    with pytest.raises(ValueError):
        get_mask(-1, 8)


def test_negative_bpi_raises():
    with pytest.raises(ValueError):
        get_mask(16, -8)


def test_negative_mask_raises():
    with pytest.raises(ValueError):
        get_bits(-0xFF00)
