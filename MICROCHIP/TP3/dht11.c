/*
 * i2c.c
 *
 * Created: 7/6/2024 14:10:00
 * Author : Javier
 */ 

#include "dht11.h"

static uint8_t datos[5];
static char mensaje[50];
static char humedad[50];
static char temperatura[50];

static void DHT11_start(){
	DDRC |= (1 << PINC0);    // Configurar el pin como salida
	PORTC &= ~(1 << PINC0);  // Enviar señal de start al DHT11 (valor en 0 )
	_delay_ms(20);            // Esperar al menos 20 ms
	PORTC |= (1 << PINC0);  // Se pone en alto el pin del DHT11 (valor en 1)
	DDRC &= ~(1 << PINC0);   // Configurar el pin como entrada
	while (  PINC & (1 << PINC0) ); //Espero hasta que el DHT11 envie señal de respuesta en 0
	while ((PINC & (1 << PINC0)) == 0); //DHT11 envia señal en bajo, aproximadamente durante 80us, espero hasta que envie señal en alto
	while (  PINC & (1 << PINC0) ); //DHT11 envia señal en alto, aproximadamente durante 80us, espero hasta que envia señal en bajo que es cuando inicia la transmicion
}

static uint8_t Leer8Bits(){
	uint8_t byte = 0;
	for (int i = 0; i < 8; i++)
	{
		while ((PINC & (1 << PINC0)) == 0); //Espera aproximadamente 50us en bajo
		_delay_us(30); //Espera 30 us, dos microsegundo mas de lo que puede durar una señal que se interpreta como bit en 0
		if (PINC & (1 << PINC0)){ //Verifica si  sigue en alto
			
			byte = ((byte << 1) | 1); // Si sigue en alto quiere decir que es un 1
		}
		else
		{
			byte = ((byte << 1) | 0); // Si no es un 0
		}
		while ((PINC & (1 << PINC0)) != 0); // Espero al proximo bit, aproximadamente 50us
	}
	return byte;
}

static uint8_t leerTemperatura(char *humedad, char *temperatura){
	uint8_t checksum = 0;
	cli();//Se desabilitan las interrupciones
	DHT11_start();
	datos[0] = Leer8Bits(); // Humedad parte entera
	datos[1] = Leer8Bits(); // Humedad parte decimal
	datos[2] = Leer8Bits(); // Temperatura parte entera
	datos[3] = Leer8Bits(); // Temperatura parte decimal
	datos[4] = Leer8Bits(); // checksum
	checksum = datos[0] + datos[1] + datos[2] + datos[3]; // Se suman los datos en una variable para comparar el dato de checksum
	
	DDRC  |= (1 <<  PINC0); //Se configura nuevamente del DHT11 como salida
	PORTC |= (1 <<  PINC0); //Vuelve a poner el pin del DHT11 en alto
	
	sei();//Se habiitan nuevamente as interrupciones
	
	if (checksum == datos[4]){
		sprintf(temperatura, "TEMP: %2dC", datos[2]); //Se guarda el valor de la temperatura en un string
		sprintf(humedad, "HUM: %2d", datos[0]); //Se guarda el valor de la humedad en un string
		return 1;
	}
	else
	{
		return 0;
	}
}

char *DHT11_enviarInformacion(){
	leerTemperatura(humedad, temperatura);
	strcpy(mensaje, humedad); //Copia el string humedad al string mensaje
	strcat(mensaje, " "); //Concatena un espacio
	strcat(mensaje, temperatura);//Concatena el temperatura  al string mensaje
	strcat(mensaje, " ");//Concatena un espacio
	return mensaje;
}