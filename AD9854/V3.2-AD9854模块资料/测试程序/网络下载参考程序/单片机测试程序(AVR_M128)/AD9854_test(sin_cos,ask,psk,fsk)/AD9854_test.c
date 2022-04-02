#include"AD9854_test.h"

static Uchar8 frq_data[6];
//��Ƶģʽ
static Ulong32 Frq_SINGAL=15000000;
static Uchar8 SINGLE_data[4]={0x10,0x47,0x00,0x40};
//ɨƵģʽ
static Ulong32 Frq_SCAN1=10000;
static Ulong32 Frq_SCAN2=100000;
static Uchar8 SCAN_data[4]={0x10,0x47,0x24,0x50};
static Uchar8 DFW_data[6]={0x00,0x00,0x00,0xff,0xff,0xff};		//����ɨƵ���� ��ֵԽ��ɨƵ����Խ��
static Uchar8 RAMP_data[3]={0xff,0xff,0xff};					//����ɨƵ���� ��ֵԽСɨƵ����Խ��
static Uchar8 Amp_data_1[2]={0xff,0xff};
static Uchar8 Amp_data_2[2]={0xff,0xff};
//����б��FSKģʽ
static Uchar8 UnFsk_data[4]={0x10,0x47,0x02,0x40};
//static Uchar8 Phase_data_1[2]={0x10,0x00};//��������Ҳ��
//ASK����ģʽ
/*********************************************
			������ε�ASK����ģʽ
*********************************************/
static Uchar8 ASK_data[4]={0x10,0x47,0x00,0x70};	//
static Ulong32 Frq_ASK=100000;
static Uchar8 FSK_Amp_1[2]={0xff,0xff};
static Uchar8 FSK_Amp_2[2]={0x00,0x00};
/**********************************************
			������ε�ASK����ģʽ
**********************************************/
static Uchar8 Wind_ASK_data[4]={0x14,0x47,0x01,0x60};	//
static Ulong32 Wind_Frq_ASK=30000;
static Uchar8 Wind_FSK_Amp_1[2]={0xff,0xff};
static Uchar8 Wind_FSK_Amp_2[2]={0x00,0x00};
//BPSK����ģʽ
static Uchar8 BPSK_data[4]={0x10,0x47,0x08,0x40};
static Uchar8 Phase_data_1[2]={0x10,0x00};
static Uchar8 Phase_data_2[2]={0x30,0x00};

/*----------------������ʱ-------------*/

void Delay_bus(Uchar8 i)
{
	while(i--)
	{
		asm("nop");asm("nop");	
	}
}

/*----------------��ͨ��ʱ--------------*/

void Delay(Uint16 t)
{
	Uint16 ii;
	while(t--)
	{
		for(ii=0;ii<10;ii++);
	}
}

/*---------------IO�ڳ�ʼ��-------------*/

void IO_init(void)
{
	ADDRESS_DDR |= 0xff;
	DATA_DDR |= 0xff;
	CONTRL_DDR |= 0xff;
	CONTRL_PORT |= 0xc0;
}

/*--------------Updata���º���----------*/

void Updata_CLK(void)
{
	IO_UPDATA_LOW();
	Delay(1);
	IO_UPDATA_HIGH();
}

/*------------��λ���ų�ʼ������---------*/

void MASTER_RST_Init(void)
{
	MASTER_RST_HIGH();
	Delay(10);
	MASTER_RST_LOW();
}

/*----------------�ܵĳ�ʼ��------------*/

void Init_all(void)
{
	IO_init();
	MASTER_RST_Init();	
}

/*-------------д��һ��������------------*/

void WriteByte(Uchar8 address,Uchar8 data)
{
	WR_HIGH();
	ADDRESS_PORT = address;
	asm("nop");
	WR_LOW();
	DATA_PORT = data;
	asm("nop");
	WR_HIGH();
	asm("nop");
}

/*-----------Ƶ�ʿ����ּ��㺯��---------*/

void Count_frq(double frq)
{
	 frq_data[0]=((Ulong32)((frq*256)/280000000))&(0x0000000000ff);						  //��λ
	 frq_data[1]=((Ulong32)((frq*65536)/280000000))&(0x0000000000ff);
	 frq_data[2]=((Ulong32)((frq*16777216)/280000000))&(0x0000000000ff);
	 frq_data[3]=((Ulong32)((frq*4294967296)/280000000))&(0x0000000000ff);
	 frq_data[4]=((Ulong32)((frq*1099511627776)/280000000))&(0x0000000000ff);
	 frq_data[5]=((Ulong32)((frq*281474976710656)/280000000))&(0x0000000000ff);			  //��λ
}

