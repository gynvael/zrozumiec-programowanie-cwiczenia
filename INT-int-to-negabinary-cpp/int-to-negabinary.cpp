/* MIT License
Copyright (c) 2018 mattmaniak
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE. */

#include <iostream>

#define MIN_INT -2147483648
#define MAX_INT 2147483647

void intCheck(int decimal) {
	if(decimal <= MIN_INT || decimal >= MAX_INT) {
		std::cout << "Wrong integer size!" << std::endl;
		exit(1);
	}
}

void toNegabinary(int decimal) {
	char bitPosition = 0, binSize = 8 * sizeof(int);
	bool negabinary[binSize];

	if(decimal >= 0) {
		while(1) {
			if(decimal % 2 == 0) {
				negabinary[bitPosition] = 0;
			}
			else {
				negabinary[bitPosition] = 1;
			}
			decimal = decimal / 2;
			bitPosition++;
			if(bitPosition > binSize - 1) {
				break;
			}
		}
	}
	else if(decimal == -1) { // Special case to make first bit = 1 when int < 0.
		for(bitPosition = 0; bitPosition < binSize; bitPosition++) {
			negabinary[bitPosition] = 1;
		}
	}
	else {
		while(1) {
			if(-decimal % 2 == 0) {
				negabinary[bitPosition] = 1;
			}
			else {
				negabinary[bitPosition] = 0;
			}
			decimal = decimal / 2;
			bitPosition++;
			if(bitPosition > binSize - 1) {
				break;
			}
		}
	}
	for(bitPosition = binSize - 1; bitPosition >= 0; bitPosition--) {
		std::cout << negabinary[bitPosition]; // Printer.
	}
}

int main() {
	int decimal;
	std::cout << "Type decimal: ";
	std::cin >> decimal;
	intCheck(decimal);

	std::cout << "Your negabinary [BE]: ";
	toNegabinary(decimal);
	std::cout << std::endl;
}

