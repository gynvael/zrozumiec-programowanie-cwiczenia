/*
    Interpreter Brainfuck (VM-bf)
    Autor: Adam Bac (abac00s)

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

    Sposob uzycia (dla programu skompilowanego do pliku VM-bf.exe):
    > VM-bf <nazwa pliku do wykonania>

    Jesli potrzeba wiekszej ilosci pamieci operacyjnej programu, mozna zmienic stala MEMSIZE.
*/

#include <cstdio>
#include <cstdlib>
#include <stack>
#include <map>
#define MEMSIZE 256

char* mem;
char* prog;
int pt;
std::map <int, int> bm;

void clean() {
    delete [] mem;
    delete [] prog;
}

void load_file(const char* fname) {
    FILE* fp = fopen(fname, "rb");
    if(fp == NULL) {
        printf("Error: Couldn't open the file %s - process terminated.\n", fname);
        fclose(fp);
        exit(0);
    }
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    rewind(fp);

    prog = new char[len+1];
    int i = 0;
    do {
        prog[i++] = fgetc(fp);
    } while(prog[i-1] != EOF);

    fclose(fp);
}

void init() {
    mem = new char[MEMSIZE];
    if(mem == NULL) {
        printf("Error: Memory allocation error - process terminated.\n");
        delete [] prog;
        exit(0);
    }
    for(int i=0; i<MEMSIZE; i++) mem[i] = 0;
    pt = 0;
}

void check_mem_range() {
    if(pt >= MEMSIZE || pt < 0) {
        printf("Error: Program tries to access unavailable memory (the pointer is set to a byte out of range 0-%x).\n", MEMSIZE-1);
        clean();
        exit(0);
    }
}

void bracket_map() {
    std::stack <int> st;
    for(int i = 0; prog[i] != EOF; i++) {
        if(prog[i] == '[') st.push(i);
        else if(prog[i] == ']') {
            if(st.empty()) {
                printf("Error: Invalid bracket series.\n");
                clean();
                exit(0);
            }
            bm[i] = st.top();
            bm[st.top()] = i;
            st.pop();
        }
    }
}

int main(int argc, const char** argv) {
    if(argc < 2) {
        printf("BRAINFUCK v1.0\nAuthor: Adam Bac\n\nUsage: bf [file to execute]\n");
        exit(0);
    }
    load_file(argv[1]);
    init();
    bracket_map();
    int i = 0;
    while(prog[i] != EOF) {
        char c = prog[i];
        switch(c) {
            case '>':
                pt++;
                i++;
                check_mem_range();
                break;
            case '<':
                pt--;
                i++;
                check_mem_range();
                break;
            case '+':
                mem[pt]++;
                i++;
                break;
            case '-':
                mem[pt]--;
                i++;
                break;
            case '.':
                putchar(mem[pt]);
                i++;
                break;
            case ',':
                mem[pt] = getchar();
                i++;
                break;
            case '[': {
                if(mem[pt] == 0) {
                    i = bm[i]+1;
                }
                else i++;
                break;
            }
            case ']':
                i = bm[i];
                break;

            default:
                i++;
                break;
        }
    }

    clean();
    return 0;
}
