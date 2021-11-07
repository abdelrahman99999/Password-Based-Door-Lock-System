 /******************************************************************************
 *
 * File Name: App_HMI.c
 *
 * Description: Door Locker Security System (Human Machine Interface micro-controller)
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#include "Application_support.h"

int main(void){
	/*Enable global interrupts*/
	SREG |=1<<7;

	/////////////////////////* initializations *////////////////////////////////////

	/*LCD Initialization */
	LCD_init();

	/*Timer Initialization */
	Timer0_SetCallBack( CallBack_Function );
	Timer0_ConfigType Timer_config={Timer0_CTC,Timer0_1024_Prescaler,0,250};
	Timer0_init(&Timer_config);

	/*Uart Initialization */
	Uart_ConfigType Uart_config={ Parity_Disabled,Single_stop_bit,Eight_bits,rate_9600};
	UART_init(&Uart_config);

	/////////////////////////* start of User interface *////////////////////////////////////

	/*
	 * First time to visit the program
	 */

	/*
	 * we receive 2 password(first password,confirm password) from user using keypad and send them
	 * 			to Control-Ecu to check the Matching
	 * if (2 password is matched) we save save password in EEPROM in Control-Ecu
	 * else we repeat the operation till getting matched 2 password(first password,confirm password)
	 *
	 */
	Set_Password();

	/*variable used to get the keypad button pressed by user */
	uint8 Keypad_PressedKey=0;

	/*
	 * variable used to receive special byte  by uart from control-Ecu
	 * can be : Opening_Door_Action
	 * 		  : Changing_Password_Action
	 * 		  : Passwords_NotMatch
	 * 		  : Danger
	 */
	uint8 state=0;

	/*
	 * temporary array to use it in application functions like
	 * Get_Password_From_User() or
	 * Send_Password_To_CTRL_ECU
	 */
	uint8 Entered_Password[PASSWORD_LENGTH];

	while(1){
		/*
		 * LCD showing HMI options that user can select from it
		 * 1- open door
		 * 2- changing password
		 */
		HMI_Options1();


		/*
		 * do not accept any pressed key - accept only '+' or '-'
		 * do not exit from do-while until the user enter '+' or '-'
		 * 	+ : Open_Door_Option
		 * 	- : Change_Password_Option
		 */
		do
		{
			Keypad_PressedKey = KEYPAD_getPressedKey();
		}while ( (Keypad_PressedKey != Open_Door_Option) && (Keypad_PressedKey != Change_Password_Option) );

		/* if the user Enter '+' to open the door */
		if (Keypad_PressedKey == Open_Door_Option)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0,"Enter The Password");

			/*
			 * get 5 numbers password from the user to check if he is the owner or not
			 * 	only accept numbers [0:9]
			 */
			Get_Password_From_User(Entered_Password);

			/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
			UART_sendByte(READY);

			/*send password to Control ECU to check if the entered password is same as the real application
			 * 		password that stored in the  Control-Ecu in the memory (EEPROM)
			 */
			Send_Password_To_CTRL_ECU(Entered_Password);

			/*
			 * send desired option which the user needed to execute
			 * in this case the option is Open_Door_Option
			 */
			UART_sendByte(Open_Door_Option);

			/*
			 * Receive from Control-Ecu if the password entered by the user is the same as real application password
			 * 		the received byte  may be 3 cases
			 * 1- in case of matching of 2 passwords (entered password,real application password)
			 * 		we will receive (Opening_Door_Action)
			 * 2- in case of Mis-matching of 2 passwords (entered password,real application password)
			 * 		in first try or second try
			 * 		we will receive (Passwords_NotMatch)
			 * 3- in case of Mis-matching of 2 passwords (entered password,real application password)
			 * 		in third try
			 * 		we will receive (Danger)
			 */
			state = UART_recieveByte();

			if (state == Opening_Door_Action )
			{
				/*
				 * in case of matching of 2 passwords
				 * start execution of opening and hold and closing Door mission
				 * LCD will display the state of the door in the Control-Ecu
				 */
				Opening_hold_Closing_Door();

			} else if (state == Passwords_NotMatch)
			{
				/*
				 * in case of mis-matching of 2 passwords for first and second try
				 * start execution of Mismatching Password Mission
				 * LCD will display some string to make the user know that
				 * 		the entered password is not the same of application password
				 */
				MisMatch_Answer();
			}else if(state ==Danger)
			{
				/*
				 * in case of mis-matching of 2 passwords for third try
				 * start execution of Danger Mission
				 * LCD will display some string to show that someone try thieving
				 */
				Danger_Mission();
			}

		}/* if the user Enter '-' to change password*/
		else if (Keypad_PressedKey == Change_Password_Option)
		{
			LCD_clearScreen();
			LCD_displayStringRowColumn(0, 0,"Enter The Password");
			/*
			 * get 5 numbers password from the user to check if he is the owner or not
			 * only accept numbers [0:9]
			 */
			Get_Password_From_User(Entered_Password);

			/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
			UART_sendByte(READY);

			/*send password to Control ECU to check if the entered password is same as the real application
			 * 		password that stored in the  Control-Ecu in the memory (EEPROM)
			 */
			Send_Password_To_CTRL_ECU(Entered_Password);

			/*
			 * send desired option which the user needed to execute
			 * in this case the option is Change_Password_Option
			 */
			UART_sendByte(Change_Password_Option);

			/*
			 * Receive from Control-Ecu if the password entered by the user is the same as real application password
			 * 		the received byte  may be 3 cases
			 * 1- in case of matching of 2 passwords (entered password,real application password)
			 * 		we will receive (Changing_Password_Action)
			 * 2- in case of Mis-matching of 2 passwords (entered password,real application password)
			 * 		in first try or second try
			 * 		we will receive (Passwords_NotMatch)
			 * 3- in case of Mis-matching of 2 passwords (entered password,real application password)
			 * 		in third try
			 * 		we will receive (Danger)
			 */
			state = UART_recieveByte();
			if (state == Changing_Password_Action)
			{
				/*
				 * in case of matching of 2 passwords
				 * start execution of Set Password mission to change application password
				 * 		and store it again in EEPROM in CONTROL ECU
				 * LCD will display the state of the door in the Control-Ecu
				 */
				Set_Password();

			} else if (state == Passwords_NotMatch)
			{
				/*
				 * in case of mis-matching of 2 passwords for first and second try
				 * start execution of Mismatching Password Mission
				 * LCD will display some string to make the user know that
				 * 		the entered password is not the same of the application password
				 */
				MisMatch_Answer();

			}else if(state == Danger)
			{
				/*
				 * in case of mis-matching of 2 passwords for third try
				 * start execution of Danger Mission
				 * LCD will display some string to show that someone try thieving
				 */
				Danger_Mission();

			}

		}

	}

    return 0;
}

