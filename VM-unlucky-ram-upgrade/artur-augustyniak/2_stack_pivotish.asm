%include "vm.inc"

                    ; sytuacja niemal identyczna jak poprzednio
                    ; ale można to zrobic prosciej poniewaz
                    ; mozemy dowlnie modyfikowac sp

vset r14, 0x10400   ; przesuwam sp w obszar powyzej 0xffff

vset r0, 0xff
vpush r0            ; umieszczam na stosie dowolny kod, demonstracyjnie opcode voff

vset r15, 0x103fc   ; ustawiam pc na sp po wrzuceniu push voff