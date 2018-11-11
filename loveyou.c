/*
 * loveyou.c
 *
 * Created: 11.11.2018 14:35:19
 *  Author: pino
 */ 

#define F_CPU 1000000UL

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int msg[] = {
	0b0110000,	// I	0
	0b0000000,	// _	1
	0b0001110,	// L	2
	0b1111110,	// O	3
	0b0110010,	// V	4
	0b1001111,	// E	5
	0b0000000,	// _	6
	0b0111011,	// Y	7
	0b1111110,	// O	8
	0b0111110,	// U	9
	0b0000000	// _	10
	
};

unsigned int point = 0;

unsigned int segment = 0;

ISR (TIMER0_OVF_vect) {
	/*
	unsigned int l1, l2, l3;
	
	l1 = msg[point];
	
	if (point == 8) {
		l2 = msg[point+1];
		l3 = msg[0];
	} else if (point == 9) {
		l2 = msg[0];
		l3 = msg[1];
	} else {
		l2 = msg[point+1];
		l3 = msg[point+2];
	}
	*/
	
	switch (segment) {
		case 0: {
			PORTB &= ~(1<<PB3);
			PORTB |= (1<<PB1);
			PORTD = msg[point%11];
			segment++;
			break;
		}
		case 1: {
			PORTB &= ~(1<<PB1);
			PORTB |= (1<<PB2);
			PORTD = msg[(point+1)%11];
			segment++;
			break;
		}
		case 2: {
			PORTB &= ~(1<<PB2);
			PORTB |= (1<<PB3);
			PORTD = msg[(point+2)%11];
			segment = 0;
			break;
		}
	}
}

ISR (TIMER2_COMP_vect) {
	if (point == 10)
		point = 0;
	else
		point++;
}

int main(void)
{    
    DDRB = 0b00001110;	// set pins 1-3 to output
    DDRD = 0xFF;		// set all port to output

	TCCR0 |= (1<<CS01); // 1/256		
	TIMSK |= 1 << 0;
	
	TCCR2 |= (1<<CS22)|(1 << CS21)|(1 << CS20);	
	TCCR2 |= 1 << WGM21;
	TIMSK |= (1 << OCIE2);
	
	OCR2 = 255;
		
	asm("sei");
		
	while (1) {asm("sleep");}    
}