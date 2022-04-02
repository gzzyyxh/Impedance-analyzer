#include <AD9854.h>
#include <intrins.h>
#include <reg52.h>
#include <init.h>



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
sbit AD9854_RST   = P3^4;    		// ���ص�Դ��
sbit AD9854_UDCLK = P3^5;
sbit AD9854_SCLK  = P3^6;				// WR/SCLK
sbit AD9854_CS    = P3^7;				// RD/CS
sbit AD9854_FDATA = P3^2;       // fsk/bpsk/hold
sbit AD9854_OSK   = P3^1; 
sbit AD9854_SP    = P3^0;

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

