//-----------------------------------------------------------------
// 程序描述:
//     基于程序延时的不同延时时基的程序：us,ms
// 作    者: 凌智电子
// 开始日期: 2018-08-04
// 完成日期: 2018-08-04
// 当前版本: V1.0
// 历史版本:
//   - V1.0: 基于STM32的延时：us,ms
// 调试工具: 凌智STM32F429+CycloneIV电子系统设计开发板、LZE_ST_LINK2
// 说    明: 
//
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "delay.h"
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void delay_20ns(u32 nns)
//-----------------------------------------------------------------
// 
// 函数功能: 延时20ns
// 入口参数: u32 nns：要延时的ns数.	
// 返 回 值: 无
// 注意事项: 当nns=1时，理论上是延时20ns，但实际上是延时22ns，误差为10%。
//
//-----------------------------------------------------------------
void delay_20ns(u32 nns)
{
	do {
			;
	} while (--nns); 
}

//-----------------------------------------------------------------
// void delay_us(u32 nus)
//-----------------------------------------------------------------
// 
// 函数功能: 延时us
// 入口参数: u32 nus：要延时的us数.	
// 返 回 值: 无
// 注意事项: nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	
//
//-----------------------------------------------------------------
void delay_us(u32 nus)
{		
	u32 i;
	while(nus--)
	{
		i=FAC_us-2;
		while(i--);
	}
}

//-----------------------------------------------------------------
// void delay_ms(u16 nms)
//-----------------------------------------------------------------
// 
// 函数功能: 延时ms
// 入口参数: u32 nms：要延时的ms数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
		delay_us(1000);
}

static u32 fac_us=0;	// us延时倍乘数
	   
//-----------------------------------------------------------------
// void SysTick_clkconfig(u8 SYSCLK)
//-----------------------------------------------------------------
// 
// 函数功能: SysTick参数配置
// 入口参数: u8 SYSCLK：系统时钟频率
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void SysTick_clkconfig(u8 SYSCLK)
{
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);// SysTick频率为HCLK
	fac_us=SYSCLK;
}								    

//-----------------------------------------------------------------
// void SysTick_delay_us(u32 nus)
//-----------------------------------------------------------------
// 
// 函数功能: SysTick延时us
// 入口参数: u32 nus：要延时的us数.	
// 返 回 值: 无
// 注意事项: nus:0~190887435(最大值即2^32/fac_us@fac_us=22.5)	
//
//-----------------------------------------------------------------
void SysTick_delay_us(u32 nus)
{		
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;			// LOAD的值	    	 
	ticks=nus*fac_us; 						// 需要的节拍数 
	told=SysTick->VAL;        		// 刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)				// 这里注意一下SYSTICK是一个递减的计数器就可以了.
				tcnt+=told-tnow;	
			else 
				tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)
				break;						// 时间超过/等于要延迟的时间,则退出.
		}  
	};
}

//-----------------------------------------------------------------
// void SysTick_delay_ms(u16 nms)
//-----------------------------------------------------------------
// 
// 函数功能: SysTick延时ms
// 入口参数: u32 nms：要延时的ms数
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void SysTick_delay_ms(u16 nms)
{
	u32 i;
	for(i=0;i<nms;i++) 
		SysTick_delay_us(1000);
}

//-----------------------------------------------------------------
// End Of File
//----------------------------------------------------------------- 	 
