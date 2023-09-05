#! /usr/bin/python3
# -*- coding: utf-8 -*-

"""This is yet another BMP format parser.
Supports:
    24 and 32 bit RGB uncompressed BMP
    16 bit 565 RGB and 555 RGB (with alpha mask)
    8 bit with RLE 8 bit compression
"""

import array
import struct
import math
from dataclasses import dataclass

import masks


class OutOfBitmapTargetException(Exception):
    pass


@dataclass
class BITMAPFILEHEADER:
    """Bitmap file header"""

    magic: int
    file_size: int
    reserved_1: int
    reserved_2: int
    bitmap_offset: int


@dataclass
class BITMAPCOREHEADER:
    """DIB bitmap header"""

    size: int = 0
    width: int = 0
    height: int = 0
    planes: int = 0
    bits_per_pixel: int = 0


@dataclass
class BITMAPINFOHEADER:
    """DIB bitmap header"""

    size: int = 0
    width: int = 0
    height: int = 0
    planes: int = 0
    bits_per_pixel: int = 0
    compression: int = 0
    size_image: int = 0
    x_pixels_pmeter: int = 0
    y_pixels_pmeter: int = 0
    color_palette: int = 0
    color_important: int = 0


@dataclass
class BITMAPV3INFOHEADER:
    """DIB bitmap header"""

    size: int = 0
    width: int = 0
    height: int = 0
    planes: int = 0
    bits_per_pixel: int = 0
    compression: int = 0
    size_image: int = 0
    x_pixels_pmeter: int = 0
    y_pixels_pmeter: int = 0
    color_palette: int = 0
    color_important: int = 0
    red_mask: int = 0
    green_mask: int = 0
    blue_mask: int = 0
    alpha_mask: int = 0


DIB_HEADERS = {
    12: (BITMAPCOREHEADER, "<IHHHH"),
    64: ("BITMAPCOREHEADER2", ""),
    40: (BITMAPINFOHEADER, "<IIIHHIIIIII"),
    52: ("BITMAPV2INFOHEADER", ""),
    56: (BITMAPV3INFOHEADER, "<IIIHHIIIIIIIIII"),
    108: ("BITMAPV4HEADER", ""),
    124: ("BITMAPV5HEADER", ""),
}


