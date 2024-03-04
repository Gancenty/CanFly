#ifndef _QUEUE_H
#define _QUEUE_H
#include "includes.h"

uint16_t queue_insert(uint8_t *data,uint16_t len);
uint8_t queue_peek(uint16_t index);
void queue_pop(void);
uint16_t queue_size(void);
bool queue_full(void);

#endif
