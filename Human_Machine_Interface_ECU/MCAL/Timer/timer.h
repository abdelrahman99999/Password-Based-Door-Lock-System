 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.h
 *
 * Description: Header file for the Timer AVR driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#ifndef TIMER_H_
#define TIMER_H_

#include "../../Utilities/std_types.h"  /*to use uint8 data type*/
#include "../../Utilities/system_config.h"

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	Timer0_normal,Timer0_CTC
}Timer0_mode;

typedef enum{
	Timer0_NoClock,Timer0_NoPrescaler,Timer0_8_Prescaler,Timer0_64_Prescaler,
	Timer0_256_Prescaler,Timer0_1024_Prescaler,Timer0_External_clock_failling, Timer0_External_clock_rising
}Timer0_clock;


typedef struct{
	Timer0_mode mode;
	Timer0_clock clock;
	uint8 initial_value;
	uint8 compare_value;
}Timer0_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * function responsible for initialization of timer
 */
void Timer0_init(const Timer0_ConfigType* ConfigType);

/*
 * function responsible for setting call back function used by timer
 */
void Timer0_SetCallBack( void(*a_ptr)(void) );

/*
 * function responsible for De-initialization of timer
 */
void Timer0_Deinit();



#endif /* TIMER_H_ */
