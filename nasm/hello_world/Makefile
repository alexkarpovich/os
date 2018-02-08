AC=nasm
LNK=ld
ACFLAGS=-f macho64
LNKFLAGS=-macosx_version_min 10.7.0 -lSystem

all: main

main: main.o
	$(LNK) $(LNKFLAGS) -o main main.o

main.o: main.asm
	$(AC) $(ACFLAGS) main.asm

clean:
	rm -rf *.o *.bin main
