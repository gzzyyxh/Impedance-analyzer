//-----------------------------------------------------------------
// ��������:
// ����AD9854��������
// ��������: ���ǵ���
// ��ʼ����: 2019-05-12
// �������: 2019-05-12
// �޸�����: 2019-05-12
// ��ǰ�汾: V2.1
// ��ʷ�汾:
// ��- V2.1: ͨ�ŷ�ʽ�����п��ƣ�LCDҺ����ʾ��ʾ��������Σ����Ҳ����������Ʋ���
// ���Թ���: ����STM3���Ŀ����塢1602Һ������LZE_ST_LINK2
// ˵������: 
//				
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include <stm32f10x.h>
#include "AD9854.h"
#include "Delay.h"
#include "tim3.h"

//uint16_t Sinx_data[256] = {
//	
//0x7FF,0x831,0x863,0x896,0x8C8,0x8FA,0x92B,0x95D,0x98E,0x9C0,0x9F1,0xA21,0xA51,0xA81,0xAB1,0xAE0,
//0xB0F,0xB3D,0xB6A,0xB98,0xBC4,0xBF0,0xC1C,0xC46,0xC71,0xC9A,0xCC3,0xCEB,0xD12,0xD38,0xD5E,0xD83,
//0xDA7,0xDCA,0xDEC,0xE0D,0xE2E,0xE4D,0xE6C,0xE89,0xEA5,0xEC1,0xEDB,0xEF5,0xF0D,0xF24,0xF3A,0xF4F,
//0xF63,0xF75,0xF87,0xF97,0xFA6,0xFB4,0xFC1,0xFCD,0xFD7,0xFE0,0xFE8,0xFEF,0xFF5,0xFF9,0xFFC,0xFFE,
//0xFFE,0xFFE,0xFFC,0xFF9,0xFF5,0xFEF,0xFE8,0xFE0,0xFD7,0xFCD,0xFC1,0xFB4,0xFA6,0xF97,0xF87,0xF75,
//0xF63,0xF4F,0xF3A,0xF24,0xF0D,0xEF5,0xEDB,0xEC1,0xEA5,0xE89,0xE6C,0xE4D,0xE2E,0xE0D,0xDEC,0xDCA,
//0xDA7,0xD83,0xD5E,0xD38,0xD12,0xCEB,0xCC3,0xC9A,0xC71,0xC46,0xC1C,0xBF0,0xBC4,0xB98,0xB6A,0xB3D,
//0xB0F,0xAE0,0xAB1,0xA81,0xA51,0xA21,0x9F1,0x9C0,0x98E,0x95D,0x92B,0x8FA,0x8C8,0x896,0x863,0x831,
//0x7FF,0x7CD,0x79B,0x768,0x736,0x704,0x6D3,0x6A1,0x670,0x63E,0x60D,0x5DD,0x5AD,0x57D,0x54D,0x51E,
//0x4EF,0x4C1,0x494,0x466,0x43A,0x40E,0x3E2,0x3B8,0x38D,0x364,0x33B,0x313,0x2EC,0x2C6,0x2A0,0x27B,
//0x257,0x234,0x212,0x1F1,0x1D0,0x1B1,0x192,0x175,0x159,0x13D,0x123,0x109,0x0F1,0x0DA,0x0C4,0x0AF,
//0x09B,0x089,0x077,0x067,0x058,0x04A,0x03D,0x031,0x027,0x01E,0x016,0x00F,0x009,0x005,0x002,0x000,
//0x000,0x000,0x002,0x005,0x009,0x00F,0x016,0x01E,0x027,0x031,0x03D,0x04A,0x058,0x067,0x077,0x089,
//0x09B,0x0AF,0x0C4,0x0DA,0x0F1,0x10A,0x123,0x13D,0x159,0x175,0x192,0x1B1,0x1D0,0x1F1,0x212,0x234,
//0x257,0x27B,0x2A0,0x2C6,0x2EC,0x313,0x33B,0x364,0x38D,0x3B8,0x3E2,0x40E,0x43A,0x466,0x494,0x4C1,
//0x4EF,0x51E,0x54D,0x57D,0x5AD,0x5DD,0x60E,0x63E,0x670,0x6A1,0x6D3,0x704,0x736,0x768,0x79B,0x7CD

//};

