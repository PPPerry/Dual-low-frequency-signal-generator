#include "sys.h"
#include "dac.h"
#include "lcd.h"
#include "pwm.h"
#include "delay.h"
#include "usart.h"
#include "touch.h"
#include "key.h"


int main(void)
{
	double sinf1=6.58,sinf2=6.58,sinv1=1.52,sinv2=1.52,pwmf1=5.00,pwmf2=5.00,pwmr1=50,pwmr2=50;
	u8 choice1=0,choice2=0;
	u32 position=0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init(168);      //初始化延时函数
	uart_init(115200);		//初始化串口波特率为115200
	
	
 	LCD_Init();					
 	LCD_BackInit();    //LCD初始化
	
	KEY_Init();
	
	tp_dev.init();				//触摸屏初始化
	
	sin_Generation1(0.5);
	sin_Generation2(0.5,0);
	GPIO_Configuration();
	TIM6_Configuration();
	TIM7_Configuration();
	
	DAC_DMA_Configuration1();   
	DAC_DMA_Configuration2(); 
	
	TIM14_PWM_Init(200-1,84-1,100); //84 
	TIM10_PWM_Init(200-1,168-1,100); //168MHz
	
	
	while(1)
	{		
		if(IfTouch(25,25,200,200))//1 PA4
		{
			choice1=1; 
			RedDot(1);
			
			POINT_COLOR=BLUE;
			LCD_ShowDouble(280,50,sinf1,1);
			LCD_ShowDouble(300,50+88,sinv1,2);
			TIM_Cmd(TIM14, DISABLE);
			DAC_DMACmd(DAC_Channel_1, ENABLE);
			
		}	
		if(IfTouch(25,25+175,200,200+175))//2 PF9
		{
			choice1=2;
			RedDot(2);
			
			POINT_COLOR=BLUE;
			LCD_ShowDouble(280,50+175,pwmf1,1);
			LCD_ShowDouble(325,50+88+175,pwmr1,3);
			DAC_DMACmd(DAC_Channel_1, DISABLE);
			TIM_Cmd(TIM14, ENABLE);  //使能TIM14
			
		}
		if(IfTouch(25,25+400,200,200+400))//3 PA5
		{
			choice2=1;
			RedDot(3);
			
			POINT_COLOR=BLUE;
			LCD_ShowDouble(280,50+400,sinf2,1);
			LCD_ShowDouble(300,50+88+400,sinv2,2);
			TIM_Cmd(TIM10, DISABLE);
			DAC_DMACmd(DAC_Channel_2, ENABLE);
			
		}
		if(IfTouch(25,25+175+400,200,200+400+175))//4 PF6
		{
			choice2=2;
			RedDot(4);
			
			POINT_COLOR=BLUE;
			LCD_ShowDouble(280,50+175+400,pwmf2,1);
			LCD_ShowDouble(325,50+88+175+400,pwmr2,3);
			DAC_DMACmd(DAC_Channel_2, DISABLE);
			TIM_Cmd(TIM10, ENABLE);  //使能TIM10
			
		}
		
		if(choice1==1)
		{
			
			
			while(IfTouch(259-44,47,259,47+44)||IfTouch(396,47,396+44,47+44))
			{
				
				if(IfTouch(259-44,47,259,47+44))
				{
					if(sinf1<=1)break;
					sinf1-=0.01;
					
				}
				if(IfTouch(396,47,396+44,47+44))
				{
					if(sinf1>=10)break;
					sinf1+=0.01;
				}
				delay_ms(20);
				TIM_Cmd(TIM6, DISABLE);
				TIM_SetAutoreload(TIM6, (u32)(328.125/sinf1)-1);
				TIM_Cmd(TIM6, ENABLE);
				LCD_ShowDouble(280,50,sinf1,1);
			}
			while(IfTouch(259-44,47+88,259,47+44+88)||IfTouch(396,47+88,396+44,47+44+88))
			{
				
				if(IfTouch(259-44,47+88,259,47+44+88))
				{
					if(sinv1<=0.1)break;
					sinv1-=0.01;
				}
				if(IfTouch(396,47+88,396+44,47+44+88))
				{
					if(sinv1>=3.3)break;
					sinv1+=0.01;
				}
				delay_ms(20);
				DMA_Cmd(DMA1_Stream5, DISABLE);
				sin_Generation1(sinv1/3.3);
				DMA_Cmd(DMA1_Stream5, ENABLE);
				LCD_ShowDouble(300,50+88,sinv1,2);
			}
			
			
		}else if(choice1==2)
		{
			
			while(IfTouch(259-44,47+175,259,47+44+175)||IfTouch(396,47+175,396+44,47+44+175))
			{
				
				if(IfTouch(259-44,47+175,259,47+44+175))
				{
					if(pwmf1<=1)break;
					pwmf1-=0.01;
				}
				if(IfTouch(396,47+175,396+44,47+44+175))
				{
					if(pwmf1>=10)break;
					pwmf1+=0.01;
				}
				delay_ms(20);
				TIM_Cmd(TIM14, DISABLE);
				TIM_SetAutoreload(TIM14,1000/pwmf1-1);
				TIM_SetCompare1(TIM14,(u32)(10*pwmr1/pwmf1));
				TIM_Cmd(TIM14, ENABLE);
				LCD_ShowDouble(280,50+175,pwmf1,1);
			}
			while(IfTouch(259-44,47+88+175,259,47+44+88+175)||IfTouch(396,47+88+175,396+44,47+44+88+175))
			{
				
				if(IfTouch(396,47+88+175,396+44,47+44+88+175))
				{
					if(pwmr1<=10)break;
					pwmr1-=1;
				}
				if(IfTouch(259-44,47+88+175,259,47+44+88+175))
				{
					if(pwmr1>=90)break;
					pwmr1+=1;
				}
				delay_ms(20);
				TIM_Cmd(TIM14, DISABLE);
				TIM_SetCompare1(TIM14,(u32)(10*pwmr1/pwmf1));
				TIM_Cmd(TIM14, ENABLE);
				LCD_ShowDouble(325,50+88+175,100-pwmr1,3);
			}
			
		}
		
		if(choice2==1)
		{
			
			while(IfTouch(259-44,47+400,259,47+44+400)||IfTouch(396,47+400,396+44,47+44+400))
			{
				
				if(IfTouch(259-44,47+400,259,47+44+400))
				{
					if(sinf2<=1)break;
					sinf2-=0.01;
				}
				if(IfTouch(396,47+400,396+44,47+44+400))
				{
					if(sinf2>=10)break;
					sinf2+=0.01;
				}
				delay_ms(20);
				TIM_Cmd(TIM7, DISABLE);
				TIM_SetAutoreload(TIM6, (u32)(328.125/sinf2)-1);
				TIM_Cmd(TIM7, ENABLE);
				LCD_ShowDouble(280,50+400,sinf2,1);
			}
			while(IfTouch(259-44,47+88+400,259,47+44+88+400)||IfTouch(396,47+88+400,396+44,47+44+88+400))
			{
				
				if(IfTouch(259-44,47+88+400,259,47+44+88+400))
				{
					if(sinv2<=0.1)break;
					sinv2-=0.01;
				}
				if(IfTouch(396,47+88+400,396+44,47+44+88+400))
				{
					if(sinv2>=3.3)break;
					sinv2+=0.01;
				}
				delay_ms(20);
				DMA_Cmd(DMA1_Stream6, DISABLE);
				sin_Generation2(sinv2/3.3,position);
				DMA_Cmd(DMA1_Stream6, ENABLE);
				LCD_ShowDouble(300,50+88+400,sinv2,2);
			}
		
		}else if(choice2==2)
		{
			
			while(IfTouch(259-44,47+175+400,259,47+44+175+400)||IfTouch(396,47+175+400,396+44,47+44+175+400))
			{
				
				if(IfTouch(259-44,47+175+400,259,47+44+175+400))
				{
					if(pwmf2<=1)break;
					pwmf2-=0.01;
				}
				if(IfTouch(396,47+175+400,396+44,47+44+175+400))
				{
					if(pwmf2>=10)break;
					pwmf2+=0.01;
				}
				delay_ms(20);
				TIM_Cmd(TIM10, DISABLE);
				TIM_SetAutoreload(TIM10,1000/pwmf2-1);
				TIM_SetCompare1(TIM10,(u32)(10*pwmr2/pwmf2));
				TIM_Cmd(TIM10, ENABLE);
				LCD_ShowDouble(280,50+175+400,pwmf2,1);
			}
			while(IfTouch(259-44,47+88+175+400,259,47+44+88+175+400)||IfTouch(396,47+88+175+400,396+44,47+44+88+175+400))
			{
				
				if(IfTouch(396,47+88+175+400,396+44,47+44+88+175+400))
				{
					if(pwmr2<=10)break;
					pwmr2-=1;
				}
				if(IfTouch(259-44,47+88+175+400,259,47+44+88+175+400))
				{
					if(pwmr2>=90)break;
					pwmr2+=1;
				}
				delay_ms(20);
				TIM_Cmd(TIM10, DISABLE);
				TIM_SetCompare1(TIM10,(u32)(10*pwmr2/pwmf2));
				TIM_Cmd(TIM10, ENABLE);
				LCD_ShowDouble(325,50+88+175+400,100-pwmr2,3);
			}
		}
		delay_ms(100);
		if(KEY_Scan())
		{
			while(KEY_Scan()==1)
				position++;
			while(KEY_Scan()==3)
				position--;
			DMA_Cmd(DMA1_Stream6, DISABLE);
			sin_Generation2(sinv2/3.3,position);
			DMA_Cmd(DMA1_Stream6, ENABLE);
				
		}
	}
}



