clean: 
	rm *.o
	rm RogoForthress

all:
	nasm -f elf64 -g RogoForthress.asm
	nasm -f elf64 -g lib.asm
	ld -o RogoForthress *.o

third_part:
	cat forth_lib/third_part.frt - | ./RogoForthress