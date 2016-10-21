
#include "LCD.h"

void set_direction_registers(void)
{
   DDRB = 0xFF;   // All pins are outputs				- 1111 1111
   DDRD = 0xFF;   // All pins are outputs				- 1111 1111
   DDRF |= 0xF1;  // Bits 0, 4, 5, 6 and 7 are outputs	- 1111 xxx1
}

void LCD_Write_Bus(uint8_t VH,uint8_t VL)
{
   // Set the Data on the pins
   LCD_HDATA_PORT = VH;
   LCD_LDATA_PORT = VL;
   pulse_low(LCD_WR_PORT, LCD_WR);
}

void LCD_Write_COM(uint8_t VL)
{
	cbi(LCD_RS_PORT, LCD_RS);
	LCD_Write_Bus(0x00,VL);
}

void LCD_Write_DATA_HL(uint8_t VH, uint8_t VL)
{
   sbi(LCD_RS_PORT, LCD_RS);
   LCD_Write_Bus(VH, VL);
}

void LCD_Write_DATA(uint8_t VL)
{
   sbi(LCD_RS_PORT, LCD_RS);
   LCD_Write_Bus(0x00,VL);
}

void LCD_Write_COM_DATA(uint8_t com1, uint16_t dat1)
{
     LCD_Write_COM(com1);
     LCD_Write_DATA_HL(dat1>>8,dat1);
}

void UTFT_init()
{
   disp_x_size =			271;  // max width
	disp_y_size =			479;  // max height
	display_transfer_mode =	16;   // transfer mode (16 bits, not latched)
	// display_model =			model;   // TFT01_32WD

   set_direction_registers();
}

void UTFT_initLCD(uint8_t orientationation)
{
	orientationation = orientationation;
   inverted = 0;

   // We need to set the RD line, this is active low
   sbi(LCD_RD_PORT, LCD_RD);

   // Reset the LCD
	sbi(LCD_RST_PORT, LCD_RST);
	_delay_ms(5);
	cbi(LCD_RST_PORT, LCD_RST);
	_delay_ms(15);
	sbi(LCD_RST_PORT, LCD_RST);
	_delay_ms(15);

	cbi(LCD_CS_PORT, LCD_CS);

   #include "ssd1963/initlcd.h"

	sbi(LCD_CS_PORT, LCD_CS);

	setFrontColor(255, 255, 255);
	setBackColor(0, 0, 0);
	current_font.font=0;
	_transparent = 0;
   putChar_x = 0;
   putChar_y = 0;
}

void setFrontColor(uint8_t red, uint8_t green, uint8_t blue)
{
	fch=((red&248)|green>>5);
	fcl=((green&28)<<3|blue>>3);
}

void setBackColor(uint8_t red, uint8_t green, uint8_t blue)
{
	bch=((red&248)|green>>5);
	bcl=((green&28)<<3|blue>>3);
	_transparent = 0;
}

void setXY(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if (orientation==LANDSCAPE)
	{
		swap(uint16_t, x1, y1);
		swap(uint16_t, x2, y2)
		y1=disp_y_size-y1;
		y2=disp_y_size-y2;
		swap(uint16_t, y1, y2)
	}

	#include "ssd1963/setxy.h"
}

void clrXY()
{
	if (orientation==PORTRAIT)
		setXY(0,0,disp_x_size,disp_y_size);
	else
		setXY(0,0,disp_y_size,disp_x_size);
}

void clrScr()
{
	cbi(LCD_CS_PORT, LCD_CS);
	clrXY();
   sbi(LCD_RS_PORT, LCD_RS);
   fast_fill_16(0, 0, (uint32_t)((uint32_t)(disp_x_size+1)*(uint32_t)(disp_y_size+1)));
	sbi(LCD_CS_PORT, LCD_CS);
}

void fillScr(uint8_t r, uint8_t g, uint8_t b)
{
	uint16_t color = ((r&248)<<8 | (g&252)<<3 | (b&248)>>3);
	fillScreen(color);
}

void fillScreen(uint16_t color)
{
	uint8_t ch, cl;

	ch = (uint8_t)(color>>8);
	cl = (uint8_t)(color & 0xFF);

	cbi(LCD_CS_PORT, LCD_CS);
	clrXY();
   sbi(LCD_RS_PORT, LCD_RS);
   fast_fill_16(ch, cl, (uint32_t)((uint32_t)(disp_x_size+1)*(uint32_t)(disp_y_size+1)));
	sbi(LCD_CS_PORT, LCD_CS);
}

