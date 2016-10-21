
#include <avr/io.h>
#include <avr/power.h>
// #include <stdio.h>
#include <util/delay.h>
#include <compat/deprecated.h>

#include "LCD.h"
#include "Touch.h"

const char text[] PROGMEM = "National Instruments";
const char text2[] PROGMEM = "rodrigo@volium.com";

// Create a stream to direct stdio.h calls to the right function (putChar)
static FILE lcd_str = FDEV_SETUP_STREAM (putChar, NULL, _FDEV_SETUP_WRITE);

uint16_t touch_x, touch_y;

int main(void)
{
   // This will divide the Main Clock by 2
   // clock_prescale_set(clock_div_2);

   // Set up LED on board and toggle twice
   sbi(DDRE,6);

   sbi(PORTE,6);
   _delay_ms(200);
   cbi(PORTE,6);
   _delay_ms(200);
   sbi(PORTE,6);
   _delay_ms(200);
   cbi(PORTE,6);

   orientation=LANDSCAPE;

   UTFT_init();

   UTFT_initLCD(orientation);
   setFrontColor(0, 0, 0);
   setBackColor(255, 255, 255);

   uint16_t colors[] = {
      VGA_BLACK,
      VGA_WHITE,
      VGA_RED,
      VGA_GREEN,
      VGA_BLUE,
      VGA_SILVER,
      VGA_GRAY,
      VGA_MAROON,
      VGA_YELLOW,
      VGA_OLIVE,
      VGA_LIME,
      VGA_AQUA,
      VGA_TEAL,
      VGA_NAVY,
      VGA_FUCHSIA,
      VGA_PURPLE,
   };

//    uint16_t color = 0;
//    while(1)
//    {
//       for (color = 0; color < sizeof(colors)/sizeof(colors[0]); color++)
//       {
//          fillScreen(colors[color]);
//          _delay_ms(2000);
//       }
//    }

   setFont(SmallFont);

   // Clear the screen and draw the frame
   clrScr();

   // Draw crosshairs
   setFrontColor(0, 0, 255);
   setBackColor(0, 0, 0);
   drawLine(239, 15, 239, 255);
   drawLine(1, 135, 478, 135);
   for (int i=9; i<470; i+=10)
      drawLine(i, 133, i, 138);
   for (int i=15; i<256; i+=10)
      drawLine(237, i, 241, i);

   // Draw sin-, cos- and tan-lines
   setFrontColor(0,255,255);
   print("Sin", 5, 15);
   for (int i=1; i<478; i++)
   {
      drawPixel(i,135+(sin(((i*1.13)*3.14)/180)*95));
   }

   setFrontColor(255,0,0);
   print("Cos", 5, 27);
   for (int i=1; i<478; i++)
   {
      drawPixel(i,135+(cos(((i*1.13)*3.14)/180)*95));
   }

   setFrontColor(255,255,0);
   print("Tan", 5, 39);
   for (int i=1; i<478; i++)
   {
      drawPixel(i,135+(tan(((i*1.13)*3.14)/180)));

   }

   // setFrontColor(255,0, 255);
   // print("Mansour's", 5, 51);
   // for (int i=1; i<478; i++)
   // {
      // drawPixel(i,135+((((i-240)*(i-240))/pow(2.71,(i-240)))));

   // }


  _delay_ms(2000);
  clrScr();

  // Draw crosshairs
   setFrontColor(0, 0, 255);
   setBackColor(0, 0, 0);
   drawLine(239, 15, 239, 255);
   drawLine(1, 135, 478, 135);
   for (int i=9; i<470; i+=10)
      drawLine(i, 133, i, 138);
   for (int i=15; i<256; i+=10)
      drawLine(237, i, 241, i);

   // Draw a moving sinewave
  int buf[478];
   int x=1;
   int y=1;
   for (int i=1; i<(478*20); i++)
   {
      x++;
      if (x==479)
         x=1;
      if (i>479)
      {
         if ((x==239)||(buf[x-1]==135))
            setFrontColor(0,0,255);
         else
            setFrontColor(0,0,0);

         drawPixel(x,buf[x-1]);
      }
      setFrontColor(255,0,0);
      y=135+(sin(((i*1.65)*3.14)/180)*(90-(i / 100)));
      drawPixel(x,y);
      buf[x-1]=y;
   }

  _delay_ms(2000);



   // InitTouch(orientation);

   // setFont(BigFont);

   // clrScr();

   // setFrontColor(0, 0, 0);
   // setBackColor(255, 255, 255);
   // fillScreen(VGA_WHITE);


   // fillScreen(VGA_WHITE);
   // print_P(text, 0, 0);

   // setFont(SmallFont);
   // setFrontColor(255, 0, 0);
   // print_P(text2, 0, 100);

   // setColor(VGA_BLUE);
   // setFont(arial_17ptBitmaps);
   // uint16_t number = 0;

   // setFrontColor(0, 0, 0);

   // while(1)
   // {
      // if (dataAvailable())
      // {
         // read();
         // touch_x=getX();
         // touch_y=getY();
         // setCharXY(200, 136);
         // fprintf_P(&lcd_str, PSTR("X  = %d"), touch_x);
         // setCharXY(200, 166);
         // fprintf_P(&lcd_str, PSTR("Y  = %d"), touch_y);
      // }
   // }


   // while(1)
   // {
      // for(number = 0; number < 65535; number++)
      // {
         // setCharXY(200, 136);
         // fprintf_P(&lcd_str, PSTR("0X%X"), number);
      // }
   // }



   // uint16_t fontIndex = 0;
   // for(fontIndex = 0; fontIndex < current_font.numchars; fontIndex++)
   // {
      // setCharXY(150, 120);
      // putChar(current_font.offset+fontIndex);
      // _delay_ms(500);
   // }

   while(1)
   {
   }

   // setScrollArea(0,272);
   // uint16_t scroll = 0;
   // while(1)
   // {
      // for(scroll = 0; scroll < 272; scroll++)
      // {
         // _delay_ms(5);
         // setScrollPosition(scroll);
         // _delay_ms(5);
      // }
   // }
}
