//-----------------------------------------------------------------------------
// 程序描述:   延时程序头文件:DELAY.h
// 创建人  :	雨中雪
// 编写时间:	2013年4月17日
// 版本号  :	V1.0
//-----------------------------------------------------------------------------
// 修改日期:
// 修改人  :
//	修改说明: 
//
//
//--------------------------------------------------------------

#ifndef _DELAY_H
#define _DELAY_H

//-----------------------------------------------------------------------------
// 数据类型宏定义
//-----------------------------------------------------------------------------
//#define uchar unsigned char
//#define uint  unsigned int

//-----------------------------------------------------------------------------
// 函数声明
//-----------------------------------------------------------------------------
extern void Delay_ns (unsigned char t);			// ns延时
extern void Delay_1us (unsigned char t);
extern void Delay_10us (unsigned char t);     // 延时时基:10us
extern void Delay_882us (void);					// 延时822us
extern void Delay_250us (unsigned char t);    // 延时时基:250us
extern void Delay_5ms (unsigned char t);		// 延时时基:5ms
extern void Delay_1ms (unsigned char t);		// 延时时基:1ms
extern void Delay_50ms (unsigned char t);		// 延时时基:50ms

#endif

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
