#include"public.h"
#include"AD9854_test.h"

int main(void)
{
	Init_all();

	DDRA=0xff;				//BPSK��FSK���������ASK
	PORTA =0x00;

//	DDRA=0x00;				//ASKģʽ
//	PORTA |=0x01; 

//	SINGLE_MODE();			//��Ƶ��ģʽ
//	SCAN_MODE();			//ɨƵģʽ
	UNRAMPFSK_MODE();		//FSKģʽ
//	ASK_MODE();				//����Ƶ�������ASKģʽ
//	BPSK_MODE();			//BPSKģʽ
//	Wind_ASK_MODE();		//����Ƶ�������ASKģʽ

	while(1)
	{
	
		PORTA |=0x01;				//��PA0����һ������������BPSK��FSK���������ASK�ȵ���ģʽ
		Delay(10);		
		PORTA &=~0x01;
		Delay(10);
	
	/*	if((PINA & 0x01)==0)		//ASKģʽ
		{							//��ʾ������1KHZ��׼������Ϊ���Ʋ������뵽��Ƭ��������Ƶ�ʿ����ֵ�д��
			CHOISE(0);
		}
		else
		{
			CHOISE(1);
		}*/
	}

	return 0;
}
