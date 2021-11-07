 /******************************************************************************
 *
 * Module: Timer
 *
 * File Name: timer.c
 *
 * Description: Source file for the Timer AVR driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#include "timer.h"
#include "avr/interrupt.h"
#include "avr/io.h" /*to use timer registers*/
#include "../../Utilities/common_macros.h" /* To use the macros like SET_BIT() ,CLEAR_BIT() */

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_Timer0_callBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/

ISR(TIMER0_OVF_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{
		(*g_Timer0_callBackPtr)();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_Timer0_callBackPtr != NULL_PTR)
	{

		(*g_Timer0_callBackPtr)();
		/* another method to call the function using pointer to function g_callBackPtr(); */
	}
}



/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * function responsible for initialization of timer
 */
void Timer0_init(const Timer0_ConfigType* ConfigType){

		TCNT0 =ConfigType->initial_value ; //Set Timer initial value
		SET_BIT(TCCR0,FOC0);  /*for normal and CTC mode,FOC0=1  */

		TCCR0 &= ~(1<<COM00) & ~(1<<COM01);

		TCCR0 = (TCCR0 & 0xF8) | (ConfigType->clock) ;

		if(ConfigType->mode == Timer0_normal){
			TIMSK = (TIMSK & 0xFE) | (1<<TOIE0); // Enable Timer0 Overflow Interrupt
			/* Configure the timer control register
			 * 1. Non PWM mode FOC0=1
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Normal Mode COM00=0 & COM01=0
			 * 4. clock -> CS00 CS01 CS02
			 */

			TCCR0 &= ~(1<<WGM01) & ~(1<<WGM00) ;

		}
		else if(ConfigType->mode == Timer0_CTC){

			OCR0  = ConfigType->compare_value; // Set Compare Value
			TIMSK = (TIMSK & 0xFD) |(1<<OCIE0); // Enable Timer0 Compare Interrupt
			/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0  so COM00=0 & COM01=0
			 * 4. clock -> CS00 CS01 CS02
			 */
			CLEAR_BIT(TCCR0,WGM00);
			SET_BIT(TCCR0,WGM01);
		}

}

/*
 * function responsible for setting call back function used by timer
 */
void Timer0_SetCallBack( void(*a_ptr)(void) ){
	g_Timer0_callBackPtr=a_ptr;
}

/*
 * function responsible for De-initialization of timer
 */
void Timer0_Deinit(){
	TCCR0 =0; //stop clock
	CLEAR_BIT(TIMSK,OCIE0); /* disable timer0 interrupts for CTC mode */
	CLEAR_BIT(TIMSK,TOIE0); /* disable timer0 interrupts for overflow mode */
	g_Timer0_callBackPtr = NULL_PTR; /* clear the call-back function */
}
