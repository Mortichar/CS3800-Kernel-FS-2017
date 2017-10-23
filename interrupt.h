#pragma once
#include "port.h"

#define PIC1  0x20  /* IO base address for master PIC */
#define PIC2  0xA0  /* IO base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PIC1_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)

/* Enable or disable interrupts */
void enable_interrupts();
void disable_interrupts();

/* Remap PIC interrupts */
void PIC_remap(int offset1, int offset2);

/* Disable all IRQs */
void IRQ_mask_all();
/* Mask (disable) a given IRQ */
void IRQ_set_mask(unsigned char IRQline);
/* Clear (enable) a given IRQ */
void IRQ_clear_mask(unsigned char IRQline);


/* Indicate that we are done handling an IRQ */
void PIC_sendEOI(unsigned char irq);
