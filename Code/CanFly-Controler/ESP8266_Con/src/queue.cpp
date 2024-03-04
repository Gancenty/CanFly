#include "queue.h"

#define MAX_QUEUE_SIZE 1024
uint8_t queue[MAX_QUEUE_SIZE];
int rear,front;

void queue_init(void)
{
    rear=front=0;
}
uint16_t queue_insert(uint8_t *data,uint16_t len)
{
    static bool isinit=0;
    if(isinit==0){
        queue_init();
        isinit=1;
    }
    uint16_t ret=0;
    while(!queue_full()&&(ret!=len)){
        queue[rear]=data[ret];
        rear=(rear+1)%MAX_QUEUE_SIZE;
        ret++;
    }
    return ret;
}
uint8_t queue_peek(uint16_t index)
{
    uint16_t idx=front;
    index--;
    while (index--)
    {
        idx=(idx+1)%MAX_QUEUE_SIZE;
    }
    return queue[idx];
}
void queue_pop(void)
{
    front=(front+1)%MAX_QUEUE_SIZE;
}
uint16_t queue_size(void)
{
    return (rear-front+MAX_QUEUE_SIZE)%MAX_QUEUE_SIZE;
}
bool queue_full(void)
{
    return (rear+1)%MAX_QUEUE_SIZE==front;
}