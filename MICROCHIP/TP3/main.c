/*
 * TP3.c
 *
 */
#include "utils.h"
#include "rtc.h"
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "cli.h"
#include "dht11.h"

int main(void)
{
	UART_Init(); // Inicializa el UART
	rtc_init();	 // Inicializa el RTC
	TIMER_init();
	TIMER_Enable();
	// CLI_Init();
	RTC_Time now;

	sei();
	while (1)
	{
		/*
		if (UART_GetCmdFlag()) // Recibo un comando
		{
			UART_ClearCmdFlag();
			CLI_cmd(UART_ReadBuffer());
			UART_Enable_Rx();
		}*/

		if (TIMER_GetClkFlag())
		{
			TIMER_ClearClkFlag();
			now = rtc_get_time();
			char buffer[54]; // Buffer para almacenar el mensaje
			char time[33];
			strcpy(buffer, DHT11_enviarInformacion());
			sprintf(time, "FECHA: %02d/%02d/%02d HORA: %02d:%02d:%02d\r\n", now.date, now.month, now.year,now.hours, now.minutes, now.seconds);
			strcat(buffer, time);
			UART_SendMsg(buffer); // Envía la cadena por UART
		}
	}

	return 0;
}
