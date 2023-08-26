#! /usr/bin/python3
# -*- coding: utf-8 -*-
"""This is simple BMP graphical viewer"""

import argparse
import pygame
from parser import BMPParser


def read_cli() -> bytes:
    """Reads command line arguments
    Returns:
        Bitmap file bytes.
    """

    parser = argparse.ArgumentParser(
        prog="BMP viewer",
        description=__doc__,
    )

    parser.add_argument("filename", help="Path to the BMP file.")
    args = parser.parse_args()
    with open(args.filename, "rb") as f:
        bitmap_file_bytes = f.read()
    return bitmap_file_bytes


def display(width: int, height: int, parser: BMPParser):
    """Copy pixel data form parser object to frame buffer.
    Args:
        width: the width of the display window.
        height: the height of the display window.
        parser: parser object that keeps the pixel data.
    """
    window = pygame.display.set_mode([width, height], 0, 32)
    pixels = pygame.PixelArray(window)
    for y in range(height):
        for x in range(width):
            pixel = parser.bitmap[
                (x + y * width)
                * parser.bytes_per_pixel : (x + y * width)
                * parser.bytes_per_pixel
                + parser.bytes_per_pixel
            ]
            pixels[x, y : y + 1] = pixel[0]


def main():
    """Read and display provided bitmap file. Wait for a key press or window close, then end the program."""
    parser = BMPParser(read_cli())
    parser.parse()
    parser.display_info()
    w, h = parser.dib_header.width, parser.dib_header.height
    pygame.display.init()
    display(w, h, parser)
    pygame.display.flip()
    run = True
    while run:
        for event in pygame.event.get():
            if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
                run = False


if __name__ == "__main__":
    main()