class BMPParser:
    """BMP format parser. Struct unpack method is used to unpack headers data from the file. The format string consists
    of set of characters representing byte ranges where I stand for unsigned integer (4 bytes) and H for unsigned short
    (2 bytes)"""

    def __init__(self, data):
        self.data = data
        self.palette = False
        self.bottom_up = True
        self.bitmap = None

        self.file_header_size = 14  # bytes
        self.file_header = BITMAPFILEHEADER(
            *struct.unpack("<HIHHI", self.data[: self.file_header_size])
        )
        self.dib_header_size = int(
            *struct.unpack(
                "<I", data[self.file_header_size : self.file_header_size + 4]
            )
        )
        self.headers_size = self.file_header_size + self.dib_header_size
        dib_header_cls, dib_header_format = DIB_HEADERS.get(self.dib_header_size)
        if isinstance(dib_header_cls, str):
            raise NotImplementedError

        self.dib_header = dib_header_cls(
            *struct.unpack(
                dib_header_format,
                self.data[
                    self.file_header_size : self.file_header_size + self.dib_header_size
                ],
            )
        )

        if self.dib_header.height < 0:
            self.bottom_up = False
            self.dib_header.height = -self.dib_header.height

        self.bytes_per_pixel = {8: 1, 16: 2, 24: 3, 32: 4}.get(
            self.dib_header.bits_per_pixel
        )

        if type(self.dib_header) == BITMAPV3INFOHEADER:  # prepare color offsets
            offsets = [o[0] for o in list(map(masks.get_bits, self.color_masks))]
            self.masks_offsets = {k: v for k, v in zip(self.color_masks, offsets)}
        else:
            self.masks_offsets = None

    @property
    def color_masks(self) -> list[int]:
        if type(self.dib_header) == BITMAPV3INFOHEADER:  # pixels are masked
            return [
                m
                for m in [
                    self.dib_header.alpha_mask,
                    self.dib_header.red_mask,
                    self.dib_header.green_mask,
                    self.dib_header.blue_mask,
                ]
                if m
            ]

        return []

    @property
    def get_row_range(self) -> range:
        if self.bottom_up:
            return range(self.dib_header.height - 1, -1, -1)
        return range(0, self.dib_header.height)

    @property
    def get_data_iterator(self) -> iter:
        return iter(self.data[self.file_header.bitmap_offset :])

    def display_info(self):
        """Print out the basic file info"""
        print(
            f"{self.file_header}\n"
            f"{self.dib_header}\n"
            f"Bottom up: {self.bottom_up}\n"
            f"Bytes per pixel: {self.bytes_per_pixel}"
        )

    def parse(self) -> None:
        """Parse data dispatcher"""
        if type(self.dib_header) not in (BITMAPINFOHEADER, BITMAPV3INFOHEADER):
            raise NotImplementedError

        if self.dib_header.compression in [0, 3]:
            self.load_bitmap_RGB()
        elif self.dib_header.compression == 1 and self.dib_header.bits_per_pixel == 8:
            self.load_bitmap_RLE8()
        else:
            raise NotImplementedError

    def _put_pixel(self, pixel: int, repeat: int, x: int, y: int) -> int:
        """Puts specific number of pixels into bimap field and returns cursor position
        Args:
            pixel: a pixel to put
            repeat: the count of repeat put pixel loop
            x: cursor x position
            y: row number
        Returns:
            The current cursor position after put pixel(s)
        """
        if type(self.dib_header) == BITMAPV3INFOHEADER:  # pixels are masked
            pixel = self.unmask_pixel(pixel)

        if not pixel:
            pixel = 0x0
        for _ in range(repeat):
            position = (x + y * self.dib_header.width) * self.bytes_per_pixel
            self.bitmap[position] = pixel
            x += 1
        return x

    def unmask_pixel(self, pixel: int) -> int:
        """Oh, that's actually a good one :)
        Translates color values from the real values in pixel to 8 bit per color range.
        source (modified):
        https://stackoverflow.com/questions/2442576/how-does-one-convert-16-bit-rgb565-to-24-bit-rgb888
        Args:
            pixel: pixel to modify
        Returns:
            pixel translated into 8 bit per color range
        """

        masked_values = [
            (pixel & mask) >> self.masks_offsets.get(mask) for mask in self.color_masks
        ]
        values = [
            math.floor(v * 0xFF / (m >> self.masks_offsets.get(m)) + 0.5)
            for v, m in zip(masked_values, self.masks_offsets)
        ]
        if len(self.color_masks) == 4:
            pixel = values[0] << 24 | values[1] << 16 | values[2] << 8 | values[3]
        else:  # assume it's 3
            pixel = values[0] << 16 | values[1] << 8 | values[2]

        return pixel

    def load_bitmap_RGB(self):
        """Loads uncompressed bitmap bytes data into bitmap pixel array"""
        self.bitmap = (
            array.array("I", [0])
            * self.dib_header.width
            * self.dib_header.height
            * self.bytes_per_pixel
        )

        pitch = (self.dib_header.width * self.bytes_per_pixel + 3) & ~3
        rows = self.get_row_range
        offset = self.file_header.bitmap_offset
        for y in rows:
            for x in range(0, self.dib_header.width):
                pixel = int.from_bytes(
                    self.data[
                        offset
                        + x * self.bytes_per_pixel : offset
                        + x * self.bytes_per_pixel
                        + self.bytes_per_pixel
                    ],
                    byteorder="little",
                )
                self._put_pixel(pixel, 1, x, y)
            offset += pitch

    def load_bitmap_RLE8(self):
        """Loads compressed bitmap bytes data into bitmap pixel array"""
        # Get color palette start and end indices and create palette array
        palette_start, palette_end = (
            self.headers_size,
            self.headers_size + self.dib_header.color_palette * 4,
        )
        self.palette = [b for b in self.data[palette_start:palette_end]]
        # Fill the bitmap array with the first color from the palette
        self.bitmap = (
            array.array("I", self.palette[:3])
            * self.dib_header.width
            * self.dib_header.height
        )

        # Get data and row range iterators
        row_iter = iter(self.get_row_range)
        data_iter = self.get_data_iterator

        # Go through the data and rows and try to decompress RLE
        x, y = 0, next(row_iter)
        while True:
            rle_opcode = next(data_iter)

            # Usual RLE compression.
            if rle_opcode > 0:
                repeat_count = rle_opcode
                # Prevent cursor to exceed the bitmap range
                if repeat_count + x > self.dib_header.width:
                    raise OutOfBitmapTargetException
                palette_index = next(data_iter)
                pixel = int.from_bytes(
                    self.palette[palette_index * 4 : palette_index * 4 + 4],
                    byteorder="little",
                )
                x = self._put_pixel(pixel, repeat_count, x, y)
                continue

            # Get extended opcode
            rle_opcode = next(data_iter)
            match rle_opcode:
                case 0:  # 00 00 - End of the row
                    x, y = 0, next(row_iter)
                case 1:  # 00 01 - End of the bitmap
                    break
                case 2:  # 00 02 XX YY - Move the cursor to XX,YY cords
                    x += next(data_iter)
                    row_num = next(data_iter)
                    # Prevent cursor to exceed the bitmap range
                    if x > self.dib_header.width or row_num > self.dib_header.height:
                        raise OutOfBitmapTargetException
                    for _ in range(row_num):  # Get YY row range.
                        y = next(row_iter)

                case _:  # 00 NN - Uncompressed data.
                    raw_count = rle_opcode
                    for _ in range(raw_count):
                        palette_index = next(data_iter)
                        pixel = int.from_bytes(
                            self.palette[palette_index * 4 : palette_index * 4 + 4],
                            byteorder="little",
                        )
                        x = self._put_pixel(pixel, 1, x, y)

                    if raw_count % 2 != 0:  # Padding byte
                        next(data_iter)
