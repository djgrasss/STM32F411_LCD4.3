
#include "Touch.h"

void touch_WriteData(uint8_t data)
{
	uint8_t temp;

	temp=data;
	cbi(TOUCH_CLK_PORT, TOUCH_CLK);

	for(uint8_t count=0; count<8; count++)
	{
		if(temp & 0x80)
			sbi(TOUCH_DIN_PORT, TOUCH_DIN);
		else
			cbi(TOUCH_DIN_PORT, TOUCH_DIN);
		temp = temp << 1;
		cbi(TOUCH_CLK_PORT, TOUCH_CLK);
		sbi(TOUCH_CLK_PORT, TOUCH_CLK);
	}
}

uint16_t touch_ReadData(void)
{
	uint16_t data = 0;

	for(uint8_t count=0; count<12; count++)
	{
		data <<= 1;
		sbi(TOUCH_CLK_PORT, TOUCH_CLK);
		cbi(TOUCH_CLK_PORT, TOUCH_CLK);
		if (rbi(TOUCH_DOUT_PORT, TOUCH_DOUT))
			data++;
	}
	return(data);
}

void InitTouch(uint8_t orientation)
{
   // Port E6     ->    TOUCH_CLK
   // Port F1     ->    TOUCH_CS
   // Port C7     ->    TOUCH_DIN
   // Port C6     ->    TOUCH_DOUT
   // Port F6     ->    TOUCH_IRQ

   sbi(TOUCH_CLK_DPORT, TOUCH_CLK);    // Pin E6 (TOUCH_CLK) is ouput
   sbi(TOUCH_CS_DPORT, TOUCH_CS);      // Pin F1 (TOUCH_CS) is ouput
   sbi(TOUCH_DIN_DPORT, TOUCH_DIN);    // Pin C7 (TOUCH_DIN) is output
   cbi(TOUCH_DOUT_DPORT, TOUCH_DOUT);  // Pin C6 (TOUCH_DOUT) is input
   sbi(TOUCH_IRQ_DPORT, TOUCH_IRQ);    // Pin F6 (TOUCH_DIN) is output // original value


   touch_orientation               = orientation;
   _default_orientation = CAL_S>>31;
   touch_x_left         = (CAL_X>>14) & 0x3FFF;
   touch_x_right        = CAL_X & 0x3FFF;
   touch_y_top          = (CAL_Y>>14) & 0x3FFF;
   touch_y_bottom       = CAL_Y & 0x3FFF;
   touch_disp_x_size    = (CAL_S>>12) & 0x0FFF;
   touch_disp_y_size    = CAL_S & 0x0FFF;
   precision            = 10;
}

void read(void)
{
   uint16_t tx=0, temp_x=0;
   uint16_t ty=0, temp_y=0;
   uint32_t minx=99999, maxx=0;
   uint32_t miny=99999, maxy=0;
   int datacount=0;

   cbi(TOUCH_CS_PORT, TOUCH_CS);

   // pinMode(T_IRQ,  INPUT);
   cbi(TOUCH_IRQ_DPORT, TOUCH_IRQ); // Set direction of IRQ Pin to input
   for (int i=0; i<precision; i++)
   {
      if (!rbi(TOUCH_IRQ_PORT, TOUCH_IRQ)) // Read current IRQ value
      {
         touch_WriteData(0x90);
         pulse_high(TOUCH_CLK_PORT, TOUCH_CLK);
         temp_x=touch_ReadData();

         if (!rbi(TOUCH_IRQ_PORT, TOUCH_IRQ))
         {
            touch_WriteData(0xD0);
            pulse_high(TOUCH_CLK_PORT, TOUCH_CLK);
            temp_y=touch_ReadData();

            if ((temp_x>0) && (temp_x<4096) && (temp_y>0) && (temp_y<4096))
            {
               tx+=temp_x;
               ty+=temp_y;
               if (precision>5)
               {
                  if (temp_x<minx)
                     minx=temp_x;
                  if (temp_x>maxx)
                     maxx=temp_x;
                  if (temp_y<miny)
                     miny=temp_y;
                  if (temp_y>maxy)
                     maxy=temp_y;
               }
               datacount++;
            }
         }
      }
   }
   //pinMode(T_IRQ,  OUTPUT);
   sbi(TOUCH_IRQ_DPORT, TOUCH_IRQ); // Set direction of IRQ Pin to output

   if (precision>5)
   {
      tx = tx-(minx+maxx);
      ty = ty-(miny+maxy);
      datacount -= 2;
   }

   sbi(TOUCH_CS_PORT, TOUCH_CS);
   if ((datacount==(precision-2)) || (datacount==PREC_LOW))
   {
      if (touch_orientation == _default_orientation)
      {
         TP_X=ty/datacount;
         TP_Y=tx/datacount;
      }
      else
      {
         TP_X=tx/datacount;
         TP_Y=ty/datacount;
      }
   }
   else
   {
      TP_X=-1;
      TP_Y=-1;
   }
}

