/*
 * Stopwatch.c
 *
 * Created: 28.10.2018 21:51:34
 *  Author: pino
 */ 

#define F_CPU 1000000UL


#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int numbers[] = {	0b1111110,
							0b0110000,
							0b1101101,
							0b1111001,
							0b0110011,
							0b1011011,
							0b1011111,
							0b1110000,
							0b1111111,
							0b1111011};
							
unsigned int data[] = {0,0,0};
							
unsigned int segment = 0;

ISR (TIMER0_OVF_vect) {	
	//asm("nop");
	
	switch (segment) {
		case 0: {			
			PORTB &= ~(1<<PB3);			
			PORTB |= (1<<PB1);
			PORTD = numbers[data[0]];			
			segment++;			
			break;
		}
		case 1: {
			PORTB &= ~(1<<PB1);
			PORTB |= (1<<PB2);
			PORTD = numbers[data[1]];
			segment++;
			break;
		}
		case 2: {
			PORTB &= ~(1<<PB2);
			PORTB |= (1<<PB3);
			PORTD = numbers[data[2]];
			segment = 0;
			break;
		}
	}
	
}

ISR (TIMER2_COMP_vect) {
	//PORTC ^= 1 << PC0;
	
	if (data[2] != 9) {
		data[2]++;
	} else {
		data[2] = 0;
		if (data[1] != 9) {
			data[1]++;
		} else {
			data[1] = 0;
			if (data[0] != 9) {
				data[0]++;
			}			
		}
	}
}

int main(void)
{	
	DDRB = 0b00001110;	// set pins 1-3 to output
	DDRD = 0xFF;		// set all port to output
	
	DDRC = 0b1;

	TCCR0 |= (1<<CS01); // 1/8
	
	TIMSK |= 1 << 0;
	
	TCCR2 |= (1<<CS22)|(1 << CS21)|(1 << CS20);	
	
	TCCR2 |= 1 << WGM21;
	TIMSK |= (1 << OCIE2);
	
	OCR2 = 255;
	
	asm("sei");
	
	while (1) {asm("sleep");}
}