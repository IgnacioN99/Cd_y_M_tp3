#include "uart.h"
#include "timer.h"

#define USART_BAUDRATE 9600 // Velocidad de baudios deseada (velocidad de tansferencia)
#define BAUD_PRESCALER (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

#define ASYNCHRONOUS (0 << UMSEL00) // USART Mode Selection

#define DISABLED (0 << UPM00)
#define PARITY_MODE DISABLED // USART Parity Bit Selection

#define ONE_BIT (0 << USBS0)
#define STOP_BIT ONE_BIT // USART Stop Bit Selection

#define EIGHT_BIT (3 << UCSZ00)
#define DATA_BIT EIGHT_BIT // USART Data Bit Selection

static char buffer_tx[10]; // Buffer de transmisión
uint8_t cmd_flag = 1; //Flag para controlar si se presion 'S' o 's'

void UART_Init()
{
	// Establecer baud rate
	UBRR0L = BAUD_PRESCALER; //103, pero el numero calculado no es un numero entera ya que posee decimales, por lo que habra un pequeño error y no sera exactamente 9600 bps

	// Establecer formato de trama
	UCSR0C = ASYNCHRONOUS | PARITY_MODE | STOP_BIT | DATA_BIT; // Establezco Modo asincrono, paridad desactivada, 1 bit de stop, tamaño del dato 8 bits

	//Permitir que la USART sobreescriba la operacion normal en los pines TX y RX
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	
	// Activar interrupciones para la transmicion y recepcion
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
* Interrupcion de transimision

* EL flag de UDRE0 se activa cuando el UDR0 está listo para recibir nuevos datos.
*
* Este bit se establece cuando UDR0 está vacío, y se borra cuando el UDR0 contiene
* datos que aún no han sido movidos al registro de desplazamiento para ser transmitidos.

* Cargo el caracter de mensaje a enviar hasta alcanzar el fin del mensaje
*/
ISR(USART_UDRE_vect)
{
	cli();//Se desabilitan las interrupciones
	static volatile uint8_t i = 0;
	if (buffer_tx[i] != '\0')
	{
		UDR0 = buffer_tx[i]; //Guardo el caracter del buffer de transmision en el registro de datos
		i++;
	}
	else
	{
		i = 0;
		UART_Disable_Tx();//Es necesario desactivar las interrupciones para evitar que se produzca otra interrupcion cuando sea leido ya que no se vacio UDR0
	}
	sei();//Se habilitan las interrupciones
}

/*
 * Interrupcion de recepcion
 * Disparada por la UART cuando hay un nuevo dato en UDR0 listo para ser leido
 * Se guarda el valor y revisa que no sea "S" o "s"
 */
ISR(USART_RX_vect)
{
	char dato;

	dato = UDR0; //Guardo el caracter del registro de datos en una variable
	
	if(dato == 's' || dato == 'S'){
		if (cmd_flag){
			TIMER_Disable(); //Desactivo el contador del timer
			cmd_flag = 0;
		}else{
			TIMER_Enable(); //Vuelvo a activar el contador del timer
			cmd_flag = 1;
		}
	}

}
