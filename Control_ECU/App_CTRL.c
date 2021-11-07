 /******************************************************************************
 *
 * File Name: App_CTRL.c
 *
 * Description: Door Locker Security System (Control micro-controller)
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/
#include "Application_support.h"


int main(void){
	/*Enable global interrupts*/
	SREG |=1<<7;

	/////////////////////////* initializations *////////////////////////////////////

	/*DC Motor Initialization */
	DcMotor_Init();

	/*Timer Initialization */
	Timer0_SetCallBack( CallBack_Function );
	Timer0_ConfigType Timer_config={Timer0_CTC,Timer0_1024_Prescaler,0,250};
	Timer0_init(&Timer_config);

	/*Buzzer Initialization */
	BUZZER_init();

	/*Uart Initialization */
	Uart_ConfigType Uart_config={ Parity_Disabled,Single_stop_bit,Eight_bits,rate_9600};
	UART_init(&Uart_config);

	/////////////////////////* start of control *////////////////////////////////////

	/*
	 * First time to visit the program
	 */

	/*
	 * we receive 2 password(first password,confirm password) from HMI-ECU by Uart to compare
	 * 		and check the Matching
	 *  if (2 password is matched) we send message to HMI-Ecu to tell it that 2 passwords are matched
	 *  		and we save password in EEPROM
	 *  else we repeat the operation till received 2 password are matched
	 *
	 */
	Set_Password();


	/*
	 * temporary array to use it in application functions like
	 * Recieve_Password_From_HMI_ECU() or
	 * Compare_Two_Passwords()
	 */
	uint8 recieved_password[PASSWORD_LENGTH];

	/*
	 * variable used to count the number of MisMatching between received Password and Application password
	 */
	uint8 no_of_wrong_entered_password=0;

	/*
	 * variable used to store receive option  needed by user
	 * can be: Open_Door_Option
	 *    	 : Change_Password_Option
	 */
		uint8 option=0;

	/*
	 * variable used to store the result of Comparison between received Password and Application password
	 * can be : Passwords_Match
	 * 		  : Passwords_NotMatch
	 */
		uint8 state=0;


	while(1){

		/*
		 * check if HMI-ECU will send password that we will compare it with Application Password
		 * HMI-Ecu send ready byte to Control-Ecu before sending password
		 * 		to tell it that  HMI-Ecu will send password next
		 */
		if (UART_recieveByte() == READY){
			/*
			 * Receive 5 numbers password from HMI by UART
			 * to check if received password is equal to the application password
			 */
			Recieve_Password_From_HMI_ECU(recieved_password);

			/*
			 * receive option desired by user in HMI-ECU
			 * may be   '+' that means  Open_Door_Option
			 * 			or
			 * 			'-' that means Change_Password_Option
			 */
			option = UART_recieveByte();

			/* if the option is '+'	 */
			if ( option == Open_Door_Option){
				/*
				 * doing comparison operation between real Application password and received password
				 * 		and store the result of comparison operation in the state Variable
				 * result of comparison operation may be: Passwords_Match
				 * 										: Passwords_NotMatch
				 */
				state=Compare_Two_Passwords(App_Password, recieved_password);

				/*if the received password is matched with Application Password*/
				if (state == Passwords_Match){

					/*
					 * sending to HMI-ECU that password matched and tell it that
					 * 		the door will be opening so HMI can show the state of Door
					 *  	(opening hold closing)
					 */
					UART_sendByte(Opening_Door_Action);

					/*
					 * reset the counter of wrong password received from HMI by user
					 * as if one matched password recieved will break the counter to start again from zero
					 */
					no_of_wrong_entered_password=0;

					/*
					 * start execution of operations on the Door
					 * 		open the door
					 * 		then hold it for some time
					 * 		then close it
					 */
					Opening_hold_Closing_Door();

				}/*if the received password is Mis-matched with Application Password*/
				else if (state == Passwords_NotMatch){
					/*
					 * Increment the counter of wrong password received from HMI by user
					 * 		to use it in case repeated Mismatched password(may be thief)
					 */
					no_of_wrong_entered_password++;

					/*if we received Mismatched passwords for third try	 */
					if (no_of_wrong_entered_password == Max_Number_Of_Allowed_NotCorrect_Password){
						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application for 3 times consecutively
						 * 		so the lCD in HMI-ECU show state of Danger
						 */
						UART_sendByte(Danger);

						/*
						 * start execution of Danger mission
						 * 		buzzer will work by high sound to show the danger state
						 */
						Danger_Mission();

						/*
						 * reset the counter of wrong password received from HMI by user
						 */
						no_of_wrong_entered_password=0;

					}/*if we received Mismatched passwords for first or second try	 */
					else{
						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application
						 */
						UART_sendByte(Passwords_NotMatch);
					}
				}
			} /* if the option is '-'	 */
			else if (option == Change_Password_Option) {

				/*
				 * doing comparison operation between real Application password and received password
				 * 		and store the result of comparison operation in the state Variable
				 * result of comparison operation may be: Passwords_Match
				 * 										: Passwords_NotMatch
				 */
				state=Compare_Two_Passwords(App_Password, recieved_password);

				/*if the received password is matched with Application Password*/
				if ( state== Passwords_Match) {
					/*
					 * sending to HMI-ECU that password matched and tell it to change password
					 */
					UART_sendByte(Changing_Password_Action);

					/*
					 * reset the counter of wrong password received from HMI by user
					 * as if one matched password recieved will break the counter to start again from zero
					 */
					no_of_wrong_entered_password=0;

					/*
					 * we receive 2 password(first password,confirm password) from HMI-ECU by Uart to compare
					 * 		and check the Matching
					 *  if (2 password is matched) we send message to HMI-Ecu to tell it that 2 passwords are matched
					 *  		and we save password in EEPROM
					 *  else we repeat the operation till received 2 password are matched
					 *
					 */
					Set_Password();

				}/*if the received password is Mis-matched with Application Password*/
				else if(state==Passwords_NotMatch){

					/*
					 * Increment the counter of wrong password received from HMI by user
					 * 		to use it in case repeated Mismatched password(may be thief)
					 */
					no_of_wrong_entered_password++;

					/*if we received Mismatched passwords for third try	 */
					if (no_of_wrong_entered_password == Max_Number_Of_Allowed_NotCorrect_Password){

						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application for 3 times consecutively
						 * 		so the lCD in HMI-ECU show state of Danger
						 */
						UART_sendByte(Danger);

						/*
						 * start execution of Danger mission
						 * 		buzzer will work by high sound to show the danger state
						 */
						Danger_Mission();

						/*
						 * reset the counter of wrong password received from HMI by user
						 */
						no_of_wrong_entered_password=0;

					}/*if we received Mismatched passwords for first or second try	 */
					else{

						/*
						 * sending password state to HMI-ECU telling that the entered password by user
						 * 		is mismatched with the real password application
						 */
						UART_sendByte(Passwords_NotMatch);
					}
				}
			}
		}
	}

		return 0;

}