void fast_fill_16(uint8_t ch, uint8_t cl, uint32_t pix)
{
	uint32_t blocks;

   LCD_HDATA_PORT = ch;
   LCD_LDATA_PORT = cl;

	blocks = pix>>4;
	for (uint32_t i=0; i<blocks; i++)
	{
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
		pulse_low(LCD_WR_PORT, LCD_WR);
	}
	if ((pix % 16) != 0)
		for (uint32_t i=0; i<(pix % 16)+1; i++)
		{
			pulse_low(LCD_WR_PORT, LCD_WR);
		}
}

void setPixel(uint16_t color)
{
	LCD_Write_DATA_HL((color>>8),(color&0xFF));	// rrrrrggggggbbbbb
}

void drawPixel(uint16_t x, uint16_t y)
{
	cbi(LCD_CS_PORT, LCD_CS);
	setXY(x, y, x, y);
	setPixel((fch<<8)|fcl);
	sbi(LCD_CS_PORT, LCD_CS);
	clrXY();
}

void drawPixelWithColor(uint16_t x, uint16_t y, uint16_t color)
{
	cbi(LCD_CS_PORT, LCD_CS);
	setXY(x, y, x, y);
	setPixel(color);
	sbi(LCD_CS_PORT, LCD_CS);
	clrXY();
}

void setColor(uint16_t color)
{
	fch=(uint8_t)(color>>8);
	fcl=(uint8_t)(color & 0xFF);
}

void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if (y1==y2)
		drawHLine(x1, y1, x2-x1);
	else if (x1==x2)
		drawVLine(x1, y1, y2-y1);
	else
	{
		uint16_t	dx = (x2 > x1 ? x2 - x1 : x1 - x2);
		uint8_t xstep =  x2 > x1 ? 1 : -1;
		uint16_t	dy = (y2 > y1 ? y2 - y1 : y1 - y2);
		uint8_t ystep =  y2 > y1 ? 1 : -1;
		uint16_t col = x1, row = y1;

      cbi(LCD_CS_PORT, LCD_CS);
		if (dx < dy)
		{
			uint16_t t = - (dy >> 1);
			while (1)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA_HL (fch, fcl);
				if (row == y2)
					return;
				row += ystep;
				t += dx;
				if (t >= 0)
				{
					col += xstep;
					t   -= dy;
				}
			}
		}
		else
		{
			uint16_t t = - (dx >> 1);
			while (1)
			{
				setXY (col, row, col, row);
				LCD_Write_DATA_HL (fch, fcl);
				if (col == x2)
					return;
				col += xstep;
				t += dy;
				if (t >= 0)
				{
					row += ystep;
					t   -= dx;
				}
			}
		}
      sbi(LCD_CS_PORT, LCD_CS);
	}
	clrXY();
}

void drawHLine(uint16_t x, uint16_t y, uint16_t l)
{
	if (l<0)
	{
		l = -l;
		x -= l;
	}
	cbi(LCD_CS_PORT, LCD_CS);
	setXY(x, y, x+l, y);
   sbi(LCD_RS_PORT, LCD_RS);
   fast_fill_16(fch,fcl,l);
	sbi(LCD_CS_PORT, LCD_CS);
	clrXY();
}

void drawVLine(uint16_t x, uint16_t y, uint16_t l)
{
	if (l<0)
	{
		l = -l;
		y -= l;
	}
	cbi(LCD_CS_PORT, LCD_CS);
	setXY(x, y, x, y+l);
   sbi(LCD_RS_PORT, LCD_RS);
   fast_fill_16(fch,fcl,l);
	sbi(LCD_CS_PORT, LCD_CS);
	clrXY();
}

void setCharXY(uint16_t x, uint16_t y)
{
   putChar_x = x;
   putChar_y = y;
}

