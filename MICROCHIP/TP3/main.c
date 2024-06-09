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
			char buffer[30]; // Buffer para almacenar la cadena de tiempo
			sprintf(buffer, "Hora actual: %02d:%02d:%02d\r\n", now.hours, now.minutes, now.seconds);
			UART_SendMsg(buffer); // Envía la cadena por UART
			_delay_ms(500);
			UART_SendMsg(DHT11_enviarInformacion());
		}
	}

	return 0;
}
