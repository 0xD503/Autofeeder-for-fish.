/*
 * Autofeeder.h
 *
 * Created: 1/10/2018 5:33:09 PM
 *  Author: Developer
 */ 


#ifndef AUTOFEEDER_H_
#define AUTOFEEDER_H_

#include "main.h"

//#include <avr/sleep.h>

#define AUTOFEEDER_DDR DDRB
#define AUTOFEEDER_PORT PORTB
#define AUTOFEEDER_PINS PINB

#define AUTOFEEDER_LED_PIN_1 0
#define AUTOFEEDER_LED_PIN_2 1
#define AUTOFEEDER_LED_PIN_3 2
#define AUTOFEEDER_LED_PIN_4 4
#define AUTOFEEDER_LED_PIN_5 5
#define AUTOFEEDER_SERVO_PIN 3

extern volatile uint8_t feedCounter;

void AutoFeeder_Init (void);

#endif /* AUTOFEEDER_H_ */