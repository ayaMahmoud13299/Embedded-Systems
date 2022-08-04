/*
 * Author : Abdullah Drwesh
 * Date   : 8/12/2021
 * Version: 1.0.0
 */

#ifndef KEYPAD_H
#define KEYPAD_H
void delay30ms(void);
void Keypad_init(void);
void Keypad_manage(void);
unsigned char Keypad_GetKey(void);
void keypad_callout_button_pressed(void);
#endif /* KEYPAD_H */
