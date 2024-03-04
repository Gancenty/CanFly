#ifndef _CONTROL_H
#define _CONTROL_H
#include "stm32f10x.h"
#include "config.h"
#include "pwm.h"
#include "pid.h"
void CanFly_Control(void);
void CanFly_PID_Call(void);

#endif
