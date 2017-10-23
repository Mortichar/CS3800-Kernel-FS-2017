CFLAGS= -std=gnu99 -ffreestanding -fno-stack-protector -Wall -Wextra -m32 -g
LDFLAGS= -fno-stack-protector -ffreestanding -nostdlib -m32 -g

.PHONY: all qemu debug clean

all: myos.bin

qemu: myos.bin
	qemu-system-x86_64 -kernel myos.bin

debug: myos.bin
	echo 'set auto-load safe-path /' > ~/.gdbinit
	qemu-system-x86_64 -kernel myos.bin -s -S

myos.iso: myos.bin grub.cfg
	mkdir -p isodir/boot/grub
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub2-mkrescue -o myos.iso isodir

myos.bin: linker.ld kernel.o ps2.o boot.o tables.o interrupt.o interrupt_handlers.o screen.o
	gcc ${LDFLAGS} -T linker.ld -o myos.bin boot.o kernel.o ps2.o tables.o interrupt.o interrupt_handlers.o screen.o
	# To check that the file was created correctly, run
	# grub-file --is-x86-multiboot myos.bin

kernel.o: kernel.c ps2.h tables.h
	gcc ${CFLAGS} -c kernel.c -o kernel.o

ps2.o: ps2.c ps2.h
	gcc ${CFLAGS} -c ps2.c -o ps2.o

tables.o: tables.c tables.h
	gcc ${CFLAGS} -c tables.c -o tables.o

screen.o: screen.c screen.h
	gcc ${CFLAGS} -c screen.c -o screen.o

interrupt.o: interrupt.c interrupt.h
	gcc ${CFLAGS} -c interrupt.c -o interrupt.o

# Interrupts don't save the values of some CPU registers;
# we need to tell the compiler to not use those registers
# in our interrupt code.
# Also, ignore the unused parameter; these handlers MUST
# take a parameter even if you aren't going to use it.
interrupt_handlers.o: interrupt_handlers.c interrupt_handlers.h
	gcc ${CFLAGS} -mgeneral-regs-only -Wno-unused-parameter -c interrupt_handlers.c -o interrupt_handlers.o

boot.o: boot.s
	as --32 boot.s -o boot.o

clean:
	rm -f *.o myos.bin myos.iso
	rm -rf isodir
