#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

typedef enum enemy_state
{
	HLR = 56,
	HL = 5,
	HR = 6,
	LH = 7,
	LM = 8,
	LB = 9,
	RH = 10,
	RM = 11,
	RB = 12,
	BL = 13,
	BR = 14,
	BLR = 1314,
	BORDER = 486,
	BORDER_CL = 426
	
}enemy_state_Type;



void MotorInit(void);
void fwd(int speedL,int speedR);
void bkw(int speedL,int speedR);
void turn(int speedL,int speedR);
void stop(void);
void rst(void);
void contron_motor(void (*motion)(int speedL,int speedR),int speedL,int speedR,u16 time,enemy_state_Type state);
#endif
