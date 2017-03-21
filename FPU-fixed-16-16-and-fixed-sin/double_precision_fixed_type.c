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
#include "fixed_16_16.c"

int main() {
    fixed_16_16_t a = double_to_fixed_16_16(12.6798d);
    fixed_16_16_t b = double_to_fixed_16_16(1.2d);
    fixed_16_16_t c = double_to_fixed_16_16(0.6669776578d);
    fixed_16_16_t d = double_to_fixed_16_16(-3.1d);
    fixed_16_16_t e = double_to_fixed_16_16(55.0d);
    fixed_16_16_t f = double_to_fixed_16_16(0.2d);

    printf("initial a=%.4f\n", fixed_16_16_to_double(a));
    printf("initial b=%.2f\n", fixed_16_16_to_double(b));
    printf("initial c=%.2f\n", fixed_16_16_to_double(c));
    printf("initial d=%.2f\n", fixed_16_16_to_double(d));
    printf("initial e=%.2f\n", fixed_16_16_to_double(e));
    printf("initial f=%.16f\n", fixed_16_16_to_double(f));

    fixed_16_16_t adding_value = add(a, b);

    printf("adding result=%.4f\n", fixed_16_16_to_double(adding_value));

    fixed_16_16_t substraction_value = substract(d, f);
    printf("substraction result=%.2f\n", fixed_16_16_to_double(substraction_value));

    fixed_16_16_t multiplication_value = multiply(e, d);
    printf("multiplication result=%.4f\n", fixed_16_16_to_double(multiplication_value));

    fixed_16_16_t division_value = divide(b, f);
    printf("division result=%.16f\n", fixed_16_16_to_double(division_value));

    return 0;
}