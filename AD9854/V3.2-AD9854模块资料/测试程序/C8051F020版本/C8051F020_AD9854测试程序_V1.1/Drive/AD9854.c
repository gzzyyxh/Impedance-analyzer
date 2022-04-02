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
 // AD9854�Ĵ�����ַ
#define PHASE1	0x00	  //phase adjust register #1
#define PHASE2  0x01		//phase adjust register #2
#define FREQ1   0x02		//frequency tuning word 1
#define FREQ2   0x03		//frequency tuning word 2
#define DELFQ   0x04		//delta frequency word
#define UPDCK   0x05		//update clock
#define RAMPF   0x06		//ramp rate clock
#define CONTR   0x07		//control register
#define SHAPEI  0x08		//output shape key I mult
#define SHAPEQ  0x09		//output shape key Q mult 
#define RAMPO   0x0A		//output shape key ramp rate
#define CDAC    0x0B		//QDAC

// ����������
sbit AD9854_RST   = P0^4;    		// ���ص�Դ��
sbit AD9854_UDCLK = P0^5;
sbit AD9854_SCLK  = P0^6;				// WR/SCLK
sbit AD9854_CS    = P0^7;				// RD/CS
sbit AD9854_FDATA = P0^2;       // fsk/bpsk/hold
sbit AD9854_OSK   = P0^1; 
sbit AD9854_SP    = P0^0;

// I/O ����
sbit SDI          = P1^0;				// A0/SDIO
sbit SDO          = P1^1;				// A1/SDO 
sbit IO_RST       = P1^2;				// A2/IORST

// I/O����  ʹ�ñ���
#define SPI_IO_RST_Set IO_RST = 1
#define SPI_IO_RST_Clr IO_RST = 0
#define SPI_SDO_Set    SDO = 1
#define SPI_SDO_Clr    SDO = 0
#define SPI_SDI_Set 	 SDI = 1
#define SPI_SDI_Clr 	 SDI = 0

//====================================================================================
//��������:void AD9854_WR_Byte(uchar Adata)
//��������:AD9854���п�д������
//��ڲ���:Adata : 8λ��������
//���ڲ���:��
//====================================================================================
void AD9854_WR_Byte(uchar Adata)
{
	unsigned char  i;
	
	for(i=8;i>0;i--)
	{
		if(Adata&0x80)						
			SPI_SDI_Set;
		else
			SPI_SDI_Clr;
		Adata <<= 1;	  
		AD9854_SCLK  = 1;
		AD9854_SCLK  = 0;
	}
}

//====================================================================================
//��������:void AD9854_Init(void)
//��������:AD9854��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
void AD9854_Init(void)
{	
	AD9854_SP = 0;									//����ģʽѡ��
	AD9854_SCLK= 0;                 //������д���ƶ˿���Ϊ��Ч
	AD9854_UDCLK = 0;
	AD9854_RST = 1;                 //��λAD9854
	AD9854_RST = 0;
	IO_RST = 0;
	AD9854_CS= 0;
  
	AD9854_WR_Byte(CONTR);
	AD9854_WR_Byte(0x10);							//�رձȽ���
	AD9854_WR_Byte(CLK_Set);					//����ϵͳʱ�ӱ�Ƶ       
	AD9854_WR_Byte(0x00);							//����ϵͳΪģʽ0�����ⲿ����
	AD9854_WR_Byte(0x60);						  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,�ر�SincЧӦ

	AD9854_UDCLK=1;                   //����AD9854���
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
	uchar i=0;
	
	Freq_convert(Freq);		           //Ƶ��ת��
	
	for(count=6;count>0;)	              //д��6�ֽڵ�Ƶ�ʿ�����  
  {
		if(i==0)
			AD9854_WR_Byte(FREQ1);
		AD9854_WR_Byte(FreqWord[--count]);
		i++;
  }
	
	AD9854_WR_Byte(SHAPEI);						  //����Iͨ������
	AD9854_WR_Byte(Shape>>8);
	AD9854_WR_Byte((uchar)(Shape&0xff));
	
	AD9854_WR_Byte(SHAPEQ);							//����Qͨ������
	AD9854_WR_Byte(Shape>>8);
	AD9854_WR_Byte((uchar)(Shape&0xff));

	AD9854_UDCLK=1;                     //����AD9854���
  AD9854_UDCLK=0;
}

