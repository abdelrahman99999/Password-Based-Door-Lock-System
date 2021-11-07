 /******************************************************************************
 *
 *
 * File Name: dc_motor.c
 *
 * Description: source file for the dc motor driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#include "dc_motor.h"


/*
 * Description :
 * function responsible for initialize dc-motor
 */
void DcMotor_Init(void){
	/* configure pins responsible for direction as output pins */
		GPIO_setupPinDirection(MOTOR_PORT_CONTROL, MOTOR_PIN0_CONTROL, PIN_OUTPUT);
		GPIO_setupPinDirection(MOTOR_PORT_CONTROL, MOTOR_PIN1_CONTROL, PIN_OUTPUT);


		/* Motor is stop at the beginning */
		GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN0_CONTROL, LOGIC_LOW);
		GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN1_CONTROL, LOGIC_LOW);
}

/*
 * Description :
 * setup dc-motor state as clock wise or anti-clock wise or stop
 */
void DcMotor_Rotate(DcMotorState state){

	switch(state){
		//case motor_stop:
		case 0:
			GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN0_CONTROL, LOGIC_LOW);
			GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN1_CONTROL, LOGIC_LOW);
			break;
		//case motor_A_CW:
		case 1:
			GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN0_CONTROL, LOGIC_LOW);
			GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN1_CONTROL, LOGIC_HIGH);
			break;
		//case motor_CW:
		case 2:
			GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN0_CONTROL, LOGIC_HIGH);
			GPIO_writePin(MOTOR_PORT_CONTROL, MOTOR_PIN1_CONTROL, LOGIC_LOW);
			break;
	}

}
