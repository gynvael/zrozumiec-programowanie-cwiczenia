; Zadanie [VM:not-so-simple-calc]
; Autor: Adam Bac (abac00s)
;
;The MIT License (MIT)
;Copyright (c) 2016 Adam Bac
;
;Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation file
;(the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
;publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
;so, subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
;FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
;WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

%include "vm.inc"

vxor r2, r2

;1 komunikat ---------------------------------
vset r0, com1
vset r1, 1
vxor r3, r3
loop1:
	vldb r2, r0
	vcmp r2, r3
	vje endloop1
	voutb 0x20, r2
	vadd r0, r1
	vjmp loop1
endloop1:
;---------------------------------------------

;wczytaj 1 liczbe ----------------------------
vxor r3, r3
loop2:
	vinb 0x21, r2
	vcmp r2, r3
	vje loop2

vxor r10, r10
vset r3, 0xa
vset r1, 0x30
vset r4, 10
loop3:
	vinb 0x20, r2
	vcmp r2, r3
	vje endloop3
	vsub r2, r1
	vmul r10, r4
	vadd r10, r2
	vjmp loop3
endloop3:
;---------------------------------------------


;2 komunikat ---------------------------------
vset r0, com2
vset r1, 1
vxor r3, r3
loop4:
	vldb r2, r0
	vcmp r2, r3
	vje endloop4
	voutb 0x20, r2
	vadd r0, r1
	vjmp loop4
endloop4:
;---------------------------------------------

;wczytaj 2 liczbe ----------------------------
vxor r3, r3
loop5:
	vinb 0x21, r2
	vcmp r2, r3
	vje loop5

vxor r11, r11
vset r3, 0xa
vset r1, 0x30
vset r4, 10
loop6:
	vinb 0x20, r2
	vcmp r2, r3
	vje endloop6
	vsub r2, r1
	vmul r11, r4
	vadd r11, r2
	vjmp loop6
endloop6:
;---------------------------------------------

vadd r10, r11

;wypisz wynik --------------------------------
vset r0, wynik
vset r4, 10
vset r5, 1
vxor r3, r3
loop7:
	vmov r2, r10
	vmod r2, r4
	vadd r2, r1
	vstb r0, r2
	vadd r0, r5
	vdiv r10, r4
	vcmp r10, r3
	vjne loop7

vsub r0, r5
vxor r2, r2
loop8:
	vldb r2, r0
	vsub r0, r5
	vcmp r2, r3
	vje endloop8
	voutb 0x20, r2
	vjmp loop8
endloop8:

vset r2, 0xa
voutb 0x20, r2
;---------------------------------------------

voff

com1:
	db "Podaj pierwszą liczbe: ", 0
com2:
	db "Podaj drugą liczbe: ", 0

;tu przygotowywany jest wynik (zakladam max 10 cyfr)
wynik:
	times 10 db 0
