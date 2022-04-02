//-----------------------------------------------------------------------------
// 程序描述:
//     调试不同时基的延时程序:ns,10us,250us,1ms,5ms,50ms
// 作    者: 薛小铃
// 开始日期: 2009-11-21
// 完成日期: 2009-11-21
// 修改日期:
// 版    本: V2.0:修改V1.0的格式和说明方式
// 调试工具: C8051F02xTBC
// 说    明: 
//     (1)调试使用的系统时钟频率Fsysclk=22.1184MHz;
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------------------
//#include <c8051f020.h>
#include <intrins.h>
#include "delay.h"


//-----------------------------------------------------------------------------
// 功能函数区
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// void Delay_ns (unsigned char t)
//-----------------------------------------------------------------------------
//
// 函数功能: 时基为ns的延时
// 例子提示: 
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
/*
  2008.01.11修改
  mov lcall djnz     last_djnz ret
  2T +4T   +3T*(t-1)+2T       +5T  =(3t+10)T
              	单位： 1SYSCLK  SYSCLK=11.0592MHz   
delay_ns(1):	延时        13            1 175ns
delay_ns(2):	延时        16            1 447ns
delay_ns(3):	延时        19            1 718ns
delay_ns(4):	延时        22            1 989ns     
delay_ns(5):	延时        25            2 261ns
delay_ns(6):	延时        28            2 532ns 
delay_ns(7):	延时        31            2 803ns
delay_ns(8):	延时        34            3 074ns
delay_ns(9):	延时        37            3 346ns
delay_ns(10):	延时        40            3 617ns     
delay_ns(20):	延时        70            6 330ns
delay_ns(50):	延时       160           11 447ns 
*/
//-----------------------------------------------------------------------------
void Delay_ns (unsigned char t)
{
	do {
		_nop_();						
	} while (--t); 
}

//-----------------------------------------------------------------------------
// void Delay_1us (unsigned char t)
//-----------------------------------------------------------------------------
// 函数功能: 时基为10us的延时
// 例子提示: 调用Delay_10us(20),得到200us延时
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
//-----------------------------------------------------------------------------
//void Delay_1us (unsigned char t)
//{
//	do {
//		_nop_();					//4ge
//		_nop_();
//		_nop_();
//		_nop_();
//	} while ( -- t);	
//
//}

//-----------------------------------------------------------------------------
// void Delay_10us (unsigned char t)
//-----------------------------------------------------------------------------
//
// 函数功能: 时基为10us的延时
// 例子提示: 调用Delay_10us(20),得到200us延时
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
/* 
2008.06.14编写
延时公式：令x=9,y=1,n=8 注：x=10效果可能会更好些
	{[4y+(n+3)xy+4]*t+10}T=(107t+10)T  (118t+10)T
*/
//-----------------------------------------------------------------------------
void Delay_10us (unsigned char t)
{
	unsigned char i,j;
   
	do {
		j = 1;	
		do {
			i = 9;//251;
			do {										// 9ge
			   _nop_();_nop_();_nop_();_nop_();
			   _nop_();_nop_();_nop_();_nop_();
			   _nop_();_nop_();//									
			   _nop_();_nop_();_nop_();_nop_();
			   _nop_();_nop_();_nop_();_nop_();
			   _nop_();_nop_();//									
         } while (--i);	
		} while (--j);		
	} while (--t); 
}	

//-----------------------------------------------------------------------------
// void Delay_250us (unsigned char t)
//-----------------------------------------------------------------------------
//
// 函数功能: 时基为250us的延时
// 例子提示: 调用Delay_250us(20),得到5ms延时
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
/* 
2008.06.14编写
延时公式：令x=250,y=1,n=8
	{[4y+(n+3)xy+4]*t+10}T=(2769t+10)T
*/
//-----------------------------------------------------------------------------
//void Delay_250us (unsigned char t)
//{
//	unsigned char i,j;
//   
//	do {
//		j = 1;	
//		do {
//			i = 250;//251;
//			do {										// 9ge
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();//									
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();//									
//         } while (--i);	
//		} while (--j);		
//	} while (--t); 
//}

//-----------------------------------------------------------------------------
// void Delay_882us (void)
//-----------------------------------------------------------------------------
//
// 函数功能: 延时882us
// 入口参数: 无
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无     
// 注意事项: 延时时间为876us,误差为6us
//-----------------------------------------------------------------------------
//void Delay_882us (void)			  			
//{	
//	unsigned char i,j;   
//	j = 85;
//	
//	do {
//		i = 10;
//		do {										// 9ge
//			_nop_();_nop_();_nop_();_nop_();
//			_nop_();_nop_();_nop_();_nop_();
//			_nop_();_nop_();//									
////			_nop_();_nop_();_nop_();_nop_();
////			_nop_();_nop_();_nop_();_nop_();
////			_nop_();_nop_();//									
//		} while (--i);	
//	} while (--j);			
//}

