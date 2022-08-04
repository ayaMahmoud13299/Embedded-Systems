#ifndef _7SEGMENT_H
#define _7SEGMENT_H


void HAL_7Segment_Init(char PORT,unsigned char START_PIN);
void HAL_7Segment_display(char PORT,unsigned char START_PIN,unsigned char num);
void HAL_7Segment_turnOF(char PORT,unsigned char START_PIN);


#endif