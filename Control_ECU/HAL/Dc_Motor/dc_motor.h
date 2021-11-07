 /******************************************************************************
 *
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the dc motor driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

/* Description:
  * Control the DC Motor direction using L293D H-bridge.
  * Control The DC Motor Speed using PWM from MC.
  */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "../../Utilities/std_types.h"
#include "../../MCAL/gpio/gpio.h"
#include "../../Utilities/system_config.h"
/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

typedef enum{
	motor_stop, motor_A_CW, motor_CW
}DcMotorState;


/*
 * to control direction
 */
#define MOTOR_PORT_CONTROL PORTC_ID
#define MOTOR_PIN0_CONTROL PIN6_ID
#define MOTOR_PIN1_CONTROL PIN7_ID


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * function responsible for initialize dc-motor
 */
void DcMotor_Init(void);

/*
 * Description :
 * setup dc-motor state as clock wise or anti-clock wise or stop
 */
void DcMotor_Rotate(DcMotorState state);

#endif /* DC_MOTOR_H_ */