int putChar(char c, FILE *stream)
{
	uint8_t i,ch;
	uint16_t j;
	uint16_t temp;

	cbi(LCD_CS_PORT, LCD_CS);

	if (!_transparent)
	{
		if (orientation==PORTRAIT)
		{
			setXY(putChar_x,putChar_y,putChar_x+current_font.x_size-1,putChar_y+current_font.y_size-1);

			temp=((c-current_font.offset)*((current_font.x_size/8)*current_font.y_size))+4;
			for(j=0;j<((current_font.x_size/8)*current_font.y_size);j++)
			{
				ch=pgm_read_byte(&current_font.font[temp]);
				for(i=0;i<8;i++)
				{
					if((ch&(1<<(7-i)))!=0)
					{
						setPixel((fch<<8)|fcl);
					}
					else
					{
						setPixel((bch<<8)|bcl);
					}
				}
				temp++;
			}
		}
		else
		{
			temp=((c-current_font.offset)*((current_font.x_size/8)*current_font.y_size))+4;

			for(j=0;j<((current_font.x_size/8)*current_font.y_size);j+=(current_font.x_size/8))
			{
				setXY(putChar_x,putChar_y+(j/(current_font.x_size/8)),putChar_x+current_font.x_size-1,putChar_y+(j/(current_font.x_size/8)));
				for (int zz=(current_font.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&current_font.font[temp+zz]);
					for(i=0;i<8;i++)
					{
						if((ch&(1<<i))!=0)
						{
							setPixel((fch<<8)|fcl);
						}
						else
						{
							setPixel((bch<<8)|bcl);
						}
					}
				}
				temp+=(current_font.x_size/8);
			}
		}
	}
	else
	{
		temp=((c-current_font.offset)*((current_font.x_size/8)*current_font.y_size))+4;
		for(j=0;j<current_font.y_size;j++)
		{
			for (int zz=0; zz<(current_font.x_size/8); zz++)
			{
				ch=pgm_read_byte(&current_font.font[temp+zz]);
				for(i=0;i<8;i++)
				{
					setXY(putChar_x+i+(zz*8),putChar_y+j,putChar_x+i+(zz*8)+1,putChar_y+j+1);

					if((ch&(1<<(7-i)))!=0)
					{
						setPixel((fch<<8)|fcl);
					}
				}
			}
			temp+=(current_font.x_size/8);
		}
	}

	sbi(LCD_CS_PORT, LCD_CS);
	clrXY();
   putChar_x += (current_font.x_size);
   return 0;
}

void printChar(char c, uint16_t x, uint16_t y)
{
	uint8_t i,ch;
	uint16_t j;
	uint16_t temp;

	cbi(LCD_CS_PORT, LCD_CS);

	if (!_transparent)
	{
		if (orientation==PORTRAIT)
		{
			setXY(x,y,x+current_font.x_size-1,y+current_font.y_size-1);

			temp=((c-current_font.offset)*((current_font.x_size/8)*current_font.y_size))+4;
			for(j=0;j<((current_font.x_size/8)*current_font.y_size);j++)
			{
				ch=pgm_read_byte(&current_font.font[temp]);
				for(i=0;i<8;i++)
				{
					if((ch&(1<<(7-i)))!=0)
					{
						setPixel((fch<<8)|fcl);
					}
					else
					{
						setPixel((bch<<8)|bcl);
					}
				}
				temp++;
			}
		}
		else
		{
			temp=((c-current_font.offset)*((current_font.x_size/8)*current_font.y_size))+4;

			for(j=0;j<((current_font.x_size/8)*current_font.y_size);j+=(current_font.x_size/8))
			{
				setXY(x,y+(j/(current_font.x_size/8)),x+current_font.x_size-1,y+(j/(current_font.x_size/8)));
				for (int zz=(current_font.x_size/8)-1; zz>=0; zz--)
				{
					ch=pgm_read_byte(&current_font.font[temp+zz]);
					for(i=0;i<8;i++)
					{
						if((ch&(1<<i))!=0)
						{
							setPixel((fch<<8)|fcl);
						}
						else
						{
							setPixel((bch<<8)|bcl);
						}
					}
				}
				temp+=(current_font.x_size/8);
			}
		}
	}
	else
	{
		temp=((c-current_font.offset)*((current_font.x_size/8)*current_font.y_size))+4;
		for(j=0;j<current_font.y_size;j++)
		{
			for (int zz=0; zz<(current_font.x_size/8); zz++)
			{
				ch=pgm_read_byte(&current_font.font[temp+zz]);
				for(i=0;i<8;i++)
				{
					setXY(x+i+(zz*8),y+j,x+i+(zz*8)+1,y+j+1);

					if((ch&(1<<(7-i)))!=0)
					{
						setPixel((fch<<8)|fcl);
					}
				}
			}
			temp+=(current_font.x_size/8);
		}
	}

	sbi(LCD_CS_PORT, LCD_CS);
	clrXY();
}

