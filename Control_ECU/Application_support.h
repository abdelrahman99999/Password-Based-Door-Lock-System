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
#include "MCAL/UART/uart.h"
#include "MCAL/Timer/timer.h"
#include "HAL/Buzzer/buzzer.h"
#include "HAL/Dc_Motor/dc_motor.h"
#include "HAL/External_EEPROM/external_eeprom.h"
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

#define READY 0x10

#define Max_Number_Of_Allowed_NotCorrect_Password 3
#define PASSWORD_LENGTH 5
#define PASSWORD_STORED_ADRESS 0x0311
#define Passwords_Match 1
#define Passwords_NotMatch 0


/*******************************************************************************
 *                              Global Variables                                *
 *******************************************************************************/
extern uint32 g_ticks;
extern uint8 App_Password[PASSWORD_LENGTH];

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/***************************************************************************************************
 * [Function Name]: Compare_Two_Passwords
 *
 * [Description]:  Function to compare to passwords with each other
 *
 * [Args]:         password1: pointer to uint8
 * 				   password2: pointer to uint8
 *
 * [Returns]:      uint8 (Passwords_Match or Passwords_NotMatch)
 *
 ***************************************************************************************************/
uint8 Compare_Two_Passwords(uint8* password1,uint8* password2);


/***************************************************************************************************
 * [Function Name]: Recieve_Password_From_HMI_ECU
 *
 * [Description]:  Function to receive password(array of password-length) from HMI-Ecu using uart
 *
 * [Args]:         pass: pointer to uint8
 *
 * [Returns]:      Void
 *
 ***************************************************************************************************/
void Recieve_Password_From_HMI_ECU(uint8 *pass);


/***************************************************************************************************
 * [Function Name]: Save_Pasword_IN_MEMMORY
 *
 * [Description]:  Function to store password (array of password-length) in EEPROM
 * 					and store it also in global array called App_Password
 *
 * [Args]:         pass: pointer to uint8
 *
 * [Returns]:      Void
 *
 ***************************************************************************************************/
void Save_Pasword_IN_MEMMORY(uint8 *pass);


/***************************************************************************************************
 * [Function Name]: Set_Password
 *
 * [Description]:  Function to change application password
 *
 * [Args]:         Void
 *
 * [Returns]:      Void
 *
 ***************************************************************************************************/
void Set_Password(void);


/***************************************************************************************************
 * [Function Name]: CallBack_Function
 *
* [Description]:  CallBack Function to use by timer
 * 					it is only increment the global variable of g_ticks
 *
 * [Args]:         Void
 *
 * [Returns]:      Void
 *
 ***************************************************************************************************/
void CallBack_Function(void);


/***************************************************************************************************
 * [Function Name]: Opening_hold_Closing_Door
 *
 * [Description]:  Function to open door then hold it for specific time then close the door
 *
 * [Args]:         Void
 *
 * [Returns]:      Void
 *
 ***************************************************************************************************/
void Opening_hold_Closing_Door(void);


/***************************************************************************************************
 * [Function Name]: Danger_Mission
 *
 * [Description]:  function to turn on buzzer  for specific time to show state of trying of thieving
 *
 * [Args]:        void
 *
 * [Returns]:     void
 *
 ***************************************************************************************************/
void Danger_Mission(void);


#endif /* APPLICATION_SUPPORT_H_ */
