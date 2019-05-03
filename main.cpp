/*-----------------------------------------------------------------------------
                            _______ _    _  ______      
                            |_____|  \  /  |_____/      
                            |     |   \/   |    \_   
                            
                    _______ _____ _______ _______  ______
                       |      |   |  |  | |______ |_____/
                       |    __|__ |  |  | |______ |    \_   
					   
--------------------------------------------------------------------------------
OVERVIEW
--------------------------------------------------------------------------------
  This implements a timer via an interrupt on the Atmega328P and flashes the
  built in LED on an Arduino Nano development board.
--------------------------------------------------------------------------------
VERSION HISTORY
--------------------------------------------------------------------------------
  VERSION         DATE         AUTHOR                DESCRIPTION
  -------         ----         ------                -----------
  1.0             03/05/19     J. Ingram-Wright      Initial version
------------------------------------------------------------------------------*/


#include<avr/io.h>
#include<avr/interrupt.h>

#define BUILT_IN_LED PB5
// Desired time between interrupts.
#define TIMER_PERIOD 0.5
// Overflow time with a 1024 prescaler.
#define TIMER_RESOLUTION 6.4e-5


void initialiseGpio()
{
	// Set BUILT_IN_LED pin as output.
	DDRB = (1 << BUILT_IN_LED);
}


void initialiseTimer()
{
	// Disable global interrupts.
	cli();
	
	// Set TCCR1A and TCCR1B registers to 0.
	TCCR1A = 0;
	TCCR1B = 0;
	
	// Calculate timerCount for use in the match register.
	// NOTE: The "- 1" is to account for the clock cycle used to perform the
	//       reset to 0.
	int timerCount = (TIMER_PERIOD / TIMER_RESOLUTION) - 1.0;
	
	// Set compare match register to desired timer count.
	OCR1A = timerCount;
	// Turn on CTC mode.
	TCCR1B |= (1 << WGM12);
	// Set CS10 and CS12 bits for 1024 prescaler.
	TCCR1B |= (1 << CS10);
	TCCR1B |= (1 << CS12);
	// Enable Timer1 compare interrupt.
	TIMSK1 = (1 << OCIE1A);
	
	// Enable global interrupts.
	sei();
}


int main()
{	
	initialiseGpio();
	initialiseTimer();
	
	while(1)
	{
		
	}
}


ISR (TIMER1_COMPA_vect)
{
	// Invert BUILT_IN_LED pin.
	PORTB ^= (1 << BUILT_IN_LED);
}