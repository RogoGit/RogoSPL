clean: 
	rm *.o
	rm RogoForthress

all:
	nasm -f elf64 -g RogoForthress.asm
	ld -o RogoForthress *.o