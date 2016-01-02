; Interpreter jezyka Brainfuck, rozwiazanie cwiczenia [VM:bf-inception]
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
;
;Uwaga! Program wpisujemy na stdin (powinien byc zalonczony [LF]), a nie ładujemy z pliku.
;
;Testowe programy:
;
;	++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.
;	Wyswietla "Hello World"
;	
;	,.
;	Wyswietla wpisany przed chwila znak
;
;	,--------------------------------.
;	Zmienia mala litere na wielka.

%include "vm.inc"
%define PROG_SIZE 1000
%define MEM_SIZE 1000

vset r0, prog
vset r1, brack
vset r13, mem

;wczytaj program -----------------------------
vxor r2, r2
vset r3, 0xa
vset r4, 1

loop1:
	vinb 0x21, r2
	vcmp r2, r4
	vjne loop1

loop2:
	vinb 0x20, r2
	vcmp r2, r3
	vje endloop2
	vstb r0, r2
	vadd r0, r4
	vjmp loop2
endloop2:

vset r0, prog
;---------------------------------------------

;znajdz odpowiadajace nawiasy ------------------------------
vxor r3, r3
vset r11, '['
vset r12, ']'
vset r5, prog
vset r8, 4
loop3:
	vldb r2, r0
	vcmp r2, r3
	vje endloop3
	vcmp r2, r11
	vjne addr
		vmov r6, r0
		vsub r6, r5
		vpush r6
	addr:
	vcmp r2, r12
	vjne addr1
		vpop r7
		vmov r6, r0
		vsub r6, r5
		vmul r6, r8
		vadd r1, r6
		vdiv r6, r8
		vst r1, r7
		vset r1, brack
		vmul r7, r8
		vadd r1, r7
		vst r1, r6
		vset r1, brack
	addr1:
	vadd r0, r4
	vjmp loop3
endloop3:

vset r0, prog
;--------------------------------------------

;wykonuj ------------------------------------
vset r12, prog
loop4:
	vldb r2, r0
	vcmp r2, r3
	vje endloop4
	vset r5, '>'
	vcmp r2, r5
	vjne addr2
		vadd r13, r4
		vadd r0, r4
		vjmp loop4
	addr2:
	vset r5, '<'
	vcmp r2, r5
	vjne addr3
		vsub r13, r4
		vadd r0, r4
		vjmp loop4
	addr3:
	vset r5, '+'
	vcmp r2, r5
	vjne addr4
		vldb r6, r13
		vadd r6, r4
		vstb r13, r6
		vadd r0, r4
		vjmp loop4
	addr4:
	vset r5, '-'
	vcmp r2, r5
	vjne addr5
		vldb r6, r13
		vsub r6, r4
		vstb r13, r6
		vadd r0, r4
		vjmp loop4
	addr5:
	vset r5, '['
	vcmp r2, r5
	vjne addr6
		vldb r6, r13
		vcmp r6, r3
		vjne addr7
			vmov r11, r0
			vsub r11, r12
			vmul r11, r8
			vadd r1, r11
			vld r6, r1
			vadd r6, r12
			vmov r0, r6
			vadd r0, r4
			vset r1, brack
			vjmp loop4
		addr7:
		vadd r0, r4
		vjmp loop4
	addr6:
	vset r5, ']'
	vcmp r2, r5
	vjne addr8
		vmov r11, r0
		vsub r11, r12
		vmul r11, r8
		vadd r1, r11
		vld r6, r1
		vadd r6, r12
		vmov r0, r6
		vset r1, brack
		vjmp loop4
	addr8:
	vset r5, '.'
	vcmp r2, r5
	vjne addr9
		vldb r6, r13
		voutb 0x20, r6
		vadd r0, r4
	addr9:
	vset r5, ','
	vcmp r2, r5
	vjne addr10
		loop5:
			vinb 0x21, r6
			vcmp r6, r3
			vje loop5
		vinb 0x20, r6
		vstb r13, r6
		vadd r0, r4
	addr10:
	vjmp loop4
endloop4:

voff
;--------------------------------------------------------

;przestrzen do zaladowania programu
prog:
	times PROG_SIZE+1 db 0
	
;aby przyspieszyc skoki miedzy nawiasami, w tej tablicy każedemu znakowi w programie, jeśli jest on nawiasem,
;przypisywana jest pozycja odpowiadajacego nawiasu
brack:
	times PROG_SIZE dd 0
	
;pamiec do wykorzystania przez program
mem:
	times MEM_SIZE db 0
	
