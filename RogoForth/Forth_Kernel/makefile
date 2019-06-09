clean: 
	rm *.o
	rm RogoForthress

all:
	nasm -f elf64 -g RogoForthress.asm
	nasm -f elf64 -g lib.asm
	ld -o RogoForthress *.o