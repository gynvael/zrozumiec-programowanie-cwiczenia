/*
 * utf_converter.c
 *
 *  Created on: 13 lut 2022
 *      Author: foralost
 */

#include "utf_converter.h"

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#define UTF_8_ONE_BYTE_SIZE_MASK	0x80

struct unicode_char {
	uint32_t unicode;
	struct unicode_char *next;
};

// TODO: Some test samples.

size_t __get_length_utf8(uint32_t character) {

	if (character <= 0x7F) {
		return 1;
	}

	uint32_t mask = 0x7FF;
	uint8_t byte_count;

	for (byte_count = 2; (byte_count <= 6) && (character > mask);
			byte_count++) {
		mask <<= 5;
		mask |= 0x1F;
	}

	return byte_count;
}

size_t __get_length_utf32(uint32_t character){
	return 4;
}

size_t __get_length_utf16(uint32_t character) {

	if (character <= 0xD7FF || (character >= 0xE000 && character <= 0xFFFF)) {
		return 2;
	} else if (character > 0xD7FF && character < 0xE000) {
		return -1; // Invalid UTF-16 character.
	}

	return 4;
}

void __parse_utf32(const uint32_t src, uint32_t *dest, uint8_t *step){
	*dest = src;
	*step = 4;
}
void __parse_utf32_utf8(const uint32_t src, char *dest, uint8_t *step) {

	if (src <= 0x7F) {
		dest[0] = src & 0xFF;
		*step = 1;
		return;
	}

	uint32_t mask = 0x7FF;
	uint8_t first_byte = 0xC0;
	uint8_t bit_mark = 11;
	uint8_t byte_count;

	for (byte_count = 2; (byte_count <= 6) && (src > mask); byte_count++) {
		bit_mark += 5;
		mask <<= 5;
		mask |= 0x1F;

		first_byte >>= 1;
		first_byte |= 0x80;
	}

	uint8_t free_bits = 8 - (byte_count + 1);

	uint8_t offset = bit_mark - free_bits;

	first_byte |= (src >> offset);

	dest[0] = first_byte;

	for (*step = 1; *step < byte_count; *step++) {
		dest[*step] = 0x80 | (src >> (offset - 6 * (*step)));
	}
}

void __parse_utf32_utf16(const uint32_t src, uint16_t *dest, uint8_t *step) {

	if (src <= 0xD7FF || (src >= 0xE000 && src <= 0xFFFF)) {
		*dest = (uint16_t) (src & 0xFFFF);
		*step = 2;
		return;
	}

	char32_t deducted_src = src - 0x10000;
	dest[1] = (deducted_src & 0x3FF) + 0xDC00;
	dest[0] = ((deducted_src >> 10) & 0x3FF) + 0xD800;
	*step = 4;
	return;
}

uint32_t __decompose_utf8_utf32(const char *source, uint8_t *offset) {
	uint32_t to_ret = 0;

	uint8_t count_bytes = 0;

	for (uint8_t starting_mask = source[0];
			starting_mask & UTF_8_ONE_BYTE_SIZE_MASK; starting_mask <<= 1)
		count_bytes++;

	if (!count_bytes) {
		*offset = 1;
		to_ret = source[0];
		return to_ret;
	}

	uint8_t bits_to_extract = 7 - count_bytes;

	uint8_t mask = 0;
	for (uint8_t i = 0; i < bits_to_extract; i++) {
		mask <<= 1;
		mask |= 1;
	}

	to_ret = source[0] & mask;
	count_bytes--;

	*offset = 1;
	for (uint8_t i = 0; i < count_bytes; i++) {
		if ((source[i + 1] & 0xC0) != 0x80) {
			fprintf(stderr, "ERROR: Invalid UTF-8 marker found. Aborting.\n");
			return -1;
		} else {
			to_ret <<= 6;
			*offset += 1;
		}

		to_ret |= source[i + 1] & (uint8_t) (~0xC0);
	}
	return to_ret;
}

