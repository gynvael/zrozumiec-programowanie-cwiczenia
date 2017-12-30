#include <stdio.h>
/*gcc -m32 -o inferior inferior.c*/
int main()
{
  puts("Hello");
  puts("ptrace!");
  //__asm__("int3");
  return 0;
} 
