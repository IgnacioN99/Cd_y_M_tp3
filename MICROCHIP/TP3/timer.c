/*
 * timer.c
 */
#include "timer.h"

static volatile uint8_t flagClk = 0;
static uint8_t count = 0;

/************************************************************************
Utiliza el timer 1 de 16bits para lograr interrupciones cada 100ms
Debe contar hasta 25.000 con un prescaler de 64 para este fin,
logrando interrupciones precisas de 100ms;
************************************************************************/
void TIMER_init()
{
	// Configura el timer1 en modo CTC con prescaler 64
	TCCR1A = 0;
	TCCR1B = 0;
	TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10); // modo CTC, prescaler de 64
	OCR1A = 0x61A8;										// configura el contador de comparacion para 25.000
}

void TIMER_Enable()
{
	TCNT1 = 0;				 // Inicializa el contador
	TIMSK1 |= (1 << OCIE1A); // habilita interrupci�n por igualdad de comparaci�n
}

void TIMER_Disable()
{
	TIMSK1 &= ~(1 << OCIE1A); // deshabilita interrupci�n
}

uint8_t TIMER_GetClkFlag()
{
	return flagClk;
}

void TIMER_ClearClkFlag()
{
	flagClk = 0;
}

void TIMER_ClearCount()
{
	count = 0;
}

// Rutina de servicio para el timer que se activa "On Compare Match", definido en el timer a 100ms
ISR(TIMER1_COMPA_vect)
{
	count++;
	// 1 segundo
	if (count == 10)
	{
		count = 0;
		flagClk = 1;
	}
}
