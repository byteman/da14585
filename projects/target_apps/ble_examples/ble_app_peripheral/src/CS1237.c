
#include "CS1237.h"
#include "gpio.h"
#include "user_peripheral.h"
#include "user_periph_setup.h"
//#include "hal_drivers.h"
//#include "ioCC2541.h"

//CLK p1.1 pull up
//DIO p1.2 pull down 


#define MAX_CHAN_NUM 4

typedef struct {
	GPIO_PORT clk_port;
	GPIO_PIN  clk_pin;
	GPIO_PORT data_port;
	GPIO_PIN  data_pin;
}GPIOConfig;

static  void CS1237_WriteReg(GPIOConfig* config,uint8 adreg);

static GPIOConfig cs1237_gpio_cfg[MAX_CHAN_NUM];

static void Delay(int n)
{
  while(n--);
}
static void io_dir_in(GPIOConfig* config)
{
	GPIO_ConfigurePin(config->data_port,config->data_pin,INPUT,PID_GPIO,true);

   //P1DIR &= ~0x04;  //P1_1定义为输入 DIO
}
static void io_dir_out(GPIOConfig* config)
{
   //P1DIR |= 0x04;  //P1_1定义为输出 DIO
	GPIO_ConfigurePin(config->data_port,config->data_pin,INPUT,PID_GPIO,false);


}
static void clk_high(GPIOConfig* config)
{
	GPIO_SetActive(config->clk_port,config->clk_pin);
  //P1 |= 0x2;
}
static void clk_low(GPIOConfig* config)
{
	GPIO_SetInactive(config->clk_port,config->clk_pin);
  //P1 &= ~0x2;
}
static void data_high(GPIOConfig* config)
{
  //P1 |= 0x4;
	GPIO_SetActive(config->data_port,config->data_pin);
}
static void data_low(GPIOConfig* config)
{
  //P1 &= ~0x4;
	GPIO_SetInactive(config->data_port,config->data_pin);
}


static uint8 read_io(GPIOConfig* config)
{
	return GPIO_GetPinStatus(config->data_port,config->data_pin)?1:0;
  //return (P1&0x4)?1:0;
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
static void CS1237_Clock(GPIOConfig* config)
{
    clk_high(config);
    Delay(6);
    clk_low(config);
    Delay(6);
}

/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
static uint8 CS1237_ReadReg(GPIOConfig* config)
{
  uint8 i,command = 0x56;
  
  for(i = 0; i < 27; i++)//clk1-clk27
    CS1237_Clock(config);
  io_dir_out(config);//把 MCU_Data 管脚设为输出
  CS1237_Clock(config);//clk28
  CS1237_Clock(config);//clk29
  
  for(i = 0; i < 7; i ++) //clk30-clk36(发送读命令),7bits 数据
  {
    if(command & 0x40) //MSB -> LSB
      data_high(config);
    else
      data_low(config);
    command <<= 1;
    CS1237_Clock(config);
  }
  io_dir_in(config);//把 MCU_Data 管脚设为输入
  CS1237_Clock(config);//clk37
  command = 0;
  for(i = 0; i < 8; i++) //clk38-clk45(读取寄存器),8bits 数据
  {
    command <<= 1;
    CS1237_Clock(config);
    if(read_io(config))
      command++;
  }
  CS1237_Clock(config);//clk46
  return command;
}
/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
static void CS1237_WriteReg(GPIOConfig* config,uint8 adreg)
{
  uint8 i,command = 0x65;
  
  for(i = 0; i < 27; i++)//clk1-clk27
    CS1237_Clock(config);
  io_dir_out(config);//把 MCU_Data 管脚设为输出
  CS1237_Clock(config);//clk28
  CS1237_Clock(config);//clk29
  
  for(i = 0; i < 7; i ++) //clk30-clk36(发送读命令),7bits 数据
  {
    if(command & 0x40) //MSB -> LSB
      data_high(config);
    else
      data_low(config);
    command <<= 1;
    CS1237_Clock(config);
  }
  CS1237_Clock(config);//clk37
  
  for(i = 0; i < 8; i++) //clk38-clk45(写入寄存器),8bits 数据
  {
    if(adreg & 0x80) //MSB -> LSB
      data_high(config);
    else
      data_low(config);
    adreg <<= 1;
    CS1237_Clock(config);
  }
  io_dir_in(config);//把 MCU_Data 管脚设为输入
  CS1237_Clock(config);//clk46
}

/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
void CS1237_Init(AD_HZ hz)
{
  //GPIO_Init(CS1237_DIO_GPIO_PORT, CS1237_DIO_GPIO_PINS, GPIO_MODE_IN_PU_NO_IT); //pull up dio
  //GPIO_Init(CS1237_CLK_GPIO_PORT, CS1237_CLK_GPIO_PINS, GPIO_MODE_OUT_PP_LOW_FAST); //pull down  clk

	int i = 0;
  Delay(1000);
	
	cs1237_gpio_cfg[0].data_port = ADC1_DO_PORT;
	cs1237_gpio_cfg[0].data_pin = ADC1_DO_PIN;
	cs1237_gpio_cfg[0].clk_port = ADC1_CLK_PORT;
	cs1237_gpio_cfg[0].clk_pin = ADC1_CLK_PIN;
	
	cs1237_gpio_cfg[1].data_port = ADC2_DO_PORT;
	cs1237_gpio_cfg[1].data_pin = ADC2_DO_PIN;
	cs1237_gpio_cfg[1].clk_port = ADC2_CLK_PORT;
	cs1237_gpio_cfg[1].clk_pin = ADC2_CLK_PIN;
	
	cs1237_gpio_cfg[2].data_port = ADC3_DO_PORT;
	cs1237_gpio_cfg[2].data_pin = ADC3_DO_PIN;
	cs1237_gpio_cfg[2].clk_port = ADC3_CLK_PORT;
	cs1237_gpio_cfg[2].clk_pin = ADC3_CLK_PIN;
	
	cs1237_gpio_cfg[3].data_port = ADC4_DO_PORT;
	cs1237_gpio_cfg[3].data_pin = ADC4_DO_PIN;
	cs1237_gpio_cfg[3].clk_port = ADC4_CLK_PORT;
	cs1237_gpio_cfg[3].clk_pin = ADC4_CLK_PIN;
	
	
	
	for(; i < MAX_CHAN_NUM; i++)
	{
		
		CS1237_WriteReg(&cs1237_gpio_cfg[i],0x1C);        //40Hz,128PGA
	}
  
}

/******************************************************************************
  * @brief  .
  * @param  None
  * @retval 
******************************************************************************/
int8 CS1237_ReadAD(uint8 chan,int32* ad)
{
  uint8 i;
  int32 addat = 0;
  
	GPIOConfig* config = &cs1237_gpio_cfg[chan];
  if(read_io(config))
    return 0;
  // while(IO_Data_AD);
  for(i = 0; i < 24; i++) //发送 24 个 CLK，接收数据
  {
    addat <<= 1;
    CS1237_Clock(config);
    if(read_io(config))
      addat ++;
  }
  CS1237_Clock(config);
  CS1237_Clock(config);
  CS1237_Clock(config); //CLK27，拉高 DRDY
  addat <<= 8;
  addat >>= 14;
  *ad = addat;
  return 1;
}








