//-----------------------------------------------------------------
// ��������:
// ����stm32_AD9854���Գ���V1.0
// ��������: ���ǵ���
// ��ʼ����: 2021.7.17
// �������: 
// �޸�����: 
// ��ǰ�汾: V1.0
// ���Թ���: ����STM32H750���İ塢OLEDҺ������LZE_ST_LINK2
// ˵������: 
//				(1) ����ʹ�õ�ϵͳʱ��Ƶ��Fsysclk=400MHz;
//     		(2) AD9854�����ݴ��䷽ʽ:���д��䷽ʽ;
//				(3) AD9854_IO������STM32H750���ӷ�ʽ����:
//-----------------------------------------------------------------
//               IO ����˵��
//-----------------------------------------------------------------
//					 AD9854ģ����     |    STM32���İ� 
//           PIV               -->   +5V
//           GND               -->   GND
//					 A[5:0]   				 -->   PB[5:0](GPIOB[5:0])
//					 D[7:0]    			   -->   PC[13:6](GPIOC[13:6])
//					 RD                -->   PB8(GPIOB_PIN8)
//					 WR                -->   PB9(GPIOB_PIN9)
//					 UD(UDCLK)         -->   PB10(GPIOB_PIN10)
//					 RT(RESET)         -->   PB11(GPIOB_PIN11)
//					 SP                -->   PB12(GPIOB_PIN12)
//
//					 AD9854ģ����     |    �ⲿ�źű����ź�Դ���ӿ���         
//					 OSK      					�ⲿ�����źŽ���OSK����������
//					 FD(FDSTA)       		�ⲿ�źŽ�AD9854�е�FSK(S2)��Ƶͷ������Ҳ����ֱ�����뵽AD9854�е�FD����
//-----------------------------------------------------------------
//				(4) ����AD9854���ĸ�, �뱣֤AD9854ģ��Ĺ��繦��, �����Դ����5V1A��Դ
//        (5) �����Գ���ʵ�����Ҳ��͸������Ʋ����������, 4�����ͨ�������������
//				(6) ������������: STM32���İ��ϰ���ѡ����������������
//						K1: ����ѡ���, ����K1�ֱ�����˳��ѭ��ѡ��SINE��FSK��BPSK��RFSK��AM��OSK��CHIRP��ɨƵ���ģʽ
//						K2: SINEģʽ��ʹ��, ÿ��һ��, ��ʾƵ�ʼ� 
//						K3: SINEģʽ��ʹ��, ÿ��һ��, ��ʾƵ�ʼ�
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// ͷ�ļ�����
//-----------------------------------------------------------------
#include "system.h"
#include "delay.h"
#include "led.h"
#include "oled.h"
#include "timer.h"
#include "AD9854.h"
#include "key.h"
#include "timer.h"
//-----------------------------------------------------------------
u32 Tx_Buf[32];
u8  Key_Count = 0;
u32 F=1000;  		//�������Ҳ���Ƶ�ʣ�1~100MHz��
u16 A=4095;			//�������Ҳ��ķ��ȣ�0~4095��
u8 numb;
//-----------------------------------------------------------------
// ������
//-----------------------------------------------------------------
// AM���е��Ʋ����õ����Ҳ�����
uint16_t Sinx_data[256] = {	
0x7FF,0x831,0x863,0x896,0x8C8,0x8FA,0x92B,0x95D,0x98E,0x9C0,0x9F1,0xA21,0xA51,0xA81,0xAB1,0xAE0,
0xB0F,0xB3D,0xB6A,0xB98,0xBC4,0xBF0,0xC1C,0xC46,0xC71,0xC9A,0xCC3,0xCEB,0xD12,0xD38,0xD5E,0xD83,
0xDA7,0xDCA,0xDEC,0xE0D,0xE2E,0xE4D,0xE6C,0xE89,0xEA5,0xEC1,0xEDB,0xEF5,0xF0D,0xF24,0xF3A,0xF4F,
0xF63,0xF75,0xF87,0xF97,0xFA6,0xFB4,0xFC1,0xFCD,0xFD7,0xFE0,0xFE8,0xFEF,0xFF5,0xFF9,0xFFC,0xFFE,
0xFFE,0xFFE,0xFFC,0xFF9,0xFF5,0xFEF,0xFE8,0xFE0,0xFD7,0xFCD,0xFC1,0xFB4,0xFA6,0xF97,0xF87,0xF75,
0xF63,0xF4F,0xF3A,0xF24,0xF0D,0xEF5,0xEDB,0xEC1,0xEA5,0xE89,0xE6C,0xE4D,0xE2E,0xE0D,0xDEC,0xDCA,
0xDA7,0xD83,0xD5E,0xD38,0xD12,0xCEB,0xCC3,0xC9A,0xC71,0xC46,0xC1C,0xBF0,0xBC4,0xB98,0xB6A,0xB3D,
0xB0F,0xAE0,0xAB1,0xA81,0xA51,0xA21,0x9F1,0x9C0,0x98E,0x95D,0x92B,0x8FA,0x8C8,0x896,0x863,0x831,
0x7FF,0x7CD,0x79B,0x768,0x736,0x704,0x6D3,0x6A1,0x670,0x63E,0x60D,0x5DD,0x5AD,0x57D,0x54D,0x51E,
0x4EF,0x4C1,0x494,0x466,0x43A,0x40E,0x3E2,0x3B8,0x38D,0x364,0x33B,0x313,0x2EC,0x2C6,0x2A0,0x27B,
0x257,0x234,0x212,0x1F1,0x1D0,0x1B1,0x192,0x175,0x159,0x13D,0x123,0x109,0x0F1,0x0DA,0x0C4,0x0AF,
0x09B,0x089,0x077,0x067,0x058,0x04A,0x03D,0x031,0x027,0x01E,0x016,0x00F,0x009,0x005,0x002,0x000,
0x000,0x000,0x002,0x005,0x009,0x00F,0x016,0x01E,0x027,0x031,0x03D,0x04A,0x058,0x067,0x077,0x089,
0x09B,0x0AF,0x0C4,0x0DA,0x0F1,0x10A,0x123,0x13D,0x159,0x175,0x192,0x1B1,0x1D0,0x1F1,0x212,0x234,
0x257,0x27B,0x2A0,0x2C6,0x2EC,0x313,0x33B,0x364,0x38D,0x3B8,0x3E2,0x40E,0x43A,0x466,0x494,0x4C1,
0x4EF,0x51E,0x54D,0x57D,0x5AD,0x5DD,0x60E,0x63E,0x670,0x6A1,0x6D3,0x704,0x736,0x768,0x79B,0x7CD
};
int main(void)
{
  u8 Key_Value=0;
	uint16_t amp_buf;	
	HAL_Init();          			// ��ʼ��HAL��
	MPU_Memory_Protection();	// MPU�ڴ汣�� 
	SystemClock_Config(); 		// ����ʱ��,400Mhz
	SysTick_clkconfig(400); 	// SysTick��������
	KEY_Init();             	// ��ʼ������	
  TIM3_Init(89,1000-1);     // ��ʱ��3��ʼ��
	delay_ms(10);
	GPIO_AD9854_Configuration();
	delay_ms(40);
	AD9854_Init();                       	
	OLED_Init();							// OLED��ʼ��
	OLED_Clear(); 						// OLED����
	OLED_ShowString(10,0,"TEST",16); 	 
	OLED_ShowString(10,2,"Welcome to use",16); 	
 
 	while (1)
	{	
		Key_Value=KEY_Scan(0);
		if(Key_Value==1)
		{
			Key_Count ++;
			OLED_ShowString(10,4,"                 ",16);
			switch (Key_Count)
  	  {
			 	case  1 :	{
		                AD9854_Init ();
								    OLED_ShowString(10,4,"   SINE   ",16);
						        AD9854_SetSine (F, A);
  	              }break;
       case  2 :	{
				            OLED_ShowString(10,6,"                  ",16);
				            AD9854_InitFSK();
			          	  OLED_ShowString(10,4,"   FSK   ",16);
										AD9854_SetFSK(10000,100000);										 // AD9854_SetFSK(Ƶ��1,Ƶ��2)				 
					         
  	              }break;
  	   case  3 :	{ 				          
  	  			        AD9854_InitBPSK();                               // AD9854_SetBPSK(��λ1,��λ2)
                    OLED_ShowString(10,4,"   BPSK   ",16);    				 
                    AD9854_SetBPSK(0, 8192);    			
									 
  	              }break;
  	   case 4 :	{							 	
										AD9854_InitChirp();                     
                    OLED_ShowString(10,4,"   CHIRP   ",16);
            				AD9854_SetChirp (100000,3000,5000);   				   // ���ģʽ,	AD9854_SetChirp (��Ƶ��,����,б��ʱ��)   	           
                  }break;
  	   case  5 :	{
  				          AD9854_InitRFSK();     
                    OLED_ShowString(10,4,"   RFSK   ",16);				 
                    AD9854_SetRFSK(1000000,40000000,100000,524288);  //AD9854_SetRFSK(��Ƶ�ʣ���Ƶ�ʣ�����Ƶ�ʣ�б��ʱ�ӿ���)
									 
                  }break;
  	   case  6 :	{
  	  			        AD9854_InitAM();
				            OLED_ShowString(10,4,"   A M   ",16);
										am_test (100,100);		  				            			 //am_test(���Ʋ�Ƶ�ʣ����ƶ�����)				
										

                  }break;
  	   case  7 :	{
  				          AD9854_InitOSK();      
                    OLED_ShowString(10,4,"   OSK   ",16);				 
                    AD9854_SetOSK (20);    												   // OSK�ߵ�ƽ����ֵ���ӣ��͵�ƽ��ֵ���٣�100Ϊ�仯��б�ʣ���������� 		
										
                   }break;
		   case  8 :	{  		
                    AD9854_Init ();				 
								    OLED_ShowString(10,4,"   sweepSINE   ",16);
						        Sweep_out();
									  Key_Count=0;
						
                   }break;
  	    default :	break;
		 }
	 }	
		if(Key_Value==2&&Key_Count==1)
		{
				if (F < 135000000)
				{
					if (F < 1000)									// ���Ƶ��С��1kHz����100Hz����
					{
						F += 100;
					}
					else if (F< 10000)						// ���Ƶ��С��10kHz����1kHz����
					{
						F += 1000;
					}
					else if (F < 100000)					// ���Ƶ��С��100kHz����10kHz����
					{
						F += 10000;
					}
					else if (F < 1000000)					// ���Ƶ��С��1MHz����100kHz����
					{
						F += 100000;
					}
					else if (F < 10000000)				// ���Ƶ��С��10MHz����1MHz����
					{
						F += 1000000;
					}
					else 
					{
						F += 5000000;								// ���Ƶ��С��135MHz����5MHz����
					}
				}
					AD9854_SetSine (F, A);
				OLED_ShowString(10,6,"                  ",16);
				sprintf ((char *)Tx_Buf,"%ld HZ",(long)F);	
        OLED_ShowString(10,6,(char *)Tx_Buf,16);    						 	// ��ʾƵ����Ϣ		 
			}
			if(Key_Value==3&&Key_Count==1)
	   	{
				if (F > 100)
				{
					if (F > 10000000)
					{
						F -= 5000000;
					}
					else if (F > 1000000)
					{
						F -= 1000000;
					}
					else if (F > 100000)
					{
						F -= 100000;
					}
					else if (F > 10000)
					{
						F -= 10000;
					}
					else if (F > 1000)
					{
						F -= 1000;
					}
					else if (F > 100)
					{
						F -= 100;
					}
				}
				AD9854_SetSine (F, A);
				OLED_ShowString(10,6,"                  ",16);
				sprintf ((char *)Tx_Buf,"%ld HZ",(long)F);	
        OLED_ShowString(10,6,(char *)Tx_Buf,16);    						 	// ��ʾƵ����Ϣ		 
			}


		
		if(Key_Count==6)
		{
				
			if (TIM_FLAG == 1)																			
				{
					TIM_FLAG = 0;
					add += step_up;		
					amp_buf = (uint16_t)((float)4095 * (0x0FFF - (((float)modu_buf * Sinx_data[add>>8]/100)))/ 4096);
			
					AD9854_SetAM(800000,amp_buf);
				}
		}
 

	}
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