//====================================================================================
//��������:void Freq_doublt_convert(double Freq)
//��������:�����ź�Ƶ������ת��
//��ڲ���:Freq   ��Ҫת����Ƶ�ʣ�ȡֵ��0~SYSCLK/2
//���ڲ���:��   ����Ӱ��ȫ�ֱ���FreqWord[6]��ֵ
//˵����   �й�ʽFTW = (Desired Output Frequency �� 2N)/SYSCLK�õ��ú�����
//         ����N=48��Desired Output Frequency Ϊ����Ҫ��Ƶ�ʣ���Freq��SYSCLK
//         Ϊ�ɱ�̵�ϵͳʱ�ӣ�FTWΪ48Bit��Ƶ�ʿ����֣���FreqWord[6]
//ע�⣺   �ú��������溯��������Ϊ�ú�������ڲ���Ϊdouble����ʹ�źŵ�Ƶ�ʸ���ȷ
//         ���꽨����100HZ�����ñ��������ڸ���100HZ��������ú���void Freq_convert(long Freq)
//====================================================================================
//void Freq_double_convert(double Freq)   
//{
//	ulong Low32;
//	uint  High16;
//    double Temp=Freq_mult_doulle;   	            //23ca99Ϊ2��48�η�����120M
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

//====================================================================================
//��������:void AD9854_SetSine_double(double Freq,uint Shape)
//��������:AD9854���Ҳ���������
//��ڲ���:Freq   Ƶ�����ã�ȡֵ��ΧΪ0~1/2*SYSCLK
//         Shape  ��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095) 
//���ڲ���:��
//====================================================================================
//void AD9854_SetSine_double(double Freq,uint Shape)
//{
//	uchar count = 0;
//	uchar i = 0;
//	Freq_double_convert(Freq);		           //Ƶ��ת��
//	 
//	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
//	{	
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//	}
//	
//	AD9854_WR_Byte(SHAPEI);						  //����Iͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);							//����Qͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                    //����AD9854���
//  AD9854_UDCLK=0;
//}

//====================================================================================
//��������:void AD9854_InitFSK(void)
//��������:AD9854��FSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
//void AD9854_InitFSK(void)
//{
//	AD9854_SP = 0;									//����ģʽѡ��
//	AD9854_SCLK= 0;                 //������д���ƶ˿���Ϊ��Ч
//	AD9854_UDCLK = 0;
//	AD9854_RST = 1;                 //��λAD9854
//	AD9854_RST = 0;
//	IO_RST = 0;
//	AD9854_CS= 0;	

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//�رձȽ���
//	AD9854_WR_Byte(CLK_Set);					//����ϵͳʱ�ӱ�Ƶ       
//	AD9854_WR_Byte(0x02);							//����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x60);						  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	AD9854_UDCLK=1;                  //����AD9854���
//  AD9854_UDCLK=0;
//}

//====================================================================================
//��������:void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//��������:AD9854��FSK����
//��ڲ���:Freq1   FSKƵ��1   
//         Freq2   FSKƵ��2
//���ڲ���:��
//====================================================================================
//void AD9854_SetFSK(ulong Freq1,ulong Freq2)
//{
//  uchar count=6;
//	uchar i=0,j=0;

//	const uint Shape=4000;	        //��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
//	Freq_convert(Freq1);            //Ƶ��ת��1
//	
//	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }
//	
//	Freq_convert(Freq2);            //Ƶ��ת��2

//	for(count=6;count>0;)	          //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(j==0)
//			AD9854_WR_Byte(FREQ2);
//		AD9854_WR_Byte(FreqWord[--count]);
//		j++;
//  }

//	AD9854_WR_Byte(SHAPEI);						  //����Iͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);							//����Qͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                     //����AD9854���
//  AD9854_UDCLK=0;		
//}

//====================================================================================
//��������:void AD9854_InitBPSK(void)
//��������:AD9854��BPSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
//void AD9854_InitBPSK(void)
//{
//	AD9854_SP = 0;										//����ģʽѡ��
//	AD9854_SCLK= 0;                	 	//������д���ƶ˿���Ϊ��Ч
//	AD9854_UDCLK = 0;
//	AD9854_RST = 1;                 	//��λAD9854
//	AD9854_RST = 0;
//	IO_RST = 0;
//	AD9854_CS= 0;
//	  
//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//�رձȽ���
//	AD9854_WR_Byte(CLK_Set);					//����ϵͳʱ�ӱ�Ƶ       
//	AD9854_WR_Byte(0x08);							//����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x60);						  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	AD9854_UDCLK=1;                   //����AD9854���
//  AD9854_UDCLK=0;
//}

