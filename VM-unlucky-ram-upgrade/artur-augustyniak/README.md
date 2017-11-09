VM-unlucky-ram-upgrade
========================

Modyfikacje kodu vm według tekstu ćwiczenia:

https://github.com/gynvael/zrozumiec-programowanie.git 

```git
diff --git a/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm_memory.py b/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm_memory.py
index 88243a8..6a37526 100644
--- a/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm_memory.py
+++ b/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm_memory.py
@@ -4,7 +4,8 @@
 
 class VMMemory(object):
   def __init__(self):
-    self._mem = bytearray(64 * 1024)
+    #self._mem = bytearray(64 * 1024)
+    self._mem = bytearray(1024 * 1024)
```

ułatwia zabawę także:

```git
diff --git a/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm.py b/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm.py
index 5622085..38110a3 100644
--- a/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm.py
+++ b/007-Czesc_II-Rozdzial_3-Podstawy_architektury_komputerowe/vm.py
@@ -167,9 +167,9 @@ class VMInstance(object):
 
     handler = self.opcodes[opcode][self.INSTR_HANDLER]
     # Uncomment this line to get a dump of executed instructions.
-    #print("%.4x: %s\t%s" % (self.pc.v, 
-    #                        handler.func_name, 
-    #                        str(argument_bytes).encode("hex")))
+    print("%.4x: %s\t%s" % (self.pc.v,
+                           handler.func_name,
+                           str(argument_bytes).encode("hex")))
     self.pc.v += 1 + length
     handler(self, argument_bytes)


```

Oczekiwany efekt:

```bash
$ nasm 1_stack_pivotish.asm                                                                                               │
$ hexdump -C 1_stack_pivotish                                                                                             │
00000000  31 00 31 00 31 00 31 00  31 00 31 00 01 00 ff 00  |1.1.1.1.1.1.....|                                                                                │
00000010  00 00 30 00 01 0f 14 00  01 00                    |..0.......|                                                                                      │
0000001a                                                                                                                                                      │
$ python ./vm.py ./1_stack_pivotish 
0000: VPOP      00
0002: VPOP      00
0004: VPOP      00
0006: VPOP      00
0008: VPOP      00
000a: VPOP      00
000c: VSET      00ff000000
0012: VPUSH     00
0014: VSET      0f14000100
10014: VOFF 
```

```bash
$ nasm 2_stack_pivotish.asm                                                                                               │
$ hexdump -C 2_stack_pivotish                                                                                             │
00000000  01 0e 00 04 01 00 01 00  ff 00 00 00 30 00 01 0f  |............0...|                                                                                │
00000010  fc 03 01 00                                       |....|                                                                                            │
00000014
$ python ./vm.py ./2_stack_pivotish 
0000: VSET      0e00040100
0006: VSET      00ff000000
000c: VPUSH     00
000e: VSET      0ffc030100
103fc: VOFF                 
```