/*------------����AD9854����ģʽ---------

void Set_AD9854(void)
{												
	WR_HIGH();								//bit0:(=1)->���ֲ��ֽ������ģʽ
	ADDRESS_PORT=0x1d;						//bit1:(=1)->I/Q ��·DAC�������ģʽ
	asm("nop");								//bit2:(=1)->QDAC�������ģʽ���ر�QDAC����
	WR_LOW();								//bit3:���ֵ͵�ƽ���������Ϊ1��AD9854��ֹͣ����ֱ���ٴθ�λ��
	DATA_PORT=0x10;							//bit4:(Powr Downģʽ����λ) (=1)->ʹ�ܵ���ģʽ,�ر��ڲ��Ƚ���  (=0)->����ģʽ
	asm("nop");								//bit5~7:don't care
	WR_HIGH();															
	asm("nop");

	//��Ƶ7����ʱ��Ϊ280MHz
	WR_HIGH();
	ADDRESS_PORT=0x1e;						//bit0~bit4:�ڲ�ʱ�ӱ�Ƶ�����ã�4-20����ѡ��
	asm("nop");								//bit5:��·PLLʹ��λ��(=1)->��(�ⲿʱ��)REFCLKֱ������
	WR_LOW();								//bit6:VCO�������λ  (=1)->ϵͳʱ�Ӹ���200MHZ    (=0)->ϵͳʱ�ӵ���200MHZ
	DATA_PORT=0x47;							//bit7:don't care								
	asm("nop");
	WR_HIGH();							
	asm("nop");
	
	//����Updata�ź����ⲿ���룬����ģʽΪ��Ƶģʽ
	WR_HIGH();
	ADDRESS_PORT=0x1f;						//bit0:�ڲ�ˢ��Ƶ�ʴ����ź�ѡ��˿�(=0)->���ⲿPIN20�����ش�����(=1)->���ڲ�ʱ��ˢ��
	asm("nop");								//bit1~3:ѡ��AD9854����ģʽ(000->�����ڵ�Ƶ��ģʽ)
	WR_LOW();								//bit4:DACȡֵͨ��ѡ��λ��(=0)->���������û���оƬIO���룻(=1)->������Ϊ�ڲ��Ĵ���0x26��0x27������
	DATA_PORT=0x00;							//bit5:ɨƵ����ʹ��λ��(=1)->����F1��F2Ƶ��֮��ɨƵ���������������9854���빤����Ramp FSKģʽ
	asm("nop");								//bit6:(=1)->���ۼ���ACC��ACC2ȫ����λΪ0��ֱ����λΪ0��
	WR_HIGH();								//bit7:(=1)->ֻ���ۼ���ACC1��λΪ0������ֻ��CHIRP MODE�����ã�Ϊ�˷���Ļ�þ�ݲ�									
	asm("nop");	
											
	WR_HIGH();
	ADDRESS_PORT=0x20;						//bit0:������������
	asm("nop");								//bit1:���������������˳��		bit2~3:don't care
	WR_LOW();								//bit4:���ֳ˷���ȡֵѡ��λ  (=0)->���ڲ�I/Q��·�˷�����ַȡֵ��ַI(0x21~0x22)��ַQ(0x23~0x24)--(���ںϳɲ���)��(=1)->���ڲ����Կ����������
	DATA_PORT=0x40;							//bit5:���ֳ˷���ʹ��λ (=1)->ʹ�����ֳ˷��������ź��������(=0)->�ź�����߷������
	asm("nop");								//bit6:�ڲ�SINC�˲���ʹ��λ  =1����·�ڲ�sinc�˲��������͹��ġ��˲�����I/Q��·�źŶ���Ч����ʹ��Control DACʱû���˲����ܡ�
	WR_HIGH();								//bit7:don't care
	asm("nop");
}		*/


/*-----------AD9854��������Ĵ���---------*/

void AD9854_SetSpecile(Uchar8 *spec)
{
	Uchar8 i = 0 ;
	Uchar8 spec_addr = 0;

	spec_addr = 0x1d;

	for(i = 0;i < 4;i ++ )
	{
		WriteByte(spec_addr + i,spec[i]);	
	}
}

