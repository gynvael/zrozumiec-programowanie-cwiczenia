#ifndef UTF_CONVERTER_H_
#define UTF_CONVERTER_H_
#include <uchar.h>
#include <stdint.h>
char16_t* conv_utf8_utf16(const char *source, size_t length,
		size_t *dest_length);

char32_t* conv_utf8_utf32(const char *source, const size_t length,
		size_t *dest_length);

char16_t* conv_utf32_utf16(const char32_t *source, const size_t length,
		size_t *dest_length);

uint8_t* conv_utf32_utf8(const char32_t *source, const size_t length,
		size_t *dest_length);

uint8_t* conv_utf16_utf8(const char16_t *source, const size_t length,
		size_t *dest_length);

char32_t* conv_utf16_utf32(const char16_t *source, const size_t length,
		size_t *dest_length);

#endif
