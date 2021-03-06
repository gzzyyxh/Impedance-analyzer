//-----------------------------------------------------------------
// 程序描述:
// 　　stm32_AD9854测试程序_V1.1
// 作　　者: 凌智电子
// 开始日期: 2015-07-15
// 完成日期: 2015-07-16
// 修改日期: 2015-07-16
// 当前版本: V1.1
// 历史版本:
// 　- V1.0: 基本的AD9854并行控制输出，液晶显示功能
// 　- V1.1：并行控制转串行控制
// 调试工具: 凌智STM32F103核心板V2.2、1602液晶屏、LZE_ST_LINK2
// 说　　明: 
//				(1) 调试使用的系统时钟频率Fsysclk=72MHz;
//     		(2) AD9854的数据传输方式:串行传输方式;
//				(3) AD9854_IO与凌智STM32F103核心板V2.2连接方式如下
//-----------------------------------------------------------------
//               IO 连接说明
//-----------------------------------------------------------------
//    AD9854         |      STM32
// PIV							 -->	 +5V
// GND							 -->	 GND
// A0/SDIO           -->   GPIOA_PIN0				
// A1/SDO            -->   GPIOA_PIN1			
// A2/IORST          -->   GPIOA_PIN2
// RD                -->   GPIOC_PIN8
// WR                -->   GPIOC_PIN9
// UDCLK             -->   GPIOC_PIN10
// RESET             -->   GPIOC_PIN11
// S/P               -->   GPIOC_PIN12
//-----------------------------------------------------------------
//				(4) 由于AD9854功耗高, 请保证AD9854模块的供电功率, 供电电源建议5V1A电源
//        (5) 本测试程序实现基本的AD9854控制输出，即1kHz正弦波输出。可通过修改代码(Fre值)修改输出频率
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "Delay.h"
#include "CharLCD.h"
#include "PeripheralInit.h"
#include "AD9854.h"

//-----------------------------------------------------------------
// 主程序
//-----------------------------------------------------------------
int main(void)
{
	u32 Fre;
	Delay_1ms(100);	
	PeripheralInit();
	GPIO_AD9854_Configuration();
	Delay_1ms(5);
	WrCLcdC(0x06);
	WriteString(1, 1,"  AD9854 TEST!  ");
	WriteString(2, 1,"Fre:         Hz");
	AD9854_Init ();
	
	Fre = 1000;
	if (Fre > 99999999)
	{
		Fre = 99999999;
	}
	WrCLcd_long_num (2,6,Fre);
	AD9854_SetSine ( Fre , 4095 );
	while (1)
	{
		;
	}
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