//====================================================================================
//��������:void AD9854_SetBPSK(uint Phasea,uint Phaseb)
//��������:AD9854��BPSK����
//��ڲ���:Phasea   ������λ1
//         Phaseb	������λ2
//���ڲ���:��
//˵����   ��λΪ14Bit��ȡֵ��0~16383�����꽨�����ñ�������ʱ��Phase1����Ϊ0��
//         ��Phase1����Ϊ8192��180����λ
//====================================================================================
//void AD9854_SetBPSK(uint Phasea,uint Phaseb)
//{
//	uchar count;
//	uchar i=0;
//	const ulong Freq=60000;
//  const uint Shape=4000;
//	
//	AD9854_WR_Byte(PHASE1);										//������λ1
//	AD9854_WR_Byte(Phasea>>8);
//	AD9854_WR_Byte((uchar)(Phasea&0xff));

//	AD9854_WR_Byte(PHASE2);										//������λ2
//	AD9854_WR_Byte(Phaseb>>8);
//	AD9854_WR_Byte((uchar)(Phaseb&0xff));
//	Freq_convert(Freq);                       //Ƶ��ת��

//	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	AD9854_WR_Byte(SHAPEI);						  			//����Iͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);										//����Qͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                          //����AD9854���
//  AD9854_UDCLK=0;	
//}

//====================================================================================
//��������:void AD9854_InitOSK(void)
//��������:AD9854��OSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
//void AD9854_InitOSK(void)
//{																				
//	AD9854_SP = 0;									  //����ģʽѡ��
//	AD9854_SCLK= 0;                   //������д���ƶ˿���Ϊ��Ч
//	AD9854_UDCLK = 0;
//	AD9854_RST = 1;                   //��λAD9854
//	AD9854_RST = 0;
//	IO_RST = 0;
//	AD9854_CS= 0;
//	
//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//�رձȽ���
//	AD9854_WR_Byte(CLK_Set);					//����ϵͳʱ�ӱ�Ƶ       
//	AD9854_WR_Byte(0x00);							//����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x70);						  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	AD9854_UDCLK=1;                   //����AD9854���
//	AD9854_UDCLK=0;
//}

//====================================================================================
//��������:void AD9854_SetOSK(uchar RateShape)
//��������:AD9854��OSK����
//��ڲ���: RateShape    OSKб��,ȡֵΪ4~255��С��4����Ч
//���ڲ���:��
//====================================================================================
//void AD9854_SetOSK(uchar RateShape)
//{
//	uchar count;
//	uchar i=0;
//	const ulong Freq = 60000;			 							//������Ƶ
//  const uint  Shape = 4000;										//��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
//	
//	Freq_convert(Freq);                       //Ƶ��ת��

//	for(count=6;count>0;)	                    //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	AD9854_WR_Byte(SHAPEI);						  		  //����Iͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);										//����Qͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));  	 
//       
//	AD9854_WR_Byte(RAMPO);										//����OSKб��
//	AD9854_WR_Byte(RateShape);
//	
//	AD9854_UDCLK=1;                     			//����AD9854���
//  AD9854_UDCLK=0;	
//}

//====================================================================================
//��������:void AD9854_InitAM(void)
//��������:AD9854��AM��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
//void AD9854_InitAM(void)
//{
//	uchar count;
//	uchar i=0;
//	const ulong Freq=60000;			 			//������Ƶ
//	
//	AD9854_SP = 0;									  //����ģʽѡ��
//	AD9854_SCLK= 0;                   //������д���ƶ˿���Ϊ��Ч
//	AD9854_UDCLK = 0;
//	AD9854_RST = 1;                   //��λAD9854
//	AD9854_RST = 0;
//	IO_RST = 0;
//	AD9854_CS= 0;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//�رձȽ���
//	AD9854_WR_Byte(CLK_Set);					//����ϵͳʱ�ӱ�Ƶ       
//	AD9854_WR_Byte(0x00);							//����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x60);						  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����

//	Freq_convert(Freq);                            //Ƶ��ת��

//	for(count=6;count>0;)	                         //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	AD9854_UDCLK=1;                             //����AD9854���
//  AD9854_UDCLK=0;
//}

