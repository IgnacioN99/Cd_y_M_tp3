/*
 * i2c.c
 *
 * Created: 6/6/2024 18:34:12
 * Author : Ignacio
 */ 

#include "i2c.h"

void i2c_init() {
	TWBR = (uint8_t)(((F_CPU / SCL_CLOCK) - 16) / 2); //Establece la frecuencia del clock SCL
	TWSR = 0; // Preescaler a 1
}

void i2c_start() {
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);// Establece la condición de START 
	while (!(TWCR & (1 << TWINT))); // Espera a que TWINT se ponga en 1
}

void i2c_stop() {
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN); // Establece condición de STOP
}

void i2c_write(uint8_t data) {
	TWDR = data; // Cargar dato en el registro TWDR
	TWCR = (1 << TWINT) | (1 << TWEN); // Iniciar transmisión
	while (!(TWCR & (1 << TWINT))); // Esperar a que se complete
}

uint8_t i2c_read(uint8_t ack) {
	TWCR = (1 << TWINT) | (1 << TWEN) | (ack << TWEA); // Configurar ACK, si es 1 se envio correctamente a informacion, si es 0 es que finalizo el envio de informacion o hubo un error y se detiene e envio
	while (!(TWCR & (1 << TWINT))); // Esperar a que se complete la recepción
	return TWDR; // Devolver el dato recibido
}