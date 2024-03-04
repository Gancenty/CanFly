#ifndef _PWM_H
#define _PWM_H
#include "stm32f10x.h"
#include "config.h"
#define PWM_ARR 1000-1
#define PWM_PSC 72-1
extern uint16_t PWM_MAX;
void PWM_Config(void);
void Motor_Pwm_Set(uint16_t MOTO1_PWM,uint16_t MOTO2_PWM,uint16_t MOTO3_PWM,uint16_t MOTO4_PWM);
void Motor_Max_Set(uint16_t max);



#endif
