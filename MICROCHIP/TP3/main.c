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
	TIMER_Enable();

	sei();
	while (1)
	{

		if (TIMER_GetClkFlag())
		{
			TIMER_ClearClkFlag();
			char buffer[54]; // Buffer para almacenar el mensaje
			strcpy(buffer, DHT11_enviarInformacion());
			strcat(buffer, RTC_enviarInformacion());
			UART_SendMsg(buffer); // Envía la cadena por UART
		}
	}

	return 0;
}