//-----------------------------------------------------------------------------
// void Delay_1ms (unsigned char t)
//-----------------------------------------------------------------------------
//
// 函数功能: 时基为1ms的延时
// 例子提示: 调用Delay_1ms(20),得到20ms延时
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
/* 
2008.01.11编写
令x=251,y=4,n=8,则延时时间
	       ={[4y+(n+3)xy+4]*t+10}T=(11064t+10)T
              	单位:    1SYSCLK  SYSCLK=11.0592MHz   误差
delay_5ms(1):	延时      11 074      1.001ms          1us
delay_5ms(1):	延时      22 138      2.002ms          2us
delay_5ms(5):	延时      55 330      5.002ms          2us
delay_5ms(10):	延时     110 650     10.005ms          5us
delay_5ms(20):	延时     221 290     20.010ms         10us
delay_5ms(40):	延时     442 570     40.018ms         18us
delay_5ms(60):	延时     663 850     60.030ms         30us
delay_5ms(100):延时   1 106 410    100.044ms         44us
*/
//-----------------------------------------------------------------------------
//void Delay_1ms (unsigned char t)
//{
//	unsigned char i,j;
//   
//	do {
//		j = 4;	
//		do {
//			i = 251;
//			do {										// 9ge
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();//									
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();//									
//         } while (--i);
//		} while (--j);		
//	} while (--t); 
//}

//-----------------------------------------------------------------------------
// void Delay_5ms (unsigned char t)
//-----------------------------------------------------------------------------
//
// 函数功能: 时基为5ms的延时
// 例子提示: 调用Delay_5ms(20),得到100ms延时
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
/* 
令x=251,y=20,n=8,则延时时间
	       ={[4y+(n+3)xy+4]*t+10}T=(55304t+10)T
              	单位:    1SYSCLK  SYSCLK=11.0592MHz   误差
delay_5ms(1):	延时      55 314      5.002ms          2us
delay_5ms(2):	延时     110 618     10.002ms          2us
delay_5ms(5):	延时     276 530     25.005ms          5us
delay_5ms(10):	延时     553 050     50.008ms          8us
delay_5ms(20):	延时   1 106 090    100.015ms         15us
delay_5ms(40):	延时   2 212 170    200.030ms         30us
delay_5ms(60):	延时   3 318 250    300.044ms         44us
delay_5ms(100):延时   5 530 410    500.073ms         73us
*/
//-----------------------------------------------------------------------------
//void Delay_5ms (unsigned char t)
//{
//	unsigned char i,j;
//   
//	do {
//		j = 20;	
//		do {
//			i = 251;
//			do {										// 9ge
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();//									
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();//									
//         } while (--i);
//		} while (--j);		
//	} while (--t); 
//}

//-----------------------------------------------------------------------------
// void Delay_50ms (unsigned char t)
//-----------------------------------------------------------------------------
//
// 函数功能: 时基为50ms的延时
// 例子提示: 调用Delay_50ms(20),得到1s延时
// 入口参数: 无符号8bit整数
// 返回参数: 无
// 全局变量: 无
// 调用模块: 无
// 注意事项: 基于使用外22.1184MHz晶振的C8051F020单片机
/* 
令x=251,y=200,n=8,则延时时间
	       ={[4y+(n+3)xy+4]*t+10}T=(553004t+10)T
              	单位:    1SYSCLK  SYSCLK=11.0592MHz   误差
delay_50ms(1):	延时     553 014     50.005ms          5us
delay_50ms(2):	延时   1 106 018    100.009ms          9us
delay_50ms(5):	延时   2 765 030    250.021ms         21us
delay_50ms(10):延时   5 530 050    500.041ms         41us
delay_50ms(20):延时  11 060 090   1000.080ms         80us
delay_50ms(40):延时  22 120 170   2000.160ms        160us
delay_50ms(60):延时  33 180 250   3000.240ms        240us
delay_50ms(100):延时 55 300 410   5000.399ms        399us
*/
//-----------------------------------------------------------------------------
//void Delay_50ms (unsigned char t)
//{
//	unsigned char i,j;
//   
//	do {
//		j = 201;	
//		do {
//			i = 250;
//			do {										// 9ge
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();_nop_();_nop_();
//			   _nop_();_nop_();//									
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();_nop_();_nop_();
////			   _nop_();_nop_();//									
//         } while (--i);
//		} while (--j);		
//	} while (--t); 
//}


//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
