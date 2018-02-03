/*
 * EEPROM.h
 *
 * Created: 1/31/2018 4:16:13 PM
 *  Author: Developer
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include "main.h"

void EEPROM_PutByte (int8_t byte, uint16_t address);
int8_t EEPROM_GetByte (uint16_t address);
// void EEPROM_WriteString (int8_t* string, uint16_t address);
// void EEPROM_ReadString (int8_t* placeToReceive, uint16_t address);

#endif /* EEPROM_H_ */