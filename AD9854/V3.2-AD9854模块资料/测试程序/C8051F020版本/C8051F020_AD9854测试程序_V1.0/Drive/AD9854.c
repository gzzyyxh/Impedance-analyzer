#include <c8051f020.h>
#include <intrins.h>
#include "AD9854.h"
#include "type.h" 

//**********************����Ϊϵͳʱ���Լ�����ر�������**************************/
/* 
  �˴������Լ�����Ҫ����ϵͳʱ���Լ�������ص����ӣ�һ������ֻ�迪��һ��   
  CLK_SetΪʱ�ӱ�Ƶ���ã�������4~20����Ƶ��������ܳ���300MHZ
  Freq_mult_ulong��Freq_mult_doulle��Ϊ2��48�η�����ϵͳʱ�ӣ�һ��Ϊ�����Σ�һ��Ϊ˫������
*/
//#define CLK_Set 7
//const ulong  Freq_mult_ulong  = 1340357;
//const double Freq_mult_doulle = 1340357.032;

//#define CLK_Set 9
//const ulong  Freq_mult_ulong  = 1042500;		 
//const double Freq_mult_doulle = 1042499.9137431;

//#define CLK_Set 8
//const ulong  Freq_mult_ulong  = 1172812;
//const double Freq_mult_doulle = 1172812.403;

#define CLK_Set 10
const ulong  Freq_mult_ulong  = 938250;
const double Freq_mult_doulle = 938249.9224;

uchar FreqWord[6];              //6���ֽ�Ƶ�ʿ�����

//**************************�޸�Ӳ��ʱҪ�޸ĵĲ���********************************/
 #define AD9854_DataBus P2
 #define AD9854_AdrBus  P1
 sbit AD9854_RST   = P0^4;    		//�����ź�����
 sbit AD9854_UDCLK = P0^5; 
 sbit AD9854_WR    = P0^6;
 sbit AD9854_RD    = P0^7;
 sbit AD9854_FDATA = P0^2;       //fsk/bpsk/hold
 sbit AD9854_OSK   = P0^1; 			//osk
 sbit AD9854_SP    = P0^0; 		


//====================================================================================
//��������:void AD9854_WR_Byte(uchar addr,uchar dat)
//��������:AD9854���п�д������
//��ڲ���:addr     6λ��ַ
//         dat      д�������
//���ڲ���:��
//====================================================================================
void AD9854_WR_Byte(uchar addr,uchar dat)
{
	AD9854_AdrBus  = (addr&0x3f) | (P2&0xc0);
	AD9854_DataBus = dat;
	AD9854_WR      = 0;
	AD9854_WR      = 1;	
}

