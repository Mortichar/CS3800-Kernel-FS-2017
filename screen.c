#include "screen.h"

/* Hardware text mode color constants. */
enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};

#define	VGA_AC_INDEX		0x3C0
#define	VGA_AC_WRITE		0x3C0
#define	VGA_AC_READ			0x3C1
#define	VGA_MISC_WRITE		0x3C2
#define VGA_SEQ_INDEX		0x3C4
#define VGA_SEQ_DATA		0x3C5
#define	VGA_DAC_READ_INDEX	0x3C7
#define	VGA_DAC_WRITE_INDEX	0x3C8
#define	VGA_DAC_DATA		0x3C9
#define	VGA_MISC_READ		0x3CC
#define VGA_GC_INDEX 		0x3CE
#define VGA_GC_DATA 		0x3CF
		/*			COLOR emulation		MONO emulation */
#define VGA_CRTC_INDEX		0x3D4		/* 0x3B4 */
#define VGA_CRTC_DATA		0x3D5		/* 0x3B5 */
#define	VGA_INSTAT_READ		0x3DA

#define	VGA_NUM_SEQ_REGS	5
#define	VGA_NUM_CRTC_REGS	25
#define	VGA_NUM_GC_REGS		9
#define	VGA_NUM_AC_REGS		21
#define	VGA_NUM_REGS		(1 + VGA_NUM_SEQ_REGS + VGA_NUM_CRTC_REGS + \
							VGA_NUM_GC_REGS + VGA_NUM_AC_REGS)

vga_mode VGA_MODE = TEXT;
 
static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}

size_t strlen(const char* str) {
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}
//terminal
unsigned char _80x25_text[] =
{
/* MISC */
	0x67,
/* SEQ */
	0x03, 0x00, 0x03, 0x00, 0x02,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x55, 0x81, 0xBF, 0x1F,
	0x00, 0x4F, 0x0D, 0x0E, 0x00, 0x00, 0x00, 0x50,
	0x9C, 0x0E, 0x8F, 0x28, 0x1F, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x0E, 0x00,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
	0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
	0x0C, 0x00, 0x0F, 0x08, 0x00
};  
#define VGA_WIDTH 80
#define VGA_HEIGHT 25

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint8_t* vga_buffer;
//graphical
//TODO: Dynamic resolution
//Current: 320x200x256 (256 colors)
unsigned char _320x200x256[] =
{
/* MISC */
	0x63,
/* SEQ */
	0x03, 0x01, 0x0F, 0x00, 0x0E,
/* CRTC */
	0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
	0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
	0xFF,
/* GC */
	0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
	0xFF,
/* AC */
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
	0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
	0x41, 0x00, 0x0F, 0x00,	0x00
};
#define VIDEO_WIDTH 320
#define VIDEO_HEIGHT 200

size_t terminal_contents[VIDEO_WIDTH * VIDEO_HEIGHT];

// outb(0x3D4, 0x0F);
// outb(0x3D5, (uint8_t) (pos & 0xFF));

void write_regs(unsigned char *regs)
{
	unsigned i;

/* write MISCELLANEOUS reg */
	outb(VGA_MISC_WRITE, *regs);
	regs++;
/* write SEQUENCER regs */
	for(i = 0; i < VGA_NUM_SEQ_REGS; i++)
	{
		outb(VGA_SEQ_INDEX, i);
		outb(VGA_SEQ_DATA, *regs);
		regs++;
	}
/* unlock CRTC registers */
	outb(VGA_CRTC_INDEX, 0x03);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) | 0x80);
	outb(VGA_CRTC_INDEX, 0x11);
	outb(VGA_CRTC_DATA, inb(VGA_CRTC_DATA) & ~0x80);
/* make sure they remain unlocked */
	regs[0x03] |= 0x80;
	regs[0x11] &= ~0x80;
/* write CRTC regs */
	for(i = 0; i < VGA_NUM_CRTC_REGS; i++)
	{
		outb(VGA_CRTC_INDEX, i);
		outb(VGA_CRTC_DATA, *regs);
		regs++;
	}
/* write GRAPHICS CONTROLLER regs */
	for(i = 0; i < VGA_NUM_GC_REGS; i++)
	{
		outb(VGA_GC_INDEX, i);
		outb(VGA_GC_DATA, *regs);
		regs++;
	}
/* write ATTRIBUTE CONTROLLER regs */
	for(i = 0; i < VGA_NUM_AC_REGS; i++)
	{
		(void)inb(VGA_INSTAT_READ);
		outb(VGA_AC_INDEX, i);
		outb(VGA_AC_WRITE, *regs);
		regs++;
	}
