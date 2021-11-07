 /******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.h
 *
 * Description: Header file for the External EEPROM Memory
 *
 * Author: Abdelrahman Elsayed
 *
 *******************************************************************************/


#ifndef EXTERNAL_EEPROM_H_
#define EXTERNAL_EEPROM_H_

#include "../../Utilities/std_types.h"
#include "../../Utilities/system_config.h"
/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/
#define ERROR 0
#define SUCCESS 1


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * function responsible for initialization of EEPROM
 */
void EEPROM_init(void);

/*
 * function responsible for writing 1byte of data in specific memory address
 */
uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data);

/*
 * function responsible for reading 1byte of data from specific memory address
 */
uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data);
 
#endif /* EXTERNAL_EEPROM_H_ */