//====================================================================================
//��������:void AD9854_Init(void)
//��������:AD9854��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_Init(void)
{
	AD9854_WR=1;//������д���ƶ˿���Ϊ��Ч
	AD9854_RD=1;
	AD9854_UDCLK=0;
	AD9854_RST=1;                 //��λAD9854
	AD9854_RST=0;

//	AD9854_WR_Byte(0x1d,0x10);	           //�رձȽ���
	AD9854_WR_Byte(0x1d,0x00);	           //�����Ƚ���
	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ            
	AD9854_WR_Byte(0x1f,0x00);	   //����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x20,0x60);	   //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

	AD9854_UDCLK=1;               //����AD9854���
  AD9854_UDCLK=0;
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
	ulong FreqBuf;
	ulong Temp=Freq_mult_ulong;   	       

	uchar Array_Freq[4];			     //������Ƶ�����ӷ�Ϊ�ĸ��ֽ�
	Array_Freq[0]=(uchar)Freq;
	Array_Freq[1]=(uchar)(Freq>>8);
	Array_Freq[2]=(uchar)(Freq>>16);
	Array_Freq[3]=(uchar)(Freq>>24);

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
//��������:void AD9854_SetSine(ulong Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~(1/2)*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) ,ȡֵԽ��,����Խ�� 
//���ڲ���:��
//====================================================================================
void AD9854_SetSine(ulong Freq,uint Shape)
{
	uchar count;
	uchar Adress;

	Adress = 0x04;                        //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

	Freq_convert(Freq);		           //Ƶ��ת��

	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
  {
		AD9854_WR_Byte(Adress++,FreqWord[--count]);
  }
	
	AD9854_WR_Byte(0x21,Shape>>8);	  //����Iͨ������
	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
	
	AD9854_WR_Byte(0x23,Shape>>8);	  //����Qͨ������
	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

	AD9854_UDCLK=1;                    //����AD9854���
  AD9854_UDCLK=0;
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
//	ulong Low32;
//	uint  High16;
//  double Temp=Freq_mult_doulle;   	            //23ca99Ϊ2��48�η�����120M
//	Freq*=(double)(Temp);
////	1 0000 0000 0000 0000 0000 0000 0000 0000 = 4294967295
//	High16 = (int)(Freq/4294967295);                  //2^32 = 4294967295
//	Freq -= (double)High16*4294967295;
//	Low32 = (ulong)Freq;

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
//void AD9854_SetSine_double(double Freq,uint Shape)
//{
//	uchar count=0;
//	uchar Adress;

//	Adress=0x04;						     //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

//	Freq_double_convert(Freq);		           //Ƶ��ת��
//	 
//	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress++,FreqWord[--count]);
//  }
//	
//	AD9854_WR_Byte(0x21,Shape>>8);	  //����Iͨ������
//	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	  //����Qͨ������
//	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                    //����AD9854���
//  AD9854_UDCLK=0;
//}

////====================================================================================
////��������:void AD9854_InitFSK(void)
////��������:AD9854��FSK��ʼ��
////��ڲ���:��
////���ڲ���:��
////====================================================================================
//void AD9854_InitFSK(void)
//{
//	AD9854_WR=1;                        //������д���ƶ˿���Ϊ��Ч
//  AD9854_RD=1;
//  AD9854_UDCLK=0;
//  AD9854_RST=1;                        //��λAD9854
//  AD9854_RST=0;	

//	AD9854_WR_Byte(0x1d,0x10);	       //�رձȽ���
//	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ
//	AD9854_WR_Byte(0x1f,0x02);	       //����ϵͳΪģʽ1�����ⲿ����
//	AD9854_WR_Byte(0x20,0x60);	      //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	AD9854_UDCLK=1;                  //����AD9854���
//  AD9854_UDCLK=0;
//}

////====================================================================================
////��������:void AD9854_SetFSK(ulong Freq1,ulong Freq2)
////��������:AD9854��FSK����
////��ڲ���:Freq1   FSKƵ��1   
////         Freq2   FSKƵ��2
////���ڲ���:��
////====================================================================================
//void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//{
//  uchar count=6;
//	uchar Adress1,Adress2;

//	const uint Shape=4000;	      //��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
//	
//	Adress1=0x04;				 //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ
//	Adress2=0x0a;				 //ѡ��Ƶ�ʿ�����2��ַ�ĳ�ֵ
//	
//	Freq_convert(Freq1);               //Ƶ��ת��1
//	
//	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
//  }
//	
//	Freq_convert(Freq2);               //Ƶ��ת��2

//	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
//  }

//	AD9854_WR_Byte(0x21,Shape>>8);	      //����Iͨ������
//	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	     //����Qͨ������
//	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                    //����AD9854���
//  AD9854_UDCLK=0;		
//}

////====================================================================================
////��������:void AD9854_InitBPSK(void)
////��������:AD9854��BPSK��ʼ��
////��ڲ���:��
////���ڲ���:��
////====================================================================================
//void AD9854_InitBPSK(void)
//{
//	AD9854_WR=1;                    //������д���ƶ˿���Ϊ��Ч
//  AD9854_RD=1;
//  AD9854_UDCLK=0;
//  AD9854_RST=1;                   //��λAD9854
//  AD9854_RST=0;

//	AD9854_WR_Byte(0x1d,0x10);	       //�رձȽ���
//	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ
//	AD9854_WR_Byte(0x1f,0x08);	      //����ϵͳΪģʽ4�����ⲿ����
//	AD9854_WR_Byte(0x20,0x60);	      //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	AD9854_UDCLK=1;                //����AD9854���
//  AD9854_UDCLK=0;
//}

////====================================================================================
////��������:void AD9854_SetBPSK(uint Phase1,uint Phase2)
////��������:AD9854��BPSK����
////��ڲ���:Phase1   ������λ1
////         Phase2	������λ2
////���ڲ���:��
////˵����   ��λΪ14Bit��ȡֵ��0~16383�����꽨�����ñ�������ʱ��Phase1����Ϊ0��
////         ��Phase1����Ϊ8192��180����λ
////====================================================================================
//void AD9854_SetBPSK(uint Phase1,uint Phase2)
//{
//	uchar count;

//	const ulong Freq=60000;
//  const uint Shape=4000;

//	uchar Adress;
//	Adress=0x04;                           //ѡ��Ƶ�ʿ�����1��ַ�ĳ�ֵ

//	AD9854_WR_Byte(0x00,Phase1>>8);	           //������λ1
//	AD9854_WR_Byte(0x01,(uchar)(Phase1&0xff));
//	
//	AD9854_WR_Byte(0x02,Phase2>>8);	          //������λ2
//	AD9854_WR_Byte(0x03,(uchar)(Phase2&0xff));

//	Freq_convert(Freq);                            //Ƶ��ת��

//	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress++,FreqWord[--count]);
//  }

//	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
//	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	               //����Qͨ������
//	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                                //����AD9854���
//  AD9854_UDCLK=0;	
//}

////====================================================================================
////��������:void AD9854_InitOSK(void)
////��������:AD9854��OSK��ʼ��
////��ڲ���:��
////���ڲ���:��
////====================================================================================
//void AD9854_InitOSK(void)
//{
//	AD9854_WR=1;                           //������д���ƶ˿���Ϊ��Ч
//	AD9854_RD=1;
//	AD9854_UDCLK=0;
//	AD9854_RST=1;                          //��λAD9854
//	AD9854_RST=0;

//  AD9854_WR_Byte(0x1d,0x10);	           //�رձȽ���
//	AD9854_WR_Byte(0x1e,CLK_Set);	       //����ϵͳʱ�ӱ�Ƶ
//	AD9854_WR_Byte(0x1f,0x00);	           //����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x20,0x70);	           //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,ͨ�������ڲ�����

//	AD9854_UDCLK=1;                        //����AD9854���
//	AD9854_UDCLK=0;
//}

////====================================================================================
////��������:void AD9854_SetOSK(uchar RateShape)
////��������:AD9854��OSK����
////��ڲ���: RateShape    OSKб��,ȡֵΪ4~255��С��4����Ч
////���ڲ���:��
////====================================================================================
//void AD9854_SetOSK(uchar RateShape)
//{
//	uchar count;

//	const ulong Freq=60000;			 //������Ƶ
//  const uint  Shape=4000;			//��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

//	uchar Adress;
//	Adress=0x04;               //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ

//	Freq_convert(Freq);                       //Ƶ��ת��

//	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress++,FreqWord[--count]);
//  }

//	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
//	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	                  //����Qͨ������
//	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));  	 


//  AD9854_WR_Byte(0x25,RateShape);				       //����OSKб��

//	AD9854_UDCLK=1;                                //����AD9854���
//  AD9854_UDCLK=0;	
//}

////====================================================================================
////��������:void AD9854_InitAM(void)
////��������:AD9854��AM��ʼ��
////��ڲ���:��
////���ڲ���:��
////====================================================================================
//void AD9854_InitAM(void)
//{
//	uchar count;

//	const ulong Freq=60000;			 //������Ƶ

//	uchar  Adress;
//	Adress=0x04;      //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ
//	
//  AD9854_WR=1;    //������д���ƶ˿���Ϊ��Ч
//  AD9854_RD=1;
//  AD9854_UDCLK=0;
//  AD9854_RST=1;     //��λAD9854
//  AD9854_RST=0;

//	AD9854_WR_Byte(0x1d,0x10);	                  //�رձȽ���
//	AD9854_WR_Byte(0x1e,CLK_Set);	             //����ϵͳʱ�ӱ�Ƶ
//	AD9854_WR_Byte(0x1f,0x00);	                 //����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x20,0x60);	                  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	Freq_convert(Freq);                            //Ƶ��ת��

//	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		AD9854_WR_Byte(Adress++,FreqWord[--count]);
//  }

//	AD9854_UDCLK=1;                             //����AD9854���
//  AD9854_UDCLK=0;
//}

////====================================================================================
////��������:void AD9854_SetAM(uchar Shape)
////��������:AD9854��AM����
////��ڲ���:Shape   12Bit����,ȡֵ��0~4095   
////���ڲ���:��
////====================================================================================
//void AD9854_SetAM(uint Shape)
//{
//	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
//	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	                  //����Qͨ������
//	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                                   //����AD9854���
//  AD9854_UDCLK=0;			
//}

////====================================================================================
////��������:void AD9854_InitRFSK(void)
////��������:AD9854��RFSK��ʼ��
////��ڲ���:��
////���ڲ���:��
////====================================================================================
//void AD9854_InitRFSK(void)
//{
//	AD9854_WR=1;                      //������д���ƶ˿���Ϊ��Ч
//  AD9854_RD=1;
//  AD9854_UDCLK=0;
//  AD9854_RST=1;                     //��λAD9854
//  AD9854_RST=0;
//	
//	AD9854_WR_Byte(0x1d,0x10);	       //�رձȽ���
//	AD9854_WR_Byte(0x1e,CLK_Set);	   //����ϵͳʱ�ӱ�Ƶ
//	AD9854_WR_Byte(0x1f,0x24);	        //����ϵͳΪģʽ2�����ⲿ����,ʹ�����ǲ�ɨƵ����
//	AD9854_WR_Byte(0x20,0x60);	        //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����	

//	AD9854_UDCLK=1;                   //����AD9854���
//  AD9854_UDCLK=0;
//}

////====================================================================================
////��������:void AD9854_SetRFSK(void)
////��������:AD9854��RFSK����
////��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
////         Freq_High         RFSK��Ƶ��	   48Bit
////         Freq_Up_Down		 ����Ƶ��	   48Bit
////		   FreRate           б��ʱ�ӿ���  20Bit
////���ڲ���:��
////ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
////         Ƶ�� ���������½� һ������Ƶ��
////====================================================================================
//void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate)
//{
//	uchar count=6;
//	uchar Adress1,Adress2,Adress3;
//  const uint  Shape=4000;			   //��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)

//	Adress1=0x04;		     //ѡ��Ƶ�ʿ����ֵ�ַ�ĳ�ֵ 
//	Adress2=0x0a;
//	Adress3=0x10;

//	Freq_convert(Freq_Low);                             //Ƶ��1ת��

//	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
//	{
//		AD9854_WR_Byte(Adress1++,FreqWord[--count]);
//	}

//	Freq_convert(Freq_High);                             //Ƶ��2ת��

//	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
//	{
//		AD9854_WR_Byte(Adress2++,FreqWord[--count]);
//	}

//	Freq_convert(Freq_Up_Down);                             //����Ƶ��ת��

//	for(count=6;count>0;)	                               //д��6�ֽڵ�Ƶ�ʿ�����  
//	{
//		AD9854_WR_Byte(Adress3++,FreqWord[--count]);
//	}

//	AD9854_WR_Byte(0x1a,(uchar)((FreRate>>16)&0x0f));				   //����б������
//	AD9854_WR_Byte(0x1b,(uchar)(FreRate>>8));
//	AD9854_WR_Byte(0x1c,(uchar)FreRate);				    

//	AD9854_WR_Byte(0x21,Shape>>8);	                  //����Iͨ������
//	AD9854_WR_Byte(0x22,(uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(0x23,Shape>>8);	                  //����Qͨ������
//	AD9854_WR_Byte(0x24,(uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                                //����AD9854���
//  AD9854_UDCLK=0;	
//}