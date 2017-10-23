#pragma once

#include <stdint.h>
#include <stddef.h>

/* A GDT entry */
typedef struct {
  uintptr_t base;
  uintptr_t limit;
  uint8_t  type;
} GDT_entry;

/* A TSS entry; most of this is not used */
typedef struct {
  uint16_t   link;
  uint16_t   link_h;

  uint32_t   esp0;
  uint16_t   ss0;
  uint16_t   ss0_h;

  uint32_t   esp1;
  uint16_t   ss1;
  uint16_t   ss1_h;

  uint32_t   esp2;
  uint16_t   ss2;
  uint16_t   ss2_h;

  uint32_t   cr3;
  uint32_t   eip;
  uint32_t   eflags;

  uint32_t   eax;
  uint32_t   ecx;
  uint32_t   edx;
  uint32_t    ebx;

  uint32_t   esp;
  uint32_t   ebp;

  uint32_t   esi;
  uint32_t   edi;

  uint16_t   es;
  uint16_t   es_h;

  uint16_t   cs;
  uint16_t   cs_h;

  uint16_t   ss;
  uint16_t   ss_h;

  uint16_t   ds;
  uint16_t   ds_h;

  uint16_t   fs;
  uint16_t   fs_h;

  uint16_t   gs;
  uint16_t   gs_h;

  uint16_t   ldt;
  uint16_t   ldt_h;

  uint16_t   trap;
  uint16_t   iomap;

} __attribute__((packed)) TSS_entry;


/* An IDT entry; this is the actual memory representation
 * (unlike how the GDT_entry struct is set up)
 */
typedef struct {
   uint16_t offset_1; // offset bits 0..15
   uint16_t selector; // a code segment selector in GDT or LDT
   uint8_t zero;      // unused, set to 0
   uint8_t type_attr; // type and attributes, see below
   uint16_t offset_2; // offset bits 16..31
} __attribute__((packed)) IDT_entry;


/* Add a new IDT entry for a given interrupt
 * Right now, all ISRs are run in kernel mode as a 32-bit interrupt gate
 */
void add_idt_entry(uint8_t interrupt, uintptr_t ISR);

/* Create a basic TSS entry */
void create_tss(uintptr_t esp);

/* Create a basic GDT */
void create_gdt();

/* Actually set the GDT */
void set_gdt(uint8_t* gdt, uint16_t size);

void reload_segments();
