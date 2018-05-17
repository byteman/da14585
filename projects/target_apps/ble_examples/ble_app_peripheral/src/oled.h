#ifndef OLED_H
#define OLED_H

void LCD_Init(void);

/***************������������ʾ6*8һ���׼ASCII�ַ���	��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void LCD_P6x8Str(unsigned char x, unsigned char y,unsigned char ch[]);

/*******************������������ʾ8*16һ���׼ASCII�ַ���	 ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************/
void LCD_P8x16Str(unsigned char x, unsigned y,unsigned char ch[]);
	
/*****************������������ʾ16*16����  ��ʾ�����꣨x,y����yΪҳ��Χ0��7****************************/
void LCD_P16x16Ch(unsigned char x, unsigned char y,unsigned char  N);
	
void LCD_P8x16Ch(unsigned char x, unsigned char y,unsigned char  N);

void LCD_P10x8Ch(unsigned char x, unsigned char y,unsigned char  N);
void LCD_P16x16bmp(unsigned char x, unsigned char y,unsigned char  N);
void LCD_P16x32Str(unsigned char x, unsigned y,unsigned char ch[]);

void LCD_P16x16_ZH(unsigned char x, unsigned char y,unsigned char  N);
void LCD_P16x16_ZH_Arr(unsigned char x, unsigned char y,unsigned char  N[],unsigned char nr);
void LCD_P8x8_ZH_Arr(unsigned char x, unsigned char y,unsigned char  N[],unsigned char nr);
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

#endif

