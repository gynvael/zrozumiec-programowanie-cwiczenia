/*
 * main.c
 *
 *  Created on: 13 lut 2022
 *      Author: foralost
 */

#include "include/utf_converter.h"

int main(int argc, char** argv){

	size_t length;
	char16_t* utf = conv_utf8_utf16(u8"ąśćź", 4, &length);

	char32_t* utf32 = conv_utf8_utf32(u8"ąśćź", 4, &length);

	utf = conv_utf32_utf16(utf32, 4, &length);

	char16_t* utf_2 = conv_utf32_utf16(utf32, 4, &length);
	return 0;
}
