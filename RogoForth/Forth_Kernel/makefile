clean: 
	rm *.o
	rm RogoForthress

all:
	nasm -f elf64 -g RogoForthress.asm
	nasm -f elf64 -g lib.inc
	nasm -f elf64 -g forth_words.inc
	nasm -f elf64 -g forth_macro.inc
	ld -o RogoForthress *.o