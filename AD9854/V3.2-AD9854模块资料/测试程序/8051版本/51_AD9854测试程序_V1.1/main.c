//-----------------------------------------------------------------
// 程序描述:
// 　　51_AD9854测试程序_V1.1
// 作　　者: 凌智电子
// 开始日期: 2017-06-1
// 完成日期: 2017-06-5
// 修改日期: 2017-06-5
// 当前版本: V1.1
// 历史版本:
// 　- V1.0: 基本的AD9854并行控制输出，液晶显示功能
// 　- V1.1：并行控制转串行控制
// 调试工具: 51开发板、1602液晶屏
// 说　　明: 
//				(1) 调试使用的系统时钟频率Fsysclk=12MHz;
//     		(2) AD9854的数据传输方式:并行传输方式;
//				(3) AD9854_IO与51开发板连接方式如30-39行代码提示连接
//        (4) 本测试程序实现基本的AD9854控制输出，即1MHz正弦波输出。可通过修改51行代码(Fre值)修改输出频率
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include <reg52.h>
#include <init.h>
#include <AD9854.h>
//-----------------------------------------------------------------
//               IO 连接说明
//-----------------------------------------------------------------
//    AD9854         |     51         
// A0/SDIO           -->   P1^0				
// A1/SDO            -->   P1^1			
// A2/IORST          -->   P1^2			
// RD                -->   P3^7
// WR                -->   P3^6
// UDCLK             -->   P3^5
// RESET             -->   P3^4
// S/P               -->   P3^0
// OSK               -->   P3^1
// FDSTA             -->   P3^2
//-----------------------------------------------------------------

void main()
{	
 	  ulong fre;
    init();	 
  	LCD_Clear();//清屏
	  LCD_Write_String(1, 0,"  AD9854 TEST!  ");
	  LCD_Write_String(1, 1,"Fre:         Hz");
    AD9854_Init();
     
		     fre = 1000000;
	  if (fre > 99999999)
		{
			fre = 99999999;
		}
		LCD_Write_String(6, 1,"1000000");
		AD9854_SetSine ( fre , 4095 );
	  while(1)
		{;}

	
}