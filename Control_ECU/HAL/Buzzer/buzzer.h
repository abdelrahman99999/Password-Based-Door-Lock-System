 /******************************************************************************
 *
 *
 * File Name: buzzer.h
 *
 * Description: header file for the buzzer driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#ifndef HAL_BUZZER_BUZZER_H_
#define HAL_BUZZER_BUZZER_H_


#include "../../MCAL/gpio/gpio.h"
#include "../../Utilities/system_config.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define Buzzer_Port PORTC_ID
#define Buzzer_pin PIN5_ID

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * function to initialize buzzer
 */
void BUZZER_init();

/*
 * function to turn on buzzer
 */
void BUZZER_ON();

/*
 * function to turn off buzzer
 */
void BUZZER_OFF();


#endif /* HAL_BUZZER_BUZZER_H_ */