//**********************����Ϊϵͳʱ���Լ�����ر�������**************************/
/* 
  �˴������Լ�����Ҫ����ϵͳʱ���Լ�������ص����ӣ�һ������ֻ�迪��һ��   
  CLK_SetΪʱ�ӱ�Ƶ���ã�������4~20����Ƶ��������ܳ���300MHZ
  Freq_mult_ulong��Freq_mult_doulle��Ϊ2��48�η�����ϵͳʱ�ӣ�һ��Ϊ�����Σ�һ��Ϊ˫������
*/
//#define CLK_Set 7
//const u32  Freq_mult_ulong  = 1340357;
//const double Freq_mult_doulle = 1340357.032;

//#define CLK_Set 9
//const u32  Freq_mult_ulong  = 1042500;		 
//const double Freq_mult_doulle = 1042499.9137431;

//#define CLK_Set 0x48
//const u32  Freq_mult_ulong  = 1172812;
//const double Freq_mult_doulle = 1172812.403;

 #define CLK_Set 0x4A
 const u32  Freq_mult_ulong  = 938250;
 const double Freq_mult_doulle = 938249.9224;



u8 FreqWord[6];              //6���ֽ�Ƶ�ʿ�����
uint16_t add = 0;
uint16_t step_up = 0;
uint32_t fund_fre_buf;
	
uint16_t amp_buf;
uint8_t modu_buf;
//====================================================================================
// ��ʼ��������
//====================================================================================
//====================================================================================
// void GPIO_AD9854_Configuration(void)
// ��������: AD9854�������ú���
// ��ڲ���: ��
// ���ز���: ��
// ȫ�ֱ���: ��
// ����ģ��: RCC_APB2PeriphClockCmd(); GPIO_Init();
// ע������: ��
//====================================================================================
void GPIO_AD9854_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	// ʹ��IO��ʱ��
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC |
							RCC_APB2Periph_GPIOB, ENABLE);   
	// ��ַ�˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 |	GPIO_Pin_2 | GPIO_Pin_3 
								| GPIO_Pin_4 | GPIO_Pin_5 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;		// �������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	// ���ݶ˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 |	GPIO_Pin_10 | GPIO_Pin_11 
								| GPIO_Pin_12 | GPIO_Pin_13 |	GPIO_Pin_14 | GPIO_Pin_15; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;		// �������
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
	// ���ƶ˿�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  | GPIO_Pin_9  |	GPIO_Pin_10 | GPIO_Pin_11 
								| GPIO_Pin_12 | GPIO_Pin_13 |	GPIO_Pin_14; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;		// �������
	GPIO_Init(GPIOC, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; 
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;;		// �������
//	GPIO_Init(GPIOE, &GPIO_InitStructure);
	
}

//====================================================================================
// AD9854������
//====================================================================================
//====================================================================================
//��������:void AD9854_WR_Byte(uchar addr,uchar dat)
//��������:AD9854���п�д������
//��ڲ���:addr     6λ��ַ
//         dat      д�������														
//���ڲ���:��
//====================================================================================
void AD9854_WR_Byte(u8 addr,u8 dat)
{
	u16 Tmp;
	
	Tmp =  GPIO_ReadOutputData(GPIOA);
	GPIO_Write(GPIOA, (u16)(addr&0x3f) | (Tmp & 0xFFC0));		// ADDER  	
	Tmp =  GPIO_ReadOutputData(GPIOB);
 	GPIO_Write(GPIOB, (u16)(dat<<8) | (Tmp & 0x00FF));		  // DATA
	AD9854_WR_Clr;
	AD9854_WR_Set;
}



