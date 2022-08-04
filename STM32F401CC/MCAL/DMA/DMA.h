#ifndef DMA_H
#define DMA_H
#include "stm32f401xc.h"
//#include "GPIO.h"

//transfer type
#define FIFO ((unsigned char)0x01)
#define DIRECT ((unsigned char)0x00)

// Data transfer direction
#define PERIPHERAL_TO_MEMORY ((unsigned char)0x00)
#define MEMORY_TO_PERIPHERAL ((unsigned char)0x01)
#define MEMORY_TO_MEMORY ((unsigned char)0x02)

//transfer item size 00: Byte (8-bit) 01: Half-word (16-bit) 10: Word (32-bit) 11: reserved
#define BYTE ((unsigned char)0x01)
#define HALF_WORD ((unsigned char)0x02)
#define WORD ((unsigned char)0x03)

//channels
#define CHANNEL_0 ((unsigned char)0x00)
#define CHANNEL_1 ((unsigned char)0x01)
#define CHANNEL_2 ((unsigned char)0x02)
#define CHANNEL_3 ((unsigned char)0x03)
#define CHANNEL_4 ((unsigned char)0x04)
#define CHANNEL_5 ((unsigned char)0x05)
#define CHANNEL_6 ((unsigned char)0x06)
#define CHANNEL_7 ((unsigned char)0x07)

void DMA_INIT(void);
void STREAM_CONFIG_INIT(unsigned char channel,unsigned int* Source_address ,unsigned int* destination_address ,
				unsigned int num_transfer,
				unsigned char transfer_item_size,
				unsigned char transfer_mode,
				unsigned char transfer_type);
void SET_STREAM0_EN(void);
void set_interupt_fun(void(*ptr)(void));
void DMA2_STREAM0_INTERUPT(void);
unsigned char check_transfer_state(void);


#endif
