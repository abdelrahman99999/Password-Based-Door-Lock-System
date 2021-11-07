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
uint8 App_Password[PASSWORD_LENGTH];

/*******************************************************************************
 *                      Functions Definitions                                  *
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
uint8 Compare_Two_Passwords(uint8* password1,uint8* password2){
	/*
	 * variable used to count the number of matched numbers in
	 * 		two passwords
	 */
	uint8 tempcount=0;

	for(uint8 i=0;i<PASSWORD_LENGTH;i++){
			if(password1[i]==password2[i])tempcount++;
	}
	/*
	 * if the number of matched numbers equal Password lengt this means that
	 * 		2 passwords are matched
	 */
	if(tempcount==PASSWORD_LENGTH)return Passwords_Match;
	else return Passwords_NotMatch;
}


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
void Recieve_Password_From_HMI_ECU(uint8 *pass){
	/*
	 * receive password (array)
	 * byte by byte - number by number
	 */
	for(uint8 i=0;i<PASSWORD_LENGTH;i++){
		pass[i]=UART_recieveByte();
		_delay_ms(50);
	}
}


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
void Save_Pasword_IN_MEMMORY(uint8 *pass){
	/*
	 * looping on every number in (array parameter)password and store this number in specific
	 * 		address in memory
	 */
	for(uint8 i=0;i<PASSWORD_LENGTH;i++){
		EEPROM_writeByte(PASSWORD_STORED_ADRESS+i, pass[i]);
		App_Password[i]=pass[i];
		_delay_ms(50);
	}

}


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
void Set_Password(void){
	/*
	 * 2 temporary arrays to use them to get password from the user
	 *  PasswordTemp1: Entering of password(first password)
	 *  PasswordTemp2: confirmation of password
	 */
	uint8 FirstPassword[PASSWORD_LENGTH];
	uint8 SecondPassword[PASSWORD_LENGTH];

	/*
	 * variable used to store the state of 2 passwords received by from HMI-Ecu
	 * can only be 2 cases:
	 * 						1- Passwords_NotMatch
	 * 						2- Passwords_Match
	 */
	uint8 Password_State = Passwords_NotMatch;

	while(Password_State == Passwords_NotMatch){
		/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
		while (UART_recieveByte() != READY);
		UART_sendByte(READY);
		/*
		 * receive first password from HMI-Ecu in preparation to compare it
		 *  with the confirmation password to check Matching
		 */
		Recieve_Password_From_HMI_ECU(FirstPassword);

		/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
		while (UART_recieveByte() != READY);
		UART_sendByte(READY);

		/*
		 * receive confirm password from HMI-Ecu  to compare it
		 *  with the first password to check Matching
		 */
		Recieve_Password_From_HMI_ECU(SecondPassword);

		/*
		 * start operation of comparison between first password and second password
		 * 		and return the result of comparison in   Password_State
		 * 		result should be : Passwords_Match
		 * 						 : Passwords_NotMatch
		 *
		 */
		Password_State = Compare_Two_Passwords(FirstPassword, SecondPassword);

		/*communications between the 2 ECUs(HMI and Control )to check the Readiness   */
		UART_sendByte(READY);

		/*
		 * send 2 password state of Matching or not to HMI-Ecu to
		 * 		tell it to sending another 2 passwords(it will take them from user and sending them)
		 * 		as we will not exit from this function case only if 2 passwords are matched
		 */
		UART_sendByte(Password_State);

		if ( Password_State == Passwords_Match){
			/*
			 * in case of 2 passwords are matched we store any of them
			 * 		in the EEPROM
			 */
			Save_Pasword_IN_MEMMORY(FirstPassword);

		}
	}
}


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
void CallBack_Function(void){
	g_ticks++;
}


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
void Opening_hold_Closing_Door(void){
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks = 0;
	DcMotor_Rotate(motor_CW);
	/*
	 * wait until door next state
	 */
	while (g_ticks < DOOR_OPENNING_TIME){};


	DcMotor_Rotate(motor_stop);
	/*
	 * wait until door next state
	 */
	while (g_ticks < (DOOR_LEFT_OPEN_TIME+DOOR_OPENNING_TIME)){};



	DcMotor_Rotate(motor_A_CW);
	/*
	 * wait until door next state
	 */
	while (g_ticks < (DOOR_CLOSING_TIME+DOOR_LEFT_OPEN_TIME + DOOR_OPENNING_TIME)){};

	DcMotor_Rotate(motor_stop);
}


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
void Danger_Mission(){
	/*
	 * make g_ticks zero to start calculation and comparison of time
	 */
	g_ticks=0;
	BUZZER_ON();
	/*
	 * wait until finishing danger time
	 */
	while(g_ticks < DANGER_TIME);

	BUZZER_OFF();
}