void setFont(uint8_t *font)
{
	current_font.font=font;
	current_font.x_size=fontbyte(0);
	current_font.y_size=fontbyte(1);
	current_font.offset=fontbyte(2);
	current_font.numchars=fontbyte(3);
}

void print(char *st, uint16_t x, uint16_t y)
{
	uint16_t stl, i;

	stl = strlen(st);

	if (orientation==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*current_font.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*current_font.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*current_font.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*current_font.x_size))/2;
	}

	for (i=0; i<stl; i++)
   {
      printChar(*st++, x + (i*(current_font.x_size)), y);
      _delay_ms(1);
   }
}

void print_P(uint_farptr_t st, uint16_t x, uint16_t y)
{
	uint16_t stl, i;

	stl = strlen_PF(st);

	if (orientation==PORTRAIT)
	{
	if (x==RIGHT)
		x=(disp_x_size+1)-(stl*current_font.x_size);
	if (x==CENTER)
		x=((disp_x_size+1)-(stl*current_font.x_size))/2;
	}
	else
	{
	if (x==RIGHT)
		x=(disp_y_size+1)-(stl*current_font.x_size);
	if (x==CENTER)
		x=((disp_y_size+1)-(stl*current_font.x_size))/2;
	}

	for (i=0; i<stl; i++)
   {
      printChar(pgm_read_byte(st++), x + (i*(current_font.x_size)), y);
      _delay_ms(1);
   }
}

void invertScreen(void)
{
   uint8_t invertScreen = inverted ? 0x20 : 0x21;
   cbi(LCD_CS_PORT, LCD_CS);
   LCD_Write_COM(invertScreen);
	sbi(LCD_CS_PORT, LCD_CS);
   inverted = ~inverted;
}

void setScrollArea(uint16_t y, uint16_t height)
{
   uint16_t bfa=272-height-y;

   cbi(LCD_CS_PORT, LCD_CS);
   LCD_Write_COM(0x33);
   LCD_Write_DATA(y >> 8);
   LCD_Write_DATA(y & 0xff);
   LCD_Write_DATA(height >> 8);
   LCD_Write_DATA(height & 0xff);
   LCD_Write_DATA(bfa >> 8);
   LCD_Write_DATA(bfa & 0xff);
	sbi(LCD_CS_PORT, LCD_CS);
}

void setScrollPosition(uint16_t scrollPosition)
{
   // // pull into range

   // if(scrollPosition<0)
      // scrollPosition+=TPanelTraits::SHORT_SIDE;
   // else if(scrollPosition>=TPanelTraits::SHORT_SIDE)
      // scrollPosition-=TPanelTraits::SHORT_SIDE;

   // if(scrollPosition>0)
      // scrollPosition=TPanelTraits::SHORT_SIDE-scrollPosition;

   cbi(LCD_CS_PORT, LCD_CS);
   LCD_Write_COM(0x37);
   LCD_Write_DATA(scrollPosition >> 8);
   LCD_Write_DATA(scrollPosition & 0xff);
	sbi(LCD_CS_PORT, LCD_CS);

}

void printNumI(uint32_t num, uint16_t x, uint16_t y, uint16_t length, char filler)
{
	char buf[25];
	char st[27];
	uint8_t neg=0;
	uint16_t c=0, f=0;

	if (num==0)
	{
		if (length!=0)
		{
			for (c=0; c<(length-1); c++)
				st[c]=filler;
			st[c]=48;
			st[c+1]=0;
		}
		else
		{
			st[0]=48;
			st[1]=0;
		}
	}
	else
	{
		if (num<0)
		{
			neg=1;
			num=-num;
		}

		while (num>0)
		{
			buf[c]=48+(num % 10);
			c++;
			num=(num-(num % 10))/10;
		}
		buf[c]=0;

		if (neg)
		{
			st[0]=45;
		}

		if (length>(c+neg))
		{
			for (uint16_t i=0; i<(length-c-neg); i++)
			{
				st[i+neg]=filler;
				f++;
			}
		}

		for (uint16_t i=0; i<c; i++)
		{
			st[i+neg+f]=buf[c-i-1];
		}
		st[c+neg+f]=0;

	}

	print(st,x,y);
}