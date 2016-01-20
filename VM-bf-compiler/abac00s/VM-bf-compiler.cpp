/*
  VM-bf-compiler
  Author: Adam Bac

  The MIT License (MIT)
  Copyright (c) 2016 Adam Bac

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation file
  (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do
  so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  Usage:
  VM-bf-compiler <source file> <output file>

  Note: default free byte array size is 256 bytes. To change it, simply change MEMSIZE constant.
*/

#include <cstdlib>
#include <cstdio>
#include <stdint.h>
#include <stack>
using namespace std;

// Size of free byte array
#define MEMSIZE 256

// Pointers to input and output file
FILE *fi, *fo;

/*
  Writes a single assembler command to the file.
  opcode - well, just opcode
  rd - destination register (if unnecessary, set to -1)
  rs - source register (if unnecessary, set to -1)
  val - input value (if unnecessary, set to -1)
  val_length - length of input value in bytes (possible 1, 2 or 4, if there is no input value, this argument can have any value)
*/
void write_op(int opcode, int rd, int rs, int val, int val_len) {
  int len = 1 + (rd < 0 ? 0 : 1) + (rs < 0 ? 0 : 1) + (val < 0 ? 0 : val_len);
  uint8_t buf[7];
  buf[0] = (uint8_t) opcode;
  int x = 1;
  if(rd >= 0) { buf[x] = (uint8_t) rd; x++; }
  if(rs >= 0) { buf[x] = (uint8_t) rs; x++; }
  if(val >= 0) {
    buf[x] = (uint8_t) (val & 0xff);
    buf[x+1] = (uint8_t) ((val >> 8) & 0xff);
    buf[x+2] = (uint8_t) ((val >> 16) & 0xff);
    buf[x+3] = (uint8_t) ((val >> 24) & 0xff);
  }

  if((int) fwrite(buf, 1, len, fo) != len) {
    printf("Couldn't write the file.\n");
    fclose(fi);
    fclose(fo);
    exit(0);
  }
}

/*
  Writes a number to specified position in the output file.
  pos - position to which write the number
  number - number to write
  num_len - length of the number in bytes
*/
void write_number_to_pos(int pos, int number, int num_len) {
  uint8_t buf[4];
  buf[0] = (uint8_t) (number & 0xff);
  buf[1] = (uint8_t) ((number >> 8) & 0xff);
  buf[2] = (uint8_t) ((number >> 16) & 0xff);
  buf[3] = (uint8_t) ((number >> 24) & 0xff);

  size_t temp = ftell(fo);
  fseek(fo, pos, SEEK_SET);
  if((int) fwrite(buf, 1, num_len, fo) != num_len) {
    printf("Couldn't write the file.\n");
    fclose(fi);
    fclose(fo);
    exit(0);
  }
  fseek(fo, temp, SEEK_SET);
}

/*
  Writes specified number of zero bytes. Used to create free byte array at the end of the file.
*/
void write_zeroes(int len) {
  uint8_t buf[len];
  for (int i = 0; i < len; i++) buf[i] = 0;
  if((int) fwrite(buf, 1, len, fo) != len) {
    printf("Couldn't write the file.\n");
    fclose(fi);
    fclose(fo);
    exit(0);
  }
}

int main(int argc, char **argv) {
  if(argc < 3) {
    printf("Usage: %s <source file> <output file>\n\n", argv[0]);
    exit(0);
  }

  fi = fopen(argv[1], "r");
  if(fi == NULL) {
    printf("Couldn't open the file %s", argv[1]);
    exit(0);
  }
  fo = fopen(argv[2], "wb");
  if(fo == NULL) {
    printf("Couldn't create the file %s", argv[2]);
    fclose(fi);
    exit(0);
  }

  write_op(0x1, 0, -1, 0, 4); // vset r0, 0 ; this 0 will be changed to free memeory address at the end
  write_op(0x0, 1, 0, -1, 0); // vmov r1, r0 ; r1 will be a memory pointer
  write_op(0x1, 2, -1, 1, 4); // vset r2, 1 ; used for arithmetical operations
  write_op(0x1, 4, -1, 0, 4); // vset r4, 0 ; used for comparations

  stack <int> br_stack;

  char c;
  while((c = fgetc(fi)) != EOF) {
    switch (c) {
      case '+':
        write_op(0x4, 3, 1, -1, 0); // vldb r3, r1
        write_op(0x10, 3, 2, -1, 0); // vadd r3, r2
        write_op(0x5, 1, 3, -1, 0); // vstb t1, r3
        break;
      case '-':
        write_op(0x4, 3, 1, -1, 0); // vldb r3, r1
        write_op(0x11, 3, 2, -1, 0); // vsub r3, r2
        write_op(0x5, 1, 3, -1, 0); // vstb t1, r3
        break;
      case '>':
        write_op(0x10, 1, 2, -1, 0); // vadd r1, r2
        break;
      case '<':
        write_op(0x11, 1, 2, -1, 0); // vsub r1, r2
        break;
      case '.':
        write_op(0x4, 3, 1, -1, 0); // vldb r3, r1
        write_op(0xf2, -1, 3, 0x20, 1); //voutb 0x20, r3
        break;
      case ',':
        write_op(0xf3, 3, -1, 0x20, 1); // vinb 0x20, r3
        write_op(0x5, 1, 3, -1, 0);
        break;
      case '[':
        br_stack.push(ftell(fo));
        write_op(0x4, 3, 1, -1, 0); // vldb r3, r1
        write_op(0x20, 3, 4, -1, 0); // vcmp r3, r4
        write_op(0x21, -1, -1, 0, 2); // vjz 0 ; address of the corresponding bracket, will be filled later
        break;
      case ']':
        int prev = br_stack.top();
        br_stack.pop();
        int current = ftell(fo);
        int offset1 = (prev - current - 3) & 0xffff;
        int offset2 = (current - prev - 6) & 0xffff;
        write_op(0x40, -1, -1, offset1, 2);
        write_number_to_pos(prev + 7, offset2, 2); // fill the address, which was set to zero earlier
    }
  }
  write_op(0xff, -1, -1, -1, 0);
  int free_mem = ftell(fo);
  write_zeroes(MEMSIZE);
  write_number_to_pos(2, free_mem, 4);

  fclose(fi);
  fclose(fo);
  return 0;
}
