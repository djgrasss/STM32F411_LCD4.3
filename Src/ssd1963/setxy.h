// case SSD1963_480:
	swap(uint16_t, x1, y1);
	swap(uint16_t, x2, y2);
	LCD_Write_COM(0x2a); 
  	LCD_Write_DATA(x1>>8);
  	LCD_Write_DATA(x1);
  	LCD_Write_DATA(x2>>8);
  	LCD_Write_DATA(x2);
	LCD_Write_COM(0x2b); 
  	LCD_Write_DATA(y1>>8);
  	LCD_Write_DATA(y1);
  	LCD_Write_DATA(y2>>8);
  	LCD_Write_DATA(y2);
	LCD_Write_COM(0x2c); 
	// break;