/* lock 16-color palette and unblank display */
	inb(VGA_INSTAT_READ);
	outb(VGA_AC_INDEX, 0x20);
}

void update_cursor(void)
{
	uint16_t pos = terminal_row * VGA_WIDTH + terminal_column;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

//TODO: Implement reading scancodes larger than 1 byte
//to allow for cursor movement
void move_cursor(size_t direction_enum){
	switch(direction_enum){
		case CURSOR_UP :
			terminal_row -= (terminal_row == 0 ? 0 : 1);
			break;
		case CURSOR_RIGHT :
			terminal_column += (terminal_column == VGA_WIDTH - 1 ? 0 : 1);
			break;
		case CURSOR_DOWN :
			terminal_row += (terminal_row == VGA_HEIGHT - 1 ? 0 : 1);
			break;
		case CURSOR_LEFT :
			terminal_column -= (terminal_column == 0 ? 0 : 1);
			break;
	}
	update_cursor();	
}

void get_buffer(void){
	uint32_t loc;
	//Request GC 6
	outb(VGA_GC_INDEX, 6);
	loc = inb(VGA_GC_DATA);
	loc >>= 2;
	loc &= 0x03;
	switch(loc){
		case 0 :
		case 1 :
			loc = 0xA0000;
			break;
		case 2 :
			loc = 0xB0000;
			break;
		case 3 :
			loc = 0xB8000;
			break;
	}
	vga_buffer = (uint8_t*)loc;
}

void terminal_initialize(void) {
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	get_buffer();
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			vga_buffer[index * 2] = ' ';
			vga_buffer[index * 2 + 1] = terminal_color;
		}
	}
	if(vga_buffer == 0xB8000){
		terminal_writestring("Buffer located at 0xB8000\n");
	}
}

//TODO: Save state between swaps
// void save_text(void){
// 	for(uint16_t y =  0; y < VIDEO_HEIGHT; y++){
// 		for(uint16_t x = 0; x < VIDEO_WIDTH; x++){
// 			terminal_contents[y * VIDEO_WIDTH + x] = vga_buffer[y * VIDEO_WIDTH + x];
// 		}
// 	}
// }

// void reinstate_text(void){
// 	for(uint16_t y =  0; y < VIDEO_HEIGHT; y++){
// 		for(uint16_t x = 0; x < VIDEO_WIDTH; x++){
// 			vga_buffer[y * VIDEO_WIDTH + x] = terminal_contents[y * VIDEO_WIDTH + x];
// 		}
// 	}
// }

void write_pixel(uint16_t x, uint16_t y, uint8_t c){
	uint8_t* pixelAddress = vga_buffer + y * VIDEO_WIDTH + x;
	*pixelAddress = c;
}

void clear_display(void){
	for(uint16_t y = 0; y < VIDEO_HEIGHT; y++){
		for(uint16_t x = 0; x < VIDEO_WIDTH; x++){
			write_pixel(x, y, 13);
		}
	}
}

void draw(void){
	for(uint16_t y = 0; y < VIDEO_HEIGHT; y++){
		for(uint16_t x = 0; x < VIDEO_WIDTH; x++){
			write_pixel(x, y, ((y + x) % 2 ? 0x00 : 0x0F));
		}
	}
}

void text_to_video(void){
	write_regs(_320x200x256);
	get_buffer();
}

void video_to_text(void){
	write_regs(_80x25_text);
	terminal_initialize();
}

void terminal_setcolor(uint8_t color) {
	terminal_color = color;
}
 
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	vga_buffer[index * 2] = c;
	vga_buffer[index * 2 + 1] = color;
}

void terminal_back(){
	if(!terminal_column){//beginning of row
		if(!terminal_row){ //Beginning of input
			return;//nothing to be done
		}
		terminal_row--;
		for(size_t i = VGA_WIDTH - 1; i; i--){
			if((uint8_t)(vga_buffer[(terminal_row * VGA_WIDTH + i) * 2]) > 0x20)
			{
				terminal_column = i + 1;
				break;
			}
		}
	}else
		terminal_column--;
	update_cursor();
	terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
}
 
void terminal_putchar(char c) {
	if(c == '\n') {
		terminal_row = (terminal_row + 1) % VGA_HEIGHT;
		terminal_column = 0;
		update_cursor();
		return;
	}

	terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
	update_cursor();
}

void terminal_write(const char* data, size_t size) {
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
	terminal_write(data, strlen(data));
}