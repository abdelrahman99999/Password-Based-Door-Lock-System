/******************************************************************************
 *
 * File Name: Application_support.h
 *
 * Description: header file for helpful functions and global variables used by application layer
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#ifndef APPLICATION_SUPPORT_H_
#define APPLICATION_SUPPORT_H_


#include "Utilities/system_config.h"
#include "Utilities/std_types.h"
#include "HAL/LCD/lcd.h"
#include "HAL/keypad/keypad.h"
#include "MCAL/UART/uart.h"
#include "MCAL/Timer/timer.h"
#include <util/delay.h> /* For delay functions */
#include <avr/interrupt.h> /*for access its registers like SREG */

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define DOOR_OPENNING_TIME	(469) //15sec
#define DOOR_CLOSING_TIME	(469) //15sec
#define DOOR_LEFT_OPEN_TIME	(94) //3sec
#define DANGER_TIME (1875) //60sec

#define Open_Door_Option '+'
#define Change_Password_Option '-'
#define Opening_Door_Action 88
#define Changing_Password_Action 44
#define Danger 0x33

#define KEYPAD_PRESSED_TIME 500
#define ENTER_Ascii 13

#define READY 0x10

#define PASSWORD_LENGTH 5
#define Passwords_Match 1
#define Passwords_NotMatch 0


/*******************************************************************************
 *                              Global Variables                                *
 *******************************************************************************/
extern uint32 g_ticks;


/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/


/***************************************************************************************************
 * [Function Name]: HMI_Options1
 *
 * [Description]:  Function to show main functionalities of the HMI-ECU	like
 * 					1- open the door
 * 					2- change password
 *
 * [Args]:         Void
 *
 * [Returns]:      Void
 *
 ***************************************************************************************************/
void HMI_Options1(void);


/***************************************************************************************************
 * [Function Name]: Get_Password_From_User
 *
 * [Description]:  Function to get entered password from user using by keypad
 *
 * [Args]:         pass: pointer to uint8
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void Get_Password_From_User(uint8 *pass);


/***************************************************************************************************
 * [Function Name]: Send_Password_To_CTRL_ECU
 *
 * [Description]:  Function to send password(array of password-length) to Control-Ecu using Uart.
 *
 * [Args]:         pass: pointer to uint8
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void Send_Password_To_CTRL_ECU(uint8 *pass);


/***************************************************************************************************
 * [Function Name]: Set_Password
 *
 * [Description]:  Function to change application password
 *
 * [Args]:         void
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void Set_Password(void);


/***************************************************************************************************
 * [Function Name]: CallBack_Function
 *
 * [Description]:  CallBack Function to use by timer
 * 					it is only increment the global variable of g_ticks
 *
 * [Args]:         void
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void CallBack_Function(void);


/***************************************************************************************************
 * [Function Name]: Opening_hold_Closing_Door
 *
 * [Description]:  Function to show the state of the door that is opened or hold or closed by control-ecu
 *
 * [Args]:         void
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void Opening_hold_Closing_Door(void);


/***************************************************************************************************
 * [Function Name]: Danger_Mission
 *
 * [Description]:  function to show the state of trying of thieving
 *
 * [Args]:        void
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void Danger_Mission(void);


/***************************************************************************************************
 * [Function Name]: MisMatch_Answer
 *
 * [Description]:  function to show the state when user entered wrong password
 *
 * [Args]:        void
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void MisMatch_Answer(void);



#endif /* APPLICATION_SUPPORT_H_ */
