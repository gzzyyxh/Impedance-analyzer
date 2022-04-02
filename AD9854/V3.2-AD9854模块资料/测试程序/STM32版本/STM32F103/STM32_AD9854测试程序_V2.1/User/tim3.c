#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "AD9854.h"
#include "tim3.h"
u16 TIM_FLAG=0;

//    AM����Ҫ�õ��Ķ�ʱ��		//
void VVInit(void)                                              
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler=7200;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	

}
void tim_set (uint32_t fre_out, uint32_t tim_clk)						
{
	uint16_t N1, N2;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	N1 = (uint16_t)(((float)tim_clk / fre_out) / 65536);
	N2 = (float)tim_clk / ((N1 + 1) * fre_out);
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	TIM_TimeBaseStructure.TIM_Period =N2-1;
	TIM_TimeBaseStructure.TIM_Prescaler=N1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM3, ENABLE); 

}
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  // ����NVIC�жϷ���2������2λ��ռ���ȼ���2λ��Ӧ���ȼ�
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);               
  // TIM1�ж�
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		 
	// ��ռ���ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	
	// �����ȼ�0��
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	
	// ʹ��TIM3�ж�
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								
  NVIC_Init(&NVIC_InitStructure);
}

void TIM3_IRQHandler(void)   
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)     //���ָ����TIM�жϷ��������ж�
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);          //���TIMx�ĸ����жϱ�־λ	
		TIM_FLAG = 1;
		
	}
}
void TIM3_Init(void)
{
	VVInit();
	NVIC_Configuration();
}