//====================================================================================
//��������:void Freq_convert(long Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   ���㷨λ���ֽ�����㷨���й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK
//         �õ����㷨������N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//====================================================================================
void Freq_convert(long Freq)   
{	
  u32 FreqBuf;
  u32 Temp=Freq_mult_ulong;   	       

	u8 Array_Freq[4];			     //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�
	Array_Freq[0]=(u8) Freq;
	Array_Freq[1]=(u8)(Freq>>8);
	Array_Freq[2]=(u8)(Freq>>16);
	Array_Freq[3]=(u8)(Freq>>24);

	FreqBuf=Temp*Array_Freq[0];                  
	FreqWord[0]=FreqBuf;    
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[1]);
	FreqWord[1]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[2]);
	FreqWord[2]=FreqBuf;
	FreqBuf>>=8;

	FreqBuf+=(Temp*Array_Freq[3]);
	FreqWord[3]=FreqBuf;
	FreqBuf>>=8;

	FreqWord[4]=FreqBuf;
	FreqWord[5]=FreqBuf>>8;	
}  
//====================================================================================
//��������:void AD9854_Init(void)
//��������:AD9854��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_Init(void)
{
	AD9854_WR_Set;                 // ������д���ƶ˿���Ϊ��Ч
	AD9854_RD_Set;
	AD9854_SP_Set;

//	AD9854_UDCLK_Clr;
//// 	AD9854_RST_Set;                // ��λAD9854
	AD9854_RST_Clr;
	Delay_10us (10);
	AD9854_RST_Set;
	Delay_10us (10);
	AD9854_RST_Clr;

 	AD9854_WR_Byte(0x1d,0x00);	 	 // �����Ƚ���, ������ſ����������
//	AD9854_WR_Byte(0x1d,0x10);	 // �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	 // ����ϵͳʱ�ӱ�Ƶ            
	AD9854_WR_Byte(0x1f,0x01);	   // ����ϵͳΪģʽ0�����ڲ�����
	AD9854_WR_Byte(0x20,0x60);	   // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,�ر�SincЧӦ


}
//====================================================================================
//��������:void AD9854_SetSine(ulong Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~(1/2)*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) ,ȡֵԽ��,����Խ�� 
//���ڲ���:��
//====================================================================================
void AD9854_SetSine(u32 Freq,u16 Shape)
{
	u8 count;
	u8 Adress;

	Adress = 0x04;                      //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Freq_convert(Freq);		              //Ƶ��ת��

	for(count=6;count>0;)	              //д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
	
	AD9854_WR_Byte(0x21,(Shape>>8) & 0x00FF);		// ����Iͨ������
	AD9854_WR_Byte(0x22,(Shape&0x00ff));
	
	AD9854_WR_Byte(0x23,(Shape>>8) & 0x00FF);   // ����Qͨ������
	AD9854_WR_Byte(0x24,(Shape&0x00ff));
	
	
	AD9854_UDCLK_Set;                   				// ����AD9854���
	AD9854_UDCLK_Clr;

}

////====================================================================================
////��������:void Freq_doublt_convert(double Freq)
////��������:�����ź�Ƶ������ת��
////��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
////���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
////˵����   �й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK�õ��ú�����
////         ����N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
////         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
////ע�⣺   �ú��������溯��������Ϊ�ú�������ڲ���Ϊdouble����ʹ�źŵ�Ƶ�ʸ���ȷ
////         ���꽨����100HZ�����ñ��������ڸ���100HZ��������ú���void Freq_convert(long Freq)
////====================================================================================
//void Freq_double_convert(double Freq)   
//{
//	u32 Low32;
//	u32 High16;
//  double Temp=Freq_mult_doulle;   	            //23ca99Ϊ2��48�η�����120M
//	Freq*=(double)(Temp);
////	1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
//	High16 = (int)(Freq/4294967295);                  //2^32 = 4294967295
//	Freq -= (double)High16*4294967295;
//	Low32 = (u32)Freq;

//	FreqWord[0]=Low32;	     
//	FreqWord[1]=Low32>>8;
//	FreqWord[2]=Low32>>16;
//	FreqWord[3]=Low32>>24;
//	FreqWord[4]=High16;
//	FreqWord[5]=High16>>8;			
//} 

////====================================================================================
////��������:void AD9854_SetSine_double(double Freq,uint Shape)
////��������:AD9854���Ҳ���������
////��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~1/2*SYSCLK
////         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) 
////���ڲ���:��
////====================================================================================
//void AD9854_SetSine_double(double Freq,u16 Shape)
//{
//	u8 count=0;
//	u8 Adress;

//	Adress=0x04;						     //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

//	Freq_double_convert(Freq);		           //Ƶ��ת��
//	 
//	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress++,FreqWord[--count]);
//  }
//	
//	AD9854_WR_Byte(0x21,Shape>>8);	  //����Iͨ������
//	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	  //����Qͨ������
//	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

//	AD9854_UDCLK_Set;                    //����AD9854���
//  AD9854_UDCLK_Clr;
//}

//====================================================================================
//��������:void AD9854_InitFSK(void)
//��������:AD9854��FSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitFSK(void)
{

	AD9854_WR_Byte(0x1d,0x10);	     	// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   	// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x02);	     	// ����ϵͳΪģʽ1�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	    	// ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����


}

//====================================================================================
//��������:void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//��������:AD9854��FSK����
//��ڲ���:Freq1   FSKƵ��1   
//         Freq2   FSKƵ��2
//���ڲ���:��
//====================================================================================
void AD9854_SetFSK(u32 Freq1,u32 Freq2)
{
  u8 count=6;
	u8 Adress1,Adress2;

	const u16 Shape=4000;						// ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
	
	Adress1=0x04;				 // ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ
	Adress2=0x0a;				 // ѡ��Ƶ�ʿ�����2��ַ�ĳ�ֵ
	
	Freq_convert(Freq1);           	// Ƶ��ת��1
	
	for(count=6;count>0;)	          // д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
  }
	
	Freq_convert(Freq2);						// Ƶ��ת��2

	for(count=6;count>0;)	          // д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x21,Shape>>8);	     // ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	     // ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                    // ����AD9854���
  AD9854_UDCLK_Clr;		
}

