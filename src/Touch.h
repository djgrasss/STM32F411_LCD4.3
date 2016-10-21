
#include <avr/io.h>
// #include <avr/pgmspace.h>
// #include <stdio.h>
#include <stdint.h>
// #include <string.h>
// #include <util/delay.h>
#include <compat/deprecated.h>

// Macro to read a pin from a port
#define rbi(port, bit) ((port) & (1 << (bit)))

#define pulse_high(PORT, BIT) sbi(PORT, BIT); cbi(PORT, BIT);
#define pulse_low(PORT, BIT) cbi(PORT, BIT); sbi(PORT, BIT);

#define PORTRAIT        0
#define LANDSCAPE       1

#define PREC_LOW        1
#define PREC_MEDIUM     2
#define PREC_HI         3
#define PREC_EXTREME    4


// For Adafruit's ATMega32u4 Breakout Board
// we use the following ports:

// Port E6     ->    TOUCH_CLK
// Port F1     ->    TOUCH_CS
// Port C7     ->    TOUCH_DIN
// Port C6     ->    TOUCH_DOUT
// Port F6     ->    TOUCH_IRQ


#define TOUCH_CLK_DPORT    DDRE
#define TOUCH_CLK_PORT     PORTE
#define TOUCH_CLK          6

#define TOUCH_CS_DPORT     DDRF
#define TOUCH_CS_PORT      PORTF
#define TOUCH_CS           1

#define TOUCH_DIN_DPORT    DDRC
#define TOUCH_DIN_PORT     PORTC
#define TOUCH_DIN          7

#define TOUCH_DOUT_DPORT   DDRC
#define TOUCH_DOUT_PORT    PINC
#define TOUCH_DOUT         6

#define TOUCH_IRQ_DPORT    DDRF
#define TOUCH_IRQ_PORT     PINF
#define TOUCH_IRQ          6



// UTouchCD.h
// ----------
//
// Since there are slight deviations in all touch screens you should run a
// calibration on your display module. Run the UTouch_Calibration sketch
// that came with this library and follow the on-screen instructions to
// update this file.
//
// Remember that is you have multiple display modules they will probably
// require different calibration data so you should run the calibration
// every time you switch to another module.
// You can, of course, store calibration data for all your modules here
// and comment out the ones you dont need at the moment.
//

// These calibration settings works with my ITDB02-3.2S.
// They MIGHT work on your display module, but you should run the
// calibration sketch anyway.
#define CAL_X 0x00378F66UL
#define CAL_Y 0x03C34155UL
#define CAL_S 0x000EF13FUL




int16_t  TP_X ,TP_Y;

void touch_WriteData(uint8_t data);
uint16_t touch_ReadData(void);

void  InitTouch(uint8_t orientation);
void  read(void);
uint8_t  dataAvailable(void);
int16_t  getX(void);
int16_t  getY(void);
void  setPrecision(uint8_t precision);

uint16_t _default_orientation;
uint8_t  touch_orientation;
uint8_t  precision;
uint8_t  display_model;
uint16_t touch_disp_x_size, touch_disp_y_size, default_orientation;
uint16_t touch_x_left, touch_x_right, touch_y_top, touch_y_bottom;

void  touch_WriteData(uint8_t data);
uint16_t  touch_ReadData(void);