uint32_t __decompose_utf16_utf32(uint16_t *src, uint8_t *offset) {

	if ((src[0] <= 0xD7FF || src[0] >= 0xE000) && !src[1]){
		*offset = 1;
		return src[0];
	}


	if ((0xD7FF <= src[0] && src[0] <= 0xE000)
			|| (src[1] <= 0xDC00 || src[1] >= 0xDFFF)) {
		fprintf(stderr, "ERROR: Wrong UTF-16 char.\n");
		return -1;
	}
	*offset = 2;
	return ((src[1] & 0x3FF) << 10) | (src[0] & 0x3FF);
}

uint32_t __decompose_utf32(uint32_t *src, uint8_t *offset) {
	*offset = 4;
	return *src;
}


struct unicode_char* __decompose_to_utf32(const char *source,
		const size_t length, uint32_t __utf32_decomposer(const char*, uint8_t*)) {

	size_t offset = 0;
	uint8_t step_offset = 0;
	struct unicode_char *curr = malloc(sizeof(struct unicode_char));
	struct unicode_char *start = curr;

	uint32_t curr_unicode = __utf32_decomposer(source + offset, &step_offset);

	if (curr_unicode == -1) {
		free(curr);
		return NULL;
	} else {
		curr->unicode = curr_unicode;
	}

	offset += step_offset;
	for (size_t i = 1; i < length; i++) {
		curr->next = NULL;

		curr_unicode = __utf32_decomposer(source + offset, &step_offset);
		if (curr_unicode == -1) {
			break;
		} else {
			curr->next = malloc(sizeof(struct unicode_char));
			curr = curr->next;
			curr->unicode = curr_unicode;
		}

		offset += step_offset;
	}
	curr->next = NULL;
	return start;
}

void* __conv_utfx_utfy(const char *source, size_t length, size_t *dest_length,
		uint32_t __utf32_decomposer(const char*, uint8_t*),
		size_t __length_parser(const uint32_t),
		void __utf_parser(const uint32_t, void*, void*)) {

	struct unicode_char *chars = __decompose_to_utf32(source, length,
			__utf32_decomposer);
	struct unicode_char *curr_char;

	size_t new_length = 0;
	curr_char = chars;

	while (curr_char) {
		new_length += __length_parser(curr_char->unicode);
		curr_char = curr_char->next;
	}

	char *utf_chars = malloc(new_length);

	curr_char = chars;
	size_t offset = 0;
	uint8_t step = 0;
	size_t code_points = 0;

	while (curr_char) {
		__utf_parser(curr_char->unicode,
				utf_chars + offset, &step);
		offset += step;

		code_points++;
		curr_char = curr_char->next;

	}
	*dest_length = code_points;
	return utf_chars;
}

uint16_t* conv_utf8_utf16(const char *source, size_t length,
		size_t *dest_length) {
	return __conv_utfx_utfy(source, length, dest_length, __decompose_utf8_utf32, __get_length_utf16, __parse_utf32_utf16);
}

char32_t* conv_utf8_utf32(const char *source, const size_t length,
		size_t *dest_length) {

	return __conv_utfx_utfy(source, length, dest_length, __decompose_utf8_utf32, __get_length_utf32, __parse_utf32);
}


char16_t* conv_utf32_utf16(const char32_t *source, const size_t length,
		size_t *dest_length){

	return __conv_utfx_utfy(source, length, dest_length, __decompose_utf32, __get_length_utf16, __parse_utf32_utf16);
}
uint8_t* conv_utf32_utf8(const char32_t *source, const size_t length,
		size_t *dest_length){

	return __conv_utfx_utfy(source, length, dest_length, __decompose_utf32, __get_length_utf8, __parse_utf32_utf8);
}

uint8_t* conv_utf16_utf8(const char16_t *source, const size_t length,
		size_t *dest_length){

	return __conv_utfx_utfy(source, length, dest_length, __decompose_utf16_utf32, __get_length_utf8, __parse_utf32_utf8);
}

char32_t* conv_utf16_utf32(const char16_t *source, const size_t length,
		size_t *dest_length){

	return __conv_utfx_utfy(source, length, dest_length, __decompose_utf16_utf32, __get_length_utf32, __parse_utf32);
}



