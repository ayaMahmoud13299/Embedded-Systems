#include "7Segment.h"
#include "GPIO.h"


void HAL_7Segment_Init(char PORT,unsigned char START_PIN)
{
    for(START_PIN;START_PIN<START_PIN+7;START_PIN++)
    {
        GPIO_Init(PORT, START_PIN,OUTPUT, PUSH_PULL);
    }
}

void HAL_7Segment_display(char PORT,unsigned char START_PIN,unsigned char num) //common cathode , a is least signficant bit , e is most signficant bit 
{
    unsigned char seven_segment_array[10]={0x3F, 0x06, 0x5B, 0x4F, 0x66,
                                        0x6D, 0x7D, 0x07, 0x7F, 0x6F};
    int i;
    for(i=0;i<7;i++)
    {
        GPIO_WritePin(PORT, START_PIN +i , ((seven_segment_array[num]>>i)&1)<<i);
    }
}
void HAL_7Segment_turnOF(char PORT,unsigned char START_PIN)
{
    int i;
    for(i=0;i<7;i++)
    {
         GPIO_WritePin(PORT, START_PIN +i , 0);
    }
}