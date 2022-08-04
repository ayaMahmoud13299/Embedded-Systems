
#include "DMA.h"
#include "GPIO.h"


#define DMA_REG(PORT_ID, REG_ID) *((unsigned int*)(PORT_ID+REG_ID))
#define DMA_REG_ADDRESS(PORT_ID, REG_ID) *((unsigned int**)(PORT_ID+REG_ID))
#define RCC_AHB1LPENR DMA_REG(0x40023800,0x30) // bit 22 for DMA2 enable , 21 for DMA1 enable
//0x40026400 address of DMA2 , 0x40026000 address for dma1 0x40023800
// DMA2 REGISTERS
#define DMA2_LISR DMA_REG(0x40026400, 0x00) //DMA2 low interrupt status register (transfer complete,half transfer ) stream (0:3)
#define DMA2_HISR DMA_REG(0x40026400, 0x04) //DMA2 high interrupt status register (transfer complete,half transfer ) stream (4:7)
#define DMA2_LIFCR DMA_REG(0x40026400, 0x08) // DMA2 low flag interupt clear (بترحع الريجسترات بتاعت الفلاح اللي قبلها بزيرو)
#define DMA2_HIFCR DMA_REG(0x40026400, 0x0c)//  DMA2 high flag interupt clear  stream (4:7)
#define DMA2_S0CR DMA_REG(0x40026400, 0x10)  // DMA2 stream 0 configuration register
#define DMA2_S0NDTRR DMA_REG(0x40026400,0x14) //DMA2 stream 0 number of data register
#define DMA2_S0PAR DMA_REG_ADDRESS(0x40026400,0x18) //DMA2 stream 0 peripheral address register
#define DMA2_S0M0AR DMA_REG_ADDRESS(0x40026400,0x1c) //DMA2 stream 0 memory address register
#define DMA2_S0M1AR DMA_REG_ADDRESS(0x40026400,0x20) //DMA2 stream 1 memory address register
#define DMA2_S0FCR DMA_REG(0x40026400,0x24) //DMA2 stream 0 FIFO control register
#define NVIC_ISER1 DMA_REG(0xE000E100,0x04) //NVIC enable interrupt of dma2  stream0


void(*global_ptr)(void)=(void*)0;
void DMA_INIT(void)
{
	RCC_AHB1LPENR|=(1<<22); // for DMA2
	if((DMA2_S0CR>>0==1)) //check the bit enable of stream0,
		{
			DMA2_S0CR &=~(1<<0); //if it is enable , clear it
			while((DMA2_S0CR>>0==1))
			{
			// then wait until it is disabled , becouse it is not disable directly until transfer finished
			}
		}
	else
		{
		//if it is already disabled , do nothing !
		}

}

//Trigger source, Source address, destination address, number of transfers, transfer item size, transfer mode, transfer type(Single or burst).
void STREAM_CONFIG_INIT(unsigned char channel,  unsigned int* Source_address,unsigned int* destination_address ,
				unsigned int num_transfer,
				unsigned char transfer_item_size,
				unsigned char transfer_mode,
				unsigned char transfer_type)
{
	if(transfer_mode==MEMORY_TO_PERIPHERAL)
	{
		//set the destination address of peripheral
		DMA2_S0PAR=destination_address;
		//set the source address of memory
		DMA2_S0M0AR=Source_address;
	}
	else
	{
	// set the source address of peripheral
	DMA2_S0PAR=Source_address;

	//set the destination address of memory
	DMA2_S0M0AR=destination_address;
	}

	// number of all transfer items , decreament after each transfer
	DMA2_S0NDTRR=0x00;
	DMA2_S0NDTRR=num_transfer;

	//channels 0:7
	DMA2_S0CR|=(channel<<25);

	// Peripheral-to-memory 01: Memory-to-peripheral 10: Memory-to-memory 10 :
	DMA2_S0CR &=~(0x03<<6);
	DMA2_S0CR|=(transfer_mode<<6);

	// set the PFCTRL bit in the DMA_SxCR register , peripheral flow control in case peripheral to memory or vice versa
	// when it is memory to memory , it forced by hardware to be 0
//	if(((DMA2_S0CR>>6)==0)||((DMA2_S0CR>>6)==1))
//		DMA2_S0CR|=(1<<5);

	//memory transfer size item to 16bits
	DMA2_S0CR &=~(0x03<<13);
	DMA2_S0CR|=(transfer_item_size<<13);

	//peripheral transfer size item to
	DMA2_S0CR &=~(0x03<<11);
	DMA2_S0CR|=(transfer_item_size<<11);

	//config transfer type (fifo & direct)
	DMA2_S0FCR &=~(1<<2);
	DMA2_S0FCR |=(transfer_type<<2);
	if(transfer_type)
	{
		DMA2_S0FCR|=(1<<0); // Bits 1:0 FTH[1:0]: FIFO threshold selection  half word
		DMA2_S0CR|=(1<<9)|(1<<10); //Bit10 MINC: Memory increment mode,Bit9 pINC: peripheral increment mode

	}


	//TCIE: Transfer complete interrupt enable
	DMA2_S0CR|=(1<<4);
	//t 2 TEIE: Transfer error interrupt enable
	DMA2_S0CR|=(1<<2);
	//fifo interupt enable
	DMA2_S0CR|=(1<<7);

	//transfer stream priority very high
	DMA2_S0CR&=~(0x3<<16);
	DMA2_S0CR|=(0x3<<16);


}

void SET_STREAM0_EN(void)
{
	//clear all interrupt flags
	DMA2_LIFCR|=(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<1)|(1<<0);
	//enable stream
	DMA2_S0CR |=(1<<0);
}
void set_interupt_fun(void(*ptr)(void))
{
	NVIC_ISER1|=(1<<24);
	global_ptr=ptr;
	//

}
void DMA2_STREAM0_INTERUPT(void) //irq function of dma
{

	global_ptr();
//clear flags of  transfer complete (5), half transfer(4), transfer error(3), direct mode error(2) and
// FIFO error(0) interrupt flag
	DMA2_LIFCR|=(1<<5)|(1<<4)|(1<<3)|(1<<2)|(1<<0);
}
unsigned char check_transfer_state(void)
{
	unsigned char state=0;

	if((DMA2_LISR>>5)&1) //transfer complete
	{

		state|=(1<<5);
	}
	 if((DMA2_LISR>>4)&1)//half transfer(4) interrupt flag
	{

		state|=(1<<4);
	}
	 if((DMA2_LISR>>3)&1) //transfer error(3) interrupt flag
		{

			state|=(1<<3);
		}
	 if((DMA2_LISR>>2)&1) //direct mode error(2)interrupt flag
			{

				state|=(1<<2);
			}
	 if((DMA2_LISR>>0)&1) //FIFO error(0) interrupt flag
				{

					state|=(1<<0);
				}
	return state;
}
