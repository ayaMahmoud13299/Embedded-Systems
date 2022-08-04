/*
 * Author : Abdullah Drwesh
 * Date   : 8/12/2021
 * Version: 1.0.0
 */
#include "GPIO.h"
#include "keypad.h"
#include "stm32f401xc.h"


char pressed=-1;
char released=0;
char button_state=0;


unsigned char keypad_array[4][3]={{1,2,3},
{4,5,6},
{7,8,9},
{0,0,0}};
unsigned char key;
void Keypad_init()
{	int i,j;
	for (i = 0; i < 3; i++) {
	    GPIO_Init('B', i, OUTPUT, PUSH_PULL);
	    GPIO_WritePin('B', i,1);
	  }
	for (j = 3; i < 7; j++) {
		    GPIO_Init('B', i, INPUT, PULL_UP);
		  }
}
void Keypad_manage()
{
	int row,column ;
	if(button_state==released){
for(column=0;column<3;column++)
{GPIO_WritePin('B', column,0);
for(row=0;row<4;row++)
{
	if(GPIO_ReadPin('B', (row+3)) == 0)
			{
			delay30ms();
			}
	if(GPIO_ReadPin('B', (row+3)) == 0)
			{
	GPIO_WritePin('B', column,1);
	key=keypad_array[row+3][column];
	button_state=pressed;

	keypad_callout_button_pressed();
			}
	GPIO_WritePin('B', column,1);
}
}
}
unsigned char Keypad_GetKey()
{
	button_state=released;
	return key;
}
}
