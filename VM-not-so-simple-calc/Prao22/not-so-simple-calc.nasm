%include "vm.inc"

vset r3, welcome
vcall print ; wypisujemy powitanie

vset r3, text
vcall print ; wypisujemy prosbe o liczbe

vset r3, 0x1234 ; 1-sza cyfra, 1-szej liczby bedzie pod adresem 0x1234
vcall scan

vmov r8, r3 ; zachowujemy na pozniej adres osatniej cyfry 1-szej liczby w r8

vset r3, text
vcall print ; prosba o liczbe

vset r3, 0x2345 ; 1-sza cyfra, 2-giej liczby bedzie pod adresem 0x2345
vcall scan

vmov r9, r3 ; zachowujemy na pozniej aders ostatniej cyfry 2-giej liczby w r9


vset r11, 0x3000 ; ostatnia cyfra wyniku bedzie zapisana w 0x3000
vcall count ; liczymy

vset r3, result
vcall print

vcall result_print ; i wypisujemy

voff



count:
vxor r0, r0 ; 0 do porownan
vset r1, 1 ; 1 do przesuwania
vset r7, 48 ; 48 = '0' w ASCII
vset r10, 10 ; 10 do operacji dzielenia i modulo
vxor r5, r5


count_loop:

vldb r3, r8 ; pobieranie od konca cyfr 1-szej liczby
vldb r4, r9 ; pobieranie od konca cyfr 2-giej liczby

vcmp r3, r0
vjz .end_num1 ; koniec liczby nr1

vcmp r4, r0
vjz .end_num2 ; koniec liczby nr2

; sytuacja gdy dwie liczby sie jeszcze nie skonczyly
vsub r3, r7 ; zamiana znaku na liczbe
vsub r8, r1 ; cofanie sie w pamieci

vsub r4, r7 ; to samo dla liczby nr2
vsub r9, r1
vjmp .main_count

; sytuacja gdy co najmniej jedna liczba sie juz skonczyla (nr1 napewno)
.end_num1:

vcmp r4, r0
vjz .end_count ; jesli obie sie skonczyly to jest koniec liczenia

; liczba nr1 sie skonczyla a nr2 jeszcze nie

vsub r4, r7 ; zamiana zanku cyfry, liczby nr2 na liczbe
vsub r9, r1
; liczby nr 1 nie trzeba zamieniac bo to 0
vjmp .main_count

; sytuacja gdy liczba nr2 sie skonczyla a liczba nr1 nie
.end_num2:
vsub r3, r7 ; zamiana znaku na liczbe
vsub r8, r1
vjmp .main_count

.main_count:
vadd r3, r4 ; dodawanie
vadd r3, r5 ; z przeniesienia

vmov r5, r3

vdiv r5, r10 ; liczba do przeniesienia
vmod r3, r10 ; do zapisania

vadd r3, r7 ; zamiana na znak

vstb r11, r3 ; zapisanie
 
vsub r11, r1 ; cofniecie sie w pamieci

vjmp count_loop

.end_count:

vcmp r5, r0 ; sprawdzenie czy nic nie bylo do przeniesienia
vjnz last_add
vadd r11, r1 ; przesuniecie o 1 w adresie
vret

last_add:
vadd r5, r7
vstb r11, r5
vret


; funkcja do wypisywania komunikatow
print:

vxor r0, r0 ; 0 do porownywania
vset r1, 1 ; 1 do dodawania

print_loop:
vldb r2, r3 ; zaladuj z pamieci

vcmp r2, r0
vjz .end_print ; czy jest juz koniec

voutb 0x20, r2 ; wypisz znak
vadd r3, r1 ; przesun sie o 1

vjmp print_loop

.end_print:
vret ; powrot

; funkcja do wypisania liczby (wszystko to samo co w 'print' tylko liczba jest pod r11 a nie r3)
result_print:

vxor r0, r0
vset r1, 1

rprint_loop:
vldb r2, r11

vcmp r2, r0
vjz .end_rprint

voutb 0x20, r2
vadd r11, r1

vjmp rprint_loop

.end_rprint:
vset r2, 0xa
voutb 0x20, r2 ; znak nowej linii
vret




; funkcja do pobrania liczb
scan:

vset r0, 0xa ; znak nowej linii
vset r1, 1 ; jedynka do dodawania

scan_loop:
vinb 0x20, r2 ; pobranie znaku

vcmp r2, r0 ; czy to juz jest enter?
vjz .end_scan

vstb r3, r2 ; zapisujemy znak
vadd r3, r1 ; przesuwamy sie o 1

vjmp scan_loop

.end_scan:
vsub r3, r1 ; cofamy sie o 1 w pamieci
vret ; wracamy do miejsca z ktorego wywolalismy funkcje




welcome:
db "Program dodaje dwie dowolne liczby naturalne do siebie!", 0xa, 0
text:
db "Podaj liczbe: ", 0
result:
db "Wynik dodawania: ", 0
