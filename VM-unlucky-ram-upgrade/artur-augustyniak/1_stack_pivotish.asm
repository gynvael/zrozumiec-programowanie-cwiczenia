%include "vm.inc"

vpop r0
vpop r0
vpop r0
vpop r0             ; vm inicjalnie ma sp ustawiony na self.sp.v = 0x10000
vpop r0             ; wykonujac n * vpop możemy przesunac sie w zakres niedostepny via jmp/call
vpop r0             ; $ python -c "print hex(0x10000 + n * 4)"
                    ; jesli n == 6 to sp = 0x10018

vset r0, 0xff
vpush r0            ; umieszczam na stosie dowolny kod, demonstracyjnie opcode voff

vset r15, 0x10014   ; sp = 0x10018 - 4