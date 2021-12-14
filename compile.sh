nasm bootload.asm
dd if=/dev/zero of=diskc.img bs=512 count=1000
dd if=bootload of=diskc.img bs=512 count=1 conv=notrunc
as86 kernel.asm -o kernel_asm.o
bcc -ansi -c -o kernel_c.o kernel.c
ld86 -o kernel -d kernel_c.o kernel_asm.o
gcc -o loadFile loadFile.c

as86 userlib.asm -o userlib.o
bcc -ansi -c -o shell_c.o shell.c
ld86 -o shell -d shell_c.o userlib.o
gcc -o loadFile loadFile.c




bcc -ansi -c -o tstpr1.o tstpr1.c
as86 -o userlib.o userlib.asm
ld86 -d -o tstpr1 tstpr1.o userlib.o

bcc -ansi -c -o tstpr2.o tstpr2.c
as86 -o userlib.o userlib.asm
ld86 -d -o tstpr2 tstpr2.o userlib.o

bcc -ansi -c -o number.o number.c
as86 -o userlib.o userlib.asm
ld86 -d -o number number.o userlib.o

bcc -ansi -c -o letter.o letter.c
as86 -o userlib.o userlib.asm
ld86 -d -o letter letter.o userlib.o


./loadFile kernel
./loadFile message.txt
./loadFile tstpr1
./loadFile tstpr2
./loadFile number
./loadFile letter
./loadFile shell
