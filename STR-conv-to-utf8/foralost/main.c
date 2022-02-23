/*
 * main.c
 *
 *  Created on: 13 lut 2022
 *      Author: foralost
 */

#include "include/utf_converter.h"

int main(int argc, char** argv){

	size_t length;

	char16_t* utf_16 = conv_utf8_utf16(u8"🤣🤣🤣", 3, &length);

	char32_t* utf_32 = conv_utf16_utf32(utf_16, length, &length);

	char16_t* utf_32_16 = conv_utf32_utf16(utf_32, length, &length);

	char* utf_8 = conv_utf16_utf8(utf_32_16, length, &length);

	utf_8 = conv_utf32_utf8(utf_32, length, &length);
	printf("%s", utf_8);
	return 0;
}
