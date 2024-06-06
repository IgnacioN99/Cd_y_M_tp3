#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void DHT11_start();

volatile uint8_t state = 0;
volatile uint16_t pulse_us = 0;
volatile uint16_t time_ic = 0;

int main(void)
{	
	uint16_t apulso;
	DDRD |= (1 << PORTD0);
	
	DHT11_start();
    while (1)
    {
		if(state > 1)
		{
			apulso = pulse_us;
		}
		if( (PINB & (1 << PORTB0)) == 0)
		{
			PORTD |= (1 << PORTD0);
		}
    }
}

void DHT11_start()
{
	// Configurar IC del Timer 1
	TCCR1A = 0x00; // Normal
	TCCR1B = (1 << CS11); // prescaling 8, ICSE1 = 0 (LOW)
	
	// Enviar señal de start al modulo DHT11
	PORTB &= ~(1<<PORTB0);
	DDRB |= (1<<PORTB0);
	_delay_ms(18);
	DDRB &= ~(1<<PORTB0);
	
	TIMSK1 = (1 << ICIE1); // Habilitar interrupción de IC
	sei();
	TCNT1 = 0x0000;
}

ISR (TIMER1_CAPT_vect)
{	
	state++;
	TCCR1B ^= (1 << ICES1);
	if(state != 1)
	{
		pulse_us = ICR1 - time_ic;
		time_ic = ICR1;	
	}
	else
	{
		pulse_us = 0;
		time_ic = 0;
		TCNT1 = 0x0000;
	}
	
	//if(state >= 2)
	//{
		//cli();
	//}
}