uint8_t dataAvailable(void)
{
   uint8_t avail;
   //pinMode(T_IRQ,  INPUT);
   cbi(TOUCH_IRQ_DPORT, TOUCH_IRQ); // Set direction of IRQ Pin to input

   avail = !(rbi(TOUCH_IRQ_PORT, TOUCH_IRQ));
   // pinMode(T_IRQ,  OUTPUT);
   sbi(TOUCH_IRQ_DPORT, TOUCH_IRQ); // Set direction of IRQ Pin to output
   return avail;
}

int16_t getX(void)
{
   long c;

   if ((TP_X==-1) || (TP_Y==-1))
      return -1;
   if (touch_orientation == _default_orientation)
   {
      c = (long)((long)(TP_X - touch_x_left) * (touch_disp_x_size)) / (long)(touch_x_right - touch_x_left);
      if (c<0)
         c = 0;
      if (c>touch_disp_x_size)
         c = touch_disp_x_size;
   }
   else
   {
      if (_default_orientation == PORTRAIT)
         c = (long)((long)(TP_X - touch_y_top) * (-touch_disp_y_size)) / (long)(touch_y_bottom - touch_y_top) + (long)(touch_disp_y_size);
      else
         c = (long)((long)(TP_X - touch_y_top) * (touch_disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
      if (c<0)
         c = 0;
      if (c>touch_disp_y_size)
         c = touch_disp_y_size;
   }
   return c;
}

int16_t getY(void)
{
   int c;

   if ((TP_X==-1) || (TP_Y==-1))
      return -1;
   if (touch_orientation == _default_orientation)
   {
      c = (long)((long)(TP_Y - touch_y_top) * (touch_disp_y_size)) / (long)(touch_y_bottom - touch_y_top);
      if (c<0)
         c = 0;
      if (c>touch_disp_y_size)
         c = touch_disp_y_size;
   }
   else
   {
      if (_default_orientation == PORTRAIT)
         c = (long)((long)(TP_Y - touch_x_left) * (touch_disp_x_size)) / (long)(touch_x_right - touch_x_left);
      else
         c = (long)((long)(TP_Y - touch_x_left) * (-touch_disp_x_size)) / (long)(touch_x_right - touch_x_left) + (long)(touch_disp_x_size);
      if (c<0)
         c = 0;
      if (c>touch_disp_x_size)
         c = touch_disp_x_size;
   }
   return c;
}

void setPrecision(uint8_t precision)
{
   switch (precision)
   {
      case PREC_LOW:
         precision=1;      // DO NOT CHANGE!
         break;
      case PREC_MEDIUM:
         precision=12;  // Iterations + 2
         break;
      case PREC_HI:
         precision=27;  // Iterations + 2
         break;
      case PREC_EXTREME:
         precision=102; // Iterations + 2
         break;
      default:
         precision=12;  // Iterations + 2
         break;
   }
}

void calibrateRead(void)
{
   uint16_t tx=0;
   uint16_t ty=0;

   cbi(TOUCH_CS_PORT, TOUCH_CS);

   touch_WriteData(0x90);
   pulse_high(TOUCH_CLK_PORT, TOUCH_CLK);
   tx=touch_ReadData();

   touch_WriteData(0xD0);
   pulse_high(TOUCH_CLK_PORT, TOUCH_CLK);
   ty=touch_ReadData();

   sbi(TOUCH_CS_PORT, TOUCH_CS);

   TP_X=ty;
   TP_Y=tx;
}

