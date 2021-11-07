 /******************************************************************************
 *
 * Module: TWI(I2C)
 *
 * File Name: twi.h
 *
 * Description: Source file for the TWI(I2C) AVR driver
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/
 
#include "twi.h"

#include "../../Utilities/common_macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * function used for initialization of of TWI using Configuration structure
 */
void TWI_init(const TWI_ConfigType* config_ptr )
{
	TWBR = config_ptr->BitRateRegister_TWBR;

	TWSR = ( TWSR & (0XFC) ) | (config_ptr->prescaler);
	
    /* Two Wire Bus address my address if any master device want to call me:
     * bits(7:1) 0x1 (used in case this MC is a slave device)
     * bit 0: General Call Recognition: Off
     */
    TWAR = config_ptr->My_Address << 1; // my address = 0x01 :)
	
    TWCR = (1<<TWEN); /* enable TWI */
}

/*
 * function used to Send the Start Bit
 */
void TWI_start(void)
{
    /* 
	 * Clear the TWINT flag before sending the start bit TWINT=1 (not cleared automatically)
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    
    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * function used to Send the stop Bit
 */
void TWI_stop(void)
{
    /* 
	 * Clear the TWINT flag before sending the stop bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

/*
 * function used to write one data byte
 */
void TWI_writeByte(uint8 data)
{
    /* Put data On TWI data Register */
    TWDR = data;
    /* 
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}

/*
 * function used to read one data byte with Acknowledge
 */
uint8 TWI_readByteWithACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable sending ACK after reading or receiving data TWEA=1
	 * Enable TWI Module TWEN=1 
	 */ 
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 * function used to read one data byte with NACK
 */
uint8 TWI_readByteWithNACK(void)
{
	/* 
	 * Clear the TWINT flag before reading the data TWINT=1
	 * Enable TWI Module TWEN=1 
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
    return TWDR;
}

/*
 * function used to get status of TWI module
 */
uint8 TWI_getStatus(void)
{
    uint8 status;
    /* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
    status = TWSR & 0xF8;
    return status;
}
