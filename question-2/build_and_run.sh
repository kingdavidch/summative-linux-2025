nasm -f elf count_names.asm
ld -m elf_i386 count_names.o -o count_names
./count_names
