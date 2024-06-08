/*
 * TP3.c
 *
 * Created: 6/6/2024 18:34:12
 * Author : Ignacio
 */
#include "utils.h"
#include "rtc.h"
#include <stdio.h>
#include "uart.h"
#include "timer.h"
#include "cli.h"

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
			UART_SendMsg(buffer); // Env�a la cadena por UART
		}
	}

	return 0;
}
