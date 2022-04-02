#ifndef _AD9854_TEST_H_
#define _AD9854_TEST_H_

#include"public.h"

/*--------------��Ƶ��ģʽ����------------*/  
void SINGLE_MODE(void);
/*----------------��ͨ��ʱ--------------*/
void Delay(Uint16 t);
/*----------------������ʱ-------------*/
void Delay_bus(Uchar8 i);
/*----------------�ܵĳ�ʼ��------------*/
void Init_all(void);
/*----------------ɨƵģʽ����-------------*/
void SCAN_MODE(void);
/*---------------����б��FSKģʽ------------*/
void UNRAMPFSK_MODE(void); 
/*-----------------ASK����ģʽ---------------*/
/*********************************************
			������ε�ASK����ģʽ
*********************************************/
void ASK_MODE(void);
/**********************************************
			������ε�ASK����ģʽ
**********************************************/
void Wind_ASK_MODE(void);
/*-------------------ѡ�����----------------*/
void CHOISE(Uchar8 i);
/*-----------------BPSK����ģʽ--------------*/
void BPSK_MODE(void);

#define ADDRESS_DDR				DDRE
#define ADDRESS_PORT			PORTE
#define ADDRESS_PIN				PINE

#define DATA_DDR				DDRC
#define DATA_PORT				PORTC
#define DATA_PIN				PINC

#define CONTRL_DDR				DDRB
#define CONTRL_PORT 			PORTB
#define CONTRL_PIN				PINB

//#define FBH						2
//#define SK						3
#define MASTER_RST				4
#define IO_UPDATA				5
#define RD						6
#define WR						7

#define MASTER_RST_HIGH()		CONTRL_PORT |= (1 << MASTER_RST)
#define MASTER_RST_LOW()		CONTRL_PORT &= ~(1 << MASTER_RST)

#define IO_UPDATA_HIGH()		CONTRL_PORT |= (1 << IO_UPDATA)
#define IO_UPDATA_LOW()			CONTRL_PORT &= ~(1 << IO_UPDATA)

#define RD_HIGH()				CONTRL_PORT |= (1 << RD)
#define RD_LOW()				CONTRL_PORT &= ~(1 << RD)

#define WR_HIGH()				CONTRL_PORT |= (1 << WR)
#define WR_LOW()				CONTRL_PORT &= ~(1 << WR)

#endif
