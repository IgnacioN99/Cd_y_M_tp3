/*
 * cli.c
 *
 */

#include "uart.h"
#include "timer.h"
#include "cli.h"

static volatile uint8_t commandFlag = 0;
static char invalid[] = "command invalido\r";

static int Get_Command(char command[])
{
	if (!strcasecmp(command, "s"))
	{
		return 0;
	}
	return 3; // comando invalid
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
	case 0:
		commandFlag = !commandFlag;
		if (commandFlag){
			TIMER_Enable();
		}else{
			TIMER_Disable();
		}
		break;
	case 3: // comando invalid
		UART_SendMsg(invalid);
		break;
	}
}
