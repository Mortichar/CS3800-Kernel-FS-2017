# Getting started:

This directory contains a VERY basic kernel.
Hopefully this gets most of the boring-but-tricky work out of the way for you so you can focus on doing more interesting stuff.

Right now the kernel just echos keyboard scancodes to the screen; if you boot it up and type on the keyboard you should see a beautiful
parade of hex digits.
If you want it to do anything actually fun, uh, well, that's your problem, pal.

1. Run `sudo ./setup.sh` to install the necessary software.
2. Run `make qemu` to compile and boot the kernel as it is.
3. Read the Code Structure section and start looking through the source files!

# Software Pre-reqs:

- GCC 7 (needed for __attribute__((interrupt)) )
- GDB
- Qemu

## For booting with virtualbox:

- libisoburn
- xorriso
- mtools
- grub

# Compiling:

- `make` builds a multiboot binary that qemu can boot
- `make qemu` should launch qemu with your OS!
- `make iso` builds an iso that virtualbox or physical hardware should boot

# Debugging:

- GDB can remotely debug a kernel running in Qemu. This is very cool.
- Run `make debug`, then in another terminal in the 'kernel' folder, run `gdb`
	- The .gdbinit file contains some commands that auto-connect you to the qemu debug server
- Use 'hbreak' rather than 'break' to set breakpoints

# Code structure:

This kernel is built for a 32-bit i386 architecture.
It's best to start here rather than jumping right in to an x64 kernel since there are additional complexities involved in getting that running.

By the way, you don't have most standard library things -- you're on your own for most things.
(If you want to change this, you'll need to look into setting up a cross-compiler; crosstool looks promising for this.)

- linker.ld defines the layout of myos.bin -- this is not a normal executable!
- boot.s does very early setup of system behavior; this probably won't need modification.
- kernel.c contains kernel_main(); this function is called by boot.s and is where the kernel "starts" executing
	- GDT/TSS/Interrupt/PIC/IRQ setup is all done here
	- This is probably where you should start looking
- screen.h/screen.c contain very basic terminal output stuff. No printf()!
- ps2.h/ps2.c contains some functions for getting scancodes from the keyboard
- port.h contains a couple handy asm functions for talking to IO ports
- interrupt.h/interrupt.c contain the interrupt handlers
- table.h/table.c contain structs and functions for setting up the GDT/TSS/IDT tables
- grub.cfg contains info grub needs to properly set up a bootloader when making an iso

# What you might be interested in doing from here:

- Set up handlers for error interrupts ( http://www.logix.cz/michal/doc/i386/chp09-08.htm#09-08 )
- Translate scancodes into actual letters
- Write a shell that users can enter commands to
- Configure linker.ld to put some other executables in memory and let the user execute them from the shell
- Talk to a hard drive or floppy drive; access a filesystem
- Load executables off your filesystem and execute them
- Process scheduling
- Memory allocation
- Set up fancy CPU features, like multiprocessing

# Additional reading:
## Setting up interrupts:

- Set up the GDT and TSS tables (this is boring, don't worry too much about it)
- Write some interrupt service routines (the code that runs when an interrupt is triggered)
- Set up the interrupt descriptor table (mapping of interrupt -> function to call)
- Map the PIC and enable IRQs

You'll need some extra compiler flags for your ISR code, so it is best to put those in their own c file.
Related: you don't have access to a lot of CPU features in your ISRs.

- http://wiki.osdev.org/GDT_Tutorial
- https://stackoverflow.com/questions/23978486/far-jump-in-gdt-in-bootloader
- http://wiki.osdev.org/TSS
- http://wiki.osdev.org/Interrupt_Descriptor_Table
- http://wiki.osdev.org/Interrupt_Service_Routines
- http://wiki.osdev.org/8259_PIC
- http://wiki.osdev.org/I_Can't_Get_Interrupts_Working
- http://wiki.osdev.org/Interrupts

## PS/2 Keyboard:

- http://wiki.osdev.org/PS/2_Keyboard
- http://wiki.osdev.org/"8042"_PS/2_Controller

## Disks and filesystems:

- http://wiki.osdev.org/ATA_PIO_Mode
- http://wiki.osdev.org/ATA_in_x86_RealMode_(BIOS)
- http://wiki.osdev.org/Floppy_Disk_Controller
- http://wiki.osdev.org/FAT

# Miscellaneous:

- A bunch of tutorials of varying quality: http://wiki.osdev.org/Tutorials
- Some references on programming the i386 architecture: http://www.logix.cz/michal/doc/i386/
- Nifty inline assembly functions: http://wiki.osdev.org/Inline_Assembly/Examples
- Writing a bootloader: http://wiki.osdev.org/Bootloader