//====================================================================================
//��������:void AD9854_InitBPSK(void)
//��������:AD9854��BPSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitBPSK(void)
{
	
	AD9854_WR_Byte(0x1d,0x10);				// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   	// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x08);	      // ����ϵͳΪģʽ4�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	      // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����


}

//====================================================================================
//��������:void AD9854_SetBPSK(uint Phase1,uint Phase2)
//��������:AD9854��BPSK����
//��ڲ���:Phase1   ������λ1
//         Phase2	������λ2
//���ڲ���:��
//˵����   ��λΪ14Bit��ȡֵ��0~16383�����꽨�����ñ�������ʱ��Phase1����Ϊ0��
//         ��Phase1����Ϊ8192��180����λ
//====================================================================================
void AD9854_SetBPSK(u16 Phase1,u16 Phase2)
{
	u8 count;

	const u32 Freq=60000;
  const u16 Shape=4000;

	u8 Adress;
	Adress=0x04;                           		// ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

	AD9854_WR_Byte(0x00,Phase1>>8);	       		// ������λ1
	AD9854_WR_Byte(0x01,(u8)(Phase1&0xff));
	
	AD9854_WR_Byte(0x02,Phase2>>8);	          // ������λ2
	AD9854_WR_Byte(0x03,(u8)(Phase2&0xff));

	Freq_convert(Freq);                     	// Ƶ��ת��

	for(count=6;count>0;)	                 		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x21,Shape>>8);	        	// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	        	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                      		// ����AD9854���
  AD9854_UDCLK_Clr;	
}

//====================================================================================
//��������:void AD9854_InitOSK(void)
//��������:AD9854��OSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitOSK(void)
{																				

  AD9854_WR_Byte(0x1d,0x10);	       			// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   				// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x00);	      			// ����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x70);	      			// ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,ͨ�������ڲ�����


}

//====================================================================================
//��������:void AD9854_SetOSK(uchar RateShape)
//��������:AD9854��OSK����
//��ڲ���: RateShape    OSKб��,ȡֵΪ4~255��С��4����Ч
//���ڲ���:��
//====================================================================================
void AD9854_SetOSK(u8 RateShape)
{
	u8 count;

	const u32 Freq=60000;				// ������Ƶ
  const u16  Shape=4000;			// ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	u8 Adress;
	Adress=0x04;            		// ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Freq_convert(Freq);                       		// Ƶ��ת��

	for(count=6;count>0;)	                    		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x21,Shape>>8);	            	// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	            	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));  	 


  AD9854_WR_Byte(0x25,RateShape);				    		// ����OSKб��

	AD9854_UDCLK_Set;                         		// ����AD9854���
  AD9854_UDCLK_Clr;	
}

//====================================================================================
//��������:void AD9854_InitAM(void)
//��������:AD9854��AM��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitAM(void)
{
	
	
	AD9854_WR_Byte(0x1d,0x10);	          			// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	             	// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x01);	              	// ����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	             		// ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����


}

//====================================================================================
//��������:void AD9854_SetAM(uchar Shape)
//��������:AD9854��AM����
//��ڲ���:Shape   12Bit����,ȡֵ��0~4095   
//���ڲ���:��
//====================================================================================
void AD9854_SetAM(u32 Freq,u16 Shape)
{
	u8 count;

//	 u32 Freq=60000;			 //������Ƶ

	u8  Adress;
	Adress=0x04;      				// ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ
	
		Freq_convert(Freq);                       		// Ƶ��ת��

	for(count=6;count>0;)	                      		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }
	
	AD9854_WR_Byte(0x21,Shape>>8);	             		// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	              	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                              	// ����AD9854���
  AD9854_UDCLK_Clr;			
}
//====================================================================================
//��������:void am_test (uint32_t fund_fre, uint8_t modulation)
//��������:AD9854��AM���εĵ���Ƶ�ʺ͵��ƶȵ�����
//��ڲ���:fund_fre   ����Ƶ�� ��0~100��
//				 modulation ���ƶ�   ��0~100��
//���ڲ���:��
//====================================================================================
void am_test (uint32_t fund_fre, uint8_t modulation)
{
	modu_buf = modulation;
	fund_fre_buf = fund_fre;	
	step_up = (float)fund_fre_buf * 65535 / 10000;
}


