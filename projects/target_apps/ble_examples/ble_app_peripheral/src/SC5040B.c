//
#include "SC5040B.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
#include "audio.h"
static void delay_ms(unsigned int z)
{
unsigned int x,y;
for(x=z; x>0; x--)
	for(y=1848; y>0; y--);
}

static void delay_us(int nof_us)
{
    while( nof_us-- ){
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
    }
}
void SC50X0B_SDA(unsigned char data)
{
	unsigned char i;
	GPIO_SetInactive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_ms(5);
	for(i=0;i < 8;i++)
	{
	//SDA = 1;
		GPIO_SetActive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	if(data & 0x01)
	{
	delay_us(1500);
	//SDA = 0;
		GPIO_SetInactive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_us(500);
	}else
	{
	delay_us(500);
	//SDA = 0;
	GPIO_SetInactive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_us(1500);
	}
	data >>= 1;
	}
	//SDA = 1;
	GPIO_SetActive(AUDIO_SDA_PORT,AUDIO_SDA_PIN);
	delay_ms(50);
}
/*

1	'1' 0x31
2	'2' 0x32
3	'3' 0x33
4	'4' 0x34
5	'5' 0x35
6	'6' 0x36
7	'7' 0x37
8	'8' 0x38
9	'9' 0x39

10	'0' 0x30 		
11	'��' '' 0x2E '.'
12	'ǧ��'  0x3B
13	'����'  0x3B
14	'����'  0x3B
15	'������'0x3B
16	'�Ͽ�'  0x3B
17	'����'  0x3B
18	'�ػ�'  0x3B
19	'������' 0x3B
20	'����������' 0x3B

"B"
*/
char codecs[] = {
	' ', //ռλ
	'1', //1 
	'2', //2
	'3', //3
	'4', //4
	'5', //5
	'6', //6
	'7', //7
	'8', //8
	'9', //9
	'0',  //10 
	'.', //11
	'k', //12 ǧ��
	'o', //13 ����
	'b', //14 ����
	'c', //15 ������ 
	'x', //16 �Ͽ�
	'z', //17 ����
	'p', //18 �ػ�
	'l', //19 ������
	's', //20 ����������
	0
};
uint8 sc5040b_play(char label)
{
	char pos = 1;
	
	while(codecs[pos] != 0){
		 if(codecs[pos] == label){
				SC50X0B_SDA(pos);
				return 0;
		 }
		 pos++;
	}
	return 1;
}
static audio_soc_t _sc5040;

void  sc5040b_init(void)
{
		_sc5040.play = sc5040b_play;
		audio_queue_init(&_sc5040);
}

