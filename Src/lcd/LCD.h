
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <util/delay.h>
#include <compat/deprecated.h>


// For Adafruit's ATMega32u4 Breakout Board
// we use the following ports:

// Port B0:B7  ->    LCD_DB0:LCD_DB7
// Port D0:D7  ->    LCD_DB8:LCD_DB15

// Port F0     ->    LCD_RST
// Port F4     ->    LCD_RS
// Port F5     ->    LCD_WR
// Port F6     ->    LCD_RD
// Port F7     ->    LCD_CS


#define LCD_LDATA_PORT PORTB
#define LCD_HDATA_PORT PORTD

#define LCD_RST_PORT PORTF
#define LCD_RST   0

#define LCD_RS_PORT  PORTF
#define LCD_RS    4

#define LCD_WR_PORT  PORTF
#define LCD_WR    5

#define LCD_RD_PORT  PORTF
#define LCD_RD    6

#define LCD_CS_PORT  PORTF
#define LCD_CS    7

// Macros for setting and clearing ports

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define cport(port, data) port &= data
#define sport(port, data) port |= data

#define pulse_low(PORT, BIT) cbi(PORT, BIT); sbi(PORT, BIT);

#define fontbyte(x) pgm_read_byte(&current_font.font[x])

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

//*********************************
// COLORS
//*********************************
// VGA color palette
#define VGA_BLACK       0x0000
#define VGA_WHITE       0xFFFF
#define VGA_RED         0xF800
#define VGA_GREEN       0x0400
#define VGA_BLUE        0x001F
#define VGA_SILVER      0xC618
#define VGA_GRAY        0x8410
#define VGA_MAROON      0x8000
#define VGA_YELLOW      0xFFE0
#define VGA_OLIVE       0x8400
#define VGA_LIME        0x07E0
#define VGA_AQUA        0x07FF
#define VGA_TEAL        0x0410
#define VGA_NAVY        0x0010
#define VGA_FUCHSIA     0xF81F
#define VGA_PURPLE      0x8010
#define VGA_TRANSPARENT 0xFFFFFFFF

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t arial_17ptBitmaps[];

struct font_t
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

uint8_t  fch, fcl, bch, bcl;
uint8_t  orientation;
uint8_t  inverted;
uint16_t disp_x_size, disp_y_size;
uint8_t  display_model, display_transfer_mode, display_serial_mode;
uint8_t  _transparent;
struct font_t current_font;

// These variables are used with the putChar function, which is the one called
// through stdio calls (printf style)
uint16_t putChar_x, putChar_y;

void set_direction_registers(void);
void LCD_Write_DATA_HL(uint8_t VH,uint8_t VL);
void LCD_Write_DATA(uint8_t VL);
void UTFT_init(void);
void UTFT_initLCD(uint8_t orientation);
void setFrontColor(uint8_t red, uint8_t green, uint8_t blue);
void setBackColor(uint8_t red, uint8_t green, uint8_t blue);
void clrXY(void);
void clrScr(void);
void fillScr(uint8_t r, uint8_t g, uint8_t b);
void fillScreen(uint16_t color);
void fast_fill_16(uint8_t ch, uint8_t cl, uint32_t pix);
void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void setPixel(uint16_t color);
void drawPixel(uint16_t x, uint16_t y);
void drawPixelWithColor(uint16_t x, uint16_t y, uint16_t color);
void setColor(uint16_t color);
void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void drawHLine(uint16_t x, uint16_t y, uint16_t l);
void drawVLine(uint16_t x, uint16_t y, uint16_t l);
void setCharXY(uint16_t x, uint16_t y);
int putChar(char c, FILE *stream);
void printChar(char c, uint16_t x, uint16_t y);
void setFont(uint8_t *font);
void print(char *st, uint16_t x, uint16_t y);
void print_P(uint_farptr_t st, uint16_t x, uint16_t y);
void invertScreen(void);
void setScrollArea(uint16_t y, uint16_t height);
void setScrollPosition(uint16_t scrollPosition);
void printNumI(uint32_t num, uint16_t x, uint16_t y, uint16_t length, char filler);