//====================================================================================
//��������:void AD9854_InitRFSK(void)
//��������:AD9854��RFSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitRFSK(void)
{


	AD9854_WR_Byte(0x1d,0x10);	        // �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	      // ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x24);	        // ����ϵͳΪģʽ2�����ⲿ����,ʹ�����ǲ�ɨƵ����
	AD9854_WR_Byte(0x20,0x60);	        // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,�ر�SincЧӦ	

}

//====================================================================================
//��������:void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate)
//��������:AD9854��RFSK����
//��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
//         Freq_High         RFSK��Ƶ��	   48Bit
//         Freq_Up_Down		 ����Ƶ��	   48Bit
//		     FreRate           б��ʱ�ӿ���  20Bit
//���ڲ���:��
//ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
//         Ƶ�� ���������½� һ������Ƶ��
//====================================================================================
void AD9854_SetRFSK(u32 Freq_Low,u32 Freq_High,u32 Freq_Up_Down,u32 FreRate)
{
	u8 count=6;
	u8 Adress1,Adress2,Adress3;
  const u16  Shape=3600;			  // ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	Adress1=0x04;		     					// ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ 
	Adress2=0x0a;
	Adress3=0x10;

	Freq_convert(Freq_Low);                   		// Ƶ��1ת��

	for(count=6;count>0;)	                    		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
  }

	Freq_convert(Freq_High);               				// Ƶ��2ת��

	for(count=6;count>0;)	                     		// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
  }

	Freq_convert(Freq_Up_Down);          					// ����Ƶ��ת��

	for(count=6;count>0;)	                				// д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress3++,FreqWord[--count]);
  }

	AD9854_WR_Byte(0x1a,(u8)((FreRate>>16)&0x0f));// ����б������
	AD9854_WR_Byte(0x1b,(u8)(FreRate>>8));
	AD9854_WR_Byte(0x1c,(u8)FreRate);				    

	AD9854_WR_Byte(0x21,Shape>>8);	        			// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	         			// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                           	// ����AD9854���
  AD9854_UDCLK_Clr;	
}
//====================================================================================
//��������:void  AD9854_InitChirp(void)
//��������:AD9854��Chirp��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_InitChirp(void)
{	
	AD9854_WR_Byte(0x1d,0x10);	  			// �رձȽ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   		// ����ϵͳʱ�ӱ�Ƶ
	AD9854_WR_Byte(0x1f,0x26);	        // ����ϵͳΪģʽ2�����ⲿ����,ʹ�����ǲ�ɨƵ����
	AD9854_WR_Byte(0x20,0x60);	        // ����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����	
}

//====================================================================================
//��������:void AD9854_SetChirp(unsigned long Freq_Low,unsigned long Freq_Up_Down,unsigned long FreRate)
//��������:AD9854��Chirp����
//��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
//         Freq_Up_Down		 ����Ƶ��	   48Bit
//		   FreRate           б��ʱ�ӿ���  20Bit
//���ڲ���:��
//ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
//         Ƶ�� ���������½� һ������Ƶ��
//====================================================================================
void AD9854_SetChirp(unsigned long Freq_Low,unsigned long Freq_Up_Down,unsigned long FreRate)
{
	u8 count=6;
	u8 Adress1,/*Adress2,*/Adress3;
  const u16  Shape=4000;			   // ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

	Adress1=0x04;		               // ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ 

	Adress3=0x10;

	Freq_convert(Freq_Low);                      		// Ƶ��1ת��

	for(count=6;count>0;)	                         	// д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
	}

	Freq_convert(Freq_Up_Down);            					// ����Ƶ��ת��

	for(count=6;count>0;)	                      		// д��6�ֽڵ�Ƶ�ʿ�����  
	{
		AD9854_WR_Byte(Adress3++,FreqWord[--count]);
	}

	AD9854_WR_Byte(0x1a,(u8)((FreRate>>16)&0x0f));	// ����б������
	AD9854_WR_Byte(0x1b,(u8)(FreRate>>8));
	AD9854_WR_Byte(0x1c,(u8)FreRate);				    

	AD9854_WR_Byte(0x21,Shape>>8);	               	// ����Iͨ������
	AD9854_WR_Byte(0x22,(u8)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	              	// ����Qͨ������
	AD9854_WR_Byte(0x24,(u8)(Shape&0xff));

	AD9854_UDCLK_Set;                            		// ����AD9854���
  AD9854_UDCLK_Clr;
}

//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
