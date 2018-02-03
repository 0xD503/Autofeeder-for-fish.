#include "main.h"


void IO_Init (void)
{
	
}

void setup (void)
{
	IO_Init();
//	LCD_Init();
	AutoFeeder_Init();
	
//	Power consumption preparing:
//	Disabling digital input buffers
	DIDR0 |= 1 << ADC5D | 1 << ADC4D | 1 << ADC3D | 1 << ADC2D | 1 << ADC1D | 1 << ADC0D;							//	For an ADC
	DIDR1 |= 1 << AIN1D | 1 << AIN0D;																				//	For an Analog Comparator
	
//	Power down an analog comparator, if it isn't required
	ACSR |= 1 << ACD;
	
//	
	PRR |= 1 << PRTWI | 1 << PRTIM2 | 1 << PRTIM0 | 0 << PRTIM1 | 1 << PRSPI | 1 << PRUSART0 | 1 << PRADC;			//	Shut down TWI module, Timer 2, Timer 0, SPI (with debugWIRE), USART clock, ADC
	
//	Enable interrupts
	sei();
}

int main (void)
{
	
	setup();
	
	
	
	while (1)
	{
// 		LCD_SetPosition(1, 1);
// 
// 		LCD_PutByte((feedCounter / 100) + ASCII_NULL);
// 		LCD_PutByte(((feedCounter % 100) / 10) + ASCII_NULL);
// 		LCD_PutByte(((feedCounter % 100) % 10) + ASCII_NULL);
// 
// 		_delay_ms(1000);
		
		
		
	//	Set sleep mode - IDLE
		SMCR &= ~(1 << SM2 | 1 << SM1 | 1 << SM0);
	//	Before go to the IDLE mode, disable BOD by software
	//	Writing to the BODS bit is controlled by a timed sequence and an enable bit.
		MCUCR |= 1 << BODS | 1 << BODSE;			//	Timed sequence
		MCUCR |= 1 << BODS;							//	Timed sequence
		MCUCR &= ~(1 << BODSE);						//	Timed sequence
	// And now a sleep instruction must be executed within 3 cycles, while BODS = 1;
		SMCR |= 1 << SE;
		asm ("SLEEP");
	}
}