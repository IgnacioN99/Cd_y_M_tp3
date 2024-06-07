#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>

#include "lcd.h"


void DHT11_start();
uint8_t Leer8Bits();
void ActualizarLCD(uint8_t temperatura_i, uint8_t temperatura_d, uint8_t humedad_i, uint8_t humedad_d);

uint8_t datos[5];

int main(void)
{
	uint8_t checksum = 0;

	LCDinit();
	LCDclr();
	LCDhome();

	while (1)
	{
		DHT11_start();
		
		datos[0] = Leer8Bits(); // Humedad parte entera
		datos[1] = Leer8Bits(); // Humedad parte decimal
		datos[2] = Leer8Bits(); // Temperatura parte entera
		datos[3] = Leer8Bits(); // Temperatura parte decimal
		datos[4] = Leer8Bits(); // checksum
		checksum = datos[0] + datos[1] + datos[2] + datos[3]; // suma los datos en una variable para comparar e dato de checksum

		if (checksum == datos[4]) // verificar checksum
		{
			ActualizarLCD(datos[0], datos[1], datos[2], datos[3]);
		}
		else
		{
			DHT11_start(); // Error en checksum, manejar según sea necesario
		}

		_delay_ms(2000); // Esperar antes de la próxima lectura
	}
}

void DHT11_start(){
	DDRB |= (1 << PORTB0); // Configurar el pin como salida
	PORTB &= ~(1 << PORTB0); // Enviar señal de start al DHT11
	_delay_ms(18); // Esperar al menos 18 ms
	DDRB &= ~(1 << PORTB0); // Configurar el pin como entrada, sube automaticamente e valor a 1
	while (  PINB & (1 << PORTB0) ); //Espero hasta que el DHT11 envie señal de respuesta en 0
	while ((PINB & (1 << PORTB0)) == 0); //DHT11 envia señal en bajo, aproximadamente durante 80us, espero hasta que envie señal en alto
	while (  PINB & (1 << PORTB0) ); //DHT11 envia señal en alto, aproximadamente durante 80us, espero hasta que envia señal en bajo que es cuando inicia la transmicion
}

uint8_t Leer8Bits(){
	uint8_t byte = 0;
	for (int i = 0; i < 8; i++)
	{
		while ((PINB & (1 << PORTB0)) == 0);
		_delay_us(30); //Espero 29 us, un microsegundo mas de lo que puede durar una seña en bajo
		if (PINB & (1 << PORTB0)){ //verifico si  sigue en alto
			
			byte = ((byte << 1) | 1); // si sigue en alto quiere decir que es un 1
		}
		else
		{
			byte = ((byte << 1) | 0); // si no es un 0
		}
		while ((PINB & (1 << PORTB0)) != 0); // espero al proximo bit, aproximadamente 50us
	}
	return byte;
}

void ActualizarLCD(uint8_t humedad_i, uint8_t humedad_d, uint8_t temperatura_i, uint8_t temperatura_d)
{
	LCDGotoXY(0, 0);

	LCDstring("Hum: ", 5);
	LCDescribeDato(humedad_i, 2);
	LCDstring(".", 1);
	LCDescribeDato(humedad_d, 2);
	LCDstring("%", 1);

	LCDGotoXY(0, 1);

	LCDstring("Temp: ", 6);
	LCDescribeDato(temperatura_i, 2);
	LCDstring(".", 1);
	LCDescribeDato(temperatura_d, 2);
	LCDstring("C", 1);
}