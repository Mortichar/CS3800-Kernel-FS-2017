#include "tables.h"

/* A single really boring TSS entry */
static TSS_entry TSS = {
  .ss0 = 0x10,
  .iomap = sizeof(TSS_entry),
};

#define NUM_GDTS 4

/* The GDT entries; we have a flat address space where everything is executable, writable, and readable */
static GDT_entry GDT_entries[NUM_GDTS] = {
  {.base=0, .limit=0, .type=0},                     // Selector 0x00 cannot be used
  {.base=0, .limit=0xffffffff, .type=0x9A},         // Selector 0x08 will be our code
  {.base=0, .limit=0xffffffff, .type=0x92},         // Selector 0x10 will be our data
  {.base=(uintptr_t)&TSS, .limit=sizeof(TSS_entry), .type=0x89}, // You can use LTR(0x18)
};

/* The GDT -- generated from the GDT_entries array;
 * GDT entries are stored kinda weird so we have to convert them
 * into the structure the CPU expects
 */
static uint8_t GDT[NUM_GDTS * 8];

/* The IDT -- the i386 has 256 possible IDTs
 * This table should be all 0 except for the IDTs we want
 */
static IDT_entry IDT[256] = {};

/* Set the IDT in the CPU */
void set_idt()
{
  uint8_t idtr[6];

  asm volatile(
   "movl %0, 0x2(%2) ;\n"
   "movw %1, (%2) ;\n"
   "lidtw  (%2)"
   : /* no output */
   : "r"(IDT), "r"(sizeof(IDT) - 1), "r"(idtr)
  );
}

/* Add an entry to the IDT and update the CPU's IDT info */
void add_idt_entry(uint8_t interrupt, uintptr_t ISR)
{
  IDT_entry* e = &IDT[interrupt];

  e->offset_1 = ISR & 0xFFFF;
  e->offset_2 = ISR >> 16;

  e->zero = 0;

  e->selector = 0x08; // kernel code

  e->type_attr = 0x8E; // 32-bit interrupt gate

  set_idt();
}

void encode_gdt_entry(uint8_t *target, GDT_entry source)
{
    // Check the limit to make sure that it can be encoded
    if ((source.limit > 65536) && (source.limit & 0xFFF) != 0xFFF) {
        //kerror("You can't do that!");
    }
    if (source.limit > 65536) {
        // Adjust granularity if required
        source.limit = source.limit >> 12;
        target[6] = 0xC0;
    } else {
        target[6] = 0x40;
    }
 
    // Encode the limit
    target[0] = source.limit & 0xFF;
    target[1] = (source.limit >> 8) & 0xFF;
    target[6] |= (source.limit >> 16) & 0xF;
 
    // Encode the base 
    target[2] = source.base & 0xFF;
    target[3] = (source.base >> 8) & 0xFF;
    target[4] = (source.base >> 16) & 0xFF;
    target[7] = (source.base >> 24) & 0xFF;
 
    // And... Type
    target[5] = source.type;
}

void set_ltr() {
  __asm__ __volatile__ ( "ltr %%ax" : : "a" ( 3<<3 ) );
}

void create_tss(uintptr_t esp)
{
  TSS.esp0 = esp;
}

//void create_gdt(uint8_t* target, GDT_entry* source, uint8_t num_gdts)
void create_gdt()
{
  uint8_t* t = GDT;
  GDT_entry* source = GDT_entries;
  uint8_t num_gdts = NUM_GDTS;

  for(uint8_t i = 0; i < num_gdts; i++)
  {
    encode_gdt_entry(t, source[i]);
    t += 8;
  }

  set_gdt(GDT, sizeof(GDT)-1);
  set_ltr();
  reload_segments();
}

void set_gdt(uint8_t* gdt, uint16_t len)
{
  uint8_t gdtr[sizeof(gdt) + sizeof(len)];

  asm volatile(
   "movl %0, 0x2(%2) ;\n"
   "movw %1, (%2) ;\n"
   "lgdtw  (%2)"
   : /* no output */
   : "r"(gdt), "r"(len), "r"(gdtr)
  );
}

void reload_segments()
{
  asm volatile(
      "   jmp   $0x08, $reload_CS%=\n" // 0x08 points at the new code selector
      "reload_CS%=:\n"
      // Reload data segment registers
      "   movw   $0x10, %%ax\n" // 0x10 points at the new data selector
      "   movw   %%ax,  %%ds\n"
      "   movw   %%ax,  %%es\n"
      "   movw   %%ax,  %%fs\n"
      "   movw   %%ax,  %%gs\n"
      "   movw   %%ax,  %%ss\n"
      : /* no output */
      : /* no input */
      : "eax"
      );
}
