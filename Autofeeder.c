#include "Autofeeder.h"

static volatile uint32_t currentSecond = 0;						//	Variable, which stores current second value
//static const uint32_t period = 169800;							//	Feeding period
volatile uint8_t feedCounter = 0;
static const uint32_t period = 1;								//	Variable for debugging,

void AutoFeeder_SetPulse (uint8_t pulse_time)
{
	TCCR2A |= 1 << COM2A1 | 0 << COM2A0 | 0 << COM2B1 | 0 << COM2B0 | 1 << WGM21 | 1 << WGM20;		//	Fast PWM, normal mode
	TCCR2B |= 0 << WGM22 | 1 << CS22 | 1 << CS21 | 1 << CS20;										//	Clock prescaler = 1024
	OCR2A = pulse_time;																				//	Set pulse width
}

void AutoFeeder_StopTheRotation (void)
{
	TCCR2A &= ~(1 << COM2A1 | 0 << COM2A0 | 0 << COM2B1 | 0 << COM2B0 | 1 << WGM21 | 1 << WGM20);	//	Turn off PWM
	TCCR2B &= ~(0 << WGM22 | 1 << CS22 | 1 << CS21 | 1 << CS20);									//
}

void AutoFeeder_Timer1_Init (void)
{
	OCR1AH = 0x3D;												//	F_CPU / Clock prescaler = 15625 clock cycles
	OCR1AL = 0x09;												//	This is 1 second
	
	TCCR1B |= 1 << WGM12 | 1 << CS12 | 1 << CS10;				//	Set prescaler = 1024; Set Timer Mode - CTC
	TIMSK1 |= 1 << OCIE1A;										//	Enable Interrupt on compare
}

void AutoFeeder_Init (void)
{
	AUTOFEEDER_DDR |= 1 << AUTOFEEDER_SERVO_PIN;
	
	AutoFeeder_SetPulse(8);																																							//	Set initial servo position
	_delay_ms(1500);
	AutoFeeder_StopTheRotation();
	
	AutoFeeder_Timer1_Init();																																						//	Turn on time counter
	
	AUTOFEEDER_DDR &= ~(1 << AUTOFEEDER_SERVO_PIN);
}


ISR (TIMER1_COMPA_vect)
{
	SMCR &= ~(1 << SE);																				//	To avoid the MCU entering the sleep mode, it is recommended to clear the SE bit immediately after waking up.
	uint32_t temporarySecond = currentSecond;
	if (temporarySecond < period)
	{
		temporarySecond++;																			//	Count the time to the desired value
		currentSecond = temporarySecond;
	}
	else																							//	And when the time has come
	{
		AUTOFEEDER_DDR |= 1 << AUTOFEEDER_SERVO_PIN | 1 << AUTOFEEDER_LED_PIN_1 | 1 << AUTOFEEDER_LED_PIN_2 | 1 << AUTOFEEDER_LED_PIN_3 | 1 << AUTOFEEDER_LED_PIN_4 | 1 << AUTOFEEDER_LED_PIN_5;	//	Set outputs for LED and servo pins
		
		for (uint8_t i = 0; i < 3; i++)																																								//	Turn on the cycle of LED alert
		{
			for (uint8_t j = 0; j < 17; j++)
			{
				AUTOFEEDER_PORT |= 1 << AUTOFEEDER_LED_PIN_1 | 1 << AUTOFEEDER_LED_PIN_2 | 1 << AUTOFEEDER_LED_PIN_3 | 1 << AUTOFEEDER_LED_PIN_4 | 1 << AUTOFEEDER_LED_PIN_5;
				_delay_ms(200);
				AUTOFEEDER_PORT &= ~(1 << AUTOFEEDER_LED_PIN_1 | 1 << AUTOFEEDER_LED_PIN_2 | 1 << AUTOFEEDER_LED_PIN_3);
				_delay_ms(70);
			}
			_delay_ms(1200);
		}
		
		AUTOFEEDER_PORT |= 1 << AUTOFEEDER_LED_PIN_1 | 1 << AUTOFEEDER_LED_PIN_2 | 1 << AUTOFEEDER_LED_PIN_3 | 1 << AUTOFEEDER_LED_PIN_4 | 1 << AUTOFEEDER_LED_PIN_5;				//	Turn on LED lighting
		_delay_ms(1500);
		
		
		
		PRR &= ~(1 << PRTIM2);																																						//	Enable the Timer 2
		for (uint8_t i = 0; i < 1; i++)																																				//	Rotate the servo i times
		{
			AutoFeeder_SetPulse(32);
			_delay_ms(1490);
			AutoFeeder_StopTheRotation();
			_delay_ms(100);
			AutoFeeder_SetPulse(8);
			_delay_ms(1490);
			AutoFeeder_StopTheRotation();
			_delay_ms(2000);
		}
		_delay_ms(30000);																																												//	Leave the lighting
		AUTOFEEDER_PORT &= ~(1 << AUTOFEEDER_LED_PIN_1 | 1 << AUTOFEEDER_LED_PIN_2 | 1 << AUTOFEEDER_LED_PIN_3 | 1 << AUTOFEEDER_LED_PIN_4 | 1 << AUTOFEEDER_LED_PIN_5);								//	Turn off lighting
		AUTOFEEDER_DDR &= ~( 1 << AUTOFEEDER_SERVO_PIN | 1 << AUTOFEEDER_LED_PIN_1 | 1 << AUTOFEEDER_LED_PIN_2 | 1 << AUTOFEEDER_LED_PIN_3 | 1 << AUTOFEEDER_LED_PIN_4 | 1 << AUTOFEEDER_LED_PIN_5);	//	Set PORT in Hi-Z statement
		
		currentSecond = 0;																																												//	Reset timer
		
		uint8_t temporaryFeedCounter = feedCounter;
		temporaryFeedCounter++;
		feedCounter = temporaryFeedCounter;
		EEPROM_PutByte(temporaryFeedCounter, 1);																													//	Write in the EEPROM memory, that the feed cycle was successful
	}
}