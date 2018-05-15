#ifndef OLED_H
#define OLED_H

void LCD_Init(void);

/***************功能描述：显示6*8一组标准ASCII字符串	显示的坐标（x,y），y为页范围0～7****************/
void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);

/*******************功能描述：显示8*16一组标准ASCII字符串	 显示的坐标（x,y），y为页范围0～7****************/
void LCD_P8x16Str(unsigned char x, unsigned y,unsigned char ch[]);
	
/*****************功能描述：显示16*16点阵  显示的坐标（x,y），y为页范围0～7****************************/
void LCD_P16x16Ch(unsigned char x, unsigned char y,unsigned char  N);
	
void LCD_P8x16Ch(unsigned char x, unsigned char y,unsigned char  N);

void LCD_P10x8Ch(unsigned char x, unsigned char y,unsigned char  N);
void LCD_P16x16bmp(unsigned char x, unsigned char y,unsigned char  N);
void LCD_P16x32Str(unsigned char x, unsigned y,unsigned char ch[]);

void LCD_P16x16_ZH(unsigned char x, unsigned char y,unsigned char  N);
void LCD_P16x16_ZH_Arr(unsigned char x, unsigned char y,unsigned char  N[],unsigned char nr);
void LCD_P8x8_ZH_Arr(unsigned char x, unsigned char y,unsigned char  N[],unsigned char nr);
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif

