#define uchar unsigned char
#define ulong unsigned long
#define uint unsigned int
#define data_out P6OUT
#define data_in P6IN
#define data_dir P6DIR
#define add_out P8OUT
#define add_dir P8DIR
#define ctrl_out P1OUT
#define ctrl_dir P1DIR
#define UDCLK BIT6
#define WR BIT5
#define RD BIT4
#define FBH BIT3
#define SK BIT2
#define SP BIT1
#define RST BIT0
void send_byte(uchar add,uchar data);
uchar read_byte(uchar add);
uchar table9854[8];
//**********��ʼ��AD9854*********//
void init_9854()
{                           
  data_dir=0xFF;
  add_dir=0xff;
  ctrl_dir=0xff;
  ctrl_out=0x32;     //CP,UDCLK,F/B/H,SK,RST=0;WR,RD,S/P=1 0011 0010
  ctrl_out|=RST;  //һ��һ����ĸߵ�ƽ����
  _NOP();
  ctrl_out&=~RST;          //1FΪĬ��ֵ����Triangle=0��QDACΪ�ڲ����룬ģʽ0���ڲ�ˢ��ʱ��
  send_byte(REFCLK,0x04);     //PLLС��200MHz������·PLL��4��Ƶ  dc,pllrange,bypasspll,ref4  ref3,ref2,ref1,ref0             
  _NOP();_NOP();_NOP();_NOP();   //��ʱ4us���Ա�֤����д��
  send_byte(UDCLK_1,0x20);     //��ˢ�����ڸ�Ϊ(32+1)*(1/160M)*2=412.5ns����֤����1�ֽ����ݵ����ڴ���ˢ������ 
  _NOP();_NOP();  //��ʱ2us���Ա�֤����д��
  send_byte(PD,0x17);     //power down����ģ��
  send_byte(OSK_S,0x20);     //����·Inv Sinc��ʹ��OSK EN
  send_byte(PD,0x04);         //turn on I DAC and DIG,but still off Q DAC,PD: CR,DC,DC,COMP,  0,QDAC,DAC,DIG
}                               //DAC����IDAC�Ϳ��ˣ���1�͹�
//**********����1�ֽ�����************//
void send_byte(uchar add,uchar data)
{
  data_dir=0xff;  //��������IO��Ϊ���
  ctrl_out|=WR;    //����wr
  add_out=add;        //�����ڵ�ַA
  _NOP();   //��ʱ7ns����
  ctrl_out&=~WR;      //����WR
  data_out=data;      //������
  _NOP();  //��ʱ2.5ns����
  ctrl_out|=WR;       //����WR
  _NOP();  //��ʱһ��
}
//**********��ȡһ���ֽڵ�����**********//
uchar read_byte(uchar add)
{
  uchar dat;
  data_dir=0x00;  //��������IO��Ϊ����
  ctrl_out|=RD;  //����RD
  add_out=add;  //�����ڵ�ַ
  _NOP();  //��ʱ20ns����
  ctrl_out&=~RD; //����RD
  _NOP(); //��ʱ15ns����
  dat=data_in; //���������ݣ�
  return dat;
}
void ad9854f(double f) 
{
  table9854[0]=(uchar)((f*256)/160000000);//��λ
  table9854[1]=(uchar)(((f*256)/160000000-table9854[0])*256);//ȥ���������֣���ԭ�������������ٳ�256
  table9854[2]=(uchar)((((f*256)/160000000-table9854[0])*256-table9854[1])*256);
  table9854[3]=(uchar)(((((f*256)/160000000-table9854[0])*256-table9854[1])*256-table9854[2])*256);
  table9854[4]=(uchar)((((((f*256)/160000000-table9854[0])*256-table9854[1])*256-table9854[2])*256-table9854[3])*256);
  table9854[5]=(uchar)(((((((f*256)/160000000-table9854[0])*256-table9854[1])*256-table9854[2])*256-table9854[3])*256-table9854[4])*256);
  send_byte(FTW1_6,table9854[0]);     //M=f*2^48/160 �������MתΪ2����
  send_byte(FTW1_5,table9854[1]);   
  send_byte(FTW1_4,table9854[2]);
  send_byte(FTW1_3,table9854[3]);
  send_byte(FTW1_2,table9854[4]);
  send_byte(FTW1_1,table9854[5]);
}
void ad9854v(float v) 
{
  if(v>=1)
  {
    table9854[6]=0xff;
    table9854[7]=0xff; 
  }
  else
  {
    table9854[6]=((uint)(v*4096))/256;//��λ
    table9854[7]=((uint)(v*4096))%256;//��ԭ������ȡ��256
  }
  send_byte(OSKI_2,table9854[6]);    //0~11λ  2^12  1V  (x/4096)*1=K   x=4096k
  send_byte(OSKI_1,table9854[7]);   
  send_byte(OSKQ_2,table9854[6]);   
  send_byte(OSKQ_1,table9854[7]);
}