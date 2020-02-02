#include "motor.h"
#include "usart.h"
#include "delay.h"
#include "pwm.h"


int fspeedL=0;
int fspeedR=0;
extern u8 edge_flag;
extern int border;
extern int  back;

void MotorInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOD,&GPIO_InitStructure);
  
	//robot stop init
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
}

void fwd(int speedL,int speedR)
{
	if(fspeedL<0 || fspeedR<0)
	{
	//stop();
	}
	GPIO_SetBits(GPIOD,GPIO_Pin_10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);
	GPIO_SetBits(GPIOD,GPIO_Pin_9);
  motorL(1000-1,21-1,speedL);
  motorR(1000-1,21-1,speedR);
	fspeedL=speedL;
	fspeedR=speedR;
//	sprintf((char *)USART_SHOW,"Ç°½ø£¬×ó = %4d£¬ÓÒ = %4d\n",speedL,speedR);
//  USART_OUT(USART1,(uint8_t *)USART_SHOW,strlen(USART_SHOW));
	delay_ms(2);
}

void bkw(int speedL,int speedR)
{
	if(fspeedL>0 || fspeedR>0)
	{
//	stop();
	}
	GPIO_ResetBits(GPIOD,GPIO_Pin_10);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	GPIO_SetBits(GPIOD,GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
  motorL(1000-1,21-1,speedL);
  motorR(1000-1,21-1,speedR);
	fspeedL=speedL;
	fspeedR=speedR;
//	sprintf((char *)USART_SHOW,"ºóÍË£¬×ó = %4d£¬ÓÒ = %4d\n",speedL,speedR);
//  USART_OUT(USART1,(uint8_t *)USART_SHOW,strlen(USART_SHOW));
delay_ms(2);
}

void turn(int speedL,int speedR)
{
	if(speedL<0 && speedR>=0)
	{
	
		if(fspeedL>0 || fspeedR<0)
		{stop();}
//	sprintf((char *)USART_SHOW,"×ó×ª£¬×ó = %4d£¬ÓÒ = %4d\n",speedL,speedR);
//  USART_OUT(USART1,(uint8_t *)USART_SHOW,strlen(USART_SHOW));
	fspeedL=speedL;
	fspeedR=speedR;
	speedL = -speedL;
		
  GPIO_ResetBits(GPIOD,GPIO_Pin_10);
	GPIO_SetBits(GPIOD,GPIO_Pin_8);
	GPIO_ResetBits(GPIOD,GPIO_Pin_11);
  motorL(1000-1,21-1,speedL);
  motorR(1000-1,21-1,speedR);

	delay_ms(2);

	}
	else if(speedL>=0 && speedR<0)
	{
		if(fspeedL<0 || fspeedR>0)
		{stop();}
//		sprintf((char *)USART_SHOW,"ÓÒ×ª£¬×ó = %4d£¬ÓÒ = %4d\n",speedL,speedR);
//  USART_OUT(USART1,(uint8_t *)USART_SHOW,strlen(USART_SHOW));
	fspeedL=speedL;
	fspeedR=speedR;
	speedR = -speedR;
		
	GPIO_SetBits(GPIOD,GPIO_Pin_10);
	GPIO_ResetBits(GPIOD,GPIO_Pin_8);
	GPIO_SetBits(GPIOD,GPIO_Pin_11);
	GPIO_ResetBits(GPIOD,GPIO_Pin_9);
  motorL(1000-1,21-1,speedL);
  motorR(1000-1,21-1,speedR);
	
delay_ms(2);

	}

}

void stop(void)
{
  motorL(1000-1,21-1,0);
  motorR(1000-1,21-1,0);
	delay_ms(50);
//	sprintf((char *)USART_SHOW,"Í£Ö¹£¬×ó = %4d£¬ÓÒ = %4d\n",0,0);
//  USART_OUT(USART1,(uint8_t *)USART_SHOW,strlen(USART_SHOW));
	delay_ms(50);
}

void rst(void)
{
  motorL(1000-1,21-1,0);
  motorR(1000-1,21-1,0);
		delay_ms(50);
}

void contron_motor(void (*motion)(int speedL,int speedR),int speedL,int speedR,u16 time,enemy_state_Type state)
{
	u16 i=0;
	
	while((border==0||((state==HLR||state==HL||state==HR)&&(border==1||border==2)))&&i<time/10)
	{
		motion(speedL,speedR);
		delay_ms(10);
		i++;
		if(state==BORDER_CL&&i==time/10-1)
			back=0;
	}
}