//====================================================================================
//��������:void AD9854_SetAM(uchar Shape)
//��������:AD9854��AM����
//��ڲ���:Shape   12Bit����,ȡֵ��0~4095   
//���ڲ���:��
//====================================================================================
//void AD9854_SetAM(uint Shape)
//{
//	AD9854_WR_Byte(SHAPEI);						  				//����Iͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);											//����Qͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                             //����AD9854���
//  AD9854_UDCLK=0;			
//}

//====================================================================================
//��������:void AD9854_InitRFSK(void)
//��������:AD9854��RFSK��ʼ��
//��ڲ���:��
//���ڲ���:��
//====================================================================================
//void AD9854_InitRFSK(void)
//{
//	AD9854_SP = 0;										//����ģʽѡ��
//	AD9854_SCLK= 0;                	  //������д���ƶ˿���Ϊ��Ч
//	AD9854_UDCLK = 0;
//	AD9854_RST = 1;                	  //��λAD9854
//	AD9854_RST = 0;
//	IO_RST = 0;
//	AD9854_CS= 0;
//	AD9854_FDATA = 0;

//	AD9854_WR_Byte(CONTR);
//	AD9854_WR_Byte(0x10);							//�رձȽ���
//	AD9854_WR_Byte(CLK_Set);					//����ϵͳʱ�ӱ�Ƶ       
//	AD9854_WR_Byte(0x24);							//����ϵͳΪģʽ0�����ⲿ����
//	AD9854_WR_Byte(0x20);						  //����Ϊ�ɵ��ڷ��ȣ�ȡ����ֵ����,�ر�SincЧӦ

//	AD9854_UDCLK=1;                   //����AD9854���
//  AD9854_UDCLK=0;
//}

//====================================================================================
//��������:void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate)
//��������:AD9854��RFSK����
//��ڲ���:Freq_Low          RFSK��Ƶ��	   48Bit
//         Freq_High         RFSK��Ƶ��	   48Bit
//         Freq_Up_Down		 ����Ƶ��	   48Bit
//		     FreRate           б��ʱ�ӿ���  20Bit
//���ڲ���:��
//ע��     ÿ��������֮���ʱ����������ʽ��ʾ��FreRate +1��*��System Clock ����һ������,
//         Ƶ�� ���������½� һ������Ƶ��
//====================================================================================
//void AD9854_SetRFSK(ulong Freq_Low,ulong Freq_High,ulong Freq_Up_Down,ulong FreRate)
//{
//	uchar count=6;
//	uchar i=0,j=0,k=0;
//  const uint  Shape=3600;			   					//��������. Ϊ12 Bit,ȡֵ��ΧΪ(0~4095)
//	
//	Freq_convert(Freq_Low);                 //Ƶ��1ת��

//	for(count=6;count>0;)	                  //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(i==0)
//			AD9854_WR_Byte(FREQ1);
//		AD9854_WR_Byte(FreqWord[--count]);
//		i++;
//  }

//	Freq_convert(Freq_High);                //Ƶ��2ת��

//	for(count=6;count>0;)	                  //д��6�ֽڵ�Ƶ�ʿ�����  
// {
//		if(j==0)
//			AD9854_WR_Byte(FREQ2);
//		AD9854_WR_Byte(FreqWord[--count]);
//		j++;
//  }

//	Freq_convert(Freq_Up_Down);             //����Ƶ��ת��

//	for(count=6;count>0;)	                  //д��6�ֽڵ�Ƶ�ʿ�����  
//  {
//		if(k==0)
//			AD9854_WR_Byte(DELFQ);
//		AD9854_WR_Byte(FreqWord[--count]);
//		k++;
//  }
//	AD9854_WR_Byte(RAMPF);						  				//����б������
//	AD9854_WR_Byte((uchar)((FreRate>>16)&0x0f));
//	AD9854_WR_Byte((uchar)(FreRate>>8));
//	AD9854_WR_Byte((uchar)FreRate);				    

//	AD9854_WR_Byte(SHAPEI);						  				//����Iͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));
//	
//	AD9854_WR_Byte(SHAPEQ);											//����Qͨ������
//	AD9854_WR_Byte(Shape>>8);
//	AD9854_WR_Byte((uchar)(Shape&0xff));

//	AD9854_UDCLK=1;                             //����AD9854���
//  AD9854_UDCLK=0;	
//}