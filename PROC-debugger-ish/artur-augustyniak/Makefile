CC = gcc
CC_OPTS= -std=c99 -O3
SRCDIR=src
APPNAME_32 = ptrace_write32.bin
APPNAME_64 = ptrace_write64.bin
src=$(addprefix $(SRCDIR)/, *.c)

run: run32 run64

run32: $(SRCDIR)/ptrace_write.c
	$(CC) $(CC_OPTS) -m32 $(SRCDIR)/ptrace_write.c -o $(APPNAME_32)
	$(CC) $(CC_OPTS) -m32 $(SRCDIR)/inferior.c -o inferior32.bin
	./$(APPNAME_32)

run64: $(SRCDIR)/ptrace_write.c
	$(CC) $(CC_OPTS) $(SRCDIR)/ptrace_write.c -o $(APPNAME_64)
	$(CC) $(CC_OPTS) $(SRCDIR)/inferior.c -o inferior64.bin
	./$(APPNAME_64)

.PHONY: clean
clean:
	rm -f *.bin