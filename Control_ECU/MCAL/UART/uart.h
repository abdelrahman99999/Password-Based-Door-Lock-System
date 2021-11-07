 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "../../Utilities/std_types.h"
#include "../../Utilities/system_config.h"
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/


typedef enum
{
	Parity_Disabled,Even_Parity=2,Odd_Parity
}Uart_Parity;

typedef enum
{
	Single_stop_bit,Two_stop_bit
}Uart_Stop_Bit;

typedef enum
{
	Five_bits,Six_bits,Seven_bits,Eight_bits,Nine_bits=7
}Uart_Data_Bits;

typedef enum
{
	rate_10=10, rate_300=300, rate_600=600, rate_1200=1200, rate_2400=2400,
	rate_4800=4800, rate_9600=9600, rate_14400=14400, rate_19200=19200, rate_38400=38400,
	rate_57600=57600, rate_115200=115200, rate_128000=128000, rate_256000=256000
}Uart_BaudRate;



typedef struct
{
	Uart_Parity parity;
	Uart_Stop_Bit stop_bit;
	Uart_Data_Bits no_data_bits;
	Uart_BaudRate baud_rate;
}Uart_ConfigType;



/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const Uart_ConfigType *configtype );

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
