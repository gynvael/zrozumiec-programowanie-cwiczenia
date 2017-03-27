/*The MIT License (MIT)
Copyright (c) 2017 Kinga Makowiecka

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation file
(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/


#include <stdio.h>
#include <stdint.h>
#include "tylor.c"

int main() {
    double x = 1.8;

    printf("-----------Taylor formula implementation for x=%f-----------\n", x);
    fixed_16_16_t taylor_1 = double_to_fixed_16_16(x);

    double fixed_result = fixed_16_16_to_double(taylor_fixed(taylor_1));
    double double_result = taylor_double(x);
    double relative_error = (double_result - fixed_result) / double_result;

    printf("taylor function with fixed_16_16_t = %.50f\n", fixed_result);
    printf("taylor function with double = %.50f\n", double_result);
    printf("realitive error = %.50f\n", relative_error);

    return 0;
}