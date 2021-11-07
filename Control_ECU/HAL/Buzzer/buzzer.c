 /******************************************************************************
 *
 *
 * File Name: buzzer.c
 *
 * Description: source file for the buzzer driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/
#include "buzzer.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * function to initialize buzzer
 */
void BUZZER_init(){
	GPIO_setupPinDirection(Buzzer_Port, Buzzer_pin,PIN_OUTPUT );
	GPIO_writePin(Buzzer_Port, Buzzer_pin,LOGIC_LOW);
}

/*
 * function to turn on buzzer
 */
void BUZZER_ON(){
	GPIO_writePin(Buzzer_Port, Buzzer_pin,LOGIC_HIGH);
}

/*
 * function to turn off buzzer
 */
void BUZZER_OFF(){
	GPIO_writePin(Buzzer_Port, Buzzer_pin,LOGIC_LOW);
}
