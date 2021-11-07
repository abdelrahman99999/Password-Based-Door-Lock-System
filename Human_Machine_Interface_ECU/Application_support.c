/******************************************************************************
 *
 * File Name: Application_support.c
 *
 * Description: Source file for helpful functions and global variables used by application layer
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#include "Application_support.h"


/*******************************************************************************
 *                              Global Variables                                *
 *******************************************************************************/

uint32 g_ticks = 0;


/*******************************************************************************
 *                      Functions Definitions                                  *
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
void HMI_Options1(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "+: Open Door");
	LCD_displayStringRowColumn(1, 0, "-: Change Password");
}


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
void Get_Password_From_User(uint8 *pass){
	LCD_moveCursor(1, 0);
	/*variable used to get the keypad button pressed by user */
	uint8 key;

	for(uint8 i=0;i<PASSWORD_LENGTH;i++){
		key = KEYPAD_getPressedKey();
		/*password must be numbers only*/
		if (key >= 0 && key <= 9) {
			LCD_displayCharacter('*');
			pass[i]= key;
		}else{
			/*in case of pressing not a number button
			 * i will decrement the i counter to get 5 number password only
			 */
			i--;
		}
		_delay_ms(KEYPAD_PRESSED_TIME);
	}
	/*
	 * do not exit from the function without pressing a ENTER button
	 */
	while(KEYPAD_getPressedKey() != ENTER_Ascii);

}


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
void Send_Password_To_CTRL_ECU(uint8 *pass){
	/*
	 * send password (array)
	 * byte by byte - number by number
	 */
	for (uint8 i=0;i<PASSWORD_LENGTH;i++){
		UART_sendByte(pass[i]);
		_delay_ms(50);
	}
}


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
void Set_Password(void){
	/*
	 * 2 temporary arrays to use them to get password from the user
	 *  PasswordTemp1: Entering of password(first password)
	 *  PasswordTemp2: confirmation of password
	 */
	uint8 PasswordTemp1[PASSWORD_LENGTH];
	uint8 PasswordTemp2[PASSWORD_LENGTH];
	/*
	 * variable used to store the state of 2 passwords entered by the user
	 * can only be 2 cases:
	 * 						1- Passwords_NotMatch
	 * 						2- Passwords_Match
	 */
	uint8 Password_State = Passwords_NotMatch;

	/* exit from the while only if case of matching of 2 passwords */
	while(Password_State == Passwords_NotMatch){
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0,"Enter New Password");

			/* get 5 numbers first password from the user */
			Get_Password_From_User(PasswordTemp1);

			/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
			UART_sendByte(READY);
			while (UART_recieveByte() != READY);
			/*
			 * send first password to Control-Ecu in preparation to compare it
			 *  with the confirmation password to check Matching
			 */
			Send_Password_To_CTRL_ECU(PasswordTemp1);

			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0, "Confirm The Password");

			/* get 5 numbers confirm password from the user */
			Get_Password_From_User(PasswordTemp2);

			/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
			UART_sendByte(READY);
			while (UART_recieveByte() != READY);

			/*
			 * send confirm password to Control-Ecu  to compare it
			 *  with the first password to check Matching
			 */
			Send_Password_To_CTRL_ECU(PasswordTemp2);

			/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
			while (UART_recieveByte() != READY);

			/*
			 * Receive state of two entered passwords if it is matched ore not
			 */
			Password_State = UART_recieveByte();

			if(Password_State == Passwords_Match){
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Passwords Match");
				LCD_displayStringRowColumn(1,0,"Password saved");
				_delay_ms(500);
			}
			else if(Password_State == Passwords_NotMatch){
				LCD_clearScreen();
				LCD_displayStringRowColumn(0,0,"Passwords Mismatch");
				LCD_displayStringRowColumn(1,0,"Try Again");
				_delay_ms(500);
			}
		}

	LCD_clearScreen();
}


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
void CallBack_Function(void){
	g_ticks++;
}


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
void Opening_hold_Closing_Door(void){

	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks = 0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Opening the Door...");
	/*
	 * wait until door next state
	 */
	while (g_ticks < DOOR_OPENNING_TIME){};

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Door is open");
	/*
	 * wait until door next state
	 */
	while (g_ticks < (DOOR_LEFT_OPEN_TIME+DOOR_OPENNING_TIME)){};

	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "Closing the Door...");
	/*
	 * wait until door next state
	 */
	while (g_ticks < (DOOR_CLOSING_TIME+DOOR_LEFT_OPEN_TIME + DOOR_OPENNING_TIME)){};

	//now door is closed
}


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
void Danger_Mission(void){
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks=0;
	LCD_clearScreen();
	LCD_displayStringRowColumn(0, 0, "DANGER !");
	LCD_displayStringRowColumn(1,0,"Someone Try Thieving");
	/*
	 * wait until finishing danger time
	 */
	while(g_ticks < DANGER_TIME ){};

}


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
void MisMatch_Answer(void){
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"Mis-Match Password");
	_delay_ms(500);
}



