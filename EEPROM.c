/*
 * EEPROM.c
 *
 * Created: 1/31/2018 4:16:47 PM
 *  Author: Developer
 */ 

#include "EEPROM.h"

volatile static uint16_t EEPROM_PutToAddress;
volatile static int8_t EEPROM_ByteToPut;


void EEPROM_PutByte (int8_t byte, uint16_t address)
{
	EEPROM_PutToAddress = address;
	EEPROM_ByteToPut = byte;
	
	EECR = 1 << EERIE;
}

int8_t EEPROM_GetByte (uint16_t address)
{
	while (EECR & (1 << EEPE));
	
	EEAR = address;
	EECR |= 1 << EERE;
	
	return (EEDR);
}

void EEPROM_WriteString (int8_t* string, uint16_t address)
{
	
}

void EEPROM_ReadString (int8_t* placeToReceive, uint16_t address)
{
// 	for (uint16_t i = 0; placeToReceive[i]; i++)
// 	{
// 		placeToReceive[i] = EEPROM_GetByte(address++);
// 	}
}


ISR (EE_READY_vect)
{
	EEAR = EEPROM_PutToAddress;					// 	3. Write new EEPROM address to EEAR (optional).
	EEDR = EEPROM_ByteToPut;					// 	4. Write new EEPROM data to EEDR (optional).
	
	EECR &= ~(1 << EERIE);						//	
	
	EECR |= 1 << EEMPE;							// 	5. Write a '1' to the EEMPE bit while writing a zero to EEPE in EECR.
	EECR |= 1 << EEPE;							// 	6. Within four clock cycles after setting EEMPE, write a '1' to EEPE.
}