/*----------������λ�Ĵ���1������----------*/   

void AD9854_SetPhase1(Uchar8 *phase)   
{    
      
    WriteByte( 0x00 , phase[0] );
        
    WriteByte( 0x01 , phase[1] );   
    
} 
   
/*----------������λ�Ĵ���2������----------*/   

void AD9854_SetPhase2(Uchar8 *phase)   
{    
    Uchar8 strPhase = 0 ;

	strPhase = *phase;   
    WriteByte( 0x02 , strPhase );
     
    strPhase = *(phase + 1);   
    WriteByte( 0x03 , strPhase );   
    
} 

/*------------�������ǲ�Ƶ�ʿ�����---------*/  

void AD9854_SetDfw(Uchar8 *delta)   
{   
    Uchar8 i = 0 ;
	Uchar8 delta_addr = 0 ;   

    delta_addr = 0x10 ;
	   
    for( i = 0 ; i < 6 ; i ++ )   
    {   
        WriteByte( delta_addr + i , delta[i]);   
    }   
}  
  
/*--------------б�ʼĴ���������------------*/   

void AD9854_SetRamp(Uchar8 *ramp)   
{   
    Uchar8 i ;
	Uchar8 ramp_addr = 0 ;
	   
    ramp_addr = 0x1A;   
    
	for( i = 0 ; i < 3 ; i ++ )   
    {   		    
        WriteByte(ramp_addr + i, ramp[i]);         
    }   
}

/*----------����Ƶ�ʼĴ���1������--------*/
 
void AD9854_SetFreq1(Uchar8 *freq)   
{   
    Uchar8 i = 0 ;
	Uchar8 freq_addr = 0 ;   

    freq_addr = 0x04 ;
	   
    for( i = 0 ; i < 6 ; i ++ )   
    {   		  
        WriteByte( freq_addr + i , freq[i]);        
    }   
} 

/*----------����Ƶ�ʼĴ���2������--------*/

void AD9854_SetFreq2(Uchar8 *freq)   
{   
    Uchar8 i = 0 ;
	Uchar8 freq_addr = 0 ;   

    freq_addr = 0x0A ;
	   
    for( i = 0 ; i < 6 ; i ++ )   
    {   
        
        WriteByte( freq_addr + i , freq[i]);  
    }   
}
  
/*-------����I·�������Ĵ���������------*/    

void AD9854_SetOutkeyI(Uchar8 *Ikey)   
{   
    Uchar8 i ;
	Uchar8 OutkeyI_addr = 0;   
       
    OutkeyI_addr=0x21;     						//���I����Ĵ�����ַ   
    
	for( i = 0 ; i < 2 ; i ++ )   
    {   
        
		WriteByte(OutkeyI_addr + i,Ikey[i]);  
    }   
} 
  
/*-------����Q·�������Ĵ���������------*/ 

void AD9854_SetOutkeyQ(Uchar8 *Qkey)   
{   
    Uchar8 i ;
	Uchar8 OutkeyQ_addr = 0;   
    
	OutkeyQ_addr = 0x23;                      	//���Q����Ĵ�����ַ   
    
	for( i = 0 ; i < 2 ; i ++ )   
    {   
        
		WriteByte(OutkeyQ_addr + i,Qkey[i]);      
    }   
}  
  
/*----------������б�ʼĴ���������--------*/   

void AD9854_SetOutkeyRamp(Uchar8 Key_ramp)   
{   
    WriteByte(0x25, Key_ramp);     
} 

/*-------------����QDAC�Ĵ�����ֵ-----------*/   

void AD9854_SetQDAC(unsigned char *QDAC)   
{   
    
	WriteByte(0x26 , QDAC[0]);

	WriteByte(0x27 , QDAC[1]);
	
}
/*--------------��Ƶ��ģʽ����------------*/  
/******************************************
	��Ƶģʽ��(һ��Ĵ���0x20����Ϊ0x40)���
�Ĵ���0x20-���ֳ˷���ʹ��λѡ��Ϊ0�����ź���
��߷����������ô��Ƶģʽ�����ֻ��Ҫ����Ƶ
�ʿ�����1��Ƶ�ʼ��ɡ�
	��������Ĵ���0x20Ϊ0x60ʱ�����������ȼ�
�����ֱ������·����źŷ��ȡ�
	��������Ĵ���0x20Ϊ0x70ʱ��������һ����
�ȼĴ������ڿ���һ·����źŷ��ȣ���һ·����
�ϳ��źš�
******************************************/

