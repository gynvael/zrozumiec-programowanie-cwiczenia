;The MIT License (MIT)
;
;Copyright (c) 2016 Paweł Goca
;
;Permission is hereby granted, free of charge, to any person obtaining a copy
;of this software and associated documentation files (the "Software"), to deal
;in the Software without restriction, including without limitation the rights
;to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
;copies of the Software, and to permit persons to whom the Software is
;furnished to do so, subject to the following conditions:
;
;The above copyright notice and this permission notice shall be included in all
;copies or substantial portions of the Software.
;
;THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
;IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
;FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
;AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
;LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
;OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
;SOFTWARE.


%include "vm/vm.inc"

vset r0, 0xa            ; znak nowej linii
vset r6, 48             ; kod ascii cyfry 0, 49 to bedzie cyfra 1 itd..
vset r7, 10 

vxor r10, r10           ; pierwsza liczba
vxor r11, r11           ; druga liczba


load_number_1:
	vxor r1, r1
	vinb 0x20, r1         ; wczytujemy bajt z wejścia

	vcmp r1, r0           ; sprawdzamy czy wczytany bajt to znak nowej linii
	vje load_number_2 

	vsub r1, r6           ; jeżeli nie to odejmujemy 48, zeby byla to rzeczywiscie
	                      ; wpisana cyfra, a nie jej kod ascii 
	vmul r10, r7 
	vadd r10, r1 
	 
	vjmp load_number_1


load_number_2:          ; wszystko to samo co dla pierwszej liczby
	vxor r1, r1
	vinb 0x20, r1 

	vcmp r1, r0 
	vje add_numbers 

	vsub r1, r6 
	vmul r11, r7
	vadd r11, r1 
	 
	vjmp load_number_2

add_numbers:
	vadd r10, r11            ; dodajemy obie liczby do siebie
	vxor r5, r5              ; ile cyfr ma wynik, zeby bylo wiadomo ile potem sciagnac ze stosu
	vset r4, 1               ; tym bedziemy zwiekszac licznik

	vjmp push_numbers

push_numbers:
	vxor r3, r3
	vmov r11, r10            ; kopiujemy wartosc rejestru r10 do r11
	vmod r11, r7             ; modulo 10 zeby otrzymac ostatnia cyfre
	vadd r11, r6             ; dodajemy 48 zeby uzyskac odpowiadajacy cyfrze kod ascii
	vadd r5, r4              ; dodajemy 1 do licznika
	vpush r11                ; wrzucamy cyfre na stos
	                         ; robimy to dlatego, ze modulo 10 daje nam ostatnia cyfre
	                         ; wiec korzystajac ze stosu odwracamy liczbe zeby wypisac ja prawidlowo

	vdiv r10, r7             ; dzielimy wynik dodawania na 10, zeby moc wczytac kolejna cyfre
	vcmp r10, r3             ; jezeli liczba jest rowna 0, 
	                         ; znaczy ze wrzucilismy juz wszystkie cyfry na stos
	                     
	vjne push_numbers

print_numbers:
	vxor r3, r3
	vpop r10                 ; sciagamy cyfre ze stosu
	voutb 0x20, r10          ; zapisujemy bajt na wyjscie
	vsub r5, r4              ; odejmujemy od licznika
	vcmp r5, r3              ; sprawdzamy czy licznik jest rowny 0, czyli czy wypisalismy juz cala liczbe
	vjne print_numbers
	voutb 0x20, r0


voff