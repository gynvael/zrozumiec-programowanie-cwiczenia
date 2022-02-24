/*
 * main.c
 *
 *  Created on: 13 lut 2022
 *      Author: foralost
 */

#include "include/utf_converter.h"
#include <stdio.h>
#include <stdlib.h>
// Pchnąć w tę łódź jeża lub ośm skrzyń fig.
// 🤣🤣🤣
//
int main(int argc, char **argv) {

	size_t length;

	char16_t *utf_16 = conv_utf8_utf16(u8"🤣😂😉", 3, &length);

	char32_t *utf_32 = conv_utf16_utf32(utf_16, length, &length);

	char16_t *utf_32_16 = conv_utf32_utf16(utf_32, length, &length);

	char *utf_8 = conv_utf16_utf8(utf_32_16, length, &length);

	utf_8 = conv_utf32_utf8(utf_32, length, &length);
	printf("%s", utf_8);
	fflush(stdout);
//	for (char32_t unicode = 0; unicode < 0xFFFFFFFF; unicode++) {
//
//		utf_8 = conv_utf32_utf8(&unicode, 1, &length);
//
//		utf_32 = conv_utf8_utf32(utf_8, 1, &length);
//
//		if ((char32_t) (*utf_32) != unicode) {
//			printf("ERROR\n");
//		}
//		free(utf_8);
//		free(utf_32);
//	}

	for (char16_t utf_16_char = 1; utf_16_char; utf_16_char++) {
		utf_8 = conv_utf16_utf8(&utf_16_char, 1, &length);

		utf_16 = conv_utf8_utf16(utf_8, 1, &length);

		if ((char16_t) (*utf_16) != utf_16_char) {
			printf("ERROR %x\n ", utf_16_char);
		}
		free(utf_8);
		free(utf_16);

	}
	return 0;
}