void SINGLE_MODE(void)
{      
  	AD9854_SetSpecile(SINGLE_data);

	Count_frq(Frq_SINGAL);
	AD9854_SetFreq1(frq_data); 

	Updata_CLK();
} 

/*----------------ɨƵģʽ����-------------*/

void SCAN_MODE(void) 
{
	AD9854_SetSpecile(SCAN_data);

	AD9854_SetOutkeyI(Amp_data_1);	
	AD9854_SetOutkeyQ(Amp_data_2);

	Count_frq(Frq_SCAN1);
	AD9854_SetFreq1(frq_data);
	Count_frq(Frq_SCAN2);
	AD9854_SetFreq2(frq_data); 

	AD9854_SetDfw(DFW_data);
	AD9854_SetRamp(RAMP_data);

	Updata_CLK();
}

/*---------------����б��FSKģʽ------------*/
/*********************************************
	ͨ���ⲿFBHͬ���źţ�ʹ��ģ�����Ƶ����F1
��F2֮��ת������Ϊ2FSK���ơ�����ʱʾ����Ҫѡ��
���δ���ģʽ��
*********************************************/

void UNRAMPFSK_MODE(void)
{
	Count_frq(Frq_SCAN1);
	AD9854_SetFreq1(frq_data);
	Count_frq(Frq_SCAN2);
	AD9854_SetFreq2(frq_data);

	AD9854_SetSpecile(UnFsk_data);

	Updata_CLK();	 
}


/*-----------------ASK����ģʽ---------------*/
/*********************************************
			������ε�ASK����ģʽ
	����ģʽ�Ǹ���д���OSK RAMP rate��������
������0���ȵ������ȵ�ת��ʱ�䣬��������Сֻ��
��0x03,��С�Ͳ�������������Ρ�
*********************************************/

void ASK_MODE(void)
{
	Count_frq(Frq_ASK);
	AD9854_SetFreq1(frq_data);

	AD9854_SetOutkeyI(FSK_Amp_1);	
	AD9854_SetOutkeyQ(FSK_Amp_2);

	AD9854_SetOutkeyRamp(0x03);   				//0���ȵ������ȹ���ʱ�䣬����������ʣ�ԽСת����Խ�죬Խ��ת����Խ�� 
	
	AD9854_SetSpecile(ASK_data);

	Updata_CLK();
		
}

/**********************************************
			������ε�ASK����ģʽ
	����ģʽ����ͨ������ɨ�赥Ƭ���˿�����õ�
���źţ�Ȼ����ݶ˿ڵĵ�ƽ�ߵͣ���CHOISE����ѡ��
д���Ƶ�ʿ����֣����Ƿ����Ƶ�ʡ���ͨ�����ϵĶ�
AD9854д�����֣��������Ƿ����Ƶ����ʵ��ASK���ƣ�
	���ģʽ������ڲ�ʱ�ӣ����Զ����¿����֣���
���ٶȺܿ졣
**********************************************/

void Wind_ASK_MODE(void)
{
	Count_frq(Wind_Frq_ASK);
	AD9854_SetFreq1(frq_data);
	
	AD9854_SetSpecile(Wind_ASK_data);
	
}

/*-------------------ѡ�����----------------*/

void CHOISE(Uchar8 i)
{
	if(i==0)
	{
		AD9854_SetOutkeyI(Wind_FSK_Amp_2);	
		AD9854_SetOutkeyQ(Wind_FSK_Amp_2);
	}
	else
	{
		AD9854_SetOutkeyI(Wind_FSK_Amp_1);	
		AD9854_SetOutkeyQ(Wind_FSK_Amp_1);
	}
}
		
/*-----------------BPSK����ģʽ--------------*/

void BPSK_MODE(void)
{
	AD9854_SetSpecile(BPSK_data);

	Count_frq(Frq_SCAN1);
	AD9854_SetFreq1(frq_data);

	AD9854_SetPhase1(Phase_data_1);
	AD9854_SetPhase2(Phase_data_2);

	Updata_CLK();
}




