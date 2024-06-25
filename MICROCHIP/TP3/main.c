/*
 * TP3.c
 *
 */
#include "utils.h"
#include "rtc.h"
#include "uart.h"
#include "timer.h"
#include "dht11.h"

int main(void)
{
	UART_Init(); // Inicializa el UART
	rtc_init();	 // Inicializa el RTC
	TIMER_init(); // Inicializa el Timer
	TIMER_Enable(); //Inicia el contador y habilita las interrupciones del timer

	sei();
	while (1)
	{

		if (TIMER_GetClkFlag())
		{
			TIMER_ClearClkFlag(); //Limpia el valor del flag del timer
			char buffer[54]; // Buffer para almacenar el mensaje
			strcpy(buffer, DHT11_enviarInformacion()); //Copia el string con la informacion del dht11 a buffer
			strcat(buffer, RTC_enviarInformacion()); //Concatena al string del buffer la informacion de la hora del rtc
			UART_SendMsg(buffer); // Envía la cadena por UART
		}
	}

	return 0;
}
