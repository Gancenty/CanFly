#include "led.h"
void LED_Config(void)
{
    pinMode(2,OUTPUT);
}
void LED_Blink(void)
{
    digitalWrite(2,LOW);
    delay(200);
    digitalWrite(2,HIGH);
    delay(200);
}
