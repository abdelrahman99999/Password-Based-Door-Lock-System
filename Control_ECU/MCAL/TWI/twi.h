 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Header file for the TWI(I2C) AVR driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/ 

#ifndef TWI_H_
#define TWI_H_

#include "../../Utilities/std_types.h"
#include "../../Utilities/system_config.h"
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/

/* I2C Status Bits in the TWSR Register */
#define TWI_START         0x08 /* start has been sent */
#define TWI_REP_START     0x10 /* repeated start */
#define TWI_MT_SLA_W_ACK  0x18 /* Master transmit ( slave address + Write request ) to slave + ACK received from slave. */
#define TWI_MT_SLA_R_ACK  0x40 /* Master transmit ( slave address + Read request ) to slave + ACK received from slave. */
#define TWI_MT_DATA_ACK   0x28 /* Master transmit data and ACK has been received from Slave. */
#define TWI_MR_DATA_ACK   0x50 /* Master received data and send ACK to slave. */
#define TWI_MR_DATA_NACK  0x58 /* Master received data but doesn't send ACK to slave. */


/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	TWI_Prescaler_1,TWI_Prescaler_4,TWI_Prescaler_16,TWI_Prescaler_64
}TWI_Prescaler;


typedef struct{
	uint8 BitRateRegister_TWBR;

	TWI_Prescaler prescaler;

	uint8 My_Address;

}TWI_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * function used for initialization of of TWI using Configuration structure
 */
void TWI_init(const TWI_ConfigType* config_ptr );

/*
 * function used to Send the Start Bit
 */
void TWI_start(void);

/*
 * function used to Send the stop Bit
 */
void TWI_stop(void);

/*
 * function used to write one data byte
 */
void TWI_writeByte(uint8 data);

/*
 * function used to read one data byte with Acknowledge
 */
uint8 TWI_readByteWithACK(void);

/*
 * function used to read one data byte with NACK
 */
uint8 TWI_readByteWithNACK(void);

/*
 * function used to get status of TWI module
 */
uint8 TWI_getStatus(void);


#endif /* TWI_H_ */
