/*
 * cli.c
 *
 */

#include "uart.h"
#include "timer.h"
//#include "dht11.h"
#include "cli.h"

static volatile uint8_t commandFlag = 0;
static char welcome[] = "Bienvenido\rON: para encender, OFF para apagar, RST para reiniciar\r";
static char invalid[] = "command invalido\r";

static int Get_Command(char command[])
{
	if (!strcmp(command, "ON"))
	{
		return 0; // el comando es on
	}
	if (!strcmp(command, "OFF"))
	{
		return 1; // el comando es off
	}
	if (!strcmp(command, "RST"))
	{
		return 2; // el comando es rst
	}
	return 3; // comando invalid
}

void CLI_Init()
{
	UART_SendMsg(welcome);
}

uint8_t CLI_GetCommandFlag()
{
	return commandFlag;
};

void CLI_cmd(char command[])
{

	int nroCommand = Get_Command(command);

	switch (nroCommand) // evaluo si recibi un comando o un numero invalido
	{
	case 0: // on
		commandFlag = 1;
	//	UART_SendMsg(DHT11_getMessage());
		TIMER_Enable();
		break;
	case 1: // off
		commandFlag = 0;
		UART_SendMsg("\rOff\r");
		TIMER_Disable();
		break;
	case 2: // reset
		UART_SendMsg(welcome);
		break;
	case 3: // comando invalid
		UART_SendMsg(invalid);
		break;
	}
}
