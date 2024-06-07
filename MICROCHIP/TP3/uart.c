#include "uart.h"

#define USART_BAUDRATE 9600 // Desired Baud Rate
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0 << UMSEL00) // USART Mode Selection

#define DISABLED (0 << UPM00)
#define PARITY_MODE DISABLED // USART Parity Bit Selection

#define ONE_BIT (0 << USBS0)
#define STOP_BIT ONE_BIT // USART Stop Bit Selection

#define EIGHT_BIT (3 << UCSZ00)
#define DATA_BIT EIGHT_BIT // USART Data Bit Selection

static char buffer_tx[10];
static char buffer_rx[10];
uint8_t cmd_flag = 0;

void UART_Init()
{
	// Establecer baud rate
	UBRR0H = BAUD_PRESCALER >> 8;
	UBRR0L = BAUD_PRESCALER;

	// Establecer formato de trama
	UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT;

	/*Permitir que la USART sobreescriba la operacion normal en los pines TX y RX*/
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	// Activar interrupciones para la recepcion
	UART_Enable_Rx();
	UART_Enable_Tx();
}

/*
 * Control de interrupciones de transmision y recepcion
 */
void UART_Disable_Rx()
{
	UCSR0B &= ~(1 << RXCIE0);
}

void UART_Enable_Rx()
{
	UCSR0B |= (1 << RXCIE0);
}

void UART_Disable_Tx()
{
	UCSR0B &= ~(1 << UDRIE0);
}

void UART_Enable_Tx()
{
	UCSR0B |= (1 << UDRIE0);
}

void UART_SendMsg(char *msg)
{
	strcpy(buffer_tx, msg);
	UART_Enable_Tx();
}

/*
 * Lee el buffer de la UART
 */
char *UART_ReadBuffer()
{
	return buffer_rx;
}

uint8_t UART_GetCmdFlag()
{
	return cmd_flag;
}

void UART_ClearCmdFlag()
{
	cmd_flag = 0;
}

/*
 * Interrupcion de recepcion
 * Disparada por la UART cuando el buffer esta listo para ser leido
 */
ISR(USART_RX_vect)
{
	static volatile uint8_t i = 0;
	unsigned char dato;

	dato = UDR0;

	/* Detectar fin de linea */
	if (dato == '\r')
	{
		buffer_rx[i] = '\0';
		i = 0;
		cmd_flag = 1;
		UART_Disable_Rx();
	}
	/* Detectar borrado de caracter */
	else
	{
		if (dato == '\b')
			i--;
		else
		{
			buffer_rx[i] = dato;
			i++;
		}
	}
}
/*
* The data register empty (UDREn) flag indicates whether the transmit buffer is ready to receive new data.
This bit is set when the transmit buffer is empty, and cleared when the transmit buffer
contains data to be transmitted that has not yet been moved into the shift register.
* Cargo el caracter de mensaje a enviar hasta alcanzar el fin del mensaje
*/
ISR(USART_UDRE_vect)
{
	cli();
	static volatile uint8_t i = 0;
	if (buffer_tx[i] != '\0')
	{
		UDR0 = buffer_tx[i];
		i++;
	}
	else
	{
		i = 0;
		// al no escribir el registro udr0 es necesario desactivar
		// las interrupciones para evitar que se produzca otra interrupcion
		UART_Disable_Tx();
	}
	sei